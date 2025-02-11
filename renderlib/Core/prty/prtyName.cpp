/****************************************************************************\
**	prtyName.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyName.hpp"
#include "Core/prty/prtyUndoTemplate.hpp"

#include "Core/ch/chChunkParserUtil.hpp"
#include "Core/name/nameMgr.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyName::prtyName()
  : prtyProperty("Name")
  , m_Value("")
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyName::prtyName(const std::string& i_Name)
  : prtyProperty(i_Name)
  , m_Value("")
{
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
prtyName::prtyName(const std::string& i_Name, const nameString& i_InitialValue)
  : prtyProperty(i_Name)
  , m_Value(i_InitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyName::GetType()
{
  return "Name";
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
const nameString&
prtyName::GetValue() const
{
  return m_Value;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
prtyName::SetValue(const nameString& i_Name, bool i_bDirty) // UndoFlags i_Undoable)
{
  // and we need there to be a complete match including the
  // string.
  //
  if (!m_Value.ExactMatch(i_Name)) {
    // Need to use full comparison here because the
    // nameString's operator==() may only check the UID numbers

    m_Value = i_Name;
    // NotifyCallbacksPropertyChanged(i_Undoable != eNoUndo);
    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
prtyName::SetValue(const std::string& i_Name, bool i_bDirty) // UndoFlags i_Undoable)
{
  nameString new_name(m_Value);
  new_name.SetString(i_Name);
  SetValue(new_name, i_bDirty); // i_Undoable);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
std::string
prtyName::GetString() const
{
  return m_Value.GetString();
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
prtyName::SetString(const std::string& i_String)
{
  m_Value.SetString(i_String);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
nameUID
prtyName::GetUID() const
{
  return m_Value.GetUID();
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
prtyName::SetUID(const nameUID i_UID)
{
  m_Value.SetUID(i_UID);
}

//--------------------------------------------------------------------
//	Register the name getting a unique ID
//--------------------------------------------------------------------
void
prtyName::RegisterName()
{
  nameMgr::RegisterName(m_Value);
}

//--------------------------------------------------------------------
//	Operators
//--------------------------------------------------------------------
prtyName&
prtyName::operator=(const prtyName& i_Value)
{
  // copy base data
  prtyProperty::operator=(i_Value);

  SetValue(i_Value.GetValue());
  return *this;
}
prtyName&
prtyName::operator=(const nameString& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	Comparison Operators
//--------------------------------------------------------------------
bool
prtyName::operator==(const prtyName& i_Value) const
{
  return (m_Value == i_Value.GetValue());
}
bool
prtyName::operator==(const nameString& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyName::operator==(const std::string& i_Value) const
{
  return (strcmp(m_Value.GetString().c_str(), i_Value.c_str()) == 0);
}
bool
prtyName::operator!=(const nameString& i_Value) const
{
  return !(m_Value == i_Value);
}
bool
prtyName::operator!=(const std::string& i_Value) const
{
  return (strcmp(m_Value.GetString().c_str(), i_Value.c_str()) != 0);
}

//--------------------------------------------------------------------
// Create an undo operation of the correct type for this
// property. A reference to this property should be passed in.
//--------------------------------------------------------------------
// virtual
undoUndoOperation*
prtyName::CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef)
{
  return new prtyUndoTemplate<prtyName, nameString>(i_pPropertyRef, this->GetValue());
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyName::Read(chReader& io_Reader)
{
  nameUID uid;
  chChunkParserUtil::Read(io_Reader, uid);
  m_Value.SetUID(uid);

  std::string name;
  chChunkParserUtil::Read(io_Reader, name);
  m_Value.SetString(name);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyName::Write(chWriter& io_Writer) const
{
  chChunkParserUtil::Write(io_Writer, m_Value.GetUID());
  chChunkParserUtil::Write(io_Writer, m_Value.GetString());
}
