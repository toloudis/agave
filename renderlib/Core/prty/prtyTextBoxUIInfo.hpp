/****************************************************************************\
**	prtyTextBoxUIInfo.hpp
**
**		TextBox UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_TEXTBOXUIINFO_HPP
#error prtyTextBoxUIInfo.hpp multiply included
#endif
#define PRTY_TEXTBOXUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyTextBoxUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTextBoxUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTextBoxUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	maximum number of characters this control can hold
  //--------------------------------------------------------------------
  const int GetMaxChars() const;
  void SetMaxChars(const int i_MaxChars);

  //--------------------------------------------------------------------
  //	maximum visible width in CHARACTERS
  //--------------------------------------------------------------------
  const int GetMaxWidth() const;
  void SetMaxWidth(const int i_MaxWidth);

  //--------------------------------------------------------------------
  //	Text box needs multiple line?
  //--------------------------------------------------------------------
  bool GetMultiline() const;
  void SetMultiline(bool i_bMultiline);

  //--------------------------------------------------------------------
  //	Text box needs multiple line?
  //--------------------------------------------------------------------
  bool GetMultilineFixed() const;
  void SetMultilineFixed(bool i_bMultilineFixed);

private:
  int m_MaxCharacters;
  int m_MaxWidth;
  bool m_bMultiline;
  bool m_bMultilineFixed;
};
