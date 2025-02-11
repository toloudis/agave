/****************************************************************************\
**  maIntRect.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maIntRect.hpp"

//------------------------------------------------------------------------
//	default constructors
//------------------------------------------------------------------------
maIntRect::maIntRect() {}

//------------------------------------------------------------------------
//	default constructors
//------------------------------------------------------------------------
maIntRect::maIntRect(const maIntPoint2d& i_UpperLeft, const maIntPoint2d& i_LowerRight)
  : m_UpperLeft(i_UpperLeft)
  , m_LowerRight(i_LowerRight)
{
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
maIntRect::maIntRect(const maIntRect& i_CopyFrom)
{
  *this = i_CopyFrom;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
maIntRect::~maIntRect() {}

//------------------------------------------------------------------------
//	DoesIntersect()
//
//		Returns true if the two rectangles intersect.
//------------------------------------------------------------------------
bool
maIntRect::DoesIntersect(const maIntRect& i_Rect)
{
  if ((i_Rect.GetLeft() <= GetRight()) && (i_Rect.GetUpper() <= GetLower()) && (i_Rect.GetRight() >= GetLeft()) &&
      (i_Rect.GetLower() >= GetUpper())) {
    return true;
  }

  return false;
}

//------------------------------------------------------------------------
//	IsInside()
//
//		Returns true if the point is inside the rectangle.
//------------------------------------------------------------------------
bool
maIntRect::IsInside(const maIntPoint2d& i_Point)
{
  if ((i_Point.GetX() <= GetRight()) && (i_Point.GetY() <= GetLower()) && (i_Point.GetX() >= GetLeft()) &&
      (i_Point.GetY() >= GetUpper())) {
    return true;
  }

  return false;
}
