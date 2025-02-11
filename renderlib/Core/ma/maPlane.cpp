/****************************************************************************\
**  maPlane.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maPlane.hpp"

//------------------------------------------------------------------------
//	The default constructor creates a plane facing towards positive Z
//	located at the origin.
//------------------------------------------------------------------------
maPlane::maPlane()
  : m_Normal(0, 0, 1)
  , m_Value(0)
{
}

//------------------------------------------------------------------------
//	This constructor uses values from the plane equation.
//------------------------------------------------------------------------
maPlane::maPlane(float i_X, float i_Y, float i_Z, float i_D)
  : m_Normal(i_X, i_Y, i_Z)
  , m_Value(i_D)
{
  DBG_ASSERT(m_Normal.LengthSqr() > 0, "Tried to make a plane with an non-normalized normal");
}

//------------------------------------------------------------------------
//	This constructor uses a direction and a point on
//	the plane.
//------------------------------------------------------------------------
maPlane::maPlane(const maPoint3d& i_Direction, const maPoint3d& i_Point)
  : m_Normal(i_Direction)
{
  m_Normal.Normalize();
  m_Value = i_Direction * i_Point;
}

//------------------------------------------------------------------------
//	This constructor makes the plane from any three points in space.
//	The direction of the plane is given by the cross product
//	(i_P2 - i_P1) X (i_P3 - i_P2)
//	If the points do not uniquely specify a plane an assertion will be
//	triggered in debug builds and the plane will be invalid.
//------------------------------------------------------------------------
maPlane::maPlane(const maPoint3d& i_P1, const maPoint3d& i_P2, const maPoint3d& i_P3)
{
  m_Normal = (i_P2 - i_P1).Cross(i_P3 - i_P2);
  DBG_ASSERT(m_Normal.LengthSqr() > 0, "Tried to make a plane from collinear points");
  m_Normal.Normalize();
  m_Value = m_Normal * i_P1;
}

//------------------------------------------------------------------------
//	This Set makes the plane conform to the plane equation values given.
//------------------------------------------------------------------------
void
maPlane::Set(float i_X, float i_Y, float i_Z, float i_D)
{
  m_Normal.Set(i_X, i_Y, i_Z);
  DBG_ASSERT(m_Normal.LengthSqr() > 0, "Tried to make a plane with an non-normalized normal");
  m_Value = i_D;
}

//------------------------------------------------------------------------
//	This set makes a plane which contains the three points given.
//	The direction of the plane is given by the cross product
//	(i_P2 - i_P1) X (i_P3 - i_P2).
//	If the points do not uniquely specify a plane an assertion will be
//	triggered in debug builds and the plane will be invalid.
//------------------------------------------------------------------------
void
maPlane::Set(const maPoint3d& i_P1, const maPoint3d& i_P2, const maPoint3d& i_P3)
{
  m_Normal = (i_P2 - i_P1).Cross(i_P3 - i_P2);
  DBG_ASSERT(m_Normal.LengthSqr() > 0, "Tried to make a plane from collinear points");
  m_Normal.Normalize();
  m_Value = m_Normal * i_P1;
}

//------------------------------------------------------------------------
//	SetNormal sets the plane's normal.
//------------------------------------------------------------------------
void
maPlane::SetNormal(const maPoint3d& i_Normal)
{
  // have to do this to handle weird floating point error cases where 1.0000f != 1.0f
  DBG_ASSERT(1.0000001f > i_Normal.LengthSqr() && 0.9999999f < i_Normal.LengthSqr(), "Used a non-normalized normal");
  m_Normal = i_Normal;
}

//------------------------------------------------------------------------
//	SetValue sets the plane's value.
//------------------------------------------------------------------------
void
maPlane::SetValue(float i_Value)
{
  m_Value = i_Value;
}
