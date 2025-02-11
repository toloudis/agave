/****************************************************************************\
**	prtyListBoxUIInfo.hpp
**
**		ListBox UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_LISTBOXUIINFO_HPP
#error prtyListBoxUIInfo.hpp multiply included
#endif
#define PRTY_LISTBOXUIINFO_HPP

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
class prtyListBoxUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyListBoxUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyListBoxUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	Add an item to the ListBox to display.  If no index is given
  //	it will be added to the end.
  //--------------------------------------------------------------------
  void AddItem(const std::string& i_Label, int i_Index = -1);
  void AddItem(const std::string& i_Label, bool i_bChecked, int i_Index = -1);

  //--------------------------------------------------------------------
  //	change the checked state of an item
  //--------------------------------------------------------------------
  void SetItem(int i_Index, bool i_bChecked);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  std::string& GetItemText(int i_Index);
  bool GetItemChecked(int i_Index);

  //--------------------------------------------------------------------
  //	Clear the list
  //--------------------------------------------------------------------
  void Clear();

public:
  std::vector<std::string> m_List;
  std::vector<bool> m_Checked;

  bool m_bChecked;
  bool m_bOnlyOneSelected;
};
