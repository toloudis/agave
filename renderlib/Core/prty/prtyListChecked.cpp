/****************************************************************************\
**	prtyListChecked.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyListChecked.hpp"
#include "Core/prty/prtyUndoTemplate.hpp"

#include "Core/ch/chReader.hpp"
#include "Core/ch/chWriter.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyListChecked::prtyListChecked()
  : prtyProperty("ListChecked")
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyListChecked::prtyListChecked(const std::string& i_Name)
  : prtyProperty(i_Name)
{
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
prtyListChecked::prtyListChecked(const std::string& i_Name, checked_list_type& i_bInitialValue)
  : prtyProperty(i_Name)
  , m_Value(i_bInitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyListChecked::GetType()
{
  return "ListChecked";
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
int
prtyListChecked::GetNumberOfItems() const
{
  return m_Value.size();
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
prtyListChecked::SetNumberOfItems(int i_Size)
{
  m_Value.resize(i_Size);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
prtyListChecked::ClearList()
{
  this->m_Value.clear();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
const checked_list_type&
prtyListChecked::GetValue() const
{
  return m_Value;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
const std::string&
prtyListChecked::GetValueText(int i_Index) const
{
  return (m_Value[i_Index].m_Text);
}
bool
prtyListChecked::GetValueFlag(int i_Index) const
{
  return (m_Value[i_Index].m_bChecked);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
prtyListChecked::SetValue(checked_list_type i_Value, bool i_bDirty) // UndoFlags i_Undoable)
{
  //	if ( i_Value != m_Value )		// TODO fix this so it can be uncommented
  {
    m_Value = i_Value;
    // NotifyCallbacksPropertyChanged(i_Undoable != eNoUndo);
    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
prtyListChecked::SetValueFlag(int i_Index, bool i_bValue, bool i_bDirty) // UndoFlags i_Undoable)
{
  if (i_Index >= m_Value.size())
    m_Value.resize(i_Index + 1);

  if (i_bValue != m_Value[i_Index].m_bChecked) {
    m_Value[i_Index].m_bChecked = i_bValue;
    // NotifyCallbacksPropertyChanged(i_Undoable != eNoUndo);
    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
prtyListChecked::SetValueText(int i_Index, std::string i_Value, bool i_bDirty) // UndoFlags i_Undoable)
{
  if (i_Index >= m_Value.size())
    m_Value.resize(i_Index + 1);

  if (i_Value != m_Value[i_Index].m_Text) {
    m_Value[i_Index].m_Text = i_Value;
    // NotifyCallbacksPropertyChanged(i_Undoable != eNoUndo);
    NotifyCallbacksPropertyChanged(i_bDirty);
  }
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyListChecked&
prtyListChecked::operator=(const prtyListChecked& i_Value)
{
  // copy base data
  prtyProperty::operator=(i_Value);

  SetValue(i_Value.GetValue());
  return *this;
}
prtyListChecked&
prtyListChecked::operator=(const checked_list_type& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	Compaison Operators
//--------------------------------------------------------------------
const bool
prtyListChecked::operator==(const prtyListChecked& i_Value) const
{
  return false; // TODO - implement
                // return (m_Value == i_Value.GetValue());
}
const bool
prtyListChecked::operator==(const checked_list_type& i_Value) const
{
  return false; // TODO - implement
                // return (m_Value == i_Value);
}

//--------------------------------------------------------------------
// Create an undo operation of the correct type for this
// property. A reference to this property should be passed in.
//--------------------------------------------------------------------
// virtual
undoUndoOperation*
prtyListChecked::CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef)
{
  return new prtyUndoTemplate<prtyListChecked, checked_list_type>(i_pPropertyRef, this->GetValue());
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyListChecked::Read(chReader& io_Reader)
{
  checked_list_type temp;

  envType::Int32 size;
  io_Reader.Read(size);

  temp.resize(size);

  std::string tag;
  bool flag;
  for (int i = 0; i < size; ++i) {
    io_Reader.Read(tag);
    io_Reader.Read(flag);

    temp[i].m_Text = tag;
    temp[i].m_bChecked = flag;
  }

  SetValue(temp);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyListChecked::Write(chWriter& io_Writer) const
{
  envType::Int32 size = m_Value.size();
  io_Writer.Write(size);

  for (int i = 0; i < size; ++i) {
    io_Writer.Write(m_Value[i].m_Text);
    io_Writer.Write(m_Value[i].m_bChecked);
  }
}
