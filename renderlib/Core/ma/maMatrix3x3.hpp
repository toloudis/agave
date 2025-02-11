/****************************************************************************\
**  maMatrix3x3.hpp
**
**      maMatrix3x3.hpp defines the maMatrix3x3 class
**
**
**
\****************************************************************************/

#ifdef MA_MATRIX3X3_HPP
#error maMatrix3x3.hpp multiply included
#endif
#define MA_MATRIX3X3_HPP

#ifndef MA_VECTOR3D_HPP
#include "Core/ma/maVector3d.hpp"
#endif

#ifndef MA_POINT3D_HPP
#include "Core/ma/maPoint3d.hpp"
#endif

//============================================================================
//============================================================================
class maMatrix3x3
{
public:
  //------------------------------------------------------------------------
  //	default and copy constructors
  //------------------------------------------------------------------------
  maMatrix3x3();
  maMatrix3x3(const maMatrix3x3& i_CopyFrom);

  //------------------------------------------------------------------------
  //	constructor
  //------------------------------------------------------------------------
  maMatrix3x3(float i_M0,
              float i_M1,
              float i_M2,
              float i_M3,
              float i_M4,
              float i_M5,
              float i_M6,
              float i_M7,
              float i_M8);
  maMatrix3x3(const maVector3d& iX, const maVector3d& iY, const maVector3d& iZ);

  //------------------------------------------------------------------------
  //	MakeRotate makes the matrix into a rotation about the given axis.
  //	The rotation will be CCW from a viewpoint in the direction the axis
  //	vector is pointing.
  //------------------------------------------------------------------------
  void MakeRotate(float i_AngleRad, maVector3d i_AxisVec);

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
  //	Transpose()
  //------------------------------------------------------------------------
  void Transpose();

  //------------------------------------------------------------------------
  //	Invert() makes the matrix into it's inverse.  This is the real,
  //	general, mathematical inverse.  Some matrices are not invertible
  //	and if you try to invert one of these it will assert.
  //------------------------------------------------------------------------
  void Invert();

  //------------------------------------------------------------------------
  //	Clear()
  //------------------------------------------------------------------------
  void Clear();

  //------------------------------------------------------------------------
  //	Identity()
  //------------------------------------------------------------------------
  void Identity();

  //------------------------------------------------------------------------
  //	GetDeterminant() calculates the determinant of the matrix.
  //------------------------------------------------------------------------
  float GetDeterminant() const;

  //------------------------------------------------------------------------
  //	Unary operator (+)
  //------------------------------------------------------------------------
  inline maMatrix3x3 operator+() const;

  //------------------------------------------------------------------------
  //	Unary operator (-)
  //------------------------------------------------------------------------
  inline maMatrix3x3 operator-() const;

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

  //------------------------------------------------------------------------
  //	Transform
  //------------------------------------------------------------------------
  void Transform(maVector3d& io_Vector) const;

  //------------------------------------------------------------------------
  //	operator: Multiply (*)	vector3
  //------------------------------------------------------------------------
  maVector3d operator*(const maVector3d& i_V) const;

  //------------------------------------------------------------------------
  //	operator: multiplication (*)
  //------------------------------------------------------------------------
  maMatrix3x3 operator*(const maMatrix3x3& i_A) const;

  //------------------------------------------------------------------------
  //	operator: multiplication (*=)
  //------------------------------------------------------------------------
  void operator*=(const maMatrix3x3& i_A);

  //------------------------------------------------------------------------
  //	operator: multiplication by scalar
  //------------------------------------------------------------------------
  void operator*=(float i_Val);

  //------------------------------------------------------------------------
  //	operator: addition (+)
  //------------------------------------------------------------------------
  maMatrix3x3 operator+(const maMatrix3x3& i_A) const;

  //------------------------------------------------------------------------
  //	operator: addition (+=)
  //------------------------------------------------------------------------
  void operator+=(const maMatrix3x3& i_A);

  //------------------------------------------------------------------------
  //	operator: subtraction (-)
  //------------------------------------------------------------------------
  maMatrix3x3 operator-(const maMatrix3x3& i_A) const;

  //------------------------------------------------------------------------
  //	operator: subtraction (-=)
  //------------------------------------------------------------------------
  void operator-=(const maMatrix3x3& i_A);

  // get row vector at index
  maVector3d Row(int i) const;

public:
  float m_Mat[9];
};

//------------------------------------------------------------------------
//	operator: ()()
//
//		Returns the specified matrix element (Row, Column).
//------------------------------------------------------------------------
float&
maMatrix3x3::operator()(const int i_Row, const int i_Col)
{
  return m_Mat[3 * i_Row + i_Col];
}

//------------------------------------------------------------------------
//	operator: ()()
//
//		Returns the specified matrix element (Row, Column).
//------------------------------------------------------------------------
float
maMatrix3x3::operator()(const int i_Row, const int i_Col) const
{
  return m_Mat[3 * i_Row + i_Col];
}

//------------------------------------------------------------------------
//	Unary operator (+)
//------------------------------------------------------------------------
maMatrix3x3
maMatrix3x3::operator+() const
{
  return *this;
}

//------------------------------------------------------------------------
//	Unary operator (-)
//------------------------------------------------------------------------
maMatrix3x3
maMatrix3x3::operator-() const
{
  return maMatrix3x3(-m_Mat[0], -m_Mat[1], -m_Mat[2], -m_Mat[3], -m_Mat[4], -m_Mat[5], -m_Mat[6], -m_Mat[7], -m_Mat[8]);
}
