/****************************************************************************\
**	prtyPoint3dTransform.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyPoint3dTransform.hpp"
#include "Core/prty/prtyUnits.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyPoint3dTransform::prtyPoint3dTransform()
  : prtyPoint3d("Point3d", maPoint3d(0, 0, 0))
  , m_WorldPosition(0, 0, 0)
  , m_bIsWorldSpace(false)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyPoint3dTransform::prtyPoint3dTransform(const std::string& i_Name)
  : prtyPoint3d(i_Name, maPoint3d(0, 0, 0))
  , m_WorldPosition(0, 0, 0)
  , m_bIsWorldSpace(false)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyPoint3dTransform::prtyPoint3dTransform(const std::string& i_Name, const maPoint3d& i_InitialValue)
  : prtyPoint3d(i_Name, i_InitialValue)
  , m_WorldPosition(i_InitialValue)
  , m_bIsWorldSpace(false)
{
}

//----------------------------------------------------------------------------
// Set value of property. The boolean flag is true
//	if the change is coming from the user interface and therefore
//	should mark the document containing the property as dirty.
//----------------------------------------------------------------------------
void
prtyPoint3dTransform::SetValue(const maPoint3d& i_Value, bool i_bDirty)
{
  // Calls to SetValue are setting the object space position.

  // If we were in world space, switch to object space transformation
  if (m_bIsWorldSpace) {
    m_bIsWorldSpace = false;
    m_Matrix.Invert();
  }

  // Set object space position directly and convert to world space position
  if (m_Value != i_Value) {
    m_Value = i_Value;

    m_WorldPosition = this->GetValue();
    m_Matrix.Transform(m_WorldPosition);

    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}

//--------------------------------------------------------------------
// Some prtyPoint3dTransform properties contain a transformation so that
// you can set and get the position in world space or object space.
// The traditional Get/SetValue functions would return the object
// space properties and these function return the world space values.
// Default implementation of these functions is to treat the
// object and world space positions as the same value.
//--------------------------------------------------------------------
// bool prtyPoint3dTransform::HasWorldSpace() const
//{
//	// All properties of this type have the potential to be in world space,
//	// so return true here.
//	return true;
//}
maPoint3d
prtyPoint3dTransform::GetWorldSpaceValue() const
{
  return m_WorldPosition;
}
void
prtyPoint3dTransform::SetWorldSpaceValue(const maPoint3d& i_Value, bool i_bDirty)
{
  // Calls to SetWorldSpaceValue are setting the world space position.

  // If we were in object space, switch to world space transformation
  if (!m_bIsWorldSpace) {
    m_bIsWorldSpace = true;
    m_Matrix.Invert();
  }

  // Set world space position directly and convert to object space position
  if (m_WorldPosition != i_Value) {
    m_WorldPosition = i_Value;

    maPoint3d object_space = m_WorldPosition;
    m_Matrix.Transform(object_space);
    set_object_space_value(object_space, i_bDirty);
  }
}

//--------------------------------------------------------------------
// Set transformation matrix from object space to world space
//--------------------------------------------------------------------
void
prtyPoint3dTransform::SetTransformation(const maMatrix4x4& i_Matrix)
{
  // Update our transformation matrix
  m_Matrix = i_Matrix;

  // If we are in world space already, then invert the matrix so that
  // it converts from world space to object space.
  if (m_bIsWorldSpace) {
    m_Matrix.Invert();

    // Update object position as transformation of world position
    maPoint3d object_space = m_WorldPosition;
    m_Matrix.Transform(object_space);
    const bool bDirty = false;
    set_object_space_value(object_space, bDirty);
  } else {
    // Update world position as transformation of object position
    m_WorldPosition = this->GetValue();
    m_Matrix.Transform(m_WorldPosition);
  }
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyPoint3d&
prtyPoint3dTransform::operator=(const maPoint3d& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
// Set object space position and trigger callbacks if the
// value has changed. Only changes to the object space position
// should trigger callbacks (world space changes don't trigger).
//--------------------------------------------------------------------
void
prtyPoint3dTransform::set_object_space_value(const maPoint3d& i_Value, bool i_bDirty)
{
  if (m_Value != i_Value) {
    m_Value = i_Value;
    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}
