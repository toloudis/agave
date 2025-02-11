/****************************************************************************\
**	prtyPoint3d.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyPoint3d.hpp"
#include "Core/prty/prtyUnits.hpp"

#include "Core/ch/chChunkParserUtil.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyPoint3d::prtyPoint3d()
  : prtyProperty("Point3d")
  , m_Value(0, 0, 0)
  , m_bUseUnits(true)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyPoint3d::prtyPoint3d(const std::string& i_Name)
  : prtyProperty(i_Name)
  , m_Value(0, 0, 0)
  , m_bUseUnits(true)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyPoint3d::prtyPoint3d(const std::string& i_Name, const maPoint3d& i_InitialValue)
  : prtyProperty(i_Name)
  , m_Value(i_InitialValue)
  , m_bUseUnits(true)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyPoint3d::GetType()
{
  return "Point3d";
}

//----------------------------------------------------------------------------
// Set value of property. The boolean flag is true
//	if the change is coming from the user interface and therefore
//	should mark the document containing the property as dirty.
//----------------------------------------------------------------------------
void
prtyPoint3d::SetValue(const maPoint3d& i_Value, bool i_bDirty)
{
  if (m_Value != i_Value) {
    m_Value = i_Value;
    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}

//--------------------------------------------------------------------
// Set value of property without notifying the callbacks
//--------------------------------------------------------------------
void
prtyPoint3d::SetValueWithoutNotify(const maPoint3d& i_Value)
{
  m_Value = i_Value;
}

//--------------------------------------------------------------------
// Create and return undo operation of correct type for this property.
// Ownership passes to the caller.
//--------------------------------------------------------------------
undoUndoOperation*
prtyPoint3d::CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef)
{
  return new prtyUndoTemplate<prtyPoint3d, maPoint3d>(i_pPropertyRef, this->GetValue());
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
prtyPoint3d::Set(const float i_ValueX,
                 const float i_ValueY,
                 const float i_ValueZ,
                 bool i_bDirty) // UndoFlags i_Undoable )
{
  maPoint3d newvec(i_ValueX, i_ValueY, i_ValueZ);
  SetValue(newvec, i_bDirty); // i_Undoable);
}

//--------------------------------------------------------------------
// Set whether this property should consider the current units
// when displaying its value in the user interface.
//--------------------------------------------------------------------
bool
prtyPoint3d::GetUseUnits() const
{
  return m_bUseUnits;
}
void
prtyPoint3d::SetUseUnits(bool i_bUseUnits)
{
  m_bUseUnits = true;
}

//--------------------------------------------------------------------
// Get and Set value in the current display units.
// Converts to internal units and then calls Get/SetValue()
//--------------------------------------------------------------------
maPoint3d
prtyPoint3d::GetScaledValue() const
{
  if (this->GetUseUnits())
    return (this->GetValue() / prtyUnits::GetUnitScaling());
  else
    return this->GetValue();
}
void
prtyPoint3d::SetScaledValue(const maPoint3d& i_Value, bool i_bDirty) // UndoFlags i_Undoable)
{
  if (this->GetUseUnits())
    this->SetValue(i_Value * prtyUnits::GetUnitScaling(), i_bDirty); // i_Undoable);
  else
    return this->SetValue(i_Value, i_bDirty); // i_Undoable);
}

//--------------------------------------------------------------------
// Some prtyPoint3d properties contain a transformation so that
// you can set and get the position in world space or object space.
// The traditional Get/SetValue functions would return the object
// space properties and these function return the world space values.
// Default implementation of these functions is to treat the
// object and world space positions as the same value.
//--------------------------------------------------------------------
// bool prtyPoint3d::HasWorldSpace() const
//{
//	return false;
//}
maPoint3d
prtyPoint3d::GetWorldSpaceValue() const
{
  return GetValue();
}
void
prtyPoint3d::SetWorldSpaceValue(const maPoint3d& i_Value, bool i_bDirty)
{
  this->SetValue(i_Value, i_bDirty);
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyPoint3d&
prtyPoint3d::operator=(const maPoint3d& i_Value)
{
  SetValue(i_Value);
  return *this;
}
prtyPoint3d&
prtyPoint3d::operator=(const prtyPoint3d& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);
  SetValue(i_Property.GetValue());
  m_bUseUnits = i_Property.m_bUseUnits;
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyPoint3d::operator==(const prtyPoint3d& i_Property) const
{
  return (m_Value == i_Property.GetValue());
}
bool
prtyPoint3d::operator!=(const prtyPoint3d& i_Property) const
{
  return (m_Value != i_Property.GetValue());
}
bool
prtyPoint3d::operator==(const maPoint3d& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyPoint3d::operator!=(const maPoint3d& i_Value) const
{
  return (m_Value != i_Value);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyPoint3d::Read(chReader& io_Reader)
{
  maPoint3d temp;
  chChunkParserUtil::Read(io_Reader, temp);
  SetValue(temp);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyPoint3d::Write(chWriter& io_Writer) const
{
  chChunkParserUtil::Write(io_Writer, GetValue());
}
