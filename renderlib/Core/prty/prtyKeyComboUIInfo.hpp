/****************************************************************************\
**	prtyKeyComboUIInfo.hpp
**
**		KeyCombo UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_KEYCOMBOUIINFO_HPP
#error prtyKeyComboUIInfo.hpp multiply included
#endif
#define PRTY_KEYCOMBOUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyKeyComboUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyKeyComboUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyKeyComboUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

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
  //	modifier keys
  //--------------------------------------------------------------------
  const int GetModifiers() const;
  void SetModifiers(const int i_Modifiers);

  //--------------------------------------------------------------------
  //	hot key (tied to a modifier)
  //--------------------------------------------------------------------
  const int GetHotKey() const;
  void SetHotKey(const int i_HotKey);

private:
  int m_MaxCharacters;
  int m_MaxWidth;
  int m_Modifiers;
  int m_HotKey;
};
