/****************************************************************************\
**	prtyNumericUpDownUIInfo.hpp
**
**		NumericUpDown UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_NUMERICUPDOWNUIINFO_HPP
#error prtyNumericUpDownUIInfo.hpp multiply included
#endif
#define PRTY_NUMERICUPDOWNUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyNumericUpDownUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyNumericUpDownUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyNumericUpDownUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

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
  //	Increment
  //--------------------------------------------------------------------
  const float GetIncrement() const;
  void SetIncrement(const float i_Increment);

private:
  float m_Minimum;
  float m_Maximum;
  short m_DecimalPlaces;
  float m_Increment;
  bool m_bRestrictValue;
};
