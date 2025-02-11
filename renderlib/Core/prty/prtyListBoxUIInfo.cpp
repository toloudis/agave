/****************************************************************************\
**	prtyListBoxUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyListBoxUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

#include "Core/dbg/dbgMsg.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyListBoxUIInfo::prtyListBoxUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_bChecked(true)
  , m_bOnlyOneSelected(true)
{
  SetControlName("ListBox");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyListBoxUIInfo::prtyListBoxUIInfo(prtyProperty* i_pProperty,
                                     const std::string& i_Category,
                                     const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_bChecked(true)
  , m_bOnlyOneSelected(true)
{
  SetControlName("ListBox");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyListBoxUIInfo::Clone()
{
  return new prtyListBoxUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	Add an item to the ListBox to display.  If no index is given
//	it will be added to the end.
//--------------------------------------------------------------------
void
prtyListBoxUIInfo::AddItem(const std::string& i_Label, int i_Index)
{
  AddItem(i_Label, false, i_Index);
}

void
prtyListBoxUIInfo::AddItem(const std::string& i_Label, bool i_bChecked, int i_Index)
{
  int add_index = i_Index;
  if (i_Index == -1) {
    add_index = m_List.size();
  }

  if ((m_List.empty()) || (add_index >= m_List.size())) {
    m_List.resize(add_index + 1);
    m_Checked.resize(add_index + 1);
  }

  m_List[add_index] = i_Label;
  m_Checked[add_index] = i_bChecked;
}

//--------------------------------------------------------------------
//	change the checked state of an item
//--------------------------------------------------------------------
void
prtyListBoxUIInfo::SetItem(int i_Index, bool i_bChecked)
{
  DBG_ASSERT(i_Index < m_Checked.size(), "Invalid index");

  m_Checked[i_Index] = i_bChecked;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
std::string&
prtyListBoxUIInfo::GetItemText(int i_Index)
{
  return m_List[i_Index];
}

bool
prtyListBoxUIInfo::GetItemChecked(int i_Index)
{
  return m_Checked[i_Index];
}

//--------------------------------------------------------------------
//	Clear the list
//--------------------------------------------------------------------
void
prtyListBoxUIInfo::Clear()
{
  m_List.clear();
  m_Checked.clear();
}
