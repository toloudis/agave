/*****************************************************************************
**	maFunctions.hpp
**
**		This file contains the non-component related functions for the
**	for the Math package.
**
**
**
\****************************************************************************/
#ifdef MA_FUNCTIONS_HPP
#error maFunctions.hpp multiply included
#endif
#define MA_FUNCTIONS_HPP

#ifndef DBG_MSG_HPP
#include "Core/dbg/dbgMsg.hpp"
#endif
#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif
#ifndef MA_POINT2D_HPP
#include "Core/ma/maPoint2d.hpp"
#endif
#ifndef MA_RAND_HPP
#include "Core/ma/maRand.hpp"
#endif

#include <stdlib.h> // rand, RAND_MAX
#include <vector>

//============================================================================
//	forward references
//============================================================================
class maRotation;
class maVector3d;

//============================================================================
//============================================================================
namespace maFunctions {
//------------------------------------------------------------------------
//	Highest Value
//------------------------------------------------------------------------
template<class T>
inline T
Highest(const T& i_Value1, const T& i_Value2);
template<class T>
inline T
Highest(const T& i_Value1, const T& i_Value2, const T& i_Value3);

//------------------------------------------------------------------------
//	Lowest Value
//------------------------------------------------------------------------
template<class T>
inline T
Lowest(const T& i_Value1, const T& i_Value2);
template<class T>
inline T
Lowest(const T& i_Value1, const T& i_Value2, const T& i_Value3);

//------------------------------------------------------------------------
//	Swap Value
//------------------------------------------------------------------------
template<class T>
inline void
Swap(T& io_Value1, T& io_Value2);

//------------------------------------------------------------------------
//	Clamp
//------------------------------------------------------------------------
template<class T>
inline void
Clamp(T& io_Value, const T& i_Low, const T& i_High);

//------------------------------------------------------------------------
//	Clamp
//------------------------------------------------------------------------
template<class T>
inline T
Clamp(const T& i_Value, const T& i_Low, const T& i_High);

//------------------------------------------------------------------------
//	Equal Value - are the values equal within a tolerance
//------------------------------------------------------------------------
template<class T>
inline bool
Equal(const T& i_Value1, const T& i_Value2, const float i_Tolerance);

//------------------------------------------------------------------------
//	FloatRand returns a random floating point number in the range
//	[i_Lo, i_Hi)
//------------------------------------------------------------------------
float
FloatRand(float i_Lo, float i_Hi);
float
FloatRand(float i_Lo, float i_Hi, maRand32& i_RandGen);

//------------------------------------------------------------------------
//	IntRand returns a random integer in the range [i_Lo, i_Hi)
//------------------------------------------------------------------------
int
IntRand(int i_Lo, int i_Hi);

//------------------------------------------------------------------------
//	float-specific absolute value
//------------------------------------------------------------------------
float
FAbs(float i_Val);

//------------------------------------------------------------------------
//	Returns i_Val rounded up to the nearest multiple of i_Spacing, WHICH
//	MUST BE A POWER OF 2.
//------------------------------------------------------------------------
int
RoundUp(int i_Val, int i_Spacing);

//------------------------------------------------------------------------
//	Highest Value
//------------------------------------------------------------------------
template<class T>
inline T
Highest(const T& i_Value1, const T& i_Value2)
{
  return (i_Value1 >= i_Value2) ? i_Value1 : i_Value2;
}

template<class T>
inline T
Highest(const T& i_Value1, const T& i_Value2, const T& i_Value3)
{
  if (i_Value1 > i_Value2) {
    if (i_Value1 > i_Value3)
      return i_Value1;
    else
      return i_Value3;
  } else {
    if (i_Value2 > i_Value3)
      return i_Value2;
    else
      return i_Value3;
  }
}

//------------------------------------------------------------------------
//	Lowest Value
//------------------------------------------------------------------------
template<class T>
inline T
Lowest(const T& i_Value1, const T& i_Value2)
{
  return (i_Value1 <= i_Value2) ? i_Value1 : i_Value2;
}

template<class T>
inline T
Lowest(const T& i_Value1, const T& i_Value2, const T& i_Value3)
{
  if (i_Value1 < i_Value2) {
    if (i_Value1 < i_Value3)
      return i_Value1;
    else
      return i_Value3;
  } else {
    if (i_Value2 < i_Value3)
      return i_Value2;
    else
      return i_Value3;
  }
}

//------------------------------------------------------------------------
//	Swap Value
//------------------------------------------------------------------------
template<class T>
inline void
Swap(T& io_Value1, T& io_Value2)
{
  const T temp = io_Value1;
  io_Value1 = io_Value2;
  io_Value2 = temp;
}

//------------------------------------------------------------------------
//	Clamp
//------------------------------------------------------------------------
template<class T>
inline void
Clamp(T& io_Value, const T& i_Low, const T& i_High)
{
  if (io_Value < i_Low)
    io_Value = i_Low;
  else if (io_Value > i_High)
    io_Value = i_High;
}

//------------------------------------------------------------------------
//	Clamp
//------------------------------------------------------------------------
template<class T>
inline T
Clamp(const T& i_Value, const T& i_Low, const T& i_High)
{
  if (i_Value < i_Low)
    return i_Low;
  else if (i_Value > i_High)
    return i_High;
  return i_Value;
}

//------------------------------------------------------------------------
//	Equal Value - are the values equal within a tolerance
//------------------------------------------------------------------------
template<class T>
inline bool
Equal(const T& i_Value1, const T& i_Value2, const float i_Tolerance)
{
  return i_Value1.Equal(i_Value2, i_Tolerance);
}

//------------------------------------------------------------------------
//	float-specific absolute value
//------------------------------------------------------------------------
inline float
FAbs(float i_Val)
{
  if (i_Val >= 0)
    return i_Val;
  else
    return -i_Val;
}

//------------------------------------------------------------------------
//	Returns i_Val rounded up to the nearest multiple of i_Spacing, WHICH
//	MUST BE A POWER OF 2.
//------------------------------------------------------------------------
inline int
RoundUp(int i_Val, int i_Spacing)
{
  DBG_ASSERT((i_Spacing & (i_Spacing - 1)) == 0, "i_Spacing must be a power of 2");
  return (i_Val + (i_Spacing - 1)) & (~(i_Spacing - 1));
}

//----------------------------------------------------------------------------
//  GetRelativeYawPitch returns the yaw and pitch of the direction vectors
//	world orientation
//----------------------------------------------------------------------------
void
GetYawPitch(const maVector3d& i_DirectionVec, float& o_Yaw, float& o_Pitch);

//----------------------------------------------------------------------------
//  GetRelativeYawPitch returns the yaw and pitch of the direction vector
//	relative to the orientation that is passed in.
//----------------------------------------------------------------------------
void
GetRelativeYawPitch(const maRotation& i_Rotation, const maVector3d& i_DirectionVec, float& o_Yaw, float& o_Pitch);

//----------------------------------------------------------------------------
//  GetRelativeDirection returns a new direction vector
//	relative to the orientation that is passed in.
//----------------------------------------------------------------------------
void
GetRelativeDirection(const maRotation& i_Rotation, const maVector3d& i_DirectionVec, maVector3d& o_DirectionVec);

//----------------------------------------------------------------------------
//	LinesIntersect() returns:
//	0 - no intersection
//	1 - intersection
//	2 - Collinear
//
//	Note: adapted from Graphics Gems 2
//----------------------------------------------------------------------------
int
LinesIntersect(maPoint2d& i_Pt1, maPoint2d& i_Pt2, maPoint2d& i_Pt3, maPoint2d& i_Pt4, maPoint2d& o_Pt);

//-----------------------------------------------------------------------------
// GaussianDistribution() - compute the 2 parameter Gaussian distrubution
//	using the given standard deviation rho
//-----------------------------------------------------------------------------
float
GaussianDistribution(float i_X, float i_Y, float i_Rho);

//-----------------------------------------------------------------------------
// Log() - compute the logarithm of a number in any base
//-----------------------------------------------------------------------------
float
Log(float i_Val, float i_Base);

//-----------------------------------------------------------------------------
// convert half to float
//-----------------------------------------------------------------------------
envType::UInt16
FloatToHalf(float i);

//-----------------------------------------------------------------------------
// convert half to float
//-----------------------------------------------------------------------------
float
HalfToFloat(envType::UInt16 y);

//-----------------------------------------------------------------------------
// convert half to float
//-----------------------------------------------------------------------------
void
HSV_to_RGB(float i_Hsv_H, float i_Hsv_S, float i_Hsv_V, float& o_Rgb_R, float& o_Rgb_G, float& o_Rgb_B);

//-----------------------------------------------------------------------------
// linear interpolation. by default, use a 0..1 domain. this matches HLSL lerp.
//-----------------------------------------------------------------------------
float
Interpolate(float y1, float y2, float x, float x1 = 0, float x2 = 1);
}
