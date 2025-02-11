/****************************************************************************\
**	prtyVector3dEditRangedUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyVector3dEditRangedUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

#include <limits>

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVector3dEditRangedUIInfo::prtyVector3dEditRangedUIInfo(prtyProperty* i_pProperty)
  : prtyVector3dEditUIInfo(i_pProperty)
  , m_NumTicks(200)
  , m_Minimum(0.0f)
  , m_Maximum(1.0f)
{
  SetControlName("Vector3dEditRanged");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVector3dEditRangedUIInfo::prtyVector3dEditRangedUIInfo(prtyProperty* i_pProperty,
                                                           const std::string& i_Category,
                                                           const std::string& i_Description)
  : prtyVector3dEditUIInfo(i_pProperty, i_Category, i_Description)
  , m_NumTicks(200)
  , m_Minimum(0.0f)
  , m_Maximum(1.0f)
{
  SetControlName("Vector3dEditRanged");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyVector3dEditRangedUIInfo::Clone()
{
  return new prtyVector3dEditRangedUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	NumTicks
//--------------------------------------------------------------------
int
prtyVector3dEditRangedUIInfo::GetNumTicks() const
{
  return m_NumTicks;
}
void
prtyVector3dEditRangedUIInfo::SetNumTicks(int i_NumTicks)
{
  m_NumTicks = i_NumTicks;
}

//--------------------------------------------------------------------
//	Minimum
//--------------------------------------------------------------------
float
prtyVector3dEditRangedUIInfo::GetMinimum() const
{
  return m_Minimum;
}
void
prtyVector3dEditRangedUIInfo::SetMinimum(float i_Minimum)
{
  m_Minimum = i_Minimum;
}

//--------------------------------------------------------------------
//	Maximum
//--------------------------------------------------------------------
float
prtyVector3dEditRangedUIInfo::GetMaximum() const
{
  return m_Maximum;
}
void
prtyVector3dEditRangedUIInfo::SetMaximum(float i_Maximum)
{
  m_Maximum = i_Maximum;
}
