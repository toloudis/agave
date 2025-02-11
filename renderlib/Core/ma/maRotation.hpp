/****************************************************************************\
**  maRotation.hpp
**
**      A maRotation is a pure 3D rotation transformation.
**
**
**
\****************************************************************************/

#ifdef MA_ROTATION_HPP
#error maRotation.hpp multiply included
#endif
#define MA_ROTATION_HPP

#ifndef MA_MATRIX3X3_HPP
#include "Core/ma/maMatrix3x3.hpp"
#endif

#ifndef MA_MATRIX4X4_HPP
#include "Core/ma/maMatrix4x4.hpp"
#endif

//============================================================================
//============================================================================
class maRotation
{
public:
  //------------------------------------------------------------------------
  //	The default constructor initializes the rotation to an identity
  //	rotation.
  //------------------------------------------------------------------------
  maRotation();

  //------------------------------------------------------------------------
  //	Copy constructor.
  //------------------------------------------------------------------------
  maRotation(const maRotation& i_CopyFrom);

  //------------------------------------------------------------------------
  //	This constructor makes the quaternion [0, xyz] from the given
  //	vector.
  //------------------------------------------------------------------------
  maRotation(const maVector3d& i_Vector);

  //------------------------------------------------------------------------
  //	This constructor makes a rotation about the given
  //	axis for the given angle (in radians).  The axis does not have to
  //	be normalized.
  //------------------------------------------------------------------------
  maRotation(const maVector3d& i_Axis, float i_Angle);

  maRotation(const maMatrix3x3& iRot);

  //------------------------------------------------------------------------
  //	This constructor sets the quaternion values.  Make sure you call
  //	Normalize if you are not sure that the values produce a unit
  //	quaternion.
  //------------------------------------------------------------------------
  maRotation(float i_X, float i_Y, float i_Z, float i_W);

  //------------------------------------------------------------------------
  //	This constructor sets rotation from euler angles.
  //------------------------------------------------------------------------
  maRotation(float i_X, float i_Y, float i_Z);

  //------------------------------------------------------------------------
  //	Identity makes a zero rotation (no transformation).
  //------------------------------------------------------------------------
  inline void Identity();

  //------------------------------------------------------------------------
  //	SetValue makes a rotation about the given
  //	axis for the given angle (in radians).  The axis does not
  //	have to be normalized.
  //------------------------------------------------------------------------
  void SetValue(const maVector3d& i_Axis, float i_Angle);

  //------------------------------------------------------------------------
  //	SetValue()
  //
  // Set value from rotation matrix
  //------------------------------------------------------------------------
  void SetValue(const maMatrix4x4& i_Matrix);

  //------------------------------------------------------------------------
  //	SetValue()
  //
  // Set value from rotation matrix
  //------------------------------------------------------------------------
  void SetValue(const maMatrix3x3& i_Matrix);

  //------------------------------------------------------------------------
  //	SetValue()
  //
  // Set value needed to rotate "from" vector to "to" vector
  //------------------------------------------------------------------------
  void SetValue(const maVector3d& i_From, const maVector3d& i_To);

  //------------------------------------------------------------------------
  //	Set quaternion values directly.  Make sure you call
  //	Normalize if you are not sure that the values produce a unit
  //	quaternion.
  //------------------------------------------------------------------------
  void Set(float i_X, float i_Y, float i_Z, float i_W);

  //------------------------------------------------------------------------
  //	Set rotation through 3 euler angles
  //------------------------------------------------------------------------
  void SetEuler(float i_X, float i_Y, float i_Z);

  //------------------------------------------------------------------------
  //	Get the euler angles out of the rotation
  //------------------------------------------------------------------------
  void GetEuler(float& o_X, float& o_Y, float& o_Z) const;

  //------------------------------------------------------------------------
  //	GetValue()
  //
  // Get value as axis and angle
  //------------------------------------------------------------------------
  void GetValue(maVector3d& o_Axis, float& o_Angle) const;

  //------------------------------------------------------------------------
  //	GetMatrix()
  //
  // Get value as rotation matrix
  //------------------------------------------------------------------------
  maMatrix4x4 GetMatrix() const;

  //------------------------------------------------------------------------
  //	GetMatrix()
  //
  // Get value as rotation matrix
  //------------------------------------------------------------------------
  maMatrix3x3 GetMatrix3x3() const;

  //------------------------------------------------------------------------
  //	RotateVector()
  //
  // Transform a Vector through Rotation
  //------------------------------------------------------------------------
  void RotateVector(maVector3d& io_Vector) const;

  //------------------------------------------------------------------------
  //	operator *
  //
  // Multiplication of two rotations, combining rotations
  //------------------------------------------------------------------------
  maRotation operator*(const maRotation& i_Rot) const;

  //------------------------------------------------------------------------
  //	operator *=
  //
  // Multiplication of two rotations, combining rotations
  //------------------------------------------------------------------------
  void operator*=(const maRotation& i_Rot);

  //------------------------------------------------------------------------
  //	operator *=
  //	Multiplication by a scalar is done by multiplying each component.
  //------------------------------------------------------------------------
  void operator*=(float i_Val);

  //------------------------------------------------------------------------
  //	Addition of quaternions is done by adding their components.
  //------------------------------------------------------------------------
  void operator+=(const maRotation& i_Rot);

  //------------------------------------------------------------------------
  //	comparison operators
  //------------------------------------------------------------------------
  inline bool operator==(const maRotation& i_Rot) const;
  inline bool operator!=(const maRotation& i_Rot) const;

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  inline friend maRotation operator*(const maRotation& i_Rot, float i_s);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  inline friend maRotation operator+(const maRotation& i_Rot1, const maRotation& i_Rot2);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  void Normalize();

  //------------------------------------------------------------------------
  //	Slerp()
  //
  // Spherical linear interpolation
  // "this" becomes a rotation blend between i_Rot1 and i_Rot2
  // depending on parameter i_T. If a==0.0, return i_Rot1.
  // If a==1.0, return i_Rot2.
  //------------------------------------------------------------------------
  void Slerp(const maRotation& i_Rot1, const maRotation& i_Rot2, float i_T);

  //------------------------------------------------------------------------
  //	ScaleAngle()
  //
  // Scale our rotation by given factor, producing some
  // portion or mutiple of our rotation
  //------------------------------------------------------------------------
  void ScaleAngle(float i_ScaleFactor);

  //------------------------------------------------------------------------
  //	Invert() - Changes a rotation to be its inverse
  //------------------------------------------------------------------------
  void Invert();

  //------------------------------------------------------------------------
  //	GetInverse()
  //
  //	Returns the inverse of a rotation
  //------------------------------------------------------------------------
  inline maRotation GetInverse() const;

  //------------------------------------------------------------------------
  // Easy member access
  //------------------------------------------------------------------------
  inline float GetX() const;
  inline float GetY() const;
  inline float GetZ() const;
  inline float GetW() const;

  //--------------------------------------------------------------------------------
  // The following functions (Exp, Pow, Log, Squad) come from:
  // http://ugweb.cs.ualberta.ca/~c412/tekkotsu/roboop/quaternion_8cpp-source.html
  // and are not tested. Please use as reference only now. Or test them.
  //--------------------------------------------------------------------------------

  //------------------------------------------------------------------------
  // Exponential, Power, Logarithm - used for computing tangents,
  //	cubic spherical interpolation
  //------------------------------------------------------------------------
  maRotation Exp() const;
  maRotation Pow(float i_T) const;
  maRotation Log() const;

  //------------------------------------------------------------------------
  // Squad - Spherical Cubic Interpolation
  //	The inner quaternions i_A and i_B need to be computed from the
  //	tangents at the control points.
  //------------------------------------------------------------------------
  static maRotation Squad(const maRotation& i_P,
                          const maRotation& i_A,
                          const maRotation& i_B,
                          const maRotation& i_Q,
                          float i_T);

  //------------------------------------------------------------------------
  // ComputeInnerPoint - Compute an inner point for Squad() based
  //	on neighboring quaternions
  //------------------------------------------------------------------------
  static maRotation ComputeInnerPoint(const maRotation& i_A, const maRotation& i_B, const maRotation& i_C);

private:
  float m_X, m_Y, m_Z, m_W;
};

//------------------------------------------------------------------------
//	This constructor makes the quaternion [0, xyz] from the given
//	vector.
//------------------------------------------------------------------------
inline maRotation::maRotation(const maVector3d& i_Vector)
  : m_X(i_Vector.m_X)
  , m_Y(i_Vector.m_Y)
  , m_Z(i_Vector.m_Z)
  , m_W(0.0f)
{
}

//------------------------------------------------------------------------
//	default and copy constructors
//------------------------------------------------------------------------
inline maRotation::maRotation()
  : m_X(0)
  , m_Y(0)
  , m_Z(0)
  , m_W(1)
{
}

inline maRotation::maRotation(const maRotation& i_CopyFrom)
  : m_X(i_CopyFrom.m_X)
  , m_Y(i_CopyFrom.m_Y)
  , m_Z(i_CopyFrom.m_Z)
  , m_W(i_CopyFrom.m_W)
{
}

//------------------------------------------------------------------------
//	This constructor sets the quaternion values.
//------------------------------------------------------------------------
inline maRotation::maRotation(float i_X, float i_Y, float i_Z, float i_W)
  : m_X(i_X)
  , m_Y(i_Y)
  , m_Z(i_Z)
  , m_W(i_W)
{
}

//------------------------------------------------------------------------
//	This constructor sets rotation from euler angles.
//------------------------------------------------------------------------
inline maRotation::maRotation(float i_X, float i_Y, float i_Z)
{
  SetEuler(i_X, i_Y, i_Z);
}

//------------------------------------------------------------------------
//	SetValue()
//
// Set value from rotation matrix
//------------------------------------------------------------------------
inline void
maRotation::SetValue(const maMatrix4x4& i_Matrix)
{
  this->SetValue(i_Matrix.GetSubMatrix(3, 3));
}

//------------------------------------------------------------------------
//	GetMatrix()
//
// Get value as rotation matrix
//------------------------------------------------------------------------
inline maMatrix3x3
maRotation::GetMatrix3x3() const
{
  return this->GetMatrix().GetSubMatrix(3, 3);
}

//------------------------------------------------------------------------
//	GetInverse()
//
//	Returns the inverse of a rotation
//------------------------------------------------------------------------
inline maRotation
maRotation::GetInverse() const
{
  maRotation ret_val = *this;
  ret_val.Invert();
  return ret_val;
}

//------------------------------------------------------------------------
//	Identity()
//
//------------------------------------------------------------------------
inline void
maRotation::Identity()
{
  m_X = 0.0f;
  m_Y = 0.0f;
  m_Z = 0.0f;
  m_W = 1.0f;
}

//------------------------------------------------------------------------
//	Set quaternion values directly
//------------------------------------------------------------------------
inline void
maRotation::Set(float i_X, float i_Y, float i_Z, float i_W)
{
  m_X = i_X;
  m_Y = i_Y;
  m_Z = i_Z;
  m_W = i_W;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline bool
maRotation::operator==(const maRotation& i_Rot) const
{
  return (m_X == i_Rot.m_X) && (m_Y == i_Rot.m_Y) && (m_Z == i_Rot.m_Z) && (m_W == i_Rot.m_W);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline bool
maRotation::operator!=(const maRotation& i_Rot) const
{
  return (m_X != i_Rot.m_X) || (m_Y != i_Rot.m_Y) || (m_Z != i_Rot.m_Z) || (m_W != i_Rot.m_W);
}

//------------------------------------------------------------------------
//	operator *=
//	Multiplication by a scalar is done by multiplying each component.
//------------------------------------------------------------------------
inline void
maRotation::operator*=(float i_Val)
{
  m_X *= i_Val;
  m_Y *= i_Val;
  m_Z *= i_Val;
  m_W *= i_Val;
}

//------------------------------------------------------------------------
//	free operator functions
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline maRotation
operator*(const maRotation& i_Rot, float i_s)
{
  return maRotation(i_Rot.m_X * i_s, i_Rot.m_Y * i_s, i_Rot.m_Z * i_s, i_Rot.m_W * i_s);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline maRotation
operator+(const maRotation& i_Rot1, const maRotation& i_Rot2)
{
  return maRotation(i_Rot1.m_X + i_Rot2.m_X, i_Rot1.m_Y + i_Rot2.m_Y, i_Rot1.m_Z * i_Rot2.m_Z, i_Rot1.m_W * i_Rot2.m_W);
}

//------------------------------------------------------------------------
// Easy member access
//------------------------------------------------------------------------
inline float
maRotation::GetX() const
{
  return m_X;
}
inline float
maRotation::GetY() const
{
  return m_Y;
}
inline float
maRotation::GetZ() const
{
  return m_Z;
}
inline float
maRotation::GetW() const
{
  return m_W;
}
