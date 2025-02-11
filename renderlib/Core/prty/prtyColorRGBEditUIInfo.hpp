/****************************************************************************\
**	prtyColorRGBEditUIInfo.hpp
**
**		ColorRGBEdit UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_COLORRGBEDITUIINFO_HPP
#error prtyColorRGBEditUIInfo.hpp multiply included
#endif
#define PRTY_COLORRGBEDITUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyColorRGBEditUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyColorRGBEditUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyColorRGBEditUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();
};
