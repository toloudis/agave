/****************************************************************************\
**	prtyRotation.hpp
**
**		Rotation property
**
**
**
\****************************************************************************/
#ifdef PRTY_ROTATION_HPP
#error prtyRotation.hpp multiply included
#endif
#define PRTY_ROTATION_HPP

#ifndef PRTY_PROPERTY_HPP
#include "Core/prty/prtyProperty.hpp"
#endif

#ifndef MA_ROTATION_HPP
#include "Core/ma/maRotation.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyRotation : public prtyProperty
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyRotation();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyRotation(const std::string& i_Name);

  //--------------------------------------------------------------------
  // Constructor with quaternion initial value
  //--------------------------------------------------------------------
  prtyRotation(const std::string& i_Name, const maRotation& i_InitialValue);

  //--------------------------------------------------------------------
  // Constructor with euler angle initial value
  //--------------------------------------------------------------------
  prtyRotation(const std::string& i_Name, float i_X, float i_Y, float i_Z);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  const maRotation& GetQuaternion() const;
  const maRotation& GetValue() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetQuaternion(const maRotation& i_Rotation, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);
  void SetValue(const maRotation& i_Rotation, bool i_bDirty = false);      // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  // const maRotation& GetMinimum() const;
  // void SetMinimum(const maRotation& i_Minimum);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  // const maRotation& GetMaximum() const;
  // void SetMaximum(const maRotation& i_Maximum);

  //--------------------------------------------------------------------
  //	Set rotation through 3 euler angles (in radians)
  //--------------------------------------------------------------------
  void SetEuler(float i_X, float i_Y, float i_Z, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);
  void GetEuler(float& o_X, float& o_Y, float& o_Z) const;

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyRotation& operator=(const maRotation& i_Value);
  prtyRotation& operator=(const prtyRotation& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const maRotation& i_Value) const;
  bool operator==(const prtyRotation& i_Value) const;
  bool operator!=(const prtyRotation& i_Value) const;

  //--------------------------------------------------------------------
  // Create an undo operation of the correct type for this
  // property. A reference to this property should be passed in.
  //--------------------------------------------------------------------
  virtual undoUndoOperation* CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;

private:
  maRotation m_Quaternion;
  maVector3d m_EulerAngles;
  // maRotation	m_Minimum;
  // maRotation	m_Maximum;
};
