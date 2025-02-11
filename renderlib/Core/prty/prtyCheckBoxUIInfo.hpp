/****************************************************************************\
**	prtyCheckBoxUIInfo.hpp
**
**		CheckBox UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_CHECKBOXUIINFO_HPP
#error prtyCheckBoxUIInfo.hpp multiply included
#endif
#define PRTY_CHECKBOXUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyCheckBoxUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyCheckBoxUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyCheckBoxUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();
};
