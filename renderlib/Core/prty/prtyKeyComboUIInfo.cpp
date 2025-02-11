/****************************************************************************\
**	prtyKeyComboUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyKeyComboUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//============================================================================
//============================================================================
const int c_TEXTBOX_MAXWIDTH = 80;
const int c_TEXTBOX_MAXCHARS = 256;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyKeyComboUIInfo::prtyKeyComboUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_MaxCharacters(c_TEXTBOX_MAXCHARS)
  , m_MaxWidth(c_TEXTBOX_MAXWIDTH)
{
  SetControlName("KeyCombo");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyKeyComboUIInfo::prtyKeyComboUIInfo(prtyProperty* i_pProperty,
                                       const std::string& i_Category,
                                       const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_MaxCharacters(c_TEXTBOX_MAXCHARS)
  , m_MaxWidth(c_TEXTBOX_MAXWIDTH)
{
  SetControlName("KeyCombo");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyKeyComboUIInfo::Clone()
{
  return new prtyKeyComboUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	maximum number of characters this control can hold
//--------------------------------------------------------------------
const int
prtyKeyComboUIInfo::GetMaxChars() const
{
  return m_MaxCharacters;
}
void
prtyKeyComboUIInfo::SetMaxChars(const int i_MaxChars)
{
  m_MaxCharacters = i_MaxChars;
}

//--------------------------------------------------------------------
//	maximum visible width in pixels
//--------------------------------------------------------------------
const int
prtyKeyComboUIInfo::GetMaxWidth() const
{
  return m_MaxWidth;
}
void
prtyKeyComboUIInfo::SetMaxWidth(const int i_MaxWidth)
{
  m_MaxWidth = i_MaxWidth;
}

//--------------------------------------------------------------------
//	modifier keys
//--------------------------------------------------------------------
const int
prtyKeyComboUIInfo::GetModifiers() const
{
  return m_Modifiers;
}
void
prtyKeyComboUIInfo::SetModifiers(const int i_Modifiers)
{
  m_Modifiers = i_Modifiers;
}

//--------------------------------------------------------------------
//	hot key (tied to a modifier)
//--------------------------------------------------------------------
const int
prtyKeyComboUIInfo::GetHotKey() const
{
  return m_HotKey;
}
void
prtyKeyComboUIInfo::SetHotKey(const int i_HotKey)
{
  m_HotKey = i_HotKey;
}
