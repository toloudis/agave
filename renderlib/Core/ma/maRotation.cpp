/****************************************************************************\
**  maRotation.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maRotation.hpp"
#include "Core/ma/maConstants.hpp"
#include "Core/ma/maFunctions.hpp"

//------------------------------------------------------------------------
//	This constructor makes a rotation about the given
//	axis for the given angle (in radians).
//------------------------------------------------------------------------
maRotation::maRotation(const maVector3d& i_Axis, float i_Angle)
{
  this->SetValue(i_Axis, i_Angle);
}

//------------------------------------------------------------------------
//	SetValue makes a rotation about the given
//	axis for the given angle (in radians).
//------------------------------------------------------------------------
void
maRotation::SetValue(const maVector3d& i_Axis, float i_Angle)
{
  double theta_div2 = double(i_Angle) * 0.5;
  float s = float(sin(theta_div2));
  float c = float(cos(theta_div2));
  maVector3d axis = i_Axis;
  float axis_len = axis.LengthSqr();

  if (axis_len != 0.0f)
    axis /= sqrtf(axis_len);

  m_X = axis.m_X * s;
  m_Y = axis.m_Y * s;
  m_Z = axis.m_Z * s;
  m_W = c;
}

maRotation::maRotation(const maMatrix3x3& iRot)
{
  this->SetValue(iRot);
}

//------------------------------------------------------------------------
//	SetValue()
//
// Set value from rotation matrix
//------------------------------------------------------------------------
void
maRotation::SetValue(const maMatrix3x3& i_Matrix)
{
  float tr, s, q[4];
  int i, j, k;

  int nxt[3] = { 1, 2, 0 };

  tr = i_Matrix(0, 0) + i_Matrix(1, 1) + i_Matrix(2, 2);

  // check the diagonal
  if (tr > 0.0) {
    s = float(sqrt(tr + 1.0));
    m_W = s / 2.0f;
    s = 0.5f / s;
    m_X = (i_Matrix(1, 2) - i_Matrix(2, 1)) * s;
    m_Y = (i_Matrix(2, 0) - i_Matrix(0, 2)) * s;
    m_Z = (i_Matrix(0, 1) - i_Matrix(1, 0)) * s;
  } else {
    // diagonal is negative
    i = 0;

    if (i_Matrix(1, 1) > i_Matrix(0, 0))
      i = 1;

    if (i_Matrix(2, 2) > i_Matrix(i, i))
      i = 2;

    j = nxt[i];
    k = nxt[j];

    s = sqrtf((i_Matrix(i, i) - (i_Matrix(j, j) + i_Matrix(k, k))) + 1.0f);

    q[i] = s * 0.5f;

    if (s != 0.0f)
      s = 0.5f / s;

    q[3] = (i_Matrix(j, k) - i_Matrix(k, j)) * s;
    q[j] = (i_Matrix(i, j) + i_Matrix(j, i)) * s;
    q[k] = (i_Matrix(i, k) + i_Matrix(k, i)) * s;

    m_X = q[0];
    m_Y = q[1];
    m_Z = q[2];
    m_W = q[3];
  }
}

//------------------------------------------------------------------------
//	SetValue()
//
// Set value needed to rotate "from" vector to "to" vector
//------------------------------------------------------------------------
void
maRotation::SetValue(const maVector3d& i_From, const maVector3d& i_To)
{
  maVector3d from = i_From;
  maVector3d to = i_To;

  from.Normalize();
  to.Normalize();
  maVector3d cross = from.Cross(to);
  float dot = from * to;

  // From and to vectors are opposite
  if (dot < -0.9999f) {
    cross = from.Cross(maVector3d(1, 0, 0));

    // The from and x vector are the same,
    // so use the z vector
    if (cross.LengthSqr() < 0.0001f) {
      cross = from.Cross(maVector3d(0, 0, 1));
    }

    SetValue(cross, maConstants::c_fPI);
    return;
  }

  float s = float(sqrt((dot + 1.0f) * 2.0f));
  m_X = cross.m_X / s;
  m_Y = cross.m_Y / s;
  m_Z = cross.m_Z / s;
  m_W = s * 0.5f;
}

//------------------------------------------------------------------------
//	Set rotation through 3 euler angles
//------------------------------------------------------------------------
void
maRotation::SetEuler(float i_X, float i_Y, float i_Z)
{
  // safe, but slow way
  maRotation rotx(maVector3d(1, 0, 0), i_X);
  maRotation roty(maVector3d(0, 1, 0), i_Y);
  maRotation rotz(maVector3d(0, 0, 1), i_Z);

  *this = (rotz * roty * rotx);
}

//------------------------------------------------------------------------
//	Get the euler angles out of the rotation
//------------------------------------------------------------------------
void
maRotation::GetEuler(float& o_X, float& o_Y, float& o_Z) const
{
  //    float sqw = m_W*m_W;
  float sqx = m_X * m_X;
  float sqy = m_Y * m_Y;
  float sqz = m_Z * m_Z;

  // These were the original equations that were found on the net (several sources)
  // These did not work correctly for some reason.  As you can see by the comments
  // on the end of the line and the variable name at the begining don't jibe.
  // I am not sure exactly why this is because as far as I could tell the axis
  // orientation in the examples are the same as the ones we are using.
  //	o_Roll = float(atan2f(2.0 * (m_X*m_Y + m_Z*m_W),(sqw + sqx - sqy - sqz)));  // should be yaw
  //	o_Pitch = float(atan2f(2.0 * (m_Y*m_Z + m_X*m_W),(sqw - sqx - sqy + sqz )));  // should be roll
  //	o_Yaw = float(asin(-2.0 * (m_X*m_Z - m_Y*m_W))); // should be pitch

  // These were the equations I came up with that actually returned values as they should.
  // For the most part I constructed these by recombining the original equations.
  // I am not sure they are 100% correct but the values that they are returning are
  // as far as I can tell accurate.
  //	o_Yaw = atan2f(2.0f * (m_X*m_Z + m_Y*m_W),(sqw - sqx - sqy + sqz));  // should be yaw
  //	o_Roll = atan2f(2.0f * (m_X*m_Y + m_Z*m_W),( sqw - sqx + sqy - sqz));  // should be roll
  //	o_Pitch = asinf(2.0f * (m_Y*m_Z - m_X*m_W)); // should be pitch

  // Here is a formula I found from Wikipedia:
  o_X = atan2f(2.0f * (m_W * m_X + m_Y * m_Z), (1.0f - 2.0f * (sqx + sqy)));

  // Need to clamp to -1 to 1 in order to avoid the NaN returned from asinf
  float sin_y = 2.0f * (m_W * m_Y - m_Z * m_X);
  maFunctions::Clamp(sin_y, -1.0f, 1.0f);
  o_Y = asinf(sin_y);

  o_Z = atan2f(2.0f * (m_W * m_Z + m_X * m_Y), (1.0f - 2.0f * (sqy + sqz)));
}

//------------------------------------------------------------------------
//	GetValue()
//
// Get value as axis and angle
//------------------------------------------------------------------------
void
maRotation::GetValue(maVector3d& o_Axis, float& o_Angle) const
{
  o_Axis.m_X = m_X;
  o_Axis.m_Y = m_Y;
  o_Axis.m_Z = m_Z;
  o_Axis.Normalize();
  o_Angle = 2.0f * float(acos(m_W));
}

//------------------------------------------------------------------------
//	GetMatrix()
//
// Get value as rotation matrix
//------------------------------------------------------------------------
maMatrix4x4
maRotation::GetMatrix() const
{
  maMatrix4x4 ret_val;

  float xx = m_X * m_X;
  float xy = m_X * m_Y;
  float xz = m_X * m_Z;
  float xw = m_X * m_W;

  float yy = m_Y * m_Y;
  float yz = m_Y * m_Z;
  float yw = m_Y * m_W;

  float zz = m_Z * m_Z;
  float zw = m_Z * m_W;

  ret_val(0, 0) = 1 - 2 * (yy + zz);
  ret_val(1, 0) = 2 * (xy - zw);
  ret_val(2, 0) = 2 * (xz + yw);

  ret_val(0, 1) = 2 * (xy + zw);
  ret_val(1, 1) = 1 - 2 * (xx + zz);
  ret_val(2, 1) = 2 * (yz - xw);

  ret_val(0, 2) = 2 * (xz - yw);
  ret_val(1, 2) = 2 * (yz + xw);
  ret_val(2, 2) = 1 - 2 * (xx + yy);

  //	ret_val.Transpose();

  //	because the maMatrix4x4 default constructor initializes to the
  //	identity, we don't need to worry about setting the other rows
  //	and so on.  We are done.
  //
  return ret_val;
}

//------------------------------------------------------------------------
//	RotateVector()
//
// Transform a Vector through Rotation
//------------------------------------------------------------------------
void
maRotation::RotateVector(maVector3d& io_Vector) const
{
  maRotation q(io_Vector);
  maRotation inverse(*this);
  inverse.Invert();

  maRotation ret_val(*this);

  ret_val *= q;
  ret_val *= inverse;

  io_Vector.m_X = ret_val.m_X;
  io_Vector.m_Y = ret_val.m_Y;
  io_Vector.m_Z = ret_val.m_Z;
}

//------------------------------------------------------------------------
//	Addition of quaternions is done by adding their components.
//------------------------------------------------------------------------
void
maRotation::operator+=(const maRotation& i_Rot)
{
  m_X += i_Rot.m_X;
  m_Y += i_Rot.m_Y;
  m_Z += i_Rot.m_Z;
  m_W += i_Rot.m_W;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
maRotation::Normalize()
{
  double len = m_X * m_X + m_Y * m_Y + m_Z * m_Z + m_W * m_W;

  if (len < 0.0000001)
    return;

  float flen = float(1.0 / sqrt(len));
  m_X *= flen;
  m_Y *= flen;
  m_Z *= flen;
  m_W *= flen;
}

//------------------------------------------------------------------------
//	operator * ()
//
// Multiplication of two rotations, combining rotations
//------------------------------------------------------------------------
maRotation
maRotation::operator*(const maRotation& i_Rot) const
{
  maRotation ret_val = *this;
  ret_val *= i_Rot;
  return ret_val;
}

//------------------------------------------------------------------------
//	operator *=
//
// Multiplication of two rotations, combining rotations
//------------------------------------------------------------------------
void
maRotation::operator*=(const maRotation& i_Rot)
{
  maVector3d u(m_X, m_Y, m_Z);
  maVector3d v(i_Rot.m_X, i_Rot.m_Y, i_Rot.m_Z);

  maVector3d vec = v * m_W + u * i_Rot.m_W + u.Cross(v);
  m_W = m_W * i_Rot.m_W - u * v;
  m_X = vec.m_X;
  m_Y = vec.m_Y;
  m_Z = vec.m_Z;
}

//------------------------------------------------------------------------
//	Slerp()
//
// Spherical linear interpolation
// "this" becomes a rotation blend between i_Rot1 and i_Rot2
// depending on parameter i_T. If a==0.0, return i_Rot1.
// If a==1.0, return i_Rot2.
//------------------------------------------------------------------------
void
maRotation::Slerp(const maRotation& i_Rot1, const maRotation& i_Rot2, float i_T)
{
  float to1[4];
  double omega, cosom, sinom, scale0, scale1;

  // calc cosine
  cosom = i_Rot1.m_X * i_Rot2.m_X + i_Rot1.m_Y * i_Rot2.m_Y + i_Rot1.m_Z * i_Rot2.m_Z + i_Rot1.m_W * i_Rot2.m_W;

  // adjust signs (if necessary)
  if (cosom < 0.0f) {
    cosom = -cosom;
    to1[0] = -i_Rot2.m_X;
    to1[1] = -i_Rot2.m_Y;
    to1[2] = -i_Rot2.m_Z;
    to1[3] = -i_Rot2.m_W;
  } else {
    to1[0] = i_Rot2.m_X;
    to1[1] = i_Rot2.m_Y;
    to1[2] = i_Rot2.m_Z;
    to1[3] = i_Rot2.m_W;
  }

  // calculate coefficients
  if ((1.0 - cosom) > 0.0001f) {
    // standard case (slerp)
    omega = acos(cosom);
    sinom = sin(omega);
    scale0 = sin((1.0 - i_T) * omega) / sinom;
    scale1 = sin(i_T * omega) / sinom;
  } else {
    // "from" and "to" quaternions are very close
    //  ... so we can do a linear interpolation
    scale0 = 1.0 - i_T;
    scale1 = i_T;
  }

  // calculate final values
  m_X = float(scale0 * i_Rot1.m_X + scale1 * to1[0]);
  m_Y = float(scale0 * i_Rot1.m_Y + scale1 * to1[1]);
  m_Z = float(scale0 * i_Rot1.m_Z + scale1 * to1[2]);
  m_W = float(scale0 * i_Rot1.m_W + scale1 * to1[3]);
}

//------------------------------------------------------------------------
//	ScaleAngle()
//
// Scale our rotation by given factor, producing some
// portion or mutiple of our rotation
//------------------------------------------------------------------------
void
maRotation::ScaleAngle(float i_ScaleFactor)
{
  maVector3d axis;
  float angle;

  this->GetValue(axis, angle);
  angle *= i_ScaleFactor;
  this->SetValue(axis, angle);
}

//------------------------------------------------------------------------
//	Invert()
//
// Changes a rotation to be its inverse
//------------------------------------------------------------------------
void
maRotation::Invert()
{
  m_X = -m_X;
  m_Y = -m_Y;
  m_Z = -m_Z;
}

//------------------------------------------------------------------------
// Exponential
//------------------------------------------------------------------------
maRotation
maRotation::Exp() const
{
  maVector3d v(m_X, m_Y, m_Z);
  double theta = v * v;
  double sin_theta = sin(theta);

  double W = cos(theta);
  if (fabs(sin_theta) > maConstants::c_fEpsilon) {
    v *= sin_theta / theta;
  }
  return maRotation(v.m_X, v.m_Y, v.m_Z, W);
}
//------------------------------------------------------------------------
// Power
//------------------------------------------------------------------------
maRotation
maRotation::Pow(float t) const
{
  maRotation q = (this->Log() * t).Exp();
  return q;
}
//------------------------------------------------------------------------
// Logarithm
//------------------------------------------------------------------------
maRotation
maRotation::Log() const
{
  double theta = acos(m_W);
  double sin_theta = sin(theta);

  maVector3d v(m_X, m_Y, m_Z);
  if (fabs(sin_theta) > maConstants::c_fEpsilon) {
    v *= sin_theta / theta;
  }
  return maRotation(v.m_X, v.m_Y, v.m_Z, 0);
}

//------------------------------------------------------------------------
// Squad - Spherical Cubic Interpolation
//	The inner quaternions i_A and i_B need to be computed from the
//	tangents at the control points.
//------------------------------------------------------------------------
// static
maRotation
maRotation::Squad(const maRotation& i_P, const maRotation& i_A, const maRotation& i_B, const maRotation& i_Q, float i_T)
{
  maRotation c, d, res;
  c.Slerp(i_P, i_Q, i_T);
  d.Slerp(i_A, i_B, i_T);
  res.Slerp(c, d, 2 * i_T * (1 - i_T));
  return res;
}

//------------------------------------------------------------------------
// ComputeInnerPoint - Compute an inner point for Squad() based
//	on neighboring quaternions
//------------------------------------------------------------------------
// static
maRotation
maRotation::ComputeInnerPoint(const maRotation& i_A, const maRotation& i_B, const maRotation& i_C)
{
  // q(i) = a(i) exp( -(log(a(i+1)*a(i).Inverse()) + log(a(i-1)*a(i).Inverse())) / 4 )
  maRotation b_i(i_B);
  b_i.Invert();

  maRotation log1 = (i_C * b_i).Log();
  maRotation log2 = (i_A * b_i).Log();
  return i_B * ((log1 * log2) * -0.25f).Exp();
}
