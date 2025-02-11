/****************************************************************************\
**  maMatrix4x4.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maMatrix4x4.hpp"
#include "Core/ma/maFunctions.hpp"
#include "Core/ma/maConstants.hpp"

//============================================================================
//============================================================================
namespace {
const float VIEWPORT_TOL = 0.001f;
}

//------------------------------------------------------------------------
//	default constructor
//------------------------------------------------------------------------
maMatrix4x4::maMatrix4x4()
{
  Identity();
}

//------------------------------------------------------------------------
//	copy constructors
//------------------------------------------------------------------------
maMatrix4x4::maMatrix4x4(const maMatrix4x4& i_CopyFrom)
{
  *this = i_CopyFrom;
}

//------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------
maMatrix4x4::maMatrix4x4(float i_M0,
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
                         float i_M15)
{
  m_Mat[0] = i_M0;
  m_Mat[4] = i_M4;
  m_Mat[8] = i_M8;
  m_Mat[12] = i_M12;
  m_Mat[1] = i_M1;
  m_Mat[5] = i_M5;
  m_Mat[9] = i_M9;
  m_Mat[13] = i_M13;
  m_Mat[2] = i_M2;
  m_Mat[6] = i_M6;
  m_Mat[10] = i_M10;
  m_Mat[14] = i_M14;
  m_Mat[3] = i_M3;
  m_Mat[7] = i_M7;
  m_Mat[11] = i_M11;
  m_Mat[15] = i_M15;
}

//------------------------------------------------------------------------
//	RotationDelta()
//------------------------------------------------------------------------
void
maMatrix4x4::RotateBy(float i_AngleRad, maVector3d i_AxisVec)
{
  maMatrix4x4 rotation_matrix;
  rotation_matrix.MakeRotate(i_AngleRad, i_AxisVec);
  (*this) *= rotation_matrix;
}

void
maMatrix4x4::MakeRotate(const maVector3d& iFrom, const maVector3d& iTo)
{
  // create axis and angle to rotate:
  maVector3d axis = iFrom.Cross(iTo);
  float dp = iFrom.Dot(iTo);
  float angle = acos(dp);
  MakeRotate(angle, axis);
}

//------------------------------------------------------------------------
//	Rotate()
//------------------------------------------------------------------------
void
maMatrix4x4::MakeRotate(float i_AngleRad, maVector3d i_AxisVec)
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
    m_Mat[3] = 0;
    m_Mat[4] = txy - sz;
    m_Mat[5] = tyy + c;
    m_Mat[6] = tyz + sx;
    m_Mat[7] = 0;
    m_Mat[8] = txz + sy;
    m_Mat[9] = tyz - sx;
    m_Mat[10] = tzz + c;
    m_Mat[11] = 0;
    m_Mat[12] = 0;
    m_Mat[13] = 0;
    m_Mat[14] = 0;
    m_Mat[15] = 1;
  }
}

//------------------------------------------------------------------------
//	RotateX()
//------------------------------------------------------------------------
void
maMatrix4x4::MakeRotateX(float i_AngleRad)
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
  m_Mat[4] = 0;
  m_Mat[5] = ca;
  m_Mat[6] = sa;
  m_Mat[7] = 0;
  m_Mat[8] = 0;
  m_Mat[9] = -sa;
  m_Mat[10] = ca;
  m_Mat[11] = 0;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	RotateY()
//------------------------------------------------------------------------
void
maMatrix4x4::MakeRotateY(float i_AngleRad)
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
  m_Mat[4] = 0;
  m_Mat[5] = 1;
  m_Mat[6] = 0;
  m_Mat[7] = 0;
  m_Mat[8] = sa;
  m_Mat[9] = 0;
  m_Mat[10] = ca;
  m_Mat[11] = 0;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	RotateZ()
//------------------------------------------------------------------------
void
maMatrix4x4::MakeRotateZ(float i_AngleRad)
{
  float ca = (float)cos(i_AngleRad);
  float sa = (float)sin(i_AngleRad);

  //	x -> x cos a - y sin a
  //	y -> x sin a + y cos a
  //	z -> z

  m_Mat[0] = ca;
  m_Mat[1] = sa;
  m_Mat[2] = 0;
  m_Mat[3] = 0;
  m_Mat[4] = -sa;
  m_Mat[5] = ca;
  m_Mat[6] = 0;
  m_Mat[7] = 0;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = 1;
  m_Mat[11] = 0;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	ScaleBy
//------------------------------------------------------------------------
void
maMatrix4x4::ScaleBy(float i_Sx, float i_Sy, float i_Sz)
{
  m_Mat[0] *= i_Sx;
  m_Mat[4] *= i_Sx;
  m_Mat[8] *= i_Sx;
  m_Mat[12] *= i_Sx;

  m_Mat[1] *= i_Sy;
  m_Mat[5] *= i_Sy;
  m_Mat[9] *= i_Sy;
  m_Mat[13] *= i_Sy;

  m_Mat[2] *= i_Sz;
  m_Mat[6] *= i_Sz;
  m_Mat[10] *= i_Sz;
  m_Mat[14] *= i_Sz;
}

//------------------------------------------------------------------------
//	MakeScale()
//------------------------------------------------------------------------
void
maMatrix4x4::MakeScale(float i_Sx, float i_Sy, float i_Sz)
{
  m_Mat[0] = i_Sx;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = 0;
  m_Mat[4] = 0;
  m_Mat[5] = i_Sy;
  m_Mat[6] = 0;
  m_Mat[7] = 0;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = i_Sz;
  m_Mat[11] = 0;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	TranslateBy()
//------------------------------------------------------------------------
void
maMatrix4x4::TranslateBy(float i_Tx, float i_Ty, float i_Tz)
{
  m_Mat[0] += m_Mat[3] * i_Tx;
  m_Mat[1] += m_Mat[3] * i_Ty;
  m_Mat[2] += m_Mat[3] * i_Tz;

  m_Mat[4] += m_Mat[7] * i_Tx;
  m_Mat[5] += m_Mat[7] * i_Ty;
  m_Mat[6] += m_Mat[7] * i_Tz;

  m_Mat[8] += m_Mat[11] * i_Tx;
  m_Mat[9] += m_Mat[11] * i_Ty;
  m_Mat[10] += m_Mat[11] * i_Tz;

  m_Mat[12] += m_Mat[15] * i_Tx;
  m_Mat[13] += m_Mat[15] * i_Ty;
  m_Mat[14] += m_Mat[15] * i_Tz;
}

//------------------------------------------------------------------------
//	MakeTranslate()
//------------------------------------------------------------------------
void
maMatrix4x4::MakeTranslate(float i_Tx, float i_Ty, float i_Tz)
{
  m_Mat[0] = 1;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = 0;
  m_Mat[4] = 0;
  m_Mat[5] = 1;
  m_Mat[6] = 0;
  m_Mat[7] = 0;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = 1;
  m_Mat[11] = 0;
  m_Mat[12] = i_Tx;
  m_Mat[13] = i_Ty;
  m_Mat[14] = i_Tz;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	Transpose()
//------------------------------------------------------------------------
void
maMatrix4x4::Transpose()
{
  maFunctions::Swap(m_Mat[1], m_Mat[4]);
  maFunctions::Swap(m_Mat[2], m_Mat[8]);
  maFunctions::Swap(m_Mat[6], m_Mat[9]);
  maFunctions::Swap(m_Mat[3], m_Mat[12]);
  maFunctions::Swap(m_Mat[7], m_Mat[13]);
  maFunctions::Swap(m_Mat[11], m_Mat[14]);
}

//------------------------------------------------------------------------
//	Clear()
//------------------------------------------------------------------------
void
maMatrix4x4::Clear()
{
  m_Mat[0] = m_Mat[5] = m_Mat[10] = m_Mat[15] = 0;
  m_Mat[1] = m_Mat[2] = m_Mat[3] = m_Mat[4] = m_Mat[6] = m_Mat[7] = m_Mat[8] = m_Mat[9] = m_Mat[11] = m_Mat[12] =
    m_Mat[13] = m_Mat[14] = 0;
}

//------------------------------------------------------------------------
//	Identity()
//------------------------------------------------------------------------
void
maMatrix4x4::Identity()
{
  m_Mat[0] = m_Mat[5] = m_Mat[10] = m_Mat[15] = 1;
  m_Mat[1] = m_Mat[2] = m_Mat[3] = m_Mat[4] = m_Mat[6] = m_Mat[7] = m_Mat[8] = m_Mat[9] = m_Mat[11] = m_Mat[12] =
    m_Mat[13] = m_Mat[14] = 0;
}

//------------------------------------------------------------------------
//	IsIdentity()
//------------------------------------------------------------------------
bool
maMatrix4x4::IsIdentity() const
{
  return (m_Mat[0] == 1) && (m_Mat[5] == 1) && (m_Mat[10] == 1) && (m_Mat[15] == 1) && (m_Mat[1] == 0) &&
         (m_Mat[2] == 0) && (m_Mat[3] == 0) && (m_Mat[4] == 0) && (m_Mat[6] == 0) && (m_Mat[7] == 0) &&
         (m_Mat[8] == 0) && (m_Mat[9] == 0) && (m_Mat[11] == 0) && (m_Mat[12] == 0) && (m_Mat[13] == 0) &&
         (m_Mat[14] == 0);
}

//------------------------------------------------------------------------
//	GetSubMatrix returns the sub matrix formed by removing the given
//	row and column.
//------------------------------------------------------------------------
maMatrix3x3
maMatrix4x4::GetSubMatrix(int i_Row, int i_Column) const
{
  maMatrix3x3 ret_val;

  int src_row, src_col, dest_row, dest_col;

  for (dest_row = 0; dest_row < 3; dest_row++) {
    if (dest_row >= i_Row)
      src_row = dest_row + 1;
    else
      src_row = dest_row;

    for (dest_col = 0; dest_col < 3; dest_col++) {
      if (dest_col >= i_Column)
        src_col = dest_col + 1;
      else
        src_col = dest_col;

      ret_val(dest_row, dest_col) = (*this)(src_row, src_col);
    }
  }

  return ret_val;
}

//------------------------------------------------------------------------
//	GetDeterminant() returns the determinant of the 4x4 matrix.
//------------------------------------------------------------------------
float
maMatrix4x4::GetDeterminant() const
{
  maMatrix3x3 minor;

  float ret_val = 0;

  minor = this->GetSubMatrix(0, 0);
  ret_val += (*this)(0, 0) * minor.GetDeterminant();

  minor = this->GetSubMatrix(1, 0);
  ret_val -= (*this)(1, 0) * minor.GetDeterminant();

  minor = this->GetSubMatrix(2, 0);
  ret_val += (*this)(2, 0) * minor.GetDeterminant();

  minor = this->GetSubMatrix(3, 0);
  ret_val -= (*this)(3, 0) * minor.GetDeterminant();

  return ret_val;
}

//------------------------------------------------------------------------
//	Invert() computes the mathematical inverse of the 4x4 matrix in
//	full generality.  This is not a trivial operation and should not be
//	done more often than necessary.
//------------------------------------------------------------------------
void
maMatrix4x4::Invert()
{
  float my_det = this->GetDeterminant();

  DBG_ASSERT(my_det != 0.0f, "Tried to invert non-invertible matrix");

  double inv_det = double(1.0) / double(my_det);

  maMatrix4x4 inverse;
  int row, column;

  for (row = 0; row < 4; row++) {
    for (column = 0; column < 4; column++) {
      float factor = float(((row + column + 1) & 0x01) * 2 - 1);
      inverse(row, column) = factor * float(double(this->GetSubMatrix(row, column).GetDeterminant()) * inv_det);
    }
  }

  inverse.Transpose();

  *this = inverse;
}

//------------------------------------------------------------------------
//	Transform
//------------------------------------------------------------------------
void
maMatrix4x4::Transform(maVector4d& io_Vector) const
{
  // note: this treats the point as a row vector premultiplying (to the left of) the matrix,
  // given that row 0 of the matrix is m_Mat[0,1,2,3] etc.
  // - equivalent to multiplying the transposes in reverse order
  maVector4d o_NewV((m_Mat[0] * io_Vector.GetX() + m_Mat[4] * io_Vector.GetY() + m_Mat[8] * io_Vector.GetZ() +
                     m_Mat[12] * io_Vector.GetW()),
                    (m_Mat[1] * io_Vector.GetX() + m_Mat[5] * io_Vector.GetY() + m_Mat[9] * io_Vector.GetZ() +
                     m_Mat[13] * io_Vector.GetW()),
                    (m_Mat[2] * io_Vector.GetX() + m_Mat[6] * io_Vector.GetY() + m_Mat[10] * io_Vector.GetZ() +
                     m_Mat[14] * io_Vector.GetW()),
                    (m_Mat[3] * io_Vector.GetX() + m_Mat[7] * io_Vector.GetY() + m_Mat[11] * io_Vector.GetZ() +
                     m_Mat[15] * io_Vector.GetW()));

  io_Vector = o_NewV;
}

//------------------------------------------------------------------------
//	operator: Multiply (*)	vector4
//------------------------------------------------------------------------
maVector4d
maMatrix4x4::operator*(const maVector4d& i_V) const
{
  // note: this treats the point as a row vector premultiplying (to the left of) the matrix,
  // given that row 0 of the matrix is m_Mat[0,1,2,3] etc.
  // - equivalent to multiplying the transposes in reverse order
  maVector4d o_NewV((m_Mat[0] * i_V.GetX() + m_Mat[4] * i_V.GetY() + m_Mat[8] * i_V.GetZ() + m_Mat[12] * i_V.GetW()),
                    (m_Mat[1] * i_V.GetX() + m_Mat[5] * i_V.GetY() + m_Mat[9] * i_V.GetZ() + m_Mat[13] * i_V.GetW()),
                    (m_Mat[2] * i_V.GetX() + m_Mat[6] * i_V.GetY() + m_Mat[10] * i_V.GetZ() + m_Mat[14] * i_V.GetW()),
                    (m_Mat[3] * i_V.GetX() + m_Mat[7] * i_V.GetY() + m_Mat[11] * i_V.GetZ() + m_Mat[15] * i_V.GetW()));

  return o_NewV;
}

//------------------------------------------------------------------------
//	operator: Multiply (*)	vector3
//------------------------------------------------------------------------
maVector3d
maMatrix4x4::operator*(const maVector3d& i_V) const
{
  // note: this treats the point as a row vector premultiplying (to the left of) the matrix,
  // given that row 0 of the matrix is m_Mat[0,1,2,3] etc.
  // - equivalent to multiplying the transposes in reverse order
  maVector3d o_NewV((m_Mat[0] * i_V.GetX() + m_Mat[4] * i_V.GetY() + m_Mat[8] * i_V.GetZ() + m_Mat[12]),
                    (m_Mat[1] * i_V.GetX() + m_Mat[5] * i_V.GetY() + m_Mat[9] * i_V.GetZ() + m_Mat[13]),
                    (m_Mat[2] * i_V.GetX() + m_Mat[6] * i_V.GetY() + m_Mat[10] * i_V.GetZ() + m_Mat[14]));
  return o_NewV;
}

//------------------------------------------------------------------------
//	operator: Multiply (*)
//------------------------------------------------------------------------
maMatrix4x4
maMatrix4x4::operator*(const maMatrix4x4& i_A) const
{
  return maMatrix4x4(
    m_Mat[0] * i_A.m_Mat[0] + m_Mat[1] * i_A.m_Mat[4] + m_Mat[2] * i_A.m_Mat[8] + m_Mat[3] * i_A.m_Mat[12], // row 1
    m_Mat[0] * i_A.m_Mat[1] + m_Mat[1] * i_A.m_Mat[5] + m_Mat[2] * i_A.m_Mat[9] + m_Mat[3] * i_A.m_Mat[13],
    m_Mat[0] * i_A.m_Mat[2] + m_Mat[1] * i_A.m_Mat[6] + m_Mat[2] * i_A.m_Mat[10] + m_Mat[3] * i_A.m_Mat[14],
    m_Mat[0] * i_A.m_Mat[3] + m_Mat[1] * i_A.m_Mat[7] + m_Mat[2] * i_A.m_Mat[11] + m_Mat[3] * i_A.m_Mat[15],

    m_Mat[4] * i_A.m_Mat[0] + m_Mat[5] * i_A.m_Mat[4] + m_Mat[6] * i_A.m_Mat[8] + m_Mat[7] * i_A.m_Mat[12], // row 2
    m_Mat[4] * i_A.m_Mat[1] + m_Mat[5] * i_A.m_Mat[5] + m_Mat[6] * i_A.m_Mat[9] + m_Mat[7] * i_A.m_Mat[13],
    m_Mat[4] * i_A.m_Mat[2] + m_Mat[5] * i_A.m_Mat[6] + m_Mat[6] * i_A.m_Mat[10] + m_Mat[7] * i_A.m_Mat[14],
    m_Mat[4] * i_A.m_Mat[3] + m_Mat[5] * i_A.m_Mat[7] + m_Mat[6] * i_A.m_Mat[11] + m_Mat[7] * i_A.m_Mat[15],

    m_Mat[8] * i_A.m_Mat[0] + m_Mat[9] * i_A.m_Mat[4] + m_Mat[10] * i_A.m_Mat[8] + m_Mat[11] * i_A.m_Mat[12], // row 2
    m_Mat[8] * i_A.m_Mat[1] + m_Mat[9] * i_A.m_Mat[5] + m_Mat[10] * i_A.m_Mat[9] + m_Mat[11] * i_A.m_Mat[13],
    m_Mat[8] * i_A.m_Mat[2] + m_Mat[9] * i_A.m_Mat[6] + m_Mat[10] * i_A.m_Mat[10] + m_Mat[11] * i_A.m_Mat[14],
    m_Mat[8] * i_A.m_Mat[3] + m_Mat[9] * i_A.m_Mat[7] + m_Mat[10] * i_A.m_Mat[11] + m_Mat[11] * i_A.m_Mat[15],

    m_Mat[12] * i_A.m_Mat[0] + m_Mat[13] * i_A.m_Mat[4] + m_Mat[14] * i_A.m_Mat[8] + m_Mat[15] * i_A.m_Mat[12], // row 2
    m_Mat[12] * i_A.m_Mat[1] + m_Mat[13] * i_A.m_Mat[5] + m_Mat[14] * i_A.m_Mat[9] + m_Mat[15] * i_A.m_Mat[13],
    m_Mat[12] * i_A.m_Mat[2] + m_Mat[13] * i_A.m_Mat[6] + m_Mat[14] * i_A.m_Mat[10] + m_Mat[15] * i_A.m_Mat[14],
    m_Mat[12] * i_A.m_Mat[3] + m_Mat[13] * i_A.m_Mat[7] + m_Mat[14] * i_A.m_Mat[11] + m_Mat[15] * i_A.m_Mat[15]);
}

//------------------------------------------------------------------------
//	operator: Multiply (*=)
//------------------------------------------------------------------------
void
maMatrix4x4::operator*=(const maMatrix4x4& i_A)
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
maMatrix4x4
maMatrix4x4::operator+(const maMatrix4x4& i_A)
{
  maMatrix4x4 o_NewM(m_Mat[0] + i_A.m_Mat[0],
                     m_Mat[1] + i_A.m_Mat[1],
                     m_Mat[2] + i_A.m_Mat[2],
                     m_Mat[3] + i_A.m_Mat[3],
                     m_Mat[4] + i_A.m_Mat[4],
                     m_Mat[5] + i_A.m_Mat[5],
                     m_Mat[6] + i_A.m_Mat[6],
                     m_Mat[7] + i_A.m_Mat[7],
                     m_Mat[8] + i_A.m_Mat[8],
                     m_Mat[9] + i_A.m_Mat[9],
                     m_Mat[10] + i_A.m_Mat[10],
                     m_Mat[11] + i_A.m_Mat[11],
                     m_Mat[12] + i_A.m_Mat[12],
                     m_Mat[13] + i_A.m_Mat[13],
                     m_Mat[14] + i_A.m_Mat[14],
                     m_Mat[15] + i_A.m_Mat[15]);
  return o_NewM;
}

//------------------------------------------------------------------------
//	operator: Addition (+=)
//------------------------------------------------------------------------
void
maMatrix4x4::operator+=(const maMatrix4x4& i_A)
{
  m_Mat[0] = m_Mat[0] + i_A.m_Mat[0];
  m_Mat[1] = m_Mat[1] + i_A.m_Mat[1];
  m_Mat[2] = m_Mat[2] + i_A.m_Mat[2];
  m_Mat[3] = m_Mat[3] + i_A.m_Mat[3];
  m_Mat[4] = m_Mat[4] + i_A.m_Mat[4];
  m_Mat[5] = m_Mat[5] + i_A.m_Mat[5];
  m_Mat[6] = m_Mat[6] + i_A.m_Mat[6];
  m_Mat[7] = m_Mat[7] + i_A.m_Mat[7];
  m_Mat[8] = m_Mat[8] + i_A.m_Mat[8];
  m_Mat[9] = m_Mat[9] + i_A.m_Mat[9];
  m_Mat[10] = m_Mat[10] + i_A.m_Mat[10];
  m_Mat[11] = m_Mat[11] + i_A.m_Mat[11];
  m_Mat[12] = m_Mat[12] + i_A.m_Mat[12];
  m_Mat[13] = m_Mat[13] + i_A.m_Mat[13];
  m_Mat[14] = m_Mat[14] + i_A.m_Mat[14];
  m_Mat[15] = m_Mat[15] + i_A.m_Mat[15];
}

//------------------------------------------------------------------------
//	operator: subtraction (-)
//------------------------------------------------------------------------
maMatrix4x4
maMatrix4x4::operator-(const maMatrix4x4& i_A)
{
  maMatrix4x4 o_NewM(m_Mat[0] - i_A.m_Mat[0],
                     m_Mat[1] - i_A.m_Mat[1],
                     m_Mat[2] - i_A.m_Mat[2],
                     m_Mat[3] - i_A.m_Mat[3],
                     m_Mat[4] - i_A.m_Mat[4],
                     m_Mat[5] - i_A.m_Mat[5],
                     m_Mat[6] - i_A.m_Mat[6],
                     m_Mat[7] - i_A.m_Mat[7],
                     m_Mat[8] - i_A.m_Mat[8],
                     m_Mat[9] - i_A.m_Mat[9],
                     m_Mat[10] - i_A.m_Mat[10],
                     m_Mat[11] - i_A.m_Mat[11],
                     m_Mat[12] - i_A.m_Mat[12],
                     m_Mat[13] - i_A.m_Mat[13],
                     m_Mat[14] - i_A.m_Mat[14],
                     m_Mat[15] - i_A.m_Mat[15]);
  return o_NewM;
}

//------------------------------------------------------------------------
//	operator: subtraction (-=)
//------------------------------------------------------------------------
void
maMatrix4x4::operator-=(const maMatrix4x4& i_A)
{
  m_Mat[0] = m_Mat[0] - i_A.m_Mat[0];
  m_Mat[1] = m_Mat[1] - i_A.m_Mat[1];
  m_Mat[2] = m_Mat[2] - i_A.m_Mat[2];
  m_Mat[3] = m_Mat[3] - i_A.m_Mat[3];
  m_Mat[4] = m_Mat[4] - i_A.m_Mat[4];
  m_Mat[5] = m_Mat[5] - i_A.m_Mat[5];
  m_Mat[6] = m_Mat[6] - i_A.m_Mat[6];
  m_Mat[7] = m_Mat[7] - i_A.m_Mat[7];
  m_Mat[8] = m_Mat[8] - i_A.m_Mat[8];
  m_Mat[9] = m_Mat[9] - i_A.m_Mat[9];
  m_Mat[10] = m_Mat[10] - i_A.m_Mat[10];
  m_Mat[11] = m_Mat[11] - i_A.m_Mat[11];
  m_Mat[12] = m_Mat[12] - i_A.m_Mat[12];
  m_Mat[13] = m_Mat[13] - i_A.m_Mat[13];
  m_Mat[14] = m_Mat[14] - i_A.m_Mat[14];
  m_Mat[15] = m_Mat[15] - i_A.m_Mat[15];
}

//------------------------------------------------------------------------
//	operator: Assignment (=)
//------------------------------------------------------------------------
maMatrix4x4&
maMatrix4x4::operator=(const maMatrix3x3& i_A)
{
  m_Mat[0] = i_A.m_Mat[0];
  m_Mat[4] = i_A.m_Mat[3];
  m_Mat[8] = i_A.m_Mat[6];
  m_Mat[12] = 0;
  m_Mat[1] = i_A.m_Mat[1];
  m_Mat[5] = i_A.m_Mat[4];
  m_Mat[9] = i_A.m_Mat[7];
  m_Mat[13] = 0;
  m_Mat[2] = i_A.m_Mat[2];
  m_Mat[6] = i_A.m_Mat[5];
  m_Mat[10] = i_A.m_Mat[8];
  m_Mat[14] = 0;
  m_Mat[3] = 0;
  m_Mat[7] = 0;
  m_Mat[11] = 0;
  m_Mat[15] = 1;
  return (*this);
}

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
void
maMatrix4x4::Multiply3x4(maVector4d& i_Vr, const maVector4d& i_V) const
{
  i_Vr.SetX(m_Mat[0] * i_V.GetX() + m_Mat[1] * i_V.GetY() + m_Mat[2] * i_V.GetZ() + m_Mat[3]);
  i_Vr.SetY(m_Mat[4] * i_V.GetX() + m_Mat[5] * i_V.GetY() + m_Mat[6] * i_V.GetZ() + m_Mat[7]);
  i_Vr.SetZ(m_Mat[8] * i_V.GetX() + m_Mat[9] * i_V.GetY() + m_Mat[10] * i_V.GetZ() + m_Mat[11]);
}

//------------------------------------------------------------------------
//	InverseViewport2()
//
//------------------------------------------------------------------------
void
maMatrix4x4::InverseViewport2(const int i_WindowWidth, const int i_WindowHeight)
{
  float WW2 = 2.0f / (i_WindowWidth - VIEWPORT_TOL), WH2 = 2.0f / (i_WindowHeight - VIEWPORT_TOL);
  m_Mat[0] = WW2;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = -1.0;
  m_Mat[4] = 0;
  m_Mat[5] = WH2;
  m_Mat[6] = 0;
  m_Mat[7] = -1.0;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = 2.0;
  m_Mat[11] = -1.0;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

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
void
maMatrix4x4::Viewport2(const int i_WindowWidth, const int i_WindowHeight)
{
  float WW2 = (i_WindowWidth - VIEWPORT_TOL) * 0.5f, WH2 = (i_WindowHeight - VIEWPORT_TOL) * 0.5f;
  m_Mat[0] = WW2;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = WW2;
  m_Mat[4] = 0;
  m_Mat[5] = WH2;
  m_Mat[6] = 0;
  m_Mat[7] = WH2;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = 0.5;
  m_Mat[11] = 0.5;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	InverseLookAt()
//
//------------------------------------------------------------------------
void
maMatrix4x4::InverseLookAt(const maVector3d i_Eye, const maPoint3d i_ViewRefPoint, const maVector3d i_ViewUp)
{
  maVector3d Z = i_Eye - i_ViewRefPoint; // CALC CAM AXES ("/" IS CROSS-PROD)
  Z.Normalize();
  maVector3d X = i_ViewUp / Z;
  X.Normalize();
  maVector3d Y = Z / X;
  Y.Normalize();
  m_Mat[0] = X.GetX();
  m_Mat[1] = Y.GetX();
  m_Mat[2] = Z.GetX();
  m_Mat[3] = i_Eye.GetX(); // ROT->TRANS
  m_Mat[4] = X.GetY();
  m_Mat[5] = Y.GetY();
  m_Mat[6] = Z.GetY();
  m_Mat[7] = i_Eye.GetY();
  m_Mat[8] = X.GetZ();
  m_Mat[9] = Y.GetZ();
  m_Mat[10] = Z.GetZ();
  m_Mat[11] = i_Eye.GetZ();
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	LookAt()
//
//------------------------------------------------------------------------
void
maMatrix4x4::LookAt(const maVector3d i_Eye, const maPoint3d i_ViewRefPoint, const maVector3d i_ViewUp)
{
  maVector3d Z = i_Eye - i_ViewRefPoint; // CALC CAM AXES ("/" IS CROSS-PROD)
  Z.Normalize();
  maVector3d X = i_ViewUp / Z;
  X.Normalize();
  maVector3d Y = Z / X;
  Y.Normalize();
  maVector3d Tr = -i_Eye;
  m_Mat[0] = X.GetX();
  m_Mat[1] = X.GetY();
  m_Mat[2] = X.GetZ();
  m_Mat[3] = X * Tr; // TRANS->ROT
  m_Mat[4] = Y.GetX();
  m_Mat[5] = Y.GetY();
  m_Mat[6] = Y.GetZ();
  m_Mat[7] = Y * Tr;
  m_Mat[8] = Z.GetX();
  m_Mat[9] = Z.GetY();
  m_Mat[10] = Z.GetZ();
  m_Mat[11] = Z * Tr;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	InverseViewport()
//
//------------------------------------------------------------------------
void
maMatrix4x4::InverseViewport(const int i_WindowWidth, const int i_WindowHeight)
{
  float WW2 = 2.0f / (float)i_WindowWidth, WH2 = 2.0f / (float)i_WindowHeight;
  m_Mat[0] = WW2;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = -1.0;
  m_Mat[4] = 0;
  m_Mat[5] = WH2;
  m_Mat[6] = 0;
  m_Mat[7] = -1.0;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = 2.0;
  m_Mat[11] = -1.0;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	Viewport()
//
//------------------------------------------------------------------------
void
maMatrix4x4::Viewport(const int i_WindowWidth, const int i_WindowHeight)
{
  float WW2 = (float)i_WindowWidth * 0.5f, WH2 = (float)i_WindowHeight * 0.5f;
  m_Mat[0] = WW2;
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = WW2;
  m_Mat[4] = 0;
  m_Mat[5] = WH2;
  m_Mat[6] = 0;
  m_Mat[7] = WH2;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = 0.5;
  m_Mat[11] = 0.5;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = 0;
  m_Mat[15] = 1;
}

//------------------------------------------------------------------------
//	InversePerspective()
//
//------------------------------------------------------------------------
void
maMatrix4x4::InversePerspective(float i_Yfov, float i_Aspect, float i_Ndist, float i_Fdist)
{
  i_Yfov *= maConstants::c_fAngleToRad;
  float wT = (float)tan(i_Yfov * 0.5f) * i_Ndist;
  float wB = -wT;
  float wR = wT * i_Aspect, wL = -wR;
  InverseFrustum(wL, wR, wB, wT, i_Ndist, i_Fdist);
}

//------------------------------------------------------------------------
//	Perspective()
//
//------------------------------------------------------------------------
void
maMatrix4x4::Perspective(float i_Yfov, float i_Aspect, float i_Ndist, float i_Fdist)
{
  i_Yfov *= maConstants::c_fAngleToRad;
  float wT = (float)tan(i_Yfov * 0.5f) * i_Ndist;
  float wB = -wT;
  float wR = wT * i_Aspect, wL = -wR;

  Frustum(wL, wR, wB, wT, i_Ndist, i_Fdist);
}

//------------------------------------------------------------------------
//	InverseFrustum()
//
//------------------------------------------------------------------------
void
maMatrix4x4::InverseFrustum(float i_l, float i_r, float i_b, float i_t, float i_n, float i_f)
{
  m_Mat[0] = (i_r - i_l) / (2 * i_n);
  m_Mat[1] = 0;
  m_Mat[2] = 0;
  m_Mat[3] = (i_r + i_l) / (2 * i_n);
  m_Mat[4] = 0;
  m_Mat[5] = (i_t - i_b) / (2 * i_n);
  m_Mat[6] = 0;
  m_Mat[7] = (i_t + i_b) / (2 * i_n);
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = 0;
  m_Mat[11] = -1;
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = -(i_f - i_n) / (2 * i_f * i_n);
  m_Mat[15] = (i_f + i_n) / (2 * i_f * i_n);
}

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
void
maMatrix4x4::Frustum(float i_l, float i_r, float i_b, float i_t, float i_n, float i_f)
{
  m_Mat[0] = (2 * i_n) / (i_r - i_l);
  m_Mat[1] = 0;
  m_Mat[2] = (i_r + i_l) / (i_r - i_l);
  m_Mat[3] = 0;
  m_Mat[4] = 0;
  m_Mat[5] = (2 * i_n) / (i_t - i_b);
  m_Mat[6] = (i_t + i_b) / (i_t - i_b);
  m_Mat[7] = 0;
  m_Mat[8] = 0;
  m_Mat[9] = 0;
  m_Mat[10] = -i_f / (i_f - i_n);
  m_Mat[11] = -(i_f * i_n) / (i_f - i_n);
  m_Mat[12] = 0;
  m_Mat[13] = 0;
  m_Mat[14] = -1;
  m_Mat[15] = 0;
}

//------------------------------------------------------------------------
//	RotateToFacePoint()
//------------------------------------------------------------------------
void
maMatrix4x4::RotateToFacePoint(maPoint3d i_RotatePoint, maVector3d i_RotateNormal, maPoint3d i_ToPoint)
{
  // Gimble
  // Get rotation axis  (axis of rotation is cross product of lookat and up vector)
  maVector3d LookAt(i_ToPoint - i_RotatePoint);
  LookAt.Normalize(); // (for angle)
  maVector3d Axis(LookAt / i_RotateNormal);
  // Angle is  sine(length_of_cross_product)   (both vectors are normalized)
  float VLength = Axis.Length();
  float RadAng = (float)asin(VLength); // angle in radians

  maMatrix4x4 NM;
  float ca = (float)cos(RadAng);
  float sa = (float)sin(RadAng);

  if (Axis.GetX() == 1 && Axis.GetY() == 0 && Axis.GetZ() == 0) // ABOUT X-AXIS
  {
    NM.MakeRotateX(RadAng);
  } else if (Axis.GetX() == 0 && Axis.GetY() == 1 && Axis.GetZ() == 0) // ABOUT Y-AXIS
  {
    NM.MakeRotateY(RadAng);
  } else if (Axis.GetX() == 0 && Axis.GetY() == 0 && Axis.GetZ() == 1) // ABOUT Z-AXIS
  {
    NM.MakeRotateZ(RadAng);
  } else // ARBITRARY AXIS
  {
    Axis.Normalize();
    float x = Axis.GetX(), y = Axis.GetY(), z = Axis.GetZ();
    float x2 = x * x, y2 = y * y, z2 = z * z;
    NM.m_Mat[0] = x2 + ca * (1 - x2);
    NM.m_Mat[4] = (x * y) + ca * (-x * y) + sa * (-z);
    NM.m_Mat[8] = (x * z) + ca * (-x * z) + sa * y;
    NM.m_Mat[1] = (x * y) + ca * (-x * y) + sa * z;
    NM.m_Mat[5] = y2 + ca * (1 - y2);
    NM.m_Mat[9] = (y * z) + ca * (-y * z) + sa * (-x);
    NM.m_Mat[2] = (x * z) + ca * (-x * z) + sa * (-y);
    NM.m_Mat[6] = (y * z) + ca * (-y * z) + sa * x;
    NM.m_Mat[10] = z2 + ca * (1 - z2);
    NM.m_Mat[12] = NM.m_Mat[13] = NM.m_Mat[14] = NM.m_Mat[3] = NM.m_Mat[7] = NM.m_Mat[11] = 0;
    NM.m_Mat[15] = 1;
  }

  *this = *this * NM;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool
maMatrix4x4::operator==(const maMatrix4x4& i_A) const
{
  return ((m_Mat[0] == i_A.m_Mat[0]) && (m_Mat[1] == i_A.m_Mat[1]) && (m_Mat[2] == i_A.m_Mat[2]) &&
          (m_Mat[3] == i_A.m_Mat[3]) && (m_Mat[4] == i_A.m_Mat[4]) && (m_Mat[5] == i_A.m_Mat[5]) &&
          (m_Mat[6] == i_A.m_Mat[6]) && (m_Mat[7] == i_A.m_Mat[7]) && (m_Mat[8] == i_A.m_Mat[8]) &&
          (m_Mat[9] == i_A.m_Mat[9]) && (m_Mat[10] == i_A.m_Mat[10]) && (m_Mat[11] == i_A.m_Mat[11]) &&
          (m_Mat[12] == i_A.m_Mat[12]) && (m_Mat[13] == i_A.m_Mat[13]) && (m_Mat[14] == i_A.m_Mat[14]) &&
          (m_Mat[15] == i_A.m_Mat[15]));
}
