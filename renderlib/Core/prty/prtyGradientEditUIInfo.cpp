/****************************************************************************\
**	prtyGradientEditUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyGradientEditUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyGradientEditUIInfo::prtyGradientEditUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
// m_DecimalPlaces(3)
{
  SetControlName("GradientColorEdit");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyGradientEditUIInfo::prtyGradientEditUIInfo(prtyProperty* i_pProperty,
                                               const std::string& i_Category,
                                               const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
// m_DecimalPlaces(3)//,
// m_Minimum( 0.01f, 0.01f, 0.01f )
{
  SetControlName("GradientColorEdit");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyGradientEditUIInfo::Clone()
{
  return new prtyGradientEditUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	DecimalPlaces
//--------------------------------------------------------------------
// const int prtyGradientEditUIInfo::GetDecimalPlaces() const
//{
//	return m_DecimalPlaces;
//}
// void prtyGradientEditUIInfo::SetDecimalPlaces(const int i_DecimalPlaces)
//{
//	m_DecimalPlaces = i_DecimalPlaces;
//}

//--------------------------------------------------------------------
//	Minimum
//--------------------------------------------------------------------
// const maVector3d& prtyGradientEditUIInfo::GetMinimum() const
//{
//	return m_Minimum;
//}
// void prtyGradientEditUIInfo::SetMinimum(const maVector3d& i_Minimum)
//{
//	m_Minimum = i_Minimum;
//}
// void prtyGradientEditUIInfo::SetMinimum(const float i_IncX, const float i_IncY, const float i_IncZ)
//{
//	m_Minimum.Set( i_IncX, i_IncY, i_IncZ );
//}
