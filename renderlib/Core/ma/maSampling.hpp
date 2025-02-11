/*****************************************************************************
**  maSampling.hpp
**
**      This file contains functions related to sampling and filtering.
**
**
**
\****************************************************************************/

#ifdef MA_SAMPLING_HPP
#error maSampling.hpp multiply included
#endif
#define MA_SAMPLING_HPP

#ifndef MA_POINT2D_HPP
#include "Core/ma/maPoint2d.hpp"
#endif

#include <vector>

//============================================================================
//	forward references
//============================================================================
class maVector3d;

//============================================================================
// generic class for filtering 2d pixel samples
//============================================================================
class maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const = 0;
};

//============================================================================
// equivalent to RiBoxFilter (renderman)
//------------------------------------------------------------------------
class maBoxFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;
};

//------------------------------------------------------------------------
// equivalent to RiTriangleFilter (renderman)
//------------------------------------------------------------------------
class maTriangleFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;
};

//------------------------------------------------------------------------
// equivalent to RiGaussianFilter (renderman)
//============================================================================
class maGaussianFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;
};

//============================================================================
// equivalent to RiMitchellFilter (renderman)
//============================================================================
class maMitchellFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;

private:
  static float Evaluate(float i_X);
};

//------------------------------------------------------------------------
// equivalent to RiSincFilter (renderman)
//------------------------------------------------------------------------
class maSincFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;

  //------------------------------------------------------------------------
  // 1-d separable
  //------------------------------------------------------------------------
  static float Evaluate(float i_X, float i_Width);
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class maLanczosFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;

  //------------------------------------------------------------------------
  // 1-d separable
  //------------------------------------------------------------------------
  static float Evaluate(float i_X, float i_Width);
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class maBlackmanHarrisFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class maCatmullRomFilter : public maFilter
{
public:
  virtual float Evaluate(float i_X, float i_Y, float i_XWidth, float i_YWidth) const;
};

//============================================================================
//============================================================================
namespace maSampling {
//------------------------------------------------------------------------
//	GetSamples2D() - calculate random jittered sample positions
//	these are 2d sample positions, with a z coordinate being a weight factor
//	sample positions range from 0,0 to 1,1. Jitteramout=1 means the sample
//	can be anywhere in its full cell, jitter =0 means center of sample cell.
//------------------------------------------------------------------------
void
GetSamples2D(int i_nSamplesX,
             int i_nSamplesY,
             std::vector<maVector3d>& o_Samples,
             float i_JitterAmount = 1,
             float i_Width = 1,
             float i_Height = 1);
void
GetSamples2D_Repeatable(int i_nSamplesX,
                        int i_nSamplesY,
                        std::vector<maVector3d>& o_Samples,
                        float i_JitterAmount = 1,
                        float i_Width = 1,
                        float i_Height = 1);
void
GetSamples2D_NRooks(int i_nSamples, std::vector<maVector3d>& o_Samples);

//------------------------------------------------------------------------
//	Put a weight in the z coordinate.
//	sample positions range from 0,0 to 1,1
//	assume center of weight function is (0.5,0.5)
//------------------------------------------------------------------------
void
WeightSamples(std::vector<maVector3d>& o_Samples, const maFilter* i_Filter, float i_XWidth, float i_YWidth);

// assumes image has support? samples off edge will use edge color.
void
FilterImage(float* io_RGBA, int i_Height, int i_Width, const maFilter* i_Filter, float i_XWidth, float i_YWidth);
}
