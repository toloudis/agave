/****************************************************************************\
**	prtyButtonUIInfo.hpp
**
**		Button UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_BUTTONUIINFO_HPP
#error prtyButtonUIInfo.hpp multiply included
#endif
#define PRTY_BUTTONUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyButtonUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyButtonUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyButtonUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetText(const std::string& i_ButtonText);

public:
  std::string m_ButtonText;
};
