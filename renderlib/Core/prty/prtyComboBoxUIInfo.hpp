/****************************************************************************\
**	prtyComboBoxUIInfo.hpp
**
**		ComboBox UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_COMBOBOXUIINFO_HPP
#error prtyComboBoxUIInfo.hpp multiply included
#endif
#define PRTY_COMBOBOXUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#include <string>
#include <vector>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyComboBoxUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyComboBoxUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyComboBoxUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  // Clear out the items in the list.
  //--------------------------------------------------------------------
  void ClearItems();

  //--------------------------------------------------------------------
  //	Add an item to the combobox to display.  If no index is given
  //	it will be added to the end.
  //--------------------------------------------------------------------
  void AddItem(const std::string& i_Label, int i_Index = -1);

  //--------------------------------------------------------------------
  //	width of control in pixels
  //--------------------------------------------------------------------
  const int GetWidth() const;
  void SetWidth(const int i_Width);

public:
  std::vector<std::string> m_List;

private:
  int m_Width;
};
