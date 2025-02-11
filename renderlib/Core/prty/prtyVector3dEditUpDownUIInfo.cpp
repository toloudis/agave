/****************************************************************************\
**	prtyVector3dEditUpDownUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyVector3dEditUpDownUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

#include <limits>

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVector3dEditUpDownUIInfo::prtyVector3dEditUpDownUIInfo(prtyProperty* i_pProperty)
  : prtyVector3dEditUIInfo(i_pProperty)
  , m_Increment(0.01f, 0.01f, 0.01f)
{
  SetControlName("Vector3dEditUpDown");

  float val;
  // val = numeric_limits<float>::min();
  val = -10000000.0f;
  m_Minimum.Set(val, val, val);
  // val = numeric_limits<float>::max();
  val = 1000000.0f;
  m_Maximum.Set(val, val, val);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVector3dEditUpDownUIInfo::prtyVector3dEditUpDownUIInfo(prtyProperty* i_pProperty,
                                                           const std::string& i_Category,
                                                           const std::string& i_Description)
  : prtyVector3dEditUIInfo(i_pProperty, i_Category, i_Description)
  , m_Increment(0.01f, 0.01f, 0.01f)
{
  SetControlName("Vector3dEditUpDown");

  float val;
  // val = numeric_limits<float>::min();
  val = -10000000.0f;
  m_Minimum.Set(val, val, val);
  // val = numeric_limits<float>::max();
  val = 1000000.0f;
  m_Maximum.Set(val, val, val);
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyVector3dEditUpDownUIInfo::Clone()
{
  return new prtyVector3dEditUpDownUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	Increment
//--------------------------------------------------------------------
const maVector3d&
prtyVector3dEditUpDownUIInfo::GetIncrement() const
{
  return m_Increment;
}
void
prtyVector3dEditUpDownUIInfo::SetIncrement(const maVector3d& i_Increment)
{
  m_Increment = i_Increment;
}
void
prtyVector3dEditUpDownUIInfo::SetIncrement(const float i_IncX, const float i_IncY, const float i_IncZ)
{
  m_Increment.Set(i_IncX, i_IncY, i_IncZ);
}

//--------------------------------------------------------------------
//	Minimum
//--------------------------------------------------------------------
const maVector3d&
prtyVector3dEditUpDownUIInfo::GetMinimum() const
{
  return m_Minimum;
}
void
prtyVector3dEditUpDownUIInfo::SetMinimum(const maVector3d& i_Minimum)
{
  m_Minimum = i_Minimum;
}
void
prtyVector3dEditUpDownUIInfo::SetMinimum(const float i_ValX, const float i_ValY, const float i_ValZ)
{
  m_Minimum.Set(i_ValX, i_ValY, i_ValZ);
}

//--------------------------------------------------------------------
//	Maximum
//--------------------------------------------------------------------
const maVector3d&
prtyVector3dEditUpDownUIInfo::GetMaximum() const
{
  return m_Maximum;
}
void
prtyVector3dEditUpDownUIInfo::SetMaximum(const maVector3d& i_Maximum)
{
  m_Maximum = i_Maximum;
}
void
prtyVector3dEditUpDownUIInfo::SetMaximum(const float i_ValX, const float i_ValY, const float i_ValZ)
{
  m_Maximum.Set(i_ValX, i_ValY, i_ValZ);
}
