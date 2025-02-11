/****************************************************************************\
**	prtyColorRGBEditUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyColorRGBEditUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyColorRGBEditUIInfo::prtyColorRGBEditUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
{
  SetControlName("ColorRGBEdit");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyColorRGBEditUIInfo::prtyColorRGBEditUIInfo(prtyProperty* i_pProperty,
                                               const std::string& i_Category,
                                               const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
{
  SetControlName("ColorRGBEdit");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyColorRGBEditUIInfo::Clone()
{
  return new prtyColorRGBEditUIInfo(this->GetProperty(0));
}
