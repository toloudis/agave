/****************************************************************************\
**	prtyTextBoxUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyTextBoxUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//============================================================================
//============================================================================
const int c_TEXTBOX_MAXWIDTH = 80;
const int c_TEXTBOX_MAXCHARS = 32767;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTextBoxUIInfo::prtyTextBoxUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_MaxCharacters(c_TEXTBOX_MAXCHARS)
  , m_MaxWidth(c_TEXTBOX_MAXWIDTH)
  , m_bMultiline(false)
{
  SetControlName("TextBox");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTextBoxUIInfo::prtyTextBoxUIInfo(prtyProperty* i_pProperty,
                                     const std::string& i_Category,
                                     const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_MaxCharacters(c_TEXTBOX_MAXCHARS)
  , m_MaxWidth(c_TEXTBOX_MAXWIDTH)
  , m_bMultiline(false)
{
  SetControlName("TextBox");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyTextBoxUIInfo::Clone()
{
  return new prtyTextBoxUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	maximum number of characters this control can hold
//--------------------------------------------------------------------
const int
prtyTextBoxUIInfo::GetMaxChars() const
{
  return m_MaxCharacters;
}
void
prtyTextBoxUIInfo::SetMaxChars(const int i_MaxChars)
{
  m_MaxCharacters = i_MaxChars;
}

//--------------------------------------------------------------------
//	maximum visible width in pixels
//--------------------------------------------------------------------
const int
prtyTextBoxUIInfo::GetMaxWidth() const
{
  return m_MaxWidth;
}
void
prtyTextBoxUIInfo::SetMaxWidth(const int i_MaxWidth)
{
  m_MaxWidth = i_MaxWidth;
}

//--------------------------------------------------------------------
//	Text box needs multiple line?
//--------------------------------------------------------------------
bool
prtyTextBoxUIInfo::GetMultiline() const
{
  return m_bMultiline;
}
void
prtyTextBoxUIInfo::SetMultiline(bool i_bMultiline)
{
  m_bMultiline = i_bMultiline;
}

//--------------------------------------------------------------------
//	Text box needs multiple line?
//--------------------------------------------------------------------
bool
prtyTextBoxUIInfo::GetMultilineFixed() const
{
  return m_bMultilineFixed;
}
void
prtyTextBoxUIInfo::SetMultilineFixed(bool i_bMultilineFixed)
{
  m_bMultilineFixed = i_bMultilineFixed;
}
