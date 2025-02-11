/****************************************************************************\
**	prtyCheckBoxUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyCheckBoxUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyCheckBoxUIInfo::prtyCheckBoxUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
{
  SetControlName("CheckBox");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyCheckBoxUIInfo::prtyCheckBoxUIInfo(prtyProperty* i_pProperty,
                                       const std::string& i_Category,
                                       const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
{
  SetControlName("CheckBox");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyCheckBoxUIInfo::Clone()
{
  return new prtyCheckBoxUIInfo(this->GetProperty(0));
}
