/****************************************************************************\
**  maPoint3d.hpp
**
**      maPoint3d.hpp defines the maPoint3d class
**
**
**
\****************************************************************************/

#ifdef MA_POINT3D_HPP
#error maPoint3d.hpp multiply included
#endif
#define MA_POINT3D_HPP

#ifndef MA_VECTOR3D_HPP
#include "Core/ma/maVector3d.hpp"
#endif

//============================================================================
//	maPont3d
//
//	Structurally, the point is the same as a vector.  The only difference is
//	the lack of a magnitude.  Representationally they are different so we are
//	makeing a new type definition to show this difference in the code.
//============================================================================
typedef maVector3d maPoint3d;
