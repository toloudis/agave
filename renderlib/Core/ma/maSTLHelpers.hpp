/****************************************************************************\
**  maSTLHelpers.hpp
**
**      maSTLHelpers.hpp contains some function objects useful for doing
**	math operations with the STL.  The ones which are not named "InPlace"
**	are to be used with std::transform, the ones which have "InPlace" in the
**	name are to be used with std::for_each.
**
**
**
\****************************************************************************/

#ifdef MA_STLHELPERS_HPP
#error maSTLHelpers.hpp multiply included
#endif
#define MA_STLHELPERS_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif
#ifndef MA_POINT3D_HPP
#include "Core/ma/maPoint3d.hpp"
#endif
#ifndef MA_MATRIX4X4_HPP
#include "Core/ma/maMatrix4x4.hpp"
#endif
#ifndef MA_MATRIX3X3_HPP
#include "Core/ma/maMatrix3x3.hpp"
#endif

//============================================================================
//	maPointTransformer4x4 will return a transformed point.
//============================================================================
class maPointTransformer4x4
{
public:
  maPointTransformer4x4(const maMatrix4x4& i_Matrix)
    : m_Matrix(i_Matrix)
  {
  }
  maPoint3d operator()(const maPoint3d& i_Point) const { return m_Matrix * i_Point; }

private:
  const maMatrix4x4& m_Matrix;
};

//============================================================================
//	maPointTransformer3x3 will return a rotated point.
//============================================================================
class maPointTransformer3x3
{
public:
  maPointTransformer3x3(maMatrix3x3& i_Matrix)
    : m_Matrix(i_Matrix)
  {
  }
  maVector3d operator()(const maVector3d& i_Point) const { return m_Matrix * i_Point; }

private:
  const maMatrix3x3& m_Matrix;
};

//============================================================================
//	The reason maAdder is hard-coded for envType::UInt32 is that is was
//	designed for use with indices.  It would have been nice to make it a
//	template on the integer type, but MSVCPP cannot resolve template
//	function calls using templated objects as parameters.
//============================================================================
class maAdder
{
public:
  maAdder(envType::UInt32 i_Val)
    : m_Val(i_Val)
  {
  }
  envType::UInt32 operator()(const envType::UInt32& i_Val) { return i_Val + m_Val; }

private:
  envType::UInt32 m_Val;
};

//============================================================================
//	maPointTransformer4x4 will return a transformed point.
//============================================================================
class maPointTransformer4x4InPlace
{
public:
  maPointTransformer4x4InPlace(const maMatrix4x4& i_Matrix)
    : m_Matrix(i_Matrix)
  {
  }
  void operator()(maPoint3d& o_Point) const { m_Matrix.Transform(o_Point); }

private:
  const maMatrix4x4& m_Matrix;
};

//============================================================================
//	maPointTransformer3x3 will return a rotated point.
//============================================================================
class maPointTransformer3x3InPlace
{
public:
  maPointTransformer3x3InPlace(maMatrix3x3& i_Matrix)
    : m_Matrix(i_Matrix)
  {
  }
  void operator()(maVector3d& o_Point) const { m_Matrix.Transform(o_Point); }

private:
  const maMatrix3x3& m_Matrix;
};

//============================================================================
//	The reason maAdder is hard-coded for envType::UInt32 is that is was
//	designed for use with indices.  It would have been nice to make it a
//	template on the integer type, but MSVCPP cannot resolve template
//	function calls using templated objects as parameters.
//============================================================================
class maAdderInPlace
{
public:
  maAdderInPlace(envType::UInt32 i_Val)
    : m_Val(i_Val)
  {
  }
  void operator()(envType::UInt32& o_Val) { o_Val += m_Val; }

private:
  envType::UInt32 m_Val;
};
