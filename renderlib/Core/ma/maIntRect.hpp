/****************************************************************************\
**  maIntRect.hpp
**
**      maIntRect.hpp defines the maIntRect class
**
**
**
\****************************************************************************/

#ifdef MA_INTRECT_HPP
#error maIntRect.hpp multiply included
#endif
#define MA_INTRECT_HPP

#ifndef MA_INTPOINT2D_HPP
#include "Core/ma/maIntPoint2d.hpp"
#endif

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

//============================================================================
//============================================================================
class maIntRect
{
public:
  //------------------------------------------------------------------------
  //	default and copy constructors
  //------------------------------------------------------------------------
  maIntRect();
  maIntRect(const maIntPoint2d& i_UpperLeft, const maIntPoint2d& i_LowerRight);
  maIntRect(const maIntRect& i_CopyFrom);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  ~maIntRect();

  //----------------------------------------------------------------------------
  //	maIntRect functions
  //----------------------------------------------------------------------------

  //------------------------------------------------------------------------
  //	DoesIntersect()
  //
  //		Returns true if the two rectangles intersect.
  //------------------------------------------------------------------------
  bool DoesIntersect(const maIntRect& i_Rect);

  //------------------------------------------------------------------------
  //	IsInside()
  //
  //		Returns true if the point is inside the rectangle.
  //------------------------------------------------------------------------
  bool IsInside(const maIntPoint2d& i_Point);

  //----------------------------------------------------------------------------
  //	Accessor functions
  //----------------------------------------------------------------------------

  //------------------------------------------------------------------------
  //	return the points
  //------------------------------------------------------------------------
  maIntPoint2d GetUpperLeft() const { return m_UpperLeft; }
  maIntPoint2d GetLowerRight() const { return m_LowerRight; }

  //------------------------------------------------------------------------
  //	return a points coordinate
  //------------------------------------------------------------------------
  int GetLeft() const { return m_UpperLeft.GetX(); }
  int GetRight() const { return m_LowerRight.GetX(); }
  int GetUpper() const { return m_UpperLeft.GetY(); }
  int GetLower() const { return m_LowerRight.GetY(); }

  //------------------------------------------------------------------------
  //	return the size
  //------------------------------------------------------------------------
  int GetWidth() const { return m_LowerRight.GetX() - m_UpperLeft.GetX(); }

  int GetHeight() const { return m_LowerRight.GetY() - m_UpperLeft.GetY(); }

  //------------------------------------------------------------------------
  //	Sets
  //------------------------------------------------------------------------
  void SetUpper(int i_Value) { m_UpperLeft.SetY(i_Value); }
  void SetLeft(int i_Value) { m_UpperLeft.SetX(i_Value); }
  void SetLower(int i_Value) { m_LowerRight.SetY(i_Value); }
  void SetRight(int i_Value) { m_LowerRight.SetX(i_Value); }
  void SetUpperLeft(const maIntPoint2d i_Vec2d) { m_UpperLeft.Set(i_Vec2d); }
  void SetLowerRight(const maIntPoint2d i_Vec2d) { m_LowerRight.Set(i_Vec2d); }

private:
  maIntPoint2d m_UpperLeft;
  maIntPoint2d m_LowerRight;
};
