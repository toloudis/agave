/****************************************************************************\
**	prtyName.hpp
**
**		Name property
**
**
**
\****************************************************************************/
#ifdef PRTY_NAME_HPP
#error prtyName.hpp multiply included
#endif
#define PRTY_NAME_HPP

#ifndef PRTY_PROPERTY_HPP
#include "Core/prty/prtyProperty.hpp"
#endif

#ifndef NAME_STRING_HPP
#include "Core/name/nameString.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyName : public prtyProperty
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyName();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyName(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyName(const std::string& i_Name, const nameString& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  const nameString& GetValue() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetValue(const nameString& i_Name, bool i_bDirty = false);  // UndoFlags i_Undoable = eNoUndo);
  void SetValue(const std::string& i_Name, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  std::string GetString() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetString(const std::string& i_String);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  nameUID GetUID() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetUID(const nameUID i_UID);

  //--------------------------------------------------------------------
  //	Register the name getting a unique ID
  //--------------------------------------------------------------------
  void RegisterName();

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyName& operator=(const prtyName& i_Value);
  prtyName& operator=(const nameString& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyName& i_Value) const;
  bool operator==(const nameString& i_Value) const;
  bool operator==(const std::string& i_Value) const;
  bool operator!=(const nameString& i_Value) const;
  bool operator!=(const std::string& i_Value) const;

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
  nameString m_Value;
};
