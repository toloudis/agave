/****************************************************************************\
**  maAxisBox.hpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#pragma once
#ifdef MA_AXISBOX_HPP
#error maAxisBox.hpp multiply included
#endif
#define MA_AXISBOX_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#ifndef MA_POINT3D_HPP
#include "Core/ma/maPoint3d.hpp"
#endif

#ifndef MA_POINT4D_HPP
#include "Core/ma/maPoint4d.hpp"
#endif

#include <iomanip>

//============================================================================
//============================================================================
class maPlane;

//============================================================================
//============================================================================
class maAxisBox
{
public:
  //------------------------------------------------------------------------
  //	The default constructor creates a box with all of it's coordinates
  //	at huge values, so that the first points unioned will set the limits.
  //------------------------------------------------------------------------
  maAxisBox();

  //------------------------------------------------------------------------
  //	Construct this class with another axis box.
  //------------------------------------------------------------------------
  maAxisBox(const maAxisBox& i_AxisBox);

  //------------------------------------------------------------------------
  //	This constructor makes a box which is the smallest box that
  //	can contain the two points given.
  //------------------------------------------------------------------------
  maAxisBox(const maPoint3d& i_P1, const maPoint3d& i_P2);

  //------------------------------------------------------------------------
  //	This constructor constructs the box from bounds for each axis.
  //------------------------------------------------------------------------
  maAxisBox(float i_MinX, float i_MaxX, float i_MinY, float i_MaxY, float i_MinZ, float i_MaxZ);

  //------------------------------------------------------------------------
  // Returns true if the axis box is uninitialized.
  // Note this is not a test of if the radius==0
  //------------------------------------------------------------------------
  bool IsEmpty() const;

  // test if box has volume at all
  bool HasVolume() const { return !IsEmpty() && (GetVolume() > 0); }

  float GetVolume() const { return (m_MaxX - m_MinX) * (m_MaxY - m_MinY) * (m_MaxZ - m_MinZ); }
  maVector3d GetSize() const { return maVector3d((m_MaxX - m_MinX), (m_MaxY - m_MinY), (m_MaxZ - m_MinZ)); }
  //------------------------------------------------------------------------
  //	GetBoxPoint returns a point of the box.  The order of the points
  //	returned is:
  //
  //		X	Y	Z
  //	0:	+	+	+
  //	1:	-	+	+
  //	2:	+	-	+
  //	3:	-	-	+
  //	4:	+	+	-
  //	5:	-	+	-
  //	6:	+	-	-
  //	7:	-	-	-
  //
  //	This function also provides a convenient way to get the min and max
  //	bounds of the box (by getting points 0 and 7).
  //------------------------------------------------------------------------
  maPoint3d GetBoxPoint(int i_Num) const;

  //------------------------------------------------------------------------
  //	GetBoxPoints returns all eight possible points of the bounding box.
  //	The pointer passed in must have space allocated to hold eight
  //	maVector3ds.
  //------------------------------------------------------------------------
  void GetBoxPoints(maVector3d* o_Points) const;
  void GetBoxPointsLH(maVector3d* o_Points) const;
  void GetBoxPointsRH(maVector3d* o_Points) const;

  //------------------------------------------------------------------------
  //	GetBoxPoints returns all eight possible points of the bounding box.
  //	The pointer passed in must have space allocated to hold eight
  //	maVector4ds.
  //------------------------------------------------------------------------
  void GetBoxPoints(maVector4d* o_Points) const;

  //------------------------------------------------------------------------
  //	Calculates the 6 planes of the given axis aligned box
  //	o_Planes must have space for 6 planes.
  //------------------------------------------------------------------------
  void GetPlanes(maPlane* o_Planes) const;

  //------------------------------------------------------------------------
  //	These get functions return the information about the bounds of
  //	the box.
  //------------------------------------------------------------------------
  inline float GetMaxX() const;
  inline float GetMinX() const;
  inline float GetMaxY() const;
  inline float GetMinY() const;
  inline float GetMaxZ() const;
  inline float GetMinZ() const;
  inline maVector3d GetMin() const { return maVector3d(GetMinX(), GetMinY(), GetMinZ()); }
  inline maVector3d GetMax() const { return maVector3d(GetMaxX(), GetMaxY(), GetMaxZ()); }

  //------------------------------------------------------------------------
  //	These set the various extrema of the box
  //------------------------------------------------------------------------
  inline void SetMinX(float);
  inline void SetMaxX(float);
  inline void SetMinY(float);
  inline void SetMaxY(float);
  inline void SetMinZ(float);
  inline void SetMaxZ(float);

  //------------------------------------------------------------------------
  //	GetDiff*() - returns the length of each box axis.
  //------------------------------------------------------------------------
  inline float GetDiffX() const;
  inline float GetDiffY() const;
  inline float GetDiffZ() const;

  //------------------------------------------------------------------------
  //	ContainsPoint returns true if the given point is inside the box.
  //	The box is considered to be defined "inclusively" - if a point is on
  //	the edge, it is considered inside.
  //------------------------------------------------------------------------
  bool ContainsPoint(const maPoint3d& i_Point) const;

  //------------------------------------------------------------------------
  //	GetCenter returns the center of the bounding box.
  //------------------------------------------------------------------------
  inline maPoint3d GetCenter() const;

  //------------------------------------------------------------------------
  //	GetRadius()
  //
  //	Returns the half distance between the min and max point.
  //------------------------------------------------------------------------
  inline float GetRadius() const;

  //------------------------------------------------------------------------
  //	Set makes the box into the smallest box that can contain the two
  //	points given.  This behavior is identical to the constructor with
  //	the same arguments.
  //------------------------------------------------------------------------
  void Set(const maPoint3d& i_P1, const maPoint3d& i_P2);

  //------------------------------------------------------------------------
  //	this Union expands the box to contain the given point (if necessary).
  //------------------------------------------------------------------------
  void Union(const maPoint3d& i_Point);

  //------------------------------------------------------------------------
  //	This Union expands the box to contain all of the given points
  //------------------------------------------------------------------------
  void Union(const maPoint3d* i_Points, int i_Num);

  //------------------------------------------------------------------------
  //	This Union expands the box to contain the given box (if necessary).
  //------------------------------------------------------------------------
  void Union(const maAxisBox& i_Box);

  //------------------------------------------------------------------------
  //	Translate moves the entire box by the given vector
  //------------------------------------------------------------------------
  void Translate(const maPoint3d& i_Point);

  //------------------------------------------------------------------------
  //	Overlaps returns true if the intersection of the points shared by
  //	each box is not empty.
  //------------------------------------------------------------------------
  bool Overlaps(const maAxisBox& i_Box) const;

  //------------------------------------------------------------------------
  // return length of diagonal line within bbox
  //------------------------------------------------------------------------
  inline float GetDiagonalLength() const;

  //------------------------------------------------------------------------
  // Utility functions
  //------------------------------------------------------------------------
  inline float minfp(float a, float b) const;
  inline float maxfp(float a, float b) const;

  //------------------------------------------------------------------------
  // intersect ray with bounding box
  //------------------------------------------------------------------------
  inline bool IntersectRay_Branchless(maPoint3d o, maPoint3d d, float* t_near, float* t_far) const;
  inline bool IntersectRay_Suffern(maPoint3d o, maPoint3d d, float* t_near, float* t_far) const;
  inline bool IntersectRay_Pharr(maPoint3d o, maPoint3d d, float* t_near, float* t_far) const;

  //------------------------------------------------------------------------
  //	Overlaps returns true if the intersection of the points shared by
  //	each box is not empty.
  //------------------------------------------------------------------------
  bool operator==(const maAxisBox& i_Box) const
  {
    return (m_MinX == i_Box.m_MinX && m_MinY == i_Box.m_MinY && m_MinZ == i_Box.m_MinZ && m_MaxX == i_Box.m_MaxX &&
            m_MaxY == i_Box.m_MaxY && m_MaxZ == i_Box.m_MaxZ);
  }

  //------------------------------------------------------------------------
  //	Swell causes each of the edges of the box to be moved outwards by
  //	i_Radius amount.
  //------------------------------------------------------------------------
  inline void Swell(float i_Radius);
  inline void Pad(const maVector3d& iRadius);

  //------------------------------------------------------------------------
  // return whether there is intersection or not, and calculate the intersection region
  //------------------------------------------------------------------------
  int GetIntersection(const maAxisBox& i_otherBox, maAxisBox& o_intersected);

private:
  float m_MinX, m_MaxX;
  float m_MinY, m_MaxY;
  float m_MinZ, m_MaxZ;
};

//------------------------------------------------------------------------
//	Union expands the box to contain the given point (if necessary).
//------------------------------------------------------------------------
inline void
maAxisBox::Union(const maPoint3d& i_Point)
{
  //	don't try to optimize this by putting else's in here
  //	it will cause a failure of the case when the box is reversed (NULL)
  if (i_Point.m_X > m_MaxX)
    m_MaxX = i_Point.m_X;

  if (i_Point.m_X < m_MinX)
    m_MinX = i_Point.m_X;

  if (i_Point.m_Y > m_MaxY)
    m_MaxY = i_Point.m_Y;

  if (i_Point.m_Y < m_MinY)
    m_MinY = i_Point.m_Y;

  if (i_Point.m_Z > m_MaxZ)
    m_MaxZ = i_Point.m_Z;

  if (i_Point.m_Z < m_MinZ)
    m_MinZ = i_Point.m_Z;
}

inline float
maAxisBox::GetMaxX() const
{
  return m_MaxX;
}

inline float
maAxisBox::GetMinX() const
{
  return m_MinX;
}

inline float
maAxisBox::GetMaxY() const
{
  return m_MaxY;
}

inline float
maAxisBox::GetMinY() const
{
  return m_MinY;
}

inline float
maAxisBox::GetMaxZ() const
{
  return m_MaxZ;
}

inline float
maAxisBox::GetMinZ() const
{
  return m_MinZ;
}

inline void
maAxisBox::SetMinX(float i_Val)
{
  DBG_ASSERT(i_Val < m_MaxX,
             "Trying to set MinX (" << std::setw(6) << std::setprecision(3) << i_Val << ") > MaxX (" << m_MaxX << ")");
  m_MinX = i_Val;
}

inline void
maAxisBox::SetMinY(float i_Val)
{
  DBG_ASSERT(i_Val < m_MaxY,
             "Trying to set MinY (" << std::setw(6) << std::setprecision(3) << i_Val << ") > MaxY (" << m_MaxY << ")");
  m_MinY = i_Val;
}

inline void
maAxisBox::SetMinZ(float i_Val)
{
  DBG_ASSERT(i_Val < m_MaxZ,
             "Trying to set MinZ (" << std::setw(6) << std::setprecision(3) << i_Val << ") > MaxZ (" << m_MaxZ << ")");
  m_MinZ = i_Val;
}

inline void
maAxisBox::SetMaxX(float i_Val)
{
  DBG_ASSERT(i_Val > m_MinX,
             "Trying to set MaxX (" << std::setw(6) << std::setprecision(3) << i_Val << ") < MinX (" << m_MinX << ")");
  m_MaxX = i_Val;
}

inline void
maAxisBox::SetMaxY(float i_Val)
{
  DBG_ASSERT(i_Val > m_MinY,
             "Trying to set MaxY (" << std::setw(6) << std::setprecision(3) << i_Val << ") < MinY (" << m_MinY << ")");
  m_MaxY = i_Val;
}

inline void
maAxisBox::SetMaxZ(float i_Val)
{
  DBG_ASSERT(i_Val > m_MinZ,
             "Trying to set MaxZ (" << std::setw(6) << std::setprecision(3) << i_Val << ") < MinZ (" << m_MinZ << ")");
  m_MaxZ = i_Val;
}

//------------------------------------------------------------------------
//	GetDiff*() - returns the length of each box axis.
//------------------------------------------------------------------------
inline float
maAxisBox::GetDiffX() const
{
  return (m_MaxX - m_MinX);
}
inline float
maAxisBox::GetDiffY() const
{
  return (m_MaxY - m_MinY);
}
inline float
maAxisBox::GetDiffZ() const
{
  return (m_MaxZ - m_MinZ);
}

//------------------------------------------------------------------------
//	ContainsPoint returns true if the given point is inside the box.
//	The box is considered to be defined "inclusively" - if a point is on
//	the edge, it is considered inside.
//------------------------------------------------------------------------
inline bool
maAxisBox::ContainsPoint(const maPoint3d& i_Point) const
{
  return ((i_Point.m_X <= m_MaxX) && (i_Point.m_X >= m_MinX)) && ((i_Point.m_Y <= m_MaxY) && (i_Point.m_Y >= m_MinY)) &&
         ((i_Point.m_Z <= m_MaxZ) && (i_Point.m_Z >= m_MinZ));
}

//------------------------------------------------------------------------
//	GetCenter returns the center of the bounding box.
//------------------------------------------------------------------------
inline maPoint3d
maAxisBox::GetCenter() const
{
  return maPoint3d((m_MaxX + m_MinX) * 0.5f, (m_MaxY + m_MinY) * 0.5f, (m_MaxZ + m_MinZ) * 0.5f);
}

//------------------------------------------------------------------------
//	GetRadius()
//
//	Returns the half distance between the min and max point.
//------------------------------------------------------------------------
inline float
maAxisBox::GetRadius() const
{
  return 0.5f * (float)sqrt((m_MaxX - m_MinX) * (m_MaxX - m_MinX) + (m_MaxY - m_MinY) * (m_MaxY - m_MinY) +
                            (m_MaxZ - m_MinZ) * (m_MaxZ - m_MinZ));
}

//------------------------------------------------------------------------
//	This Union expands the box to contain the given box (if necessary).
//------------------------------------------------------------------------
inline void
maAxisBox::Union(const maAxisBox& i_Box)
{
  if (i_Box.GetMaxX() > m_MaxX)
    m_MaxX = i_Box.GetMaxX();

  if (i_Box.GetMinX() < m_MinX)
    m_MinX = i_Box.GetMinX();

  if (i_Box.GetMaxY() > m_MaxY)
    m_MaxY = i_Box.GetMaxY();

  if (i_Box.GetMinY() < m_MinY)
    m_MinY = i_Box.GetMinY();

  if (i_Box.GetMaxZ() > m_MaxZ)
    m_MaxZ = i_Box.GetMaxZ();

  if (i_Box.GetMinZ() < m_MinZ)
    m_MinZ = i_Box.GetMinZ();
}

//------------------------------------------------------------------------
//	Swell causes each of the edges of the box to be moved outwards by
//	i_Radius amount.
//------------------------------------------------------------------------
inline void
maAxisBox::Swell(float i_Radius)
{
  m_MaxX += i_Radius;
  m_MaxY += i_Radius;
  m_MaxZ += i_Radius;
  m_MinX -= i_Radius;
  m_MinY -= i_Radius;
  m_MinZ -= i_Radius;
}
inline void
maAxisBox::Pad(const maVector3d& iRadius)
{
  m_MaxX += iRadius[0];
  m_MaxY += iRadius[1];
  m_MaxZ += iRadius[2];
  m_MinX -= iRadius[0];
  m_MinY -= iRadius[1];
  m_MinZ -= iRadius[2];
}

//------------------------------------------------------------------------
// return length of diagonal line within bbox
//------------------------------------------------------------------------
inline float
maAxisBox::GetDiagonalLength() const
{
  return (maPoint3d(m_MaxX, m_MaxY, m_MaxZ) - maPoint3d(m_MinX, m_MinY, m_MinZ)).Length();
}

//------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------
inline float
maAxisBox::minfp(float a, float b) const
{
  return a < b ? a : b;
}
inline float
maAxisBox::maxfp(float a, float b) const
{
  return a > b ? a : b;
}

//------------------------------------------------------------------------
// intersect ray with bounding box - branchless
//------------------------------------------------------------------------
inline bool
maAxisBox::IntersectRay_Branchless(maPoint3d o, maPoint3d d, float* t_near, float* t_far) const
{
  float ray_d_x = 1 / d.m_X;
  float ray_d_y = 1 / d.m_Y;
  float ray_d_z = 1 / d.m_Z;

  float ray_o_x = 1 / o.m_X;
  float ray_o_y = 1 / o.m_Y;
  float ray_o_z = 1 / o.m_Z;

  float l1 = (m_MinX - ray_o_x) * ray_d_x;
  float l2 = (m_MaxX - ray_o_x) * ray_d_x;
  float lmin = minfp(l1, l2);
  float lmax = maxfp(l1, l2);

  l1 = (m_MinY - ray_o_y) * ray_d_y;
  l2 = (m_MaxY - ray_o_y) * ray_d_y;
  lmin = maxfp(minfp(l1, l2), lmin);
  lmax = minfp(maxfp(l1, l2), lmax);

  l1 = (m_MinZ - ray_o_z) * ray_d_z;
  l2 = (m_MaxZ - ray_o_z) * ray_d_z;
  lmin = maxfp(minfp(l1, l2), lmin);
  lmax = minfp(maxfp(l1, l2), lmax);

  *t_near = lmin;
  *t_far = lmax;

  return ((lmax >= 0.f) & (lmax >= lmin));
}

//------------------------------------------------------------------------
// intersect ray with bounding box - kevin suffern
//------------------------------------------------------------------------
inline bool
maAxisBox::IntersectRay_Suffern(maPoint3d o, maPoint3d d, float* t_near, float* t_far) const
{
  float ox = o.m_X;
  float oy = o.m_Y;
  float oz = o.m_Z;

  float dx = d.m_X;
  float dy = d.m_Y;
  float dz = d.m_Z;

  float tx_min;
  float ty_min;
  float tz_min;

  float tx_max;
  float ty_max;
  float tz_max;

  float a = 1.0f / dx;
  if (a >= 0.0f) {
    tx_min = (m_MinX - ox) * a;
    tx_max = (m_MaxX - ox) * a;
  } else {
    tx_min = (m_MaxX - ox) * a;
    tx_max = (m_MinX - ox) * a;
  }

  float b = 1.0f / dy;
  if (b >= 0.0f) {
    ty_min = (m_MinY - oy) * b;
    ty_max = (m_MaxY - oy) * b;
  } else {
    ty_min = (m_MaxY - oy) * b;
    ty_max = (m_MinY - oy) * b;
  }

  float c = 1.0f / dz;
  if (c >= 0.0f) {
    tz_min = (m_MinZ - oz) * c;
    tz_max = (m_MaxZ - oz) * c;
  } else {
    tz_min = (m_MaxZ - oz) * c;
    tz_max = (m_MinZ - oz) * c;
  }

  float t0;
  float t1;

  // find largest entering t value
  if (tx_min > ty_min) {
    t0 = tx_min;
  } else {
    t0 = ty_min;
  }
  if (tz_min > t0) {
    t0 = tz_min;
  }

  // find smallest exiting t value
  if (tx_max < ty_max) {
    t1 = tx_max;
  } else {
    t1 = ty_max;
  }
  if (tz_max < t1) {
    t1 = tz_max;
  }

  *t_near = t0;
  *t_far = t1;

  if (t0 < t1 && t1 > 0.0001f) {

    float t;
    if (t0 > 0.0001f) {
      t = t0;
    } else {
      t = t1;
    }
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------
// intersect ray with bounding box - pbrt
//------------------------------------------------------------------------
inline bool
maAxisBox::IntersectRay_Pharr(maPoint3d o, maPoint3d d, float* t_near, float* t_far) const
{
  float t0 = 0.0001f, t1 = 9999.0f;
  float temp;

  // Update interval for _i_th bounding box slab
  float invRayDir = 1.f / d.m_X;
  float tNear = (m_MinX - o.m_X) * invRayDir;
  float tFar = (m_MaxX - o.m_X) * invRayDir;
  // Update parametric interval from slab intersection $t$s
  if (tNear > tFar) {
    temp = tFar;
    tFar = tNear;
    tNear = temp;
  }
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  if (t0 > t1)
    return false;

  // Update interval for _i_th bounding box slab
  invRayDir = 1.f / d.m_Y;
  tNear = (m_MinY - o.m_Y) * invRayDir;
  tFar = (m_MaxY - o.m_Y) * invRayDir;
  // Update parametric interval from slab intersection $t$s
  if (tNear > tFar) {
    temp = tFar;
    tFar = tNear;
    tNear = temp;
  }
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  if (t0 > t1)
    return false;

  // Update interval for _i_th bounding box slab
  invRayDir = 1.f / d.m_Z;
  tNear = (m_MinZ - o.m_Z) * invRayDir;
  tFar = (m_MaxZ - o.m_Z) * invRayDir;
  // Update parametric interval from slab intersection $t$s
  if (tNear > tFar) {
    temp = tFar;
    tFar = tNear;
    tNear = temp;
  }
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  if (t0 > t1)
    return false;

  *t_near = tNear;
  *t_far = tFar;

  return true;
}