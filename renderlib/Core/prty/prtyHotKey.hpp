/****************************************************************************\
**	prtyHotKey.hpp
**
**		HotKey property
**
**
**
\****************************************************************************/
#ifdef PRTY_HOTKEY_HPP
#error prtyHotKey.hpp multiply included
#endif
#define PRTY_HOTKEY_HPP

#ifndef PRTY_PROPERTY_HPP
#include "Core/prty/prtyProperty.hpp"
#endif

//============================================================================
//============================================================================
class prtyHotKey : public prtyProperty
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyHotKey();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyHotKey(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyHotKey(const std::string& i_Name, const std::string& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  const std::string& GetValue() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetValue(const std::string& i_Value, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  //	ID - unique identifier
  //	On windows systems this ID is used by the OS when registering and
  //	unregistering.  It is generated at the launch of app and not
  //	saved to a file.
  //--------------------------------------------------------------------
  void SetID(short i_ID);
  short GetID() const;

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyHotKey& operator=(const prtyHotKey& i_Value);
  prtyHotKey& operator=(const std::string& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const std::string& i_Value) const;
  bool operator!=(const std::string& i_Value) const;
  bool operator==(const prtyHotKey& i_Value) const;
  bool operator!=(const prtyHotKey& i_Value) const;

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
  std::string m_Value;
  short m_ID; // unique identifier
};
