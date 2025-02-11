/****************************************************************************\
**	prtyFloatEditUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyFloatEditUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFloatEditUIInfo::prtyFloatEditUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_DecimalPlaces(2)
{
  SetControlName("FloatEdit");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFloatEditUIInfo::prtyFloatEditUIInfo(prtyProperty* i_pProperty,
                                         const std::string& i_Category,
                                         const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_DecimalPlaces(2)
{
  SetControlName("FloatEdit");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyFloatEditUIInfo::Clone()
{
  return new prtyFloatEditUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	DecimalPlaces
//--------------------------------------------------------------------
const int
prtyFloatEditUIInfo::GetDecimalPlaces() const
{
  return m_DecimalPlaces;
}
void
prtyFloatEditUIInfo::SetDecimalPlaces(const int i_DecimalPlaces)
{
  m_DecimalPlaces = i_DecimalPlaces;
}
