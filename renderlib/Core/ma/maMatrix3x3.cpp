/****************************************************************************\
**  maMatrix3x3.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maMatrix3x3.hpp"
#include "Core/ma/maFunctions.hpp"

//------------------------------------------------------------------------
//	default constructor
//------------------------------------------------------------------------
maMatrix3x3::maMatrix3x3()
{
  Identity();
}

//------------------------------------------------------------------------
//	copy constructors
//------------------------------------------------------------------------
maMatrix3x3::maMatrix3x3(const maMatrix3x3& i_CopyFrom)
{
  *this = i_CopyFrom;
}

//------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------
maMatrix3x3::maMatrix3x3(float i_M0,
                         float i_M1,
                         float i_M2,
                         float i_M3,
                         float i_M4,
                         float i_M5,
                         float i_M6,
                         float i_M7,
                         float i_M8)
{
  m_Mat[0] = i_M0;
  m_Mat[1] = i_M1;
  m_Mat[2] = i_M2;
  m_Mat[3] = i_M3;
  m_Mat[4] = i_M4;
  m_Mat[5] = i_M5;
  m_Mat[6] = i_M6;
  m_Mat[7] = i_M7;
  m_Mat[8] = i_M8;
}

maMatrix3x3::maMatrix3x3(const maVector3d& iX, const maVector3d& iY, const maVector3d& iZ)
{
  m_Mat[0] = iX[0];
  m_Mat[1] = iX[1];
  m_Mat[2] = iX[2];
  m_Mat[3] = iY[0];
  m_Mat[4] = iY[1];
  m_Mat[5] = iY[2];
  m_Mat[6] = iZ[0];
  m_Mat[7] = iZ[1];
  m_Mat[8] = iZ[2];
}

//------------------------------------------------------------------------
//	RotationDelta()
//------------------------------------------------------------------------
void
maMatrix3x3::RotateBy(float i_AngleRad, maVector3d i_AxisVec)
{
  maMatrix3x3 rotation_matrix;
  rotation_matrix.MakeRotate(i_AngleRad, i_AxisVec);
  (*this) *= rotation_matrix;
}

//------------------------------------------------------------------------
//	Rotate()
//------------------------------------------------------------------------
void
maMatrix3x3::MakeRotate(float i_AngleRad, maVector3d i_AxisVec)
{
  if (i_AxisVec.GetX() == 1 && i_AxisVec.GetY() == 0 && i_AxisVec.GetZ() == 0) // ABOUT X-AXIS
  {
    MakeRotateX(i_AngleRad);
  } else if (i_AxisVec.GetX() == 0 && i_AxisVec.GetY() == 1 && i_AxisVec.GetZ() == 0) // ABOUT Y-AXIS
  {
    MakeRotateY(i_AngleRad);
  } else if (i_AxisVec.GetX() == 0 && i_AxisVec.GetY() == 0 && i_AxisVec.GetZ() == 1) // ABOUT Z-AXIS
  {
    MakeRotateZ(i_AngleRad);
  } else // ARBITRARY AXIS
  {
    i_AxisVec.Normalize();

    float c = float(cos(i_AngleRad));
    float s = float(sin(i_AngleRad));
    float t = 1 - c;

    float x = i_AxisVec.GetX();
    float y = i_AxisVec.GetY();
    float z = i_AxisVec.GetZ();

    float sx = s * x;
    float sy = s * y;
    float sz = s * z;

    float txx = t * x * x;
    float txy = t * x * y;
    float txz = t * x * z;
    float tyy = t * y * y;
    float tyz = t * y * z;
    float tzz = t * z * z;

    m_Mat[0] = txx + c;
    m_Mat[1] = txy + sz;
    m_Mat[2] = txz - sy;
    m_Mat[3] = txy - sz;
    m_Mat[4] = tyy + c;
    m_Mat[5] = tyz + sx;
    m_Mat[6] = txz + sy;
    m_Mat[7] = tyz - sx;
    m_Mat[8] = tzz + c;
  }
}

//------------------------------------------------------------------------
//	RotateX()
//------------------------------------------------------------------------
void
maMatrix3x3::MakeRotateX(float i_AngleRad)
{
  float ca = (float)cos(i_AngleRad);
  float sa = (float)sin(i_AngleRad);

  //	x -> x
  //	y -> y cos a - z sin a
  //	z -> y sin a + z cos a

  m_Mat[0] = 1;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = 0;
  m_Mat[4] = ca;
  m_Mat[5] = sa;
  m_Mat[6] = 0;
  m_Mat[7] = -sa;
  m_Mat[8] = ca;
}

//------------------------------------------------------------------------
//	RotateY()
//------------------------------------------------------------------------
void
maMatrix3x3::MakeRotateY(float i_AngleRad)
{
  float ca = (float)cos(i_AngleRad);
  float sa = (float)sin(i_AngleRad);

  //	x -> x cos a + z sin a
  //	y -> y
  //	z -> - x sin a + z cos a

  m_Mat[0] = ca;
  m_Mat[1] = 0;
  m_Mat[2] = -sa;
  m_Mat[3] = 0;
  m_Mat[4] = 1;
  m_Mat[5] = 0;
  m_Mat[6] = sa;
  m_Mat[7] = 0;
  m_Mat[8] = ca;
}

//------------------------------------------------------------------------
//	RotateZ()
//------------------------------------------------------------------------
void
maMatrix3x3::MakeRotateZ(float i_AngleRad)
{
  float ca = (float)cos(i_AngleRad);
  float sa = (float)sin(i_AngleRad);

  //	x -> x cos a - y sin a
  //	y -> x sin a + y cos a
  //	z -> z

  m_Mat[0] = ca;
  m_Mat[1] = sa;
  m_Mat[2] = 0;
  m_Mat[3] = -sa;
  m_Mat[4] = ca;
  m_Mat[5] = 0;
  m_Mat[6] = 0;
  m_Mat[7] = 0;
  m_Mat[8] = 1;
}

//------------------------------------------------------------------------
//	ScaleDelta()
//------------------------------------------------------------------------
void
maMatrix3x3::ScaleBy(float i_Sx, float i_Sy, float i_Sz)
{
  m_Mat[0] *= i_Sx;
  m_Mat[3] *= i_Sx;
  m_Mat[6] *= i_Sx;

  m_Mat[1] *= i_Sy;
  m_Mat[4] *= i_Sy;
  m_Mat[7] *= i_Sy;

  m_Mat[2] *= i_Sz;
  m_Mat[5] *= i_Sz;
  m_Mat[8] *= i_Sz;
}

//------------------------------------------------------------------------
//	Scale()
//------------------------------------------------------------------------
void
maMatrix3x3::MakeScale(float i_Sx, float i_Sy, float i_Sz)
{
  m_Mat[0] = i_Sx;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = 0;
  m_Mat[4] = i_Sy;
  m_Mat[5] = 0;
  m_Mat[6] = 0;
  m_Mat[7] = 0;
  m_Mat[8] = i_Sz;
}

//------------------------------------------------------------------------
//	Transpose()
//------------------------------------------------------------------------
void
maMatrix3x3::Transpose()
{
  maFunctions::Swap(m_Mat[3], m_Mat[1]);
  maFunctions::Swap(m_Mat[2], m_Mat[6]);
  maFunctions::Swap(m_Mat[7], m_Mat[5]);
}

//------------------------------------------------------------------------
//	Clear()
//------------------------------------------------------------------------
void
maMatrix3x3::Clear()
{
  m_Mat[0] = m_Mat[1] = m_Mat[2] = m_Mat[3] = m_Mat[4] = m_Mat[5] = m_Mat[6] = m_Mat[7] = m_Mat[8] = 0.0f;
}

//------------------------------------------------------------------------
//	Identity()
//------------------------------------------------------------------------
void
maMatrix3x3::Identity()
{
  m_Mat[0] = m_Mat[4] = m_Mat[8] = 1.0f;
  m_Mat[1] = m_Mat[2] = m_Mat[3] = m_Mat[5] = m_Mat[6] = m_Mat[7] = 0.0f;
}

//------------------------------------------------------------------------
//	GetDeterminant() calculates the determinant of the matrix.
//------------------------------------------------------------------------
float
maMatrix3x3::GetDeterminant() const
{
  return m_Mat[0] * (m_Mat[4] * m_Mat[8] - m_Mat[5] * m_Mat[7]) +
         m_Mat[1] * (m_Mat[5] * m_Mat[6] - m_Mat[3] * m_Mat[8]) +
         m_Mat[2] * (m_Mat[3] * m_Mat[7] - m_Mat[4] * m_Mat[6]);
}

//------------------------------------------------------------------------
//	Invert() makes the matrix into it's inverse.  This is the real,
//	general, mathematical inverse.  Some matrices are not invertible
//	and if you try to invert one of these it will assert.
//------------------------------------------------------------------------
void
maMatrix3x3::Invert()
{
  float det = this->GetDeterminant();

  DBG_ASSERT(det != 0.0f, "Attempted to invert a non-invertible matrix");

  double inv_det = double(1.0) / double(det);

  maMatrix3x3 new_mat;

  new_mat.m_Mat[0] = float(double(m_Mat[4] * m_Mat[8] - m_Mat[5] * m_Mat[7]) * inv_det);
  new_mat.m_Mat[1] = float(double(m_Mat[5] * m_Mat[6] - m_Mat[3] * m_Mat[8]) * inv_det);
  new_mat.m_Mat[2] = float(double(m_Mat[3] * m_Mat[7] - m_Mat[4] * m_Mat[6]) * inv_det);

  new_mat.m_Mat[3] = float(double(m_Mat[7] * m_Mat[2] - m_Mat[8] * m_Mat[1]) * inv_det);
  new_mat.m_Mat[4] = float(double(m_Mat[0] * m_Mat[8] - m_Mat[2] * m_Mat[6]) * inv_det);
  new_mat.m_Mat[5] = float(double(m_Mat[6] * m_Mat[1] - m_Mat[7] * m_Mat[0]) * inv_det);

  new_mat.m_Mat[6] = float(double(m_Mat[1] * m_Mat[5] - m_Mat[2] * m_Mat[4]) * inv_det);
  new_mat.m_Mat[7] = float(double(m_Mat[2] * m_Mat[3] - m_Mat[0] * m_Mat[5]) * inv_det);
  new_mat.m_Mat[8] = float(double(m_Mat[0] * m_Mat[4] - m_Mat[1] * m_Mat[3]) * inv_det);

  new_mat.Transpose();

  *this = new_mat;
}

//------------------------------------------------------------------------
//	Transform
//------------------------------------------------------------------------
void
maMatrix3x3::Transform(maVector3d& io_Vector) const
{
  float x = m_Mat[0] * io_Vector.GetX() + m_Mat[3] * io_Vector.GetY() + m_Mat[6] * io_Vector.GetZ();
  float y = m_Mat[1] * io_Vector.GetX() + m_Mat[4] * io_Vector.GetY() + m_Mat[7] * io_Vector.GetZ();
  float z = m_Mat[2] * io_Vector.GetX() + m_Mat[5] * io_Vector.GetY() + m_Mat[8] * io_Vector.GetZ();
  io_Vector.Set(x, y, z);
}

//------------------------------------------------------------------------
//	operator: Multiply (*)	vector3
//------------------------------------------------------------------------
maVector3d
maMatrix3x3::operator*(const maVector3d& i_V) const
{
  float x = m_Mat[0] * i_V.GetX() + m_Mat[3] * i_V.GetY() + m_Mat[6] * i_V.GetZ();
  float y = m_Mat[1] * i_V.GetX() + m_Mat[4] * i_V.GetY() + m_Mat[7] * i_V.GetZ();
  float z = m_Mat[2] * i_V.GetX() + m_Mat[5] * i_V.GetY() + m_Mat[8] * i_V.GetZ();
  return maVector3d(x, y, z);
}

//------------------------------------------------------------------------
//	operator: Multiply (*)
//------------------------------------------------------------------------
maMatrix3x3
maMatrix3x3::operator*(const maMatrix3x3& i_A) const
{
  return maMatrix3x3(m_Mat[0] * i_A.m_Mat[0] + m_Mat[1] * i_A.m_Mat[3] + m_Mat[2] * i_A.m_Mat[6], // row 1
                     m_Mat[0] * i_A.m_Mat[1] + m_Mat[1] * i_A.m_Mat[4] + m_Mat[2] * i_A.m_Mat[7],
                     m_Mat[0] * i_A.m_Mat[2] + m_Mat[1] * i_A.m_Mat[5] + m_Mat[2] * i_A.m_Mat[8],

                     m_Mat[3] * i_A.m_Mat[0] + m_Mat[4] * i_A.m_Mat[3] + m_Mat[5] * i_A.m_Mat[6], // row 1
                     m_Mat[3] * i_A.m_Mat[1] + m_Mat[4] * i_A.m_Mat[4] + m_Mat[5] * i_A.m_Mat[7],
                     m_Mat[3] * i_A.m_Mat[2] + m_Mat[4] * i_A.m_Mat[5] + m_Mat[5] * i_A.m_Mat[8],

                     m_Mat[6] * i_A.m_Mat[0] + m_Mat[7] * i_A.m_Mat[3] + m_Mat[8] * i_A.m_Mat[6], // row 1
                     m_Mat[6] * i_A.m_Mat[1] + m_Mat[7] * i_A.m_Mat[4] + m_Mat[8] * i_A.m_Mat[7],
                     m_Mat[6] * i_A.m_Mat[2] + m_Mat[7] * i_A.m_Mat[5] + m_Mat[8] * i_A.m_Mat[8]);
}

//------------------------------------------------------------------------
//	operator: Multiply (*=)
//------------------------------------------------------------------------
void
maMatrix3x3::operator*=(const maMatrix3x3& i_A)
{
  //	The old version of this function tried to do the work in place;
  //	however, it did not account for values changing in the middle
  //	of the calculation.  (For instance, computing m_Mat[0] as a new
  //	value, and then using m_Mat[0], expecting it to still be the old
  //	value).  This way is safer, simpler, easier, and also more
  //	oriented towards exception safety.
  *this = (*this) * i_A;
}

//------------------------------------------------------------------------
//	operator: Addition (+)
//------------------------------------------------------------------------
maMatrix3x3
maMatrix3x3::operator+(const maMatrix3x3& i_A) const
{
  return maMatrix3x3(m_Mat[0] + i_A.m_Mat[0],
                     m_Mat[1] + i_A.m_Mat[1],
                     m_Mat[2] + i_A.m_Mat[2],
                     m_Mat[3] + i_A.m_Mat[3],
                     m_Mat[4] + i_A.m_Mat[4],
                     m_Mat[5] + i_A.m_Mat[5],
                     m_Mat[6] + i_A.m_Mat[6],
                     m_Mat[7] + i_A.m_Mat[7],
                     m_Mat[8] + i_A.m_Mat[8]);
}

//------------------------------------------------------------------------
//	operator: Addition (+=)
//------------------------------------------------------------------------
void
maMatrix3x3::operator+=(const maMatrix3x3& i_A)
{
  m_Mat[0] += i_A.m_Mat[0];
  m_Mat[1] += i_A.m_Mat[1];
  m_Mat[2] += i_A.m_Mat[2];
  m_Mat[3] += i_A.m_Mat[3];
  m_Mat[4] += i_A.m_Mat[4];
  m_Mat[5] += i_A.m_Mat[5];
  m_Mat[6] += i_A.m_Mat[6];
  m_Mat[7] += i_A.m_Mat[7];
  m_Mat[8] += i_A.m_Mat[8];
}

//------------------------------------------------------------------------
//	operator: subtraction (-)
//------------------------------------------------------------------------
maMatrix3x3
maMatrix3x3::operator-(const maMatrix3x3& i_A) const
{
  return maMatrix3x3(m_Mat[0] - i_A.m_Mat[0],
                     m_Mat[1] - i_A.m_Mat[1],
                     m_Mat[2] - i_A.m_Mat[2],
                     m_Mat[3] - i_A.m_Mat[3],
                     m_Mat[4] - i_A.m_Mat[4],
                     m_Mat[5] - i_A.m_Mat[5],
                     m_Mat[6] - i_A.m_Mat[6],
                     m_Mat[7] - i_A.m_Mat[7],
                     m_Mat[8] - i_A.m_Mat[8]);
}

//------------------------------------------------------------------------
//	operator: subtraction (-=)
//------------------------------------------------------------------------
void
maMatrix3x3::operator-=(const maMatrix3x3& i_A)
{
  m_Mat[0] -= i_A.m_Mat[0];
  m_Mat[1] -= i_A.m_Mat[1];
  m_Mat[2] -= i_A.m_Mat[2];
  m_Mat[3] -= i_A.m_Mat[3];
  m_Mat[4] -= i_A.m_Mat[4];
  m_Mat[5] -= i_A.m_Mat[5];
  m_Mat[6] -= i_A.m_Mat[6];
  m_Mat[7] -= i_A.m_Mat[7];
  m_Mat[8] -= i_A.m_Mat[8];
}

//------------------------------------------------------------------------
//	operator: multiplication by scalar
//------------------------------------------------------------------------
void
maMatrix3x3::operator*=(float i_Val)
{
  m_Mat[0] *= i_Val;
  m_Mat[1] *= i_Val;
  m_Mat[2] *= i_Val;
  m_Mat[3] *= i_Val;
  m_Mat[4] *= i_Val;
  m_Mat[5] *= i_Val;
  m_Mat[6] *= i_Val;
  m_Mat[7] *= i_Val;
  m_Mat[8] *= i_Val;
}

// get row vector at index
maVector3d
maMatrix3x3::Row(int i) const
{
  return maVector3d(m_Mat[i * 3 + 0], m_Mat[i * 3 + 1], m_Mat[i * 3 + 2]);
}
