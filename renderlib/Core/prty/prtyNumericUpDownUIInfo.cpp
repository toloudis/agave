/****************************************************************************\
**	prtyNumericUpDownUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyNumericUpDownUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyNumericUpDownUIInfo::prtyNumericUpDownUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_Minimum(0.0f)
  , m_Maximum(0.0f)
  , m_DecimalPlaces(2)
  , m_Increment(1)
{
  SetControlName("NumericUpDown");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyNumericUpDownUIInfo::prtyNumericUpDownUIInfo(prtyProperty* i_pProperty,
                                                 const std::string& i_Category,
                                                 const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_Minimum(0.0f)
  , m_Maximum(0.0f)
  , m_DecimalPlaces(2)
  , m_bRestrictValue(false)
  , m_Increment(1)
{
  SetControlName("NumericUpDown");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyNumericUpDownUIInfo::Clone()
{
  return new prtyNumericUpDownUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	Minimum
//--------------------------------------------------------------------
const float
prtyNumericUpDownUIInfo::GetMinimum() const
{
  return m_Minimum;
}
void
prtyNumericUpDownUIInfo::SetMinimum(const float i_Minimum)
{
  m_Minimum = i_Minimum;
}

//--------------------------------------------------------------------
//	Maximum
//--------------------------------------------------------------------
const float
prtyNumericUpDownUIInfo::GetMaximum() const
{
  return m_Maximum;
}
void
prtyNumericUpDownUIInfo::SetMaximum(const float i_Maximum)
{
  m_Maximum = i_Maximum;
}

//--------------------------------------------------------------------
// Functions to check if the flag restricting the ranges are set
//--------------------------------------------------------------------

const bool
prtyNumericUpDownUIInfo::GetRestrictFlag() const
{
  return m_bRestrictValue;
}

void
prtyNumericUpDownUIInfo::SetRestrictFlag(const bool i_bRestrictValue)
{
  m_bRestrictValue = i_bRestrictValue;
}
//--------------------------------------------------------------------
//	DecimalPlaces
//--------------------------------------------------------------------
const short
prtyNumericUpDownUIInfo::GetDecimalPlaces() const
{
  return m_DecimalPlaces;
}
void
prtyNumericUpDownUIInfo::SetDecimalPlaces(const short i_DecimalPlaces)
{
  m_DecimalPlaces = i_DecimalPlaces;
}

//--------------------------------------------------------------------
//	Increment
//--------------------------------------------------------------------
const float
prtyNumericUpDownUIInfo::GetIncrement() const
{
  return m_Increment;
}
void
prtyNumericUpDownUIInfo::SetIncrement(const float i_Increment)
{
  m_Increment = i_Increment;
}
