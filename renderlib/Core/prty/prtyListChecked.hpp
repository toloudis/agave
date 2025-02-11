/****************************************************************************\
**	prtyListChecked.hpp
**
**		ListChecked property
**
**
**
\****************************************************************************/
#ifdef PRTY_LISTCHECKED_HPP
#error prtyListChecked.hpp multiply included
#endif
#define PRTY_LISTCHECKED_HPP

#ifndef PRTY_PROPERTY_HPP
#include "Core/prty/prtyProperty.hpp"
#endif

#include <map>

//============================================================================
//============================================================================
class checked_list_data
{
public:
  checked_list_data()
    : m_Text("")
    , m_bChecked(false){};

  bool operator==(const checked_list_data& i_Data) const
  {
    return ((i_Data.m_bChecked == this->m_bChecked) && (i_Data.m_Text == this->m_Text));
  }

public:
  bool m_bChecked;
  std::string m_Text;
};

typedef std::vector<checked_list_data> checked_list_type;

//============================================================================
//============================================================================
class prtyListChecked : public prtyProperty
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyListChecked();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyListChecked(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyListChecked(const std::string& i_Name, checked_list_type& i_bInitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  int GetNumberOfItems() const;
  void SetNumberOfItems(int i_Size);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void ClearList();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  const checked_list_type& GetValue() const;
  const std::string& GetValueText(int i_Index) const;
  bool GetValueFlag(int i_Index) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetValue(checked_list_type i_Value, bool i_bDirty = false);            // UndoFlags i_Undoable = eNoUndo);
  void SetValueFlag(int i_Index, bool i_bValue, bool i_bDirty = false);       // UndoFlags i_Undoable = eNoUndo);
  void SetValueText(int i_Index, std::string i_Value, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  //	Operators
  //--------------------------------------------------------------------
  prtyListChecked& operator=(const prtyListChecked& i_Value);
  prtyListChecked& operator=(const checked_list_type& i_Value);

  //--------------------------------------------------------------------
  //	Compaison Operators
  //--------------------------------------------------------------------
  const bool operator==(const prtyListChecked& i_Value) const;
  const bool operator==(const checked_list_type& i_Value) const;

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
  checked_list_type m_Value;
};
