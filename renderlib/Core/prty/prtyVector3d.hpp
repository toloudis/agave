/****************************************************************************\
**	prtyVector3d.hpp
**
**		Vector3d property
**
**
**
\****************************************************************************/
#ifdef PRTY_VECTOR3D_HPP
#error prtyVector3d.hpp multiply included
#endif
#define PRTY_VECTOR3D_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef MA_VECTOR3D_HPP
#include "Core/ma/maVector3d.hpp"
#endif

//============================================================================
//============================================================================
class prtyVector3d : public prtyPropertyTemplate<maVector3d, const maVector3d&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3d();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3d(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyVector3d(const std::string& i_Name, const maVector3d& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void Set(const float i_ValueX,
           const float i_ValueY,
           const float i_ValueZ,
           bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo );

  //--------------------------------------------------------------------
  // Set whether this property should consider the current units
  // when displaying its value in the user interface.
  // prtyVector3d has a default value of false.
  //--------------------------------------------------------------------
  bool GetUseUnits() const;
  void SetUseUnits(bool i_bUseUnits);

  //--------------------------------------------------------------------
  // Get and Set value in the current display units.
  // Converts to internal units and then calls Get/SetValue().
  // If this property does not use units, the unscaled value is used.
  //--------------------------------------------------------------------
  maVector3d GetScaledValue() const;
  void SetScaledValue(const maVector3d& i_Value, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyVector3d& operator=(const maVector3d& i_Value);
  prtyVector3d& operator=(const prtyVector3d& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyVector3d& i_Property) const;
  bool operator!=(const prtyVector3d& i_Property) const;
  bool operator==(const maVector3d& i_Value) const;
  bool operator!=(const maVector3d& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;

private:
  bool m_bUseUnits;
};
