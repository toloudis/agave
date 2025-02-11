/****************************************************************************\
**	prtyPoint3d.hpp
**
**		Point3d property
**
**
**
\****************************************************************************/
#ifdef PRTY_POINT3D_HPP
#error prtyPoint3d.hpp multiply included
#endif
#define PRTY_POINT3D_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef MA_POINT3D_HPP
#include "Core/ma/maPoint3d.hpp"
#endif

//============================================================================
//============================================================================
class prtyPoint3d : public prtyProperty
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyPoint3d();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyPoint3d(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyPoint3d(const std::string& i_Name, const maPoint3d& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //----------------------------------------------------------------------------
  // Get value of property
  //----------------------------------------------------------------------------
  inline const maPoint3d& GetValue() const { return m_Value; }

  //----------------------------------------------------------------------------
  // Set value of property. The boolean flag is true
  //	if the change is coming from the user interface and therefore
  //	should mark the document containing the property as dirty.
  //----------------------------------------------------------------------------
  virtual void SetValue(const maPoint3d& i_Value, bool i_bDirty = false);

  //--------------------------------------------------------------------
  // Set value of property without notifying the callbacks
  //--------------------------------------------------------------------
  void SetValueWithoutNotify(const maPoint3d& i_Value);

  //--------------------------------------------------------------------
  // Create and return undo operation of correct type for this property.
  // Ownership passes to the caller.
  //--------------------------------------------------------------------
  undoUndoOperation* CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void Set(const float i_ValueX,
           const float i_ValueY,
           const float i_ValueZ,
           bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo );

  //--------------------------------------------------------------------
  // Set whether this property should consider the current units
  // when displaying its value in the user interface.
  // prtyPoint3d has a default value of true.
  //--------------------------------------------------------------------
  bool GetUseUnits() const;
  void SetUseUnits(bool i_bUseUnits);

  //--------------------------------------------------------------------
  // Get and Set value in the current display units.
  // Converts to internal units and then calls Get/SetValue()
  // If this property does not use units, the unscaled value is used.
  //--------------------------------------------------------------------
  maPoint3d GetScaledValue() const;
  void SetScaledValue(const maPoint3d& i_Value, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  // Some prtyPoint3d properties contain a transformation so that
  // you can set and get the position in world space or object space.
  // The traditional Get/SetValue functions would return the object
  // space properties and these function return the world space values.
  // Default implementation of these functions is to treat the
  // object and world space positions as the same value.
  //--------------------------------------------------------------------
  // virtual bool HasWorldSpace() const;
  virtual maPoint3d GetWorldSpaceValue() const;
  virtual void SetWorldSpaceValue(const maPoint3d& i_Value, bool i_bDirty = false);

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyPoint3d& operator=(const maPoint3d& i_Value);
  prtyPoint3d& operator=(const prtyPoint3d& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyPoint3d& i_Property) const;
  bool operator!=(const prtyPoint3d& i_Property) const;
  bool operator==(const maPoint3d& i_Value) const;
  bool operator!=(const maPoint3d& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;

protected:
  maPoint3d m_Value;

private:
  bool m_bUseUnits;
};
