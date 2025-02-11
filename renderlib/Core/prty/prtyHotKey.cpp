/****************************************************************************\
**	prtyHotKey.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyHotKey.hpp"
#include "Core/prty/prtyUndoTemplate.hpp"

#include "Core/ch/chChunkParserUtil.hpp"

//============================================================================
//============================================================================
namespace {
const short lc_INVALID_ID = -1;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyHotKey::prtyHotKey()
  : prtyProperty("Hot Key")
  , m_Value("")
  , m_ID(lc_INVALID_ID)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyHotKey::prtyHotKey(const std::string& i_Name)
  : prtyProperty(i_Name)
  , m_Value("")
  , m_ID(lc_INVALID_ID)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyHotKey::prtyHotKey(const std::string& i_Name, const std::string& i_InitialValue)
  : prtyProperty(i_Name)
  , m_Value(i_InitialValue)
  , m_ID(lc_INVALID_ID)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyHotKey::GetType()
{
  return "HotKey";
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
const std::string&
prtyHotKey::GetValue() const
{
  return m_Value;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
prtyHotKey::SetValue(const std::string& i_Value, bool i_bDirty) // UndoFlags i_Undoable)
{
  if (m_Value != i_Value) {
    m_Value = i_Value;
    NotifyCallbacksPropertyChanged(i_bDirty);
    // NotifyCallbacksPropertyChanged(i_Undoable != eNoUndo);
  }
}

//--------------------------------------------------------------------
//	ID - unique identifier
//	On windows systems this ID is used by the OS when registering and
//	unregistering.  It is generated at the launch of app and not
//	saved to a file.
//--------------------------------------------------------------------
void
prtyHotKey::SetID(short i_ID)
{
  m_ID = i_ID;
}
short
prtyHotKey::GetID() const
{
  return m_ID;
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyHotKey&
prtyHotKey::operator=(const prtyHotKey& i_Value)
{
  // copy base data
  prtyProperty::operator=(i_Value);

  SetValue(i_Value.GetValue());
  SetID(i_Value.GetID());
  return *this;
}
prtyHotKey&
prtyHotKey::operator=(const std::string& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyHotKey::operator==(const std::string& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyHotKey::operator!=(const std::string& i_Value) const
{
  return (m_Value != i_Value);
}
bool
prtyHotKey::operator==(const prtyHotKey& i_Value) const
{
  return (m_Value == i_Value.GetValue());
}
bool
prtyHotKey::operator!=(const prtyHotKey& i_Value) const
{
  return (m_Value != i_Value.GetValue());
}

//--------------------------------------------------------------------
// Create an undo operation of the correct type for this
// property. A reference to this property should be passed in.
//--------------------------------------------------------------------
// virtual
undoUndoOperation*
prtyHotKey::CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef)
{
  return new prtyUndoTemplate<prtyHotKey, std::string>(i_pPropertyRef, this->GetValue());
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyHotKey::Read(chReader& io_Reader)
{
  std::string temp;
  chChunkParserUtil::Read(io_Reader, temp);
  SetValue(temp);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyHotKey::Write(chWriter& io_Writer) const
{
  chChunkParserUtil::Write(io_Writer, GetValue());
}
