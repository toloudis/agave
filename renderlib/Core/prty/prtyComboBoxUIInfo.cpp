/****************************************************************************\
**	prtyComboBoxUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyComboBoxUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//============================================================================
//============================================================================
const int c_COMBOBOX_MAXWIDTH = 120;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyComboBoxUIInfo::prtyComboBoxUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_Width(c_COMBOBOX_MAXWIDTH)
{
  SetControlName("ComboBox");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyComboBoxUIInfo::prtyComboBoxUIInfo(prtyProperty* i_pProperty,
                                       const std::string& i_Category,
                                       const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_Width(c_COMBOBOX_MAXWIDTH)
{
  SetControlName("ComboBox");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyComboBoxUIInfo::Clone()
{
  return new prtyComboBoxUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
// Clear out the items in the list.
//--------------------------------------------------------------------
void
prtyComboBoxUIInfo::ClearItems()
{
  m_List.clear();
}

//--------------------------------------------------------------------
//	Add an item to the combobox to display.  If no index is given
//	it will be added to the end.
//--------------------------------------------------------------------
void
prtyComboBoxUIInfo::AddItem(const std::string& i_Label, int i_Index)
{
  size_t add_index = i_Index;
  if (i_Index == -1) {
    add_index = m_List.size();
  }

  if ((m_List.empty()) || (add_index >= m_List.size())) {
    m_List.resize(add_index + 1);
  }

  m_List[add_index] = i_Label;
}

//--------------------------------------------------------------------
//	width of control in pixels
//--------------------------------------------------------------------
const int
prtyComboBoxUIInfo::GetWidth() const
{
  return m_Width;
}
void
prtyComboBoxUIInfo::SetWidth(const int i_Width)
{
  m_Width = i_Width;
}
