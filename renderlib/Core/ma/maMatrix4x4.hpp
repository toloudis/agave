/****************************************************************************\
**  maMatrix4x4.hpp
**
**      maMatrix4x4.hpp defines the maMatrix4x4 class
**
**
**
\****************************************************************************/
#pragma once
#ifdef MA_MATRIX4X4_HPP
#error maMatrix4x4.hpp multiply included
#endif
#define MA_MATRIX4X4_HPP

#ifndef MA_MATRIX3X3_HPP
#include "Core/ma/maMatrix3x3.hpp"
#endif
#ifndef MA_VECTOR4D_HPP
#include "Core/ma/maVector4d.hpp"
#endif
#ifndef MA_VECTOR3D_HPP
#include "Core/ma/maVector3d.hpp"
#endif
#ifndef MA_POINT3D_HPP
#include "Core/ma/maPoint3d.hpp"
#endif

#ifndef ENV_POOL_HPP
#include "Core/env/envPool.hpp"
#endif

//============================================================================
//============================================================================
class maMatrix4x4
{
public:
  //------------------------------------------------------------------------
  //	default and copy constructors
  //------------------------------------------------------------------------
  maMatrix4x4();
  maMatrix4x4(const maMatrix4x4& i_CopyFrom);

  //------------------------------------------------------------------------
  //	constructor
  //------------------------------------------------------------------------
  maMatrix4x4(float i_M0,
              float i_M1,
              float i_M2,
              float i_M3,
              float i_M4,
              float i_M5,
              float i_M6,
              float i_M7,
              float i_M8,
              float i_M9,
              float i_M10,
              float i_M11,
              float i_M12,
              float i_M13,
              float i_M14,
              float i_M15);

  //------------------------------------------------------------------------
  //	MakeRotate makes the matrix into a rotation about the given axis.
  //	The rotation will be CCW from a viewpoint in the direction the axis
  //	vector is pointing.
  //------------------------------------------------------------------------
  void MakeRotate(float i_AngleRad, maVector3d i_AxisVec);

  //	MakeRotate makes the matrix into a rotation from one vector to the other.
  //  The rotation axis is the cross product of the two vectors.
  void MakeRotate(const maVector3d& iFrom, const maVector3d& iTo);

  //------------------------------------------------------------------------
  //	RotateBy appends a rotation about the given axis to the matrix.
  //------------------------------------------------------------------------
  void RotateBy(float i_AngleRad, maVector3d i_AxisVec);

  //------------------------------------------------------------------------
  //	MakeRotateX makes the matrix into a rotation about the X axis.  The
  //	rotation will be CCW in a right handed coordinate system, from the
  //	positive X viewpoint.
  //------------------------------------------------------------------------
  void MakeRotateX(float i_AngleRad);

  //------------------------------------------------------------------------
  //	MakeRotateY makes the matrix into a rotation about the Y axis.  The
  //	rotation will be CCW in a right handed coordinate system, from the
  //	positive Y viewpoint.
  //------------------------------------------------------------------------
  void MakeRotateY(float i_AngleRad);

  //------------------------------------------------------------------------
  //	MakeRotateZ makes the matrix into a rotation about the Z axis.  The
  //	rotation will be CCW in a right handed coordinate system, from the
  //	positive Z viewpoint.
  //------------------------------------------------------------------------
  void MakeRotateZ(float i_AngleRad);

  //------------------------------------------------------------------------
  //	ScaleBy
  //------------------------------------------------------------------------
  void ScaleBy(float i_Sx, float i_y, float i_Sz);

  //------------------------------------------------------------------------
  //	MakeScale
  //------------------------------------------------------------------------
  void MakeScale(float i_Sx, float i_Sy, float i_Sz);

  //------------------------------------------------------------------------
  //	TranslateBy
  //------------------------------------------------------------------------
  void TranslateBy(float i_Tx, float i_Ty, float i_Tz);

  //------------------------------------------------------------------------
  //	TranslateBy
  //------------------------------------------------------------------------
  inline void TranslateBy(const maVector3d& i_Vector);

  //------------------------------------------------------------------------
  //	MakeTranslate
  //------------------------------------------------------------------------
  inline void MakeTranslate(const maVector3d& i_Vector);

  //------------------------------------------------------------------------
  //	MakeTranslate
  //------------------------------------------------------------------------
  void MakeTranslate(float i_Tx, float i_Ty, float i_Tz);

  //------------------------------------------------------------------------
  //	Transpose()
  //------------------------------------------------------------------------
  void Transpose();

  //------------------------------------------------------------------------
  //	Clear()
  //------------------------------------------------------------------------
  void Clear();

  //------------------------------------------------------------------------
  //	Identity()
  //------------------------------------------------------------------------
  void Identity();

  //------------------------------------------------------------------------
  //	IsIdentity()
  //------------------------------------------------------------------------
  bool IsIdentity() const;

  //------------------------------------------------------------------------
  //	GetSubMatrix returns the sub matrix formed by removing the given
  //	row and column.
  //------------------------------------------------------------------------
  maMatrix3x3 GetSubMatrix(int i_Row, int i_Column) const;

  //------------------------------------------------------------------------
  //	GetTranslation returns the tranlation portion of the matrix.
  //------------------------------------------------------------------------
  inline maVector3d GetTranslation(void) const;

  //------------------------------------------------------------------------
  //	GetDeterminant() returns the determinant of the 4x4 matrix.
  //------------------------------------------------------------------------
  float GetDeterminant() const;

  //------------------------------------------------------------------------
  //	Invert() computes the mathematical inverse of the 4x4 matrix in
  //	full generality.  This is not a trivial operation and should not be
  //	done more often than necessary.
  //------------------------------------------------------------------------
  void Invert();

  //------------------------------------------------------------------------
  //	Unary operator (+)
  //------------------------------------------------------------------------
  inline maMatrix4x4 operator+() const;

  //------------------------------------------------------------------------
  //	Unary operator (-)
  //------------------------------------------------------------------------
  inline maMatrix4x4 operator-() const;

  //------------------------------------------------------------------------
  //	operator: ()()
  //
  //		Returns the specified matrix element (Row, Column).
  //------------------------------------------------------------------------
  inline float& operator()(const int i_Row, const int i_Col);

  //------------------------------------------------------------------------
  //	operator: ()()
  //
  //		Returns the specified matrix element (Row, Column).
  //------------------------------------------------------------------------
  inline float operator()(const int i_Row, const int i_Col) const;

  inline float& operator[](const int iElem);
  inline float operator[](const int iElem) const;

  //------------------------------------------------------------------------
  //	Transform - used for points, assumes w=1
  //------------------------------------------------------------------------
  inline void Transform(maVector3d& io_Vector) const;
  // does a perspective w divide
  inline maVector3d TransformH(const maVector3d& io_Vector) const;

  //------------------------------------------------------------------------
  //	TransformDir - multiplies vector times matrix using w=0
  //------------------------------------------------------------------------
  inline void TransformDir(maVector3d& io_Vector) const;

  //------------------------------------------------------------------------
  //	Transform
  //------------------------------------------------------------------------
  void Transform(maVector4d& io_Vector) const;

  //------------------------------------------------------------------------
  //	operator: Multiply (*)	vector4
  //------------------------------------------------------------------------
  maVector4d operator*(const maVector4d& i_V) const;

  //------------------------------------------------------------------------
  //	operator: Multiply (*)	vector3
  //------------------------------------------------------------------------
  maVector3d operator*(const maVector3d& i_V) const;

  //------------------------------------------------------------------------
  //	operator: multiplication (*)
  //------------------------------------------------------------------------
  maMatrix4x4 operator*(const maMatrix4x4& i_A) const;

  //------------------------------------------------------------------------
  //	operator: multiplication (*=)
  //------------------------------------------------------------------------
  void operator*=(const maMatrix4x4& i_A);

  //------------------------------------------------------------------------
  //	operator: addition (+)
  //------------------------------------------------------------------------
  maMatrix4x4 operator+(const maMatrix4x4& i_A);

  //------------------------------------------------------------------------
  //	operator: addition (+=)
  //------------------------------------------------------------------------
  void operator+=(const maMatrix4x4& i_A);

  //------------------------------------------------------------------------
  //	operator: subtraction (-)
  //------------------------------------------------------------------------
  maMatrix4x4 operator-(const maMatrix4x4& i_A);

  //------------------------------------------------------------------------
  //	operator: subtraction (-=)
  //------------------------------------------------------------------------
  void operator-=(const maMatrix4x4& i_A);

  //------------------------------------------------------------------------
  //	operator: Assignment (=) with a maMatrix3x3.
  //------------------------------------------------------------------------
  maMatrix4x4& operator=(const maMatrix3x3& i_A);

  //------------------------------------------------------------------------
  //	operator: test for equality
  //------------------------------------------------------------------------
  bool operator==(const maMatrix4x4& i_A) const;

  //------------------------------------------------------------------------
  //	Multiply3x4()
  //
  //  Note:  This version the fourth matrix row is implicitly  [0, 0, 0, 1]
  //		   This limited form suffices for common transformations, and
  //         does %25 less work than full 4x4 transformation.
  //		   This matrix suffices for rotation and translation involved in
  //		   frame hierarchy transformations, (coordinate space conversion).
  //
  //					 _					_	 _	  _
  //			V1		|	m11 m12 m13 m14	 |	|  u1  |
  //			V2	=	|	m21 m22 m23 m24	 |	|  u2  |
  //			V3		|	m31 m32 m33 m34  |	|  u3  |
  //			1		|	0	0	0	1    |	|  1   |
  //					|_					_|	|_    _|
  //
  //
  //  v1 = m11 * u1  +  m12 * u2  +  m13 * u3  + M14
  //  v2 = m21 * u1  +  m22 * u2  +  m23 * u3  + M24
  //  v3 = m31 * u1  +  m32 * u2  +  m33 * u3  + M34
  //
  //------------------------------------------------------------------------
  void Multiply3x4(maVector4d& i_Vr, const maVector4d& i_V) const;

  //------------------------------------------------------------------------
  //	InverseViewport2()
  //
  //------------------------------------------------------------------------
  void InverseViewport2(const int i_WindowWidth, const int i_WindowHeight);

  //------------------------------------------------------------------------
  //	Viewport2()
  //
  // VIEWPORT TRANFORMATION MATRIX : given Window width and height in pixels
  // FROM "JIM BLINN'S CORNER" JULY '91. This function transforms an
  // 4-vector in homogeneous space of the form (x,y,z,w) where
  // -w<=x,y,z<=w into (x',y',z',1) where (x',y') is the vectors
  // projected pixel location in ([0,WW-1],[0,WH-1]) and z' is the
  // normalized depth value mapped into [0,1]; 0 is the near plane
  // and 1 is the far plane. VIEWPORT_TOL is introduced to have correct
  // mappings. (-1,-1,?,?) in NDC refers to the bottom-left of the
  // viewplane window and (0,0,?,?) in screen space refers to the
  // bottom-left pixel.
  //------------------------------------------------------------------------
  void Viewport2(const int i_WindowWidth, const int i_WindowHeight);

  //------------------------------------------------------------------------
  //	InverseLookAt()
  //
  //------------------------------------------------------------------------
  void InverseLookAt(const maVector3d i_Eye, const maPoint3d i_ViewRefPoint, const maVector3d i_ViewUp);

  //------------------------------------------------------------------------
  //	LookAt()
  //
  //------------------------------------------------------------------------
  void LookAt(const maVector3d i_Eye, const maPoint3d i_ViewRefPoint, const maVector3d i_ViewUp);

  //------------------------------------------------------------------------
  //	InverseViewport()
  //
  //------------------------------------------------------------------------
  void InverseViewport(const int i_WindowWidth, const int i_WindowHeight);

  //------------------------------------------------------------------------
  //	Viewport()
  //
  //------------------------------------------------------------------------
  void Viewport(const int i_WindowWidth, const int i_WindowHeight);

  //------------------------------------------------------------------------
  //	InversePerspective()
  //
  //------------------------------------------------------------------------
  void InversePerspective(float i_Yfov, float i_Aspect, float i_Ndist, float i_Fdist);

  //------------------------------------------------------------------------
  //	Perspective()
  //
  //------------------------------------------------------------------------
  void Perspective(float i_Yfov, float i_Aspect, float i_Ndist, float i_Fdist);

  //------------------------------------------------------------------------
  //	InverseFrustum()
  //
  //------------------------------------------------------------------------
  void InverseFrustum(float l, float r, float b, float t, float n, float f);

  //------------------------------------------------------------------------
  //	Frustum()
  //
  //  Same as glFrustum() : Perspective transformation matrix defined by a
  //  truncated pyramid viewing frustum that starts at the origin (eye)
  //  going in the -Z axis direction (viewer looks down -Z)
  //  with the four pyramid sides passing through the sides of a window
  //  defined through x=l, x=r, y=b, y=t on the viewplane at z=-n.
  //  The top and bottom of the pyramid are truncated by the near and far
  //  planes at z=-n and z=-f. A 4-vector (x,y,z,w) inside this frustum
  //  transformed by this matrix will have x,y,z values in the range
  //  [-w,w]. Homogeneous clipping is applied to restrict (x,y,z) to
  //  this range. Later, a perspective divide by w will result in an NDC
  //  coordinate 3-vector of the form (x/w,y/w,z/w,w/w)=(x',y',z',1) where
  //  x', y', and z' all are in the range [-1,1]. Perspectively divided z'
  //  will be in [-1,1] with -1 being the near plane and +1 being the far.
  //------------------------------------------------------------------------
  void Frustum(float i_l, float i_, float i_b, float i_t, float i_n, float i_f);

  //------------------------------------------------------------------------
  //	RotateToFacePoint()
  //------------------------------------------------------------------------
  void RotateToFacePoint(maPoint3d i_RotatePoint, maVector3d i_RotateNormal, maPoint3d i_ToPoint);

  //----------------------------------------------------------------------------
  //	maMatrix4x4 uses a pool allocator for its new and delete
  //----------------------------------------------------------------------------
  // inline void* operator new(size_t size);
  // inline void operator delete(void* i_Ptr);
  // inline void* operator new(size_t i_Size, void* i_Ptr);
  // inline void operator delete(void* i_Ptr, void* i_Mem);

  //----------------------------------------------------------------------------
  //	Return the pointer to the elements. DANGER: Caller must know not to access
  //	past the end!
  //----------------------------------------------------------------------------
  inline float* Ptr() const;
  inline const float* GetPtr() const;

public:
  float m_Mat[16];
};

//------------------------------------------------------------------------
//	operator: ()()
//
//		Returns the specified matrix element (Row, Column).
//------------------------------------------------------------------------
float&
maMatrix4x4::operator()(const int i_Row, const int i_Col)
{
  return m_Mat[4 * i_Row + i_Col];
}
float&
maMatrix4x4::operator[](const int iElem)
{
  return m_Mat[iElem];
}

//------------------------------------------------------------------------
//	operator: ()()
//
//		Returns the specified matrix element (Row, Column).
//------------------------------------------------------------------------
float
maMatrix4x4::operator()(const int i_Row, const int i_Col) const
{
  return m_Mat[4 * i_Row + i_Col];
}
float
maMatrix4x4::operator[](const int iElem) const
{
  return m_Mat[iElem];
}

//------------------------------------------------------------------------
//	Unary operator (+)
//------------------------------------------------------------------------
maMatrix4x4
maMatrix4x4::operator+() const
{
  return *this;
}

//------------------------------------------------------------------------
//	Unary operator (-)
//------------------------------------------------------------------------
maMatrix4x4
maMatrix4x4::operator-() const
{
  return maMatrix4x4(-m_Mat[0],
                     -m_Mat[1],
                     -m_Mat[2],
                     -m_Mat[3],
                     -m_Mat[4],
                     -m_Mat[5],
                     -m_Mat[6],
                     -m_Mat[7],
                     -m_Mat[8],
                     -m_Mat[9],
                     -m_Mat[10],
                     -m_Mat[11],
                     -m_Mat[12],
                     -m_Mat[13],
                     -m_Mat[14],
                     -m_Mat[15]);
}

//------------------------------------------------------------------------
//	TranslateBy
//------------------------------------------------------------------------
inline void
maMatrix4x4::TranslateBy(const maVector3d& i_Vector)
{
  this->TranslateBy(i_Vector.m_X, i_Vector.m_Y, i_Vector.m_Z);
}

//------------------------------------------------------------------------
//	MakeTranslate
//------------------------------------------------------------------------
inline void
maMatrix4x4::MakeTranslate(const maVector3d& i_Vector)
{
  this->MakeTranslate(i_Vector.m_X, i_Vector.m_Y, i_Vector.m_Z);
}

//------------------------------------------------------------------------
//	Transform
//------------------------------------------------------------------------
inline void
maMatrix4x4::Transform(maVector3d& io_Vector) const
{
  io_Vector.Set((m_Mat[0] * io_Vector.GetX() + m_Mat[4] * io_Vector.GetY() + m_Mat[8] * io_Vector.GetZ() + m_Mat[12]),
                (m_Mat[1] * io_Vector.GetX() + m_Mat[5] * io_Vector.GetY() + m_Mat[9] * io_Vector.GetZ() + m_Mat[13]),
                (m_Mat[2] * io_Vector.GetX() + m_Mat[6] * io_Vector.GetY() + m_Mat[10] * io_Vector.GetZ() + m_Mat[14]));
}
inline maVector3d
maMatrix4x4::TransformH(const maVector3d& i_Vector) const
{
  maVector4d v;
  v.Set((m_Mat[0] * i_Vector.GetX() + m_Mat[4] * i_Vector.GetY() + m_Mat[8] * i_Vector.GetZ() + m_Mat[12]),
        (m_Mat[1] * i_Vector.GetX() + m_Mat[5] * i_Vector.GetY() + m_Mat[9] * i_Vector.GetZ() + m_Mat[13]),
        (m_Mat[2] * i_Vector.GetX() + m_Mat[6] * i_Vector.GetY() + m_Mat[10] * i_Vector.GetZ() + m_Mat[14]),
        (m_Mat[3] * i_Vector.GetX() + m_Mat[7] * i_Vector.GetY() + m_Mat[11] * i_Vector.GetZ() + m_Mat[15]));
  return maVector3d(v[0] / v[3], v[1] / v[3], v[2] / v[3]);
}

//------------------------------------------------------------------------
//	TransformDir
//------------------------------------------------------------------------
inline void
maMatrix4x4::TransformDir(maVector3d& io_Vector) const
{
  io_Vector.Set((m_Mat[0] * io_Vector.GetX() + m_Mat[4] * io_Vector.GetY() + m_Mat[8] * io_Vector.GetZ()),
                (m_Mat[1] * io_Vector.GetX() + m_Mat[5] * io_Vector.GetY() + m_Mat[9] * io_Vector.GetZ()),
                (m_Mat[2] * io_Vector.GetX() + m_Mat[6] * io_Vector.GetY() + m_Mat[10] * io_Vector.GetZ()));
}

inline maVector3d
maMatrix4x4::GetTranslation(void) const

{
  return maVector3d(m_Mat[12], m_Mat[13], m_Mat[14]);
}

//----------------------------------------------------------------------------
//	Return the pointer to the elements. DANGER: Caller must know not to access
//	past the end!
//----------------------------------------------------------------------------
inline float*
maMatrix4x4::Ptr() const
{
  // look out: this is basically a const_cast.
  // oh well, we all know what we are doing, right?
  return (float*)m_Mat;
}
inline const float*
maMatrix4x4::GetPtr() const
{
  return m_Mat;
}
