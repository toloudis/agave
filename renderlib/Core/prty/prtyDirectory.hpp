/****************************************************************************\
**	prtyDirectory.hpp
**
**		Directory property
**
**
**
\****************************************************************************/
#ifdef PRTY_DIRECTORY_HPP
#error prtyDirectory.hpp multiply included
#endif
#define PRTY_DIRECTORY_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

//============================================================================
//============================================================================
class prtyDirectory : public prtyPropertyTemplate<fsLocator, const fsLocator&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyDirectory();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyDirectory(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyDirectory(const std::string& i_Name, const fsLocator& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  const std::string GetString() const;

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyDirectory& operator=(const prtyDirectory& i_Property);
  prtyDirectory& operator=(const fsLocator& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyDirectory& i_Property) const;
  bool operator!=(const prtyDirectory& i_Property) const;
  bool operator==(const fsLocator& i_Value) const;
  bool operator!=(const fsLocator& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
