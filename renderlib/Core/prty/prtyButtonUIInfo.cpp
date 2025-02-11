/****************************************************************************\
**	prtyButtonUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyButtonUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyButtonUIInfo::prtyButtonUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
{
  SetControlName("Button");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyButtonUIInfo::prtyButtonUIInfo(prtyProperty* i_pProperty,
                                   const std::string& i_Category,
                                   const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
{
  SetControlName("Button");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyButtonUIInfo::Clone()
{
  return new prtyButtonUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
prtyButtonUIInfo::SetText(const std::string& i_ButtonText)
{
  m_ButtonText = i_ButtonText;
}
