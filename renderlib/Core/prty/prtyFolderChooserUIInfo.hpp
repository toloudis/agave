/****************************************************************************\
**	prtyFolderChooserUIInfo.hpp
**
**		FolderChooser UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_FOLDERCHOOSERUIINFO_HPP
#error prtyFolderChooserUIInfo.hpp multiply included
#endif
#define PRTY_FOLDERCHOOSERUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyFolderChooserUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFolderChooserUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFolderChooserUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();
};
