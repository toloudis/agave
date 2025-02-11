/****************************************************************************\
**	prtyFilePath.hpp
**
**		FilePath property
**
**
**
\****************************************************************************/
#ifdef PRTY_FILEPATH_HPP
#error prtyFilePath.hpp multiply included
#endif
#define PRTY_FILEPATH_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

//============================================================================
//============================================================================
class prtyFilePath : public prtyPropertyTemplate<fsLocator, const fsLocator&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFilePath();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFilePath(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFilePath(const std::string& i_Name, const fsLocator& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  // Set path value by passing single string
  //--------------------------------------------------------------------
  void SetString(const std::string& i_FilePath, bool i_bDirty = false); // UndoFlags i_Undoable = eNoUndo);

  //--------------------------------------------------------------------
  // Get path as a single string
  //--------------------------------------------------------------------
  const std::string GetString() const;

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyFilePath& operator=(const prtyFilePath& i_Property);
  prtyFilePath& operator=(const fsLocator& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyFilePath& i_Property) const;
  bool operator!=(const prtyFilePath& i_Property) const;
  bool operator==(const fsLocator& i_Value) const;
  bool operator!=(const fsLocator& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
