/****************************************************************************\
**	prtyFolderChooserUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyFolderChooserUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFolderChooserUIInfo::prtyFolderChooserUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
{
  SetControlName("FolderChooser");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFolderChooserUIInfo::prtyFolderChooserUIInfo(prtyProperty* i_pProperty,
                                                 const std::string& i_Category,
                                                 const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
{
  SetControlName("FolderChooser");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyFolderChooserUIInfo::Clone()
{
  return new prtyFolderChooserUIInfo(this->GetProperty(0));
}
