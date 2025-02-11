/****************************************************************************\
**	prtyRangedFloatUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyRangedFloatUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyRangedFloatUIInfo::prtyRangedFloatUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_Minimum(0.0f)
  , m_Maximum(100.0f)
  , m_DecimalPlaces(2)
  , m_NumTicks(100)
  , m_Exponent(1)
  , m_bRestrictValue(false)
{
  SetControlName("RangedFloat");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyRangedFloatUIInfo::prtyRangedFloatUIInfo(prtyProperty* i_pProperty,
                                             const std::string& i_Category,
                                             const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_Minimum(0.0f)
  , m_Maximum(100.0f)
  , m_DecimalPlaces(2)
  , m_NumTicks(100)
  , m_Exponent(1)
  , m_bRestrictValue(false)
{
  SetControlName("RangedFloat");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyRangedFloatUIInfo::Clone()
{
  prtyRangedFloatUIInfo* prty_clone = new prtyRangedFloatUIInfo(this->GetProperty(0));
  prty_clone->SetMinimum(this->GetMinimum());
  prty_clone->SetMaximum(this->GetMaximum());
  prty_clone->SetDecimalPlaces(this->GetDecimalPlaces());
  prty_clone->SetNumTicks(this->GetNumTicks());
  prty_clone->SetExponent(this->GetExponent());
  return prty_clone;
}

//--------------------------------------------------------------------
//	Minimum
//--------------------------------------------------------------------
const float
prtyRangedFloatUIInfo::GetMinimum() const
{
  return m_Minimum;
}
void
prtyRangedFloatUIInfo::SetMinimum(const float i_Minimum)
{
  m_Minimum = i_Minimum;
}

//--------------------------------------------------------------------
//	Maximum
//--------------------------------------------------------------------
const float
prtyRangedFloatUIInfo::GetMaximum() const
{
  return m_Maximum;
}
void
prtyRangedFloatUIInfo::SetMaximum(const float i_Maximum)
{
  m_Maximum = i_Maximum;
}

//--------------------------------------------------------------------
// Functions to check if the flag restricting the ranges are set
//--------------------------------------------------------------------

const bool
prtyRangedFloatUIInfo::GetRestrictFlag() const
{
  return m_bRestrictValue;
}

void
prtyRangedFloatUIInfo::SetRestrictFlag(const bool i_bRestrictValue)
{
  m_bRestrictValue = i_bRestrictValue;
}

//--------------------------------------------------------------------
//	DecimalPlaces
//--------------------------------------------------------------------
const short
prtyRangedFloatUIInfo::GetDecimalPlaces() const
{
  return m_DecimalPlaces;
}
void
prtyRangedFloatUIInfo::SetDecimalPlaces(const short i_DecimalPlaces)
{
  m_DecimalPlaces = i_DecimalPlaces;
}

//--------------------------------------------------------------------
//	NumTicks
//--------------------------------------------------------------------
const short
prtyRangedFloatUIInfo::GetNumTicks() const
{
  return m_NumTicks;
}
void
prtyRangedFloatUIInfo::SetNumTicks(const short i_NumTicks)
{
  m_NumTicks = i_NumTicks;
}

//--------------------------------------------------------------------
//	Exponent
//--------------------------------------------------------------------
const short
prtyRangedFloatUIInfo::GetExponent() const
{
  return m_Exponent;
}
void
prtyRangedFloatUIInfo::SetExponent(const short i_Exponent)
{
  m_Exponent = i_Exponent;
}
