/****************************************************************************\
**	prtyFloatEditUIInfo.hpp
**
**		FloatEdit UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_FLOATEDITUIINFO_HPP
#error prtyFloatEditUIInfo.hpp multiply included
#endif
#define PRTY_FLOATEDITUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyFloatEditUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFloatEditUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFloatEditUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	DecimalPlaces
  //--------------------------------------------------------------------
  const int GetDecimalPlaces() const;
  void SetDecimalPlaces(const int i_DecimalPlaces);

private:
  int m_DecimalPlaces;
};
