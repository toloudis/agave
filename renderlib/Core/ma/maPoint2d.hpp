/****************************************************************************\
**  maPoint2d.hpp
**
**      maPoint2d.hpp defines the maPoint2d class
**
**
**
\****************************************************************************/

#ifdef MA_POINT2D_HPP
#error maPoint2d.hpp multiply included
#endif
#define MA_POINT2D_HPP

#ifndef MA_VECTOR2D_HPP
#include "Core/ma/maVector2d.hpp"
#endif

//============================================================================
//	maPont2d
//
//	Structurally, the point is the same as a vector.  The only difference is
//	the lack of a magnitude.  Representationally they are different so we are
//	makeing a new type definition to show this difference in the code.
//============================================================================
typedef maVector2d maPoint2d;
