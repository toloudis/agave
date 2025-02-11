/****************************************************************************\
**	prtyVector3dEditUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyVector3dEditUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVector3dEditUIInfo::prtyVector3dEditUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_DecimalPlaces(3) //,
                       // m_Minimum( 0.01f, 0.01f, 0.01f )
{
  SetControlName("Vector3dEdit");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVector3dEditUIInfo::prtyVector3dEditUIInfo(prtyProperty* i_pProperty,
                                               const std::string& i_Category,
                                               const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_DecimalPlaces(3) //,
                       // m_Minimum( 0.01f, 0.01f, 0.01f )
{
  SetControlName("Vector3dEdit");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyVector3dEditUIInfo::Clone()
{
  return new prtyVector3dEditUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	DecimalPlaces
//--------------------------------------------------------------------
const int
prtyVector3dEditUIInfo::GetDecimalPlaces() const
{
  return m_DecimalPlaces;
}
void
prtyVector3dEditUIInfo::SetDecimalPlaces(const int i_DecimalPlaces)
{
  m_DecimalPlaces = i_DecimalPlaces;
}

//--------------------------------------------------------------------
//	Minimum
//--------------------------------------------------------------------
// const maVector3d& prtyVector3dEditUIInfo::GetMinimum() const
//{
//	return m_Minimum;
//}
// void prtyVector3dEditUIInfo::SetMinimum(const maVector3d& i_Minimum)
//{
//	m_Minimum = i_Minimum;
//}
// void prtyVector3dEditUIInfo::SetMinimum(const float i_IncX, const float i_IncY, const float i_IncZ)
//{
//	m_Minimum.Set( i_IncX, i_IncY, i_IncZ );
//}
