/****************************************************************************\
**	prtyVector3dEditUIInfo.hpp
**
**		Vector3dEdit UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_VECTOR3DEDITUIINFO_HPP
#error prtyVector3dEditUIInfo.hpp multiply included
#endif
#define PRTY_VECTOR3DEDITUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

// #ifndef MA_VECTOR3D_HPP
// #include "Core/ma/maVector3d.hpp"
// #endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyVector3dEditUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3dEditUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3dEditUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	Minimum
  //--------------------------------------------------------------------
  // const maVector3d& GetMinimum() const;
  // void SetMinimum(const maVector3d& i_Minimum);
  // void SetMinimum(const float i_IncX, const float i_IncY, const float i_IncZ);

  //--------------------------------------------------------------------
  //	DecimalPlaces
  //--------------------------------------------------------------------
  const int GetDecimalPlaces() const;
  void SetDecimalPlaces(const int i_DecimalPlaces);

private:
  int m_DecimalPlaces;

  // maVector3d m_Minimum;
};
