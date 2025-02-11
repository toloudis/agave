/****************************************************************************\
**	prtyVector3dEditRangedUIInfo.hpp
**
**		Vector3dEditRanged UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_VECTOR3DEDITRANGEDUIINFO_HPP
#error prtyVector3dEditRangedUIInfo.hpp multiply included
#endif
#define PRTY_VECTOR3DEDITRANGEDUIINFO_HPP

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
class prtyVector3dEditRangedUIInfo : public prtyVector3dEditUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3dEditRangedUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3dEditRangedUIInfo(prtyProperty* i_pProperty,
                               const std::string& i_Category,
                               const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	NumTicks
  //--------------------------------------------------------------------
  int GetNumTicks() const;
  void SetNumTicks(int i_NumTicks);

  //--------------------------------------------------------------------
  //	Minimum
  //--------------------------------------------------------------------
  float GetMinimum() const;
  void SetMinimum(float i_Minimum);

  //--------------------------------------------------------------------
  //	Maximum
  //--------------------------------------------------------------------
  float GetMaximum() const;
  void SetMaximum(float i_Maximum);

private:
  int m_NumTicks;
  float m_Minimum;
  float m_Maximum;
};
