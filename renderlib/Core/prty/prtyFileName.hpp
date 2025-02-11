/****************************************************************************\
**	prtyFileName.hpp
**
**		FileName property
**
**
**
\****************************************************************************/
#ifdef PRTY_FILENAME_HPP
#error prtyFileName.hpp multiply included
#endif
#define PRTY_FILENAME_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef IT_STRING_HPP
#include "Core/it/itString.hpp"
#endif

//============================================================================
//============================================================================
class prtyFileName : public prtyPropertyTemplate<itString, const itString&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFileName();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFileName(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFileName(const std::string& i_Name, const itString& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //	return a null-terminated std::string
  //--------------------------------------------------------------------
  const std::string GetString() const;

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyFileName& operator=(const prtyFileName& i_Property);
  prtyFileName& operator=(const itString& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyFileName& i_Property) const;
  bool operator!=(const prtyFileName& i_Property) const;
  bool operator==(const itString& i_Value) const;
  bool operator!=(const itString& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
