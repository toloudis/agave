/****************************************************************************\
**  maSampling.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/ma/maSampling.hpp"

#include "Core/ma/maConstants.hpp"
#include "Core/ma/maFunctions.hpp"
#include "Core/ma/maVector3d.hpp"

#include <algorithm>

//------------------------------------------------------------------------
// equivalent to RiBoxFilter (renderman)
//------------------------------------------------------------------------
float
maBoxFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  /* x and y will be in the
   *    following intervals:
   *           -xwidth/2 <= x <= xwidth/2
   *           -ywidth/2 <= y <= ywidth/2
   *    These constraints on x and y really simplifies the
   *       the following code to just return (1.0).
   *
   */
  return std::min((fabs(i_X) <= i_XWidth / 2.0 ? 1.0 : 0.0), (fabs(i_Y) <= i_YWidth / 2.0 ? 1.0 : 0.0));
}

//------------------------------------------------------------------------
// equivalent to RiTriangleFilter (renderman)
//------------------------------------------------------------------------
float
maTriangleFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  float hxw = i_XWidth / 2.0;
  float hyw = i_YWidth / 2.0;
  float absx = fabs(i_X);
  float absy = fabs(i_Y);

  /* This function can be simplified as well by not worrying about
   *    returning zero if the sample is beyond the filter window.
   */
  return std::min((absx <= hxw ? (hxw - absx) / hxw : 0.0), (absy <= hyw ? (hyw - absy) / hyw : 0.0));
}

//------------------------------------------------------------------------
// equivalent to RiGaussianFilter (renderman)
//------------------------------------------------------------------------
float
maGaussianFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  i_X /= i_XWidth;
  i_Y /= i_YWidth;

  return expf(-8.0f * (i_X * i_X + i_Y * i_Y));
}

float
maMitchellFilter::Evaluate(float x)
{
  const float B = 1.0f / 3.0f;
  const float C = 1.0f / 3.0f;

  x = fabsf(2.f * x);
  if (x > 1.f)
    return ((-B - 6 * C) * x * x * x + (6 * B + 30 * C) * x * x + (-12 * B - 48 * C) * x + (8 * B + 24 * C)) *
           (1.f / 6.f);
  else
    return ((12 - 9 * B - 6 * C) * x * x * x + (-18 + 12 * B + 6 * C) * x * x + (6 - 2 * B)) * (1.f / 6.f);
}

//------------------------------------------------------------------------
// equivalent to RiMitchellFilter (renderman)
//------------------------------------------------------------------------
float
maMitchellFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  float invXWidth = 1.0f / i_XWidth;
  float invYWidth = 1.0f / i_YWidth;
  i_X *= invXWidth;
  i_Y *= invYWidth;

  return Evaluate(i_X) * Evaluate(i_Y);
}

// 1-d separable
float
maSincFilter::Evaluate(float i_X, float i_Width)
{
  /* Modified version of the RI Spec 3.2 sinc filter to be
   *   windowed with a positive lobe of a cosine which is half
   *   of a cosine period.
   */

  /* Uses a -PI to PI cosine window. */
  if (i_X != 0.0) {
    i_X *= maConstants::c_fPI;
    i_X = cos(0.5 * i_X / i_Width) * sin(i_X) / i_X;
  } else {
    i_X = 1.0;
  }
  return i_X;
}

//------------------------------------------------------------------------
// equivalent to RiSincFilter (renderman)
//------------------------------------------------------------------------
float
maSincFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  // RtFloat d;
  //
  // d = sqrt(x*x+y*y);
  //
  // if(d!=0)
  //   return(sin(RI_PI*d)/(RI_PI*d));
  // else
  //   return(1.0);

  i_X = Evaluate(i_X, i_XWidth);
  i_Y = Evaluate(i_Y, i_YWidth);

  /* This is a square separable filter and is the 2D Fourier
   * transform of a rectangular box outlining a lowpass bandwidth
   * filter in the frequency domain.
   */
  return i_X * i_Y;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
float
maLanczosFilter::Evaluate(float i_X, float i_Width)
{
  if ((i_X = fabs(i_X)) < i_Width)
    return maSincFilter::Evaluate(i_X, i_Width) * maSincFilter::Evaluate(i_X / i_Width, i_Width);
  return 0.f;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
float
maLanczosFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  i_X = maLanczosFilter::Evaluate(i_X, i_XWidth);
  i_Y = maLanczosFilter::Evaluate(i_Y, i_YWidth);
  return i_X * i_Y;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
float
maBlackmanHarrisFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  float xc = i_X / i_XWidth;
  float yc = i_Y / i_YWidth;
  float r2 = (xc * xc + yc * yc);
  float r = 0.5f - sqrtf(r2);

  const float N = 1;
  const float a0 = 0.35875f;
  const float a1 = 0.48829f;
  const float a2 = 0.14128f;
  const float a3 = 0.01168f;

  if (r <= N * 0.5f) {
    return (float)(a0 - a1 * cosf(2 * ((float)maConstants::c_fPI) * r / N) +
                   a2 * cosf(4 * ((float)maConstants::c_fPI) * r / N) -
                   a3 * cosf(6 * ((float)maConstants::c_fPI) * r / N));
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
float
maCatmullRomFilter::Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const
{
  float r2 = (i_X * i_X + i_Y * i_Y);
  float r = sqrtf(r2);

  if (r < 1.0f) {
    return (float)(1.5f * r * r2 - 2.5f * r2 + 1.0f);
  } else if (r < 2.0f) {
    return (float)(-0.5f * r * r2 + 2.5f * r2 - 4.0f * r + 2.0f);
  } else {
    return 0;
  }
}

namespace maSampling {

//------------------------------------------------------------------------
//	Jitter() - calculate random jittered sample positions
//	number of samples returned is i_nSamplesX*i_nSamplesY.
//	these are 2d sample positions, with a z coordinate being a weight factor
//	samples are in a grid with random shifts inside of grid cells.
//	sample positions range from (0.5-w/2,0.5-w/2) to (0.5+w/2,0.5+w/2)
//	I arbitrarily restrict the max nsamples to 8, or 64 sample positions.
//	JitterAmount is how much to wiggle the sample point within its cell
//	0 means only at center of cell, 1 means can be anywhere in entire cell
//------------------------------------------------------------------------
void
GetSamples2D(int i_nSamplesX,
             int i_nSamplesY,
             std::vector<maVector3d>& o_Samples,
             float i_JitterAmount /* = 1 */,
             float i_Width /*= 1*/,
             float i_Height /*= 1*/)
{
  DBG_ASSERT(i_nSamplesX > 0, "Bad input to GetSamples2D, number of samples < 1");
  //	DBG_ASSERT(i_nSamplesX < 9, "Bad input to GetSamples2D, number of samples > 8");
  DBG_ASSERT(i_nSamplesY > 0, "Bad input to GetSamples2D, number of samples < 1");
  //	DBG_ASSERT(i_nSamplesY < 9, "Bad input to GetSamples2D, number of samples > 8");
  DBG_ASSERT(i_JitterAmount >= 0, "Bad input to GetSamples2D, jitterAmount < 0");
  DBG_ASSERT(i_JitterAmount <= 1, "Bad input to GetSamples2D, jitterAmount > 1");

  int nSamples = i_nSamplesX * i_nSamplesY;
  o_Samples.resize(nSamples);
  o_Samples.clear();

  // never jitter the trivial case?
  if (nSamples == 1) {
    o_Samples.push_back(maVector3d(0.5f, 0.5f, 1.0f));
    return;
  }

  float sampleSpacingX = 1.0f / (float)i_nSamplesX;
  float sampleSpacingY = 1.0f / (float)i_nSamplesY;

  // example:
  // nsamples = 4, samplespacing = 0.25
  // 0                       1
  // |--*--|--*--|--*--|--*--|
  //  0.125 0.375 0.625 0.875
  //    0     1     2     3
  //

  int i, j;
  float x, y, z, jit;
  for (i = 0; i < i_nSamplesY; i++) {
    // center of jitter cell for sample

    for (j = 0; j < i_nSamplesX; j++) {
      y = ((float)i + 0.5f) * sampleSpacingY;

      // add random jitter
      jit = maFunctions::FloatRand(-0.5, 0.5);
      jit *= i_JitterAmount;
      jit *= sampleSpacingY;
      y += jit;

      // the x,y samples are now on a grid from 0,0 to 1,1.
      // spread out the samples by i_Width, keeping centered on 0.5,0.5.
      // (shift to origin, mul by i_Width, shift back by 0.5)
      y = y - 0.5f;
      y *= (i_Width);
      y = y + 0.5f;

      x = ((float)j + 0.5f) * sampleSpacingX;

      // add random jitter
      jit = maFunctions::FloatRand(-0.5, 0.5);
      jit *= i_JitterAmount;
      jit *= sampleSpacingX;
      x += jit;

      // the x,y samples are now on a grid from 0,0 to 1,1.
      // spread out the samples by i_Width, keeping centered on 0.5,0.5.
      // (shift to origin, mul by i_Width, shift back by 0.5)
      x = x - 0.5f;
      x *= (i_Height);
      x = x + 0.5f;

      // weight = area of cell (box filter)
      // this could be abstracted to pass the sample position
      // into a filter function to get a weight

      // in this case, all weights should add to 1.
      z = /*i_Width*i_Height **/ sampleSpacingX * sampleSpacingY;

      o_Samples.push_back(maVector3d(x, y, z));
    }
  }
}

//------------------------------------------------------------------------
//	Jitter() - calculate random jittered sample positions
//	number of samples returned is i_nSamplesX*i_nSamplesY.
//	these are 2d sample positions, with a z coordinate being a weight factor
//	samples are in a grid with random shifts inside of grid cells.
//	sample positions range from (0.5-w/2,0.5-w/2) to (0.5+w/2,0.5+w/2)
//	I arbitrarily restrict the max nsamples to 8, or 64 sample positions.
//	JitterAmount is how much to wiggle the sample point within its cell
//	0 means only at center of cell, 1 means can be anywhere in entire cell
//------------------------------------------------------------------------
void
GetSamples2D_Repeatable(int i_nSamplesX,
                        int i_nSamplesY,
                        std::vector<maVector3d>& o_Samples,
                        float i_JitterAmount /* = 1 */,
                        float i_Width /*= 1*/,
                        float i_Height /*= 1*/)
{
  DBG_ASSERT(i_nSamplesX > 0, "Bad input to GetSamples2D, number of samples < 1");
  //	DBG_ASSERT(i_nSamplesX < 9, "Bad input to GetSamples2D, number of samples > 8");
  DBG_ASSERT(i_nSamplesY > 0, "Bad input to GetSamples2D, number of samples < 1");
  //	DBG_ASSERT(i_nSamplesY < 9, "Bad input to GetSamples2D, number of samples > 8");
  DBG_ASSERT(i_JitterAmount >= 0, "Bad input to GetSamples2D, jitterAmount < 0");
  DBG_ASSERT(i_JitterAmount <= 1, "Bad input to GetSamples2D, jitterAmount > 1");

  int nSamples = i_nSamplesX * i_nSamplesY;
  o_Samples.resize(nSamples);
  o_Samples.clear();

  // never jitter the trivial case?
  if (nSamples == 1) {
    o_Samples.push_back(maVector3d(0.5f, 0.5f, 1.0f));
    return;
  }

  float sampleSpacingX = 1.0f / (float)i_nSamplesX;
  float sampleSpacingY = 1.0f / (float)i_nSamplesY;

  // example:
  // nsamples = 4, samplespacing = 0.25
  // 0                       1
  // |--*--|--*--|--*--|--*--|
  //  0.125 0.375 0.625 0.875
  //    0     1     2     3
  //

  const int seed = 1200;
  maRand32 rand_gen(seed);

  int i, j;
  float x, y, z, jit;
  for (i = 0; i < i_nSamplesY; i++) {
    // center of jitter cell for sample

    for (j = 0; j < i_nSamplesX; j++) {
      y = ((float)i + 0.5f) * sampleSpacingY;

      // add random jitter
      jit = maFunctions::FloatRand(-0.5, 0.5, rand_gen);
      jit *= i_JitterAmount;
      jit *= sampleSpacingY;
      y += jit;

      // the x,y samples are now on a grid from 0,0 to 1,1.
      // spread out the samples by i_Width, keeping centered on 0.5,0.5.
      // (shift to origin, mul by i_Width, shift back by 0.5)
      y = y - 0.5f;
      y *= (i_Width);
      y = y + 0.5f;

      x = ((float)j + 0.5f) * sampleSpacingX;

      // add random jitter
      jit = maFunctions::FloatRand(-0.5, 0.5, rand_gen);
      jit *= i_JitterAmount;
      jit *= sampleSpacingX;
      x += jit;

      // the x,y samples are now on a grid from 0,0 to 1,1.
      // spread out the samples by i_Width, keeping centered on 0.5,0.5.
      // (shift to origin, mul by i_Width, shift back by 0.5)
      x = x - 0.5f;
      x *= (i_Height);
      x = x + 0.5f;

      // weight = area of cell (box filter)
      // this could be abstracted to pass the sample position
      // into a filter function to get a weight

      // in this case, all weights should add to 1.
      z = /*i_Width*i_Height **/ sampleSpacingX * sampleSpacingY;

      o_Samples.push_back(maVector3d(x, y, z));
    }
  }
}

void
GetSamples2D_NRooks(int i_nSamples, std::vector<maVector3d>& o_Samples)
{
  DBG_ASSERT(i_nSamples > 0, "Bad input to GetSamples2D_NRooks, number of samples < 1");
  DBG_ASSERT(i_nSamples < 9, "Bad input to GetSamples2D_NRooks, number of samples > 8");

  o_Samples.resize(i_nSamples * i_nSamples);
  o_Samples.clear();

  // never jitter the trivial case?
  if (i_nSamples == 1) {
    o_Samples.push_back(maVector3d(0.5f, 0.5f, 1.0f));
    return;
  }

  // note that i square the sample spacing already here:
  float sampleSpacing = 1.0f / (float)(i_nSamples * i_nSamples);

  // example:
  // nsamples = 4, samplespacing = 0.25
  // 0                       1
  // |--*--|--*--|--*--|--*--|
  //  0.125 0.375 0.625 0.875
  //    0     1     2     3
  //

  // fill sample cells down diagonal (with jitter within cell?)
  int i;
  float x, y, z;
  for (i = 0; i < i_nSamples * i_nSamples; ++i) {
    x = (maFunctions::FloatRand(0, 1) + i) * sampleSpacing;
    y = (maFunctions::FloatRand(0, 1) + i) * sampleSpacing;
    // for no jitter, this would be:
    // x = (i+0.5)*sampleSpacing;
    z = sampleSpacing;
    o_Samples.push_back(maVector3d(x, y, z));
  }
  // shuffle the x coordinates.
  for (i = (i_nSamples * i_nSamples) - 1; i > 0; --i) {
    int target = maFunctions::IntRand(0, i);
    // swap i with target
    x = o_Samples[i].GetX();
    o_Samples[i].SetX(o_Samples[target].GetX());
    o_Samples[target].SetX(x);
  }
}

//------------------------------------------------------------------------
//	Put a weight in the z coordinate.
//	sample positions range from 0,0 to 1,1
//	assume center of weight function is (0.5,0.5)
//------------------------------------------------------------------------
void
WeightSamples(std::vector<maVector3d>& o_Samples, const maFilter* i_Filter, float i_XWidth, float i_YWidth)
{
  int n = o_Samples.size();
  maVector3d s;

  for (int i = 0; i < n; i++) {
    s = o_Samples[i];

    float w = i_Filter->Evaluate(s.GetX() - 0.5f, s.GetY() - 0.5f, i_XWidth, i_YWidth);
    o_Samples[i].SetZ(w);
  }

  // normalize weights:
  float weight = 0;
  for (int i = 0; i < n; i++) {
    weight += (o_Samples[i].GetZ());
  }
  float nweight = (weight);
  for (int i = 0; i < n; i++) {
    o_Samples[i].SetZ(o_Samples[i].GetZ() / nweight);
  }
  // now the sum of all the z's will equal 1.
}

} // namespace maSampling
