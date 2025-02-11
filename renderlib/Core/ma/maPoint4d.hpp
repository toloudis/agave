/****************************************************************************\
**  maPoint4d.hpp
**
**      maPoint4d.hpp defines the maPoint4d class
**
**
**
\****************************************************************************/

#ifdef MA_POINT4D_HPP
#error maPoint4d.hpp multiply included
#endif
#define MA_POINT4D_HPP

#ifndef MA_VECTOR4D_HPP
#include "Core/ma/maVector4d.hpp"
#endif

//============================================================================
//	maPont4d
//
//	Structurally, the point is the same as a vector.  The only difference is
//	the lack of a magnitude.  Representationally they are different so we are
//	makeing a new type definition to show this difference in the code.
//============================================================================
typedef maVector4d maPoint4d;
