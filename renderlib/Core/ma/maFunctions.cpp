/****************************************************************************\
**  maFunctions.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/ma/maFunctions.hpp"

#include "Core/ma/maConstants.hpp"
#include "Core/ma/maRotation.hpp"

#include <stdlib.h>
#include <time.h>

//============================================================================
//============================================================================
namespace {
// This does not require maPackage::Init().  The truly proper way to do this
// is instantiate in an maFunctions::Init() and clean up in maFunctions::CleanUp.
// we did not do that here, knowing the implementation of maRand32

//	As soon as you can notice that this is not random enough, go ahead and implement
//	a more complicated system.  Just remember you can't use time on the PS2.
const int l_ThisIsRandomEnough = 1355;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
maRand32 l_randgen(l_ThisIsRandomEnough);

//----------------------------------------------------------------------------
// ensure float is non-negative by dividing max by 2, ffffffff / 2 = 7fffffff
//----------------------------------------------------------------------------
float lc_fRandPositiveMax = (float)(l_randgen.RandMax() / 2);
}

//============================================================================
//============================================================================
namespace maFunctions {
//------------------------------------------------------------------------
//	FloatRand returns a random floating point number in the range
//	[i_Lo, i_Hi)
//------------------------------------------------------------------------
float
FloatRand(float i_Lo, float i_Hi)
{
  // old Rand code, worked fine for old rand code,
  // Runs at an average of 1 entry every 269 nanoseconds over a ten million
  // number sample. -- TMS 2/4/02
  //
  // int num = rand();
  // float ret_val = float(num) / float(RAND_MAX);

  // This runs at an average of 1 number every 818 nanoseconds over a ten million
  // number sample.
  // This is about 3 times slower than Microsoft's rand(), but the random number distribution is
  // significantly improved.  A trade-off, we choose accuracy.  -- TMS 2/4/02
  //
  envType::UInt32 num = (l_randgen.Rand() / 2); // just the positive entries
  float ret_val = float(num) / float(lc_fRandPositiveMax);

  // They share this code to the end
  ret_val *= i_Hi - i_Lo;
  ret_val += i_Lo;

  return ret_val;
}

//------------------------------------------------------------------------
//	FloatRand returns a random floating point number in the range
//	[i_Lo, i_Hi)
//------------------------------------------------------------------------
float
FloatRand(float i_Lo, float i_Hi, maRand32& i_RandGen)
{
  envType::UInt32 num = (i_RandGen.Rand() / 2); // just the positive entries
  float ret_val = float(num) / float(lc_fRandPositiveMax);

  // They share this code to the end
  ret_val *= i_Hi - i_Lo;
  ret_val += i_Lo;

  return ret_val;
}

//------------------------------------------------------------------------
//	IntRand returns a random integer in the range [i_Lo, i_Hi)
//------------------------------------------------------------------------
int
IntRand(int i_Lo, int i_Hi)
{
  // This could alternatively call into FloatRand.
  // Question: What is slower, a FP mul + integer convert, or a integer
  // mod?  Some places say that the FP mul gives better uniformity.

  envType::UInt32 num = (l_randgen.Rand() / 2); // just the positive entries
  num = num % (i_Hi - i_Lo + 1);
  return i_Lo + num;
}

//------------------------------------------------------------------------
//  GetRelativeYawPitch returns the yaw and pitch of the direction vectors
//	world orientation
//----------------------------------------------------------------------------
void
GetYawPitch(const maVector3d& i_DirectionVec, float& o_Yaw, float& o_Pitch)
{
  // find yaw and pitch between weapon_dir and target_in_obj_space
  o_Yaw = atan2f(i_DirectionVec.GetX(), i_DirectionVec.GetZ());
  float len = i_DirectionVec.GetX() * i_DirectionVec.GetX() + i_DirectionVec.GetZ() * i_DirectionVec.GetZ();
  len = sqrtf(len);
  o_Pitch = atan2f(i_DirectionVec.GetY(), len);
}

//----------------------------------------------------------------------------
//  GetRelativeyawPitch returns the yaw and pitch of the direction vector
//	relative to the orientation that is passed in.
//----------------------------------------------------------------------------
void
GetRelativeYawPitch(const maRotation& i_Rotation, const maVector3d& i_DirectionVec, float& o_Yaw, float& o_Pitch)
{
  maRotation inv_rot = i_Rotation;
  inv_rot.Invert();
  maVector3d target_dir = i_DirectionVec;
  inv_rot.RotateVector(target_dir);

  // find yaw and pitch between weapon_dir and target_in_obj_space
  o_Yaw = atan2f(target_dir.GetX(), target_dir.GetZ());
  float len = target_dir.GetX() * target_dir.GetX() + target_dir.GetZ() * target_dir.GetZ();
  len = sqrtf(len);
  o_Pitch = atan2f(target_dir.GetY(), len);
}

//----------------------------------------------------------------------------
//  GetRelativeDirection returns a new direction vector
//	relative to the orientation that is passed in.
//----------------------------------------------------------------------------
void
GetRelativeDirection(const maRotation& i_Rotation, const maVector3d& i_DirectionVec, maVector3d& o_DirectionVec)
{
  maRotation inv_rot = i_Rotation;
  inv_rot.Invert();
  o_DirectionVec = i_DirectionVec;
  inv_rot.RotateVector(o_DirectionVec);
}

//----------------------------------------------------------------------------
//	LinesIntersect() returns:
//	0 - no intersection
//	1 - intersection
//	2 - Collinear
//
//	Note: adapted from Graphics Gems 2
//----------------------------------------------------------------------------
int
LinesIntersect(maPoint2d& i_Pt1, maPoint2d& i_Pt2, maPoint2d& i_Pt3, maPoint2d& i_Pt4, maPoint2d& o_Pt)
{
// **************************************************************
// *                                                            *
// *    NOTE:  The following macro to determine if two numbers  *
// *    have the same sign, is for 2's complement number        *
// *    representation.  It will need to be modified for other  *
// *    number systems.                                         *
// *                                                            *
// **************************************************************/
#define SAME_SIGNS(a, b) (((long)a ^ (long)b) >= 0)

  float x1, y1, x2, y2, x3, y3, x4, y4;
  float a1, a2, b1, b2, c1, c2; ///* Coefficients of line eqns. */
  float r1, r2, r3, r4;         ///* 'Sign' values */
  float denom, offset, num;     ///* Intermediate values */

  x1 = i_Pt1.GetX();
  y1 = i_Pt1.GetY();
  x2 = i_Pt2.GetX();
  y2 = i_Pt2.GetY();
  x3 = i_Pt3.GetX();
  y3 = i_Pt3.GetY();
  x4 = i_Pt4.GetX();
  y4 = i_Pt4.GetY();

  //* Compute a1, b1, c1, where line joining points 1 and 2
  // * is "a1 x  +  b1 y  +  c1  =  0".
  // */

  a1 = y2 - y1;
  b1 = x1 - x2;
  c1 = x2 * y1 - x1 * y2;

  //* Compute r3 and r4.
  // */

  r3 = a1 * x3 + b1 * y3 + c1;
  r4 = a1 * x4 + b1 * y4 + c1;

  //* Check signs of r3 and r4.  If both point 3 and point 4 lie on
  // * same side of line 1, the line segments do not intersect.
  // */

  if (r3 != 0 && r4 != 0 && SAME_SIGNS(r3, r4))
    return (0); // DO NOT INTERSECT

  //* Compute a2, b2, c2 */

  a2 = y4 - y3;
  b2 = x3 - x4;
  c2 = x4 * y3 - x3 * y4;

  //* Compute r1 and r2 */

  r1 = a2 * x1 + b2 * y1 + c2;
  r2 = a2 * x2 + b2 * y2 + c2;

  //* Check signs of r1 and r2.  If both point 1 and point 2 lie
  // * on same side of second line segment, the line segments do
  // * not intersect.
  // */

  if (r1 != 0 && r2 != 0 && SAME_SIGNS(r1, r2))
    return (0); // DO NOT INTERSECT

  //* Line segments intersect: compute intersection point.
  // */

  denom = a1 * b2 - a2 * b1;
  if (denom == 0)
    return (2); // COLLINEAR
  offset = denom < 0 ? -denom / 2 : denom / 2;

  //* The denom/2 is to get rounding instead of truncating.  It
  // * is added or subtracted to the numerator, depending upon the
  // * sign of the numerator.
  // */

  num = b1 * c2 - b2 * c1;
  o_Pt.SetX((num < 0 ? num - offset : num + offset) / denom);

  num = a2 * c1 - a1 * c2;
  o_Pt.SetY((num < 0 ? num - offset : num + offset) / denom);

  return (1); // DO INTERSECT
}

//-----------------------------------------------------------------------------
// Name: GaussianDistribution
// Desc: Helper function for GetSampleOffsets function to compute the
//       2 parameter Gaussian distrubution using the given standard deviation
//       rho
//-----------------------------------------------------------------------------
float
GaussianDistribution(float i_X, float i_Y, float i_Rho)
{
  float g = 1.0f / sqrtf(2.0f * maConstants::c_fPI * i_Rho * i_Rho);
  g *= expf(-(i_X * i_X + i_Y * i_Y) / (2.0f * i_Rho * i_Rho));

  return g;
}

//-----------------------------------------------------------------------------
// Log() - compute the logarithm of a number in any base
//-----------------------------------------------------------------------------
float
Log(float i_Val, float i_Base)
{
  return log(i_Val) / log(i_Base);
}

//-----------------------------------------------------------------------------
// convert float to half, bitwise
//-----------------------------------------------------------------------------
envType::UInt16
FloatToHalfI(envType::UInt32 i)
{
  int s = (i >> 16) & 0x00008000;
  int e = ((i >> 23) & 0x000000ff) - (127 - 15);
  int m = i & 0x007fffff;

  if (e <= 0) {
    if (e < -10) {
      return 0;
    }
    m = (m | 0x00800000) >> (1 - e);

    return s | (m >> 13);
  } else if (e == 0xff - (127 - 15)) {
    if (m == 0) // Inf
    {
      return s | 0x7c00;
    } else // NAN
    {
      m >>= 13;
      return s | 0x7c00 | m | (m == 0);
    }
  } else {
    if (e > 30) // Overflow
    {
      return s | 0x7c00;
    }

    return s | (e << 10) | (m >> 13);
  }
}

//-----------------------------------------------------------------------------
// convert float to half
//-----------------------------------------------------------------------------
envType::UInt16
FloatToHalf(float i)
{
  union
  {
    float f;
    envType::UInt32 i;
  } v;
  v.f = i;
  return FloatToHalfI(v.i);
}

//-----------------------------------------------------------------------------
// convert half to float, bitwise
//-----------------------------------------------------------------------------
envType::UInt32
HalfToFloatI(envType::UInt16 y)
{
  int s = (y >> 15) & 0x00000001;
  int e = (y >> 10) & 0x0000001f;
  int m = y & 0x000003ff;

  if (e == 0) {
    if (m == 0) // Plus or minus zero
    {
      return s << 31;
    } else // Denormalized number -- renormalize it
    {
      while (!(m & 0x00000400)) {
        m <<= 1;
        e -= 1;
      }

      e += 1;
      m &= ~0x00000400;
    }
  } else if (e == 31) {
    if (m == 0) // Inf
    {
      return (s << 31) | 0x7f800000;
    } else // NaN
    {
      return (s << 31) | 0x7f800000 | (m << 13);
    }
  }

  e = e + (127 - 15);
  m = m << 13;

  return (s << 31) | (e << 23) | m;
}

//-----------------------------------------------------------------------------
// convert half to float
//-----------------------------------------------------------------------------
float
HalfToFloat(envType::UInt16 y)
{
  union
  {
    float f;
    envType::UInt32 i;
  } v;
  v.i = HalfToFloatI(y);
  return v.f;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
HSV_to_RGB(float i_Hsv_H, float i_Hsv_S, float i_Hsv_V, float& o_Rgb_R, float& o_Rgb_G, float& o_Rgb_B)
{
  float red, green, blue;

  float hsv_hue = i_Hsv_H;
  float hsv_saturation = i_Hsv_S;
  float hsv_value = i_Hsv_V;

  if (i_Hsv_S == 0) {
    // Grey
    red = green = blue = hsv_value;
  } else // not grey
  {
    float hue = hsv_hue * 6.0; // sector 0 to 5
    int i = (int)floor(hue);
    float f = hue - i; // fractional part of h
    float p = hsv_value * (1.0 - hsv_saturation);

    switch (i) {
      case 0:
        red = hsv_value;
        green = hsv_value * (1.0 - hsv_saturation * (1.0 - f));
        blue = p;
        break;

      case 1:
        red = hsv_value * (1.0 - hsv_saturation * f);
        green = hsv_value;
        blue = p;
        break;

      case 2:
        red = p;
        green = hsv_value;
        blue = hsv_value * (1.0 - hsv_saturation * (1.0 - f));
        break;

      case 3:
        red = p;
        green = hsv_value * (1.0 - hsv_saturation * f);
        blue = hsv_value;
        break;

      case 4:
        red = hsv_value * (1.0 - hsv_saturation * (1.0 - f));
        green = p;
        blue = hsv_value;
        break;

      default: // case 5:
        red = hsv_value;
        green = p;
        blue = hsv_value * (1.0 - hsv_saturation * f);
        break;
    }
  }

  // o_Rgb_R = (int)(red * 255);
  // o_Rgb_G = (int)(green * 255);
  // o_Rgb_B = (int)(blue* 255);
  o_Rgb_R = red;
  o_Rgb_G = green;
  o_Rgb_B = blue;
}
//-----------------------------------------------------------------------------
// linear interpolation. by default, use a 0..1 domain. this matches HLSL lerp.
//-----------------------------------------------------------------------------
float
Interpolate(float y1, float y2, float x, float x1 /*=0*/, float x2 /*=1*/)
{
  DBG_ASSERT(y2 > y1, "Bad range for interpolation: " << y2 << ".." << y1);
  DBG_ASSERT(x2 > x1, "Bad domain for interpolation: " << x2 << ".." << x1);
  return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
}
}
