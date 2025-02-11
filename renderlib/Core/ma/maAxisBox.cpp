/****************************************************************************\
**  maAxisBox.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maAxisBox.hpp"

#include "Core/ma/maConstants.hpp"
#include "Core/ma/maFunctions.hpp"
#include "Core/ma/maPlane.hpp"

//------------------------------------------------------------------------
//	The default constructor creates a box with all of it's coordinates
//	at huge values, so that the first points unioned will set the limits.
//------------------------------------------------------------------------
maAxisBox::maAxisBox()
  : m_MinX(maConstants::c_fLargest)
  , m_MaxX(-maConstants::c_fLargest)
  , m_MinY(maConstants::c_fLargest)
  , m_MaxY(-maConstants::c_fLargest)
  , m_MinZ(maConstants::c_fLargest)
  , m_MaxZ(-maConstants::c_fLargest)
{
}

//------------------------------------------------------------------------
//	Construct this class with another axis box.
//------------------------------------------------------------------------
maAxisBox::maAxisBox(const maAxisBox& i_AxisBox)
  : m_MinX(i_AxisBox.GetMinX())
  , m_MaxX(i_AxisBox.GetMaxX())
  , m_MinY(i_AxisBox.GetMinY())
  , m_MaxY(i_AxisBox.GetMaxY())
  , m_MinZ(i_AxisBox.GetMinZ())
  , m_MaxZ(i_AxisBox.GetMaxZ())
{
}

//------------------------------------------------------------------------
// Returns true if the axis box is uninitialized.
// Note this is not a test of if the radius==0
//------------------------------------------------------------------------
bool
maAxisBox::IsEmpty() const
{
  return ((m_MinX > m_MaxX) || (m_MinY > m_MaxY) || (m_MinZ > m_MaxZ));
}

//------------------------------------------------------------------------
//	This constructor makes a box which is the smallest box that
//	can contain the two points given.
//------------------------------------------------------------------------
maAxisBox::maAxisBox(const maPoint3d& i_P1, const maPoint3d& i_P2)
{
  if (i_P1.m_X > i_P2.m_X) {
    m_MaxX = i_P1.m_X;
    m_MinX = i_P2.m_X;
  } else {
    m_MaxX = i_P2.m_X;
    m_MinX = i_P1.m_X;
  }

  if (i_P1.m_Y > i_P2.m_Y) {
    m_MaxY = i_P1.m_Y;
    m_MinY = i_P2.m_Y;
  } else {
    m_MaxY = i_P2.m_Y;
    m_MinY = i_P1.m_Y;
  }

  if (i_P1.m_Z > i_P2.m_Z) {
    m_MaxZ = i_P1.m_Z;
    m_MinZ = i_P2.m_Z;
  } else {
    m_MaxZ = i_P2.m_Z;
    m_MinZ = i_P1.m_Z;
  }
}

//------------------------------------------------------------------------
//	This constructor constructs the box from bounds for each axis.
//------------------------------------------------------------------------
maAxisBox::maAxisBox(float i_MinX, float i_MaxX, float i_MinY, float i_MaxY, float i_MinZ, float i_MaxZ)
  : m_MaxX(i_MaxX)
  , m_MinX(i_MinX)
  , m_MaxY(i_MaxY)
  , m_MinY(i_MinY)
  , m_MaxZ(i_MaxZ)
  , m_MinZ(i_MinZ)
{
}

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
maPoint3d
maAxisBox::GetBoxPoint(int i_Num) const
{
  DBG_ASSERT((i_Num >= 0) && (i_Num < 8), "Invalid box point number - " << i_Num);

  switch (i_Num) {
    case 0: //	+	+	+
      return maPoint3d(m_MaxX, m_MaxY, m_MaxZ);
      break;

    case 1: //	-	+	+
      return maPoint3d(m_MinX, m_MaxY, m_MaxZ);
      break;

    case 2: //	+	-	+
      return maPoint3d(m_MaxX, m_MinY, m_MaxZ);
      break;

    case 3: //	-	-	+
      return maPoint3d(m_MinX, m_MinY, m_MaxZ);
      break;

    case 4: //	+	+	-
      return maPoint3d(m_MaxX, m_MaxY, m_MinZ);
      break;

    case 5: //	-	+	-
      return maPoint3d(m_MinX, m_MaxY, m_MinZ);
      break;

    case 6: //	+	-	-
      return maPoint3d(m_MaxX, m_MinY, m_MinZ);
      break;

    case 7: //	-	-	-
      return maPoint3d(m_MinX, m_MinY, m_MinZ);
      break;
  }

  return maPoint3d(0, 0, 0);
}

//------------------------------------------------------------------------
//	GetBoxPoints returns all eight possible points of the bounding box.
//	The pointer passed in must have space allocated to hold eight
//	maVector3ds.
//
//	v[7] = (m[0],m[1],m[2]); //     1+------+0
//	v[6] = (M[0],m[1],m[2]); //     /|     /|
//	v[4] = (M[0],M[1],m[2]); //    / |    / |
//	v[5] = (m[0],M[1],m[2]); //   / 3+---/--+2
//	v[3] = (m[0],m[1],M[2]); // 5+------+4 /    y   z
//	v[2] = (M[0],m[1],M[2]); //  | /    | /     |  /
//	v[0] = (M[0],M[1],M[2]); //  |/     |/      |/
//	v[1] = (m[0],M[1],M[2]); // 7+------+6      *---x
//------------------------------------------------------------------------
void
maAxisBox::GetBoxPoints(maVector3d* o_Points) const
{
  //	+++
  o_Points[0].Set(m_MaxX, m_MaxY, m_MaxZ);
  //	-++
  o_Points[1].Set(m_MinX, m_MaxY, m_MaxZ);
  //	+-+
  o_Points[2].Set(m_MaxX, m_MinY, m_MaxZ);
  //	--+
  o_Points[3].Set(m_MinX, m_MinY, m_MaxZ);
  //	++-
  o_Points[4].Set(m_MaxX, m_MaxY, m_MinZ);
  //	-+-
  o_Points[5].Set(m_MinX, m_MaxY, m_MinZ);
  //	+--
  o_Points[6].Set(m_MaxX, m_MinY, m_MinZ);
  //	---
  o_Points[7].Set(m_MinX, m_MinY, m_MinZ);
}
/*left handed (positive z direction)
        const V3& m = getMin();
        const V3& M = getMax();
        //generate 8 corners of the bbox
        v[0] = V3(m[0],m[1],m[2]); //     7+------+6
        v[1] = V3(M[0],m[1],m[2]); //     /|     /|
        v[2] = V3(M[0],M[1],m[2]); //    / |    / |
        v[3] = V3(m[0],M[1],m[2]); //   / 4+---/--+5
        v[4] = V3(m[0],m[1],M[2]); // 3+------+2 /    y   z
        v[5] = V3(M[0],m[1],M[2]); //  | /    | /     |  /
        v[6] = V3(M[0],M[1],M[2]); //  |/     |/      |/
        v[7] = V3(m[0],M[1],M[2]); // 0+------+1      *---x
*/
void
maAxisBox::GetBoxPointsLH(maVector3d* o_Points) const
{
  //	---
  o_Points[0].Set(m_MinX, m_MinY, m_MinZ);
  //	+--
  o_Points[1].Set(m_MaxX, m_MinY, m_MinZ);
  //	++-
  o_Points[2].Set(m_MaxX, m_MaxY, m_MinZ);
  //	-+-
  o_Points[3].Set(m_MinX, m_MaxY, m_MinZ);
  //	--+
  o_Points[4].Set(m_MinX, m_MinY, m_MaxZ);
  //	+-+
  o_Points[5].Set(m_MaxX, m_MinY, m_MaxZ);
  //	+++
  o_Points[6].Set(m_MaxX, m_MaxY, m_MaxZ);
  //	-++
  o_Points[7].Set(m_MinX, m_MaxY, m_MaxZ);
}
/*
// right handed (negative z direction)
        //generate 8 corners of the bbox
        v[0] = V3(m[0],m[1],M[2]); //     7+------+6
        v[1] = V3(M[0],m[1],M[2]); //     /|     /|
        v[2] = V3(M[0],M[1],M[2]); //    / |    / |
        v[3] = V3(m[0],M[1],M[2]); //   / 4+---/--+5
        v[4] = V3(m[0],m[1],m[2]); // 3+------+2 /    y  -z
        v[5] = V3(M[0],m[1],m[2]); //  | /    | /     |  /
        v[6] = V3(M[0],M[1],m[2]); //  |/     |/      |/
        v[7] = V3(m[0],M[1],m[2]); // 0+------+1      *---x
*/
void
maAxisBox::GetBoxPointsRH(maVector3d* o_Points) const
{
  //	--+
  o_Points[0].Set(m_MinX, m_MinY, m_MaxZ);
  //	+-+
  o_Points[1].Set(m_MaxX, m_MinY, m_MaxZ);
  //	+++
  o_Points[2].Set(m_MaxX, m_MaxY, m_MaxZ);
  //	-++
  o_Points[3].Set(m_MinX, m_MaxY, m_MaxZ);
  //	---
  o_Points[4].Set(m_MinX, m_MinY, m_MinZ);
  //	+--
  o_Points[5].Set(m_MaxX, m_MinY, m_MinZ);
  //	++-
  o_Points[6].Set(m_MaxX, m_MaxY, m_MinZ);
  //	-+-
  o_Points[7].Set(m_MinX, m_MaxY, m_MinZ);
}

//------------------------------------------------------------------------
//	GetBoxPoints returns all eight possible points of the bounding box.
//	The pointer passed in must have space allocated to hold eight
//	maVector4ds.
//------------------------------------------------------------------------
void
maAxisBox::GetBoxPoints(maVector4d* o_Points) const
{
  //	+++
  o_Points[0].Set(m_MaxX, m_MaxY, m_MaxZ, 1);
  //	-++
  o_Points[1].Set(m_MinX, m_MaxY, m_MaxZ, 1);
  //	+-+
  o_Points[2].Set(m_MaxX, m_MinY, m_MaxZ, 1);
  //	--+
  o_Points[3].Set(m_MinX, m_MinY, m_MaxZ, 1);
  //	++-
  o_Points[4].Set(m_MaxX, m_MaxY, m_MinZ, 1);
  //	-+-
  o_Points[5].Set(m_MinX, m_MaxY, m_MinZ, 1);
  //	+--
  o_Points[6].Set(m_MaxX, m_MinY, m_MinZ, 1);
  //	---
  o_Points[7].Set(m_MinX, m_MinY, m_MinZ, 1);
}

//------------------------------------------------------------------------
//	Calculates the 6 planes of the given axis aligned box
//	o_Planes must have space for 6 planes.
//------------------------------------------------------------------------
void
maAxisBox::GetPlanes(maPlane* o_Planes) const
{
  // bottom plane
  o_Planes[0] = maPlane(0, -1, 0, m_MinY);
  // top plane
  o_Planes[1] = maPlane(0, 1, 0, m_MaxY);
  // left plane
  o_Planes[2] = maPlane(-1, 0, 0, m_MinX);
  // right plane
  o_Planes[3] = maPlane(1, 0, 0, m_MaxX);
  // back plane
  o_Planes[4] = maPlane(0, 0, -1, m_MinZ);
  // front plane
  o_Planes[5] = maPlane(0, 0, 1, m_MaxZ);
}

//------------------------------------------------------------------------
//	Set makes the box into the smallest box that can contain the two
//	points given.  This behavior is identical to the constructor with
//	the same arguments.
//------------------------------------------------------------------------
void
maAxisBox::Set(const maPoint3d& i_P1, const maPoint3d& i_P2)
{
  if (i_P1.m_X > i_P2.m_X) {
    m_MaxX = i_P1.m_X;
    m_MinX = i_P2.m_X;
  } else {
    m_MaxX = i_P2.m_X;
    m_MinX = i_P1.m_X;
  }

  if (i_P1.m_Y > i_P2.m_Y) {
    m_MaxY = i_P1.m_Y;
    m_MinY = i_P2.m_Y;
  } else {
    m_MaxY = i_P2.m_Y;
    m_MinY = i_P1.m_Y;
  }

  if (i_P1.m_Z > i_P2.m_Z) {
    m_MaxZ = i_P1.m_Z;
    m_MinZ = i_P2.m_Z;
  } else {
    m_MaxZ = i_P2.m_Z;
    m_MinZ = i_P1.m_Z;
  }
}

//------------------------------------------------------------------------
//	Translate moves the entire box by the given vector
//------------------------------------------------------------------------
void
maAxisBox::Translate(const maPoint3d& i_Point)
{
  m_MaxX += i_Point.m_X;
  m_MinX += i_Point.m_X;
  m_MaxY += i_Point.m_Y;
  m_MinY += i_Point.m_Y;
  m_MaxZ += i_Point.m_Z;
  m_MinZ += i_Point.m_Z;
}

//------------------------------------------------------------------------
//	This Union expands the box to contain all of the given points
//------------------------------------------------------------------------
void
maAxisBox::Union(const maPoint3d* i_Points, int i_Num)
{
  int i;
  for (i = 0; i < i_Num; ++i)
    this->Union(i_Points[i]);
}

//------------------------------------------------------------------------
//	Overlaps returns true if the intersection of the points shared by
//	each box is not empty.
//------------------------------------------------------------------------
bool
maAxisBox::Overlaps(const maAxisBox& i_Box) const
{
  if (m_MaxX < i_Box.m_MinX)
    return false;
  if (m_MinX > i_Box.m_MaxX)
    return false;
  if (m_MaxY < i_Box.m_MinY)
    return false;
  if (m_MinY > i_Box.m_MaxY)
    return false;
  if (m_MaxZ < i_Box.m_MinZ)
    return false;
  if (m_MinZ > i_Box.m_MaxZ)
    return false;
  return true;
}

//------------------------------------------------------------------------
// return whether there is intersection or not, and calculate the intersection region
//------------------------------------------------------------------------
int
maAxisBox::GetIntersection(const maAxisBox& i_otherBox, maAxisBox& o_intersected)
{
  if (!Overlaps(i_otherBox))
    return 0;

  o_intersected.Union(maPoint3d(maFunctions::Lowest(m_MaxX, i_otherBox.GetMaxX()),
                                maFunctions::Lowest(m_MaxY, i_otherBox.GetMaxY()),
                                maFunctions::Lowest(m_MaxZ, i_otherBox.GetMaxZ())));
  o_intersected.Union(maPoint3d(maFunctions::Highest(m_MinX, i_otherBox.GetMinX()),
                                maFunctions::Highest(m_MinY, i_otherBox.GetMinY()),
                                maFunctions::Highest(m_MinZ, i_otherBox.GetMinZ())));

  return 1;
}