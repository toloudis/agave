/****************************************************************************\
**	prtyRangedFloatUIInfo.hpp
**
**		RangedFloat UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_RANGEDFLOATUIINFO_HPP
#error prtyRangedFloatUIInfo.hpp multiply included
#endif
#define PRTY_RANGEDFLOATUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyRangedFloatUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyRangedFloatUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyRangedFloatUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	Minimum
  //--------------------------------------------------------------------
  const float GetMinimum() const;
  void SetMinimum(const float i_Minimum);

  //--------------------------------------------------------------------
  //	Maximum
  //--------------------------------------------------------------------
  const float GetMaximum() const;
  void SetMaximum(const float i_Maximum);

  //--------------------------------------------------------------------
  //  Functions to check if the flags restricting ranges is set
  //--------------------------------------------------------------------

  const bool GetRestrictFlag() const;
  void SetRestrictFlag(const bool i_bRestrictValue);

  //--------------------------------------------------------------------
  //	DecimalPlaces
  //--------------------------------------------------------------------
  const short GetDecimalPlaces() const;
  void SetDecimalPlaces(const short i_DecimalPlaces);

  //--------------------------------------------------------------------
  //	NumTicks
  //--------------------------------------------------------------------
  const short GetNumTicks() const;
  void SetNumTicks(const short i_NumTicks);

  //--------------------------------------------------------------------
  //	Exponent
  //--------------------------------------------------------------------
  const short GetExponent() const;
  void SetExponent(const short i_Exponent);

private:
  float m_Minimum;
  float m_Maximum;
  short m_DecimalPlaces;
  short m_NumTicks;
  short m_Exponent;
  bool m_bRestrictValue;
};
