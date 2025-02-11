/****************************************************************************\
**  maPlane.hpp
**
**      maPlane.hpp defines the class maPlane which represents an (infinite)
**	plane in 3-dimensional space.  The equation for the plane is
**
**	A*x + B*y + C*z = D
**
**	or equivalently
**
**	N . R = D    (. == dot product, N and R are vectors)
**
**
**
\****************************************************************************/

#ifdef MA_PLANE_HPP
#error maPlane.hpp multiply included
#endif
#define MA_PLANE_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#ifndef MA_POINT3D_HPP
#include "Core/ma/maPoint3d.hpp"
#endif

//============================================================================
//============================================================================
class maPlane
{
public:
  //------------------------------------------------------------------------
  //	The default constructor creates a plane facing towards positive Z
  //	located at the origin.
  //------------------------------------------------------------------------
  maPlane();

  //------------------------------------------------------------------------
  //	This constructor uses values from the plane equation.  An assertion
  //	will be triggered if (i_X, i_Y, i_Z) is not normalized.
  //------------------------------------------------------------------------
  maPlane(float i_X, float i_Y, float i_Z, float i_D);

  //------------------------------------------------------------------------
  //	This constructor uses a direction and a point on
  //	the plane.
  //------------------------------------------------------------------------
  maPlane(const maPoint3d& i_Direction, const maPoint3d& i_Point);

  //------------------------------------------------------------------------
  //	This constructor makes the plane from any three points in space.
  //	The direction of the plane is given by the cross product
  //	(i_P2 - i_P1) X (i_P3 - i_P2)
  //	If the points do not uniquely specify a plane an assertion will be
  //	triggered in debug builds and the plane will be invalid.
  //------------------------------------------------------------------------
  maPlane(const maPoint3d& i_P1, const maPoint3d& i_P2, const maPoint3d& i_P3);

  //------------------------------------------------------------------------
  //	GetNormal returns the normal direction of the plane.
  //------------------------------------------------------------------------
  const maPoint3d& GetNormal() const;

  //------------------------------------------------------------------------
  //	GetValue returns the value of the plane (the "D" in the equation
  //	above).
  //------------------------------------------------------------------------
  float GetValue() const;

  //------------------------------------------------------------------------
  //	TestPoint determines how far from a plane the given point is.  The
  //	return value is the closest distance from the plane to the point.
  //	It is positive if the point is on the side of the plane that the
  //	normal faces towards.
  //------------------------------------------------------------------------
  float TestPoint(const maPoint3d& i_Point) const;

  //------------------------------------------------------------------------
  //	This Set makes the plane conform to the plane equation values given.
  //	An assertion will be triggered if (i_X, i_Y, i_Z) is not normalized.
  //------------------------------------------------------------------------
  void Set(float i_X, float i_Y, float i_Z, float i_D);

  //------------------------------------------------------------------------
  //	This set makes a plane which contains the three points given.
  //	The direction of the plane is given by the cross product
  //	(i_P2 - i_P1) X (i_P3 - i_P2).
  //	If the points do not uniquely specify a plane an assertion will be
  //	triggered in debug builds and the plane will be invalid.
  //------------------------------------------------------------------------
  void Set(const maPoint3d& i_P1, const maPoint3d& i_P2, const maPoint3d& i_P3);

  //------------------------------------------------------------------------
  //	SetNormal sets the plane's normal.
  //------------------------------------------------------------------------
  void SetNormal(const maPoint3d& i_Normal);

  //------------------------------------------------------------------------
  //	SetValue sets the plane's value.
  //------------------------------------------------------------------------
  void SetValue(float i_Value);

private:
  maPoint3d m_Normal;
  float m_Value;
};

//------------------------------------------------------------------------
//	TestPoint determines how far from a plane the given point is.  The
//	return value is the closest distance from the plane to the point.
//	It is positive if the point is on the side of the plane that the
//	normal faces towards.
//------------------------------------------------------------------------
inline float
maPlane::TestPoint(const maPoint3d& i_Point) const
{
  float dot = i_Point * m_Normal;
  return dot - m_Value;
}

//------------------------------------------------------------------------
//	GetNormal returns the normal direction of the plane.
//------------------------------------------------------------------------
inline const maPoint3d&
maPlane::GetNormal() const
{
  return m_Normal;
}

//------------------------------------------------------------------------
//	GetValue returns the value of the plane (the "D" in the equation
//	above).
//------------------------------------------------------------------------
inline float
maPlane::GetValue() const
{
  return m_Value;
}