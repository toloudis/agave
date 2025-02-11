/****************************************************************************\
**	prtyVector3dEditUpDownUIInfo.hpp
**
**		Vector3dEditUpDown UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_VECTOR3DEDITUPDOWNUIINFO_HPP
#error prtyVector3dEditUpDownUIInfo.hpp multiply included
#endif
#define PRTY_VECTOR3DEDITUPDOWNUIINFO_HPP

#ifndef PRTY_VECTOR3DEDITUIINFO_HPP
#include "Core/prty/prtyVector3dEditUIInfo.hpp"
#endif

#ifndef MA_VECTOR3D_HPP
#include "Core/ma/maVector3d.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyVector3dEditUpDownUIInfo : public prtyVector3dEditUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3dEditUpDownUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3dEditUpDownUIInfo(prtyProperty* i_pProperty,
                               const std::string& i_Category,
                               const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	Increment
  //--------------------------------------------------------------------
  const maVector3d& GetIncrement() const;
  void SetIncrement(const maVector3d& i_Increment);
  void SetIncrement(const float i_IncX, const float i_IncY, const float i_IncZ);

  //--------------------------------------------------------------------
  //	Minimum
  //--------------------------------------------------------------------
  const maVector3d& GetMinimum() const;
  void SetMinimum(const maVector3d& i_Minimum);
  void SetMinimum(const float i_ValX, const float i_ValY, const float i_ValZ);

  //--------------------------------------------------------------------
  //	Maximum
  //--------------------------------------------------------------------
  const maVector3d& GetMaximum() const;
  void SetMaximum(const maVector3d& i_Maximum);
  void SetMaximum(const float i_ValX, const float i_ValY, const float i_ValZ);

private:
  maVector3d m_Increment;
  maVector3d m_Minimum;
  maVector3d m_Maximum;
};
