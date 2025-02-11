/****************************************************************************\
**	prtyInt8.hpp
**
**		Int8 property
**
**	NOTE: a possible code optimization would be to elminate the int8 and
**	just use int32 and set the min and max values to be correct.
**
**
**
\****************************************************************************/
#ifdef PRTY_INT8_HPP
#error prtyInt8.hpp multiply included
#endif
#define PRTY_INT8_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif
#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

//============================================================================
//============================================================================
class prtyInt8 : public prtyPropertyTemplate<envType::Int8, envType::Int8>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyInt8();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyInt8(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyInt8(const std::string& i_Name, envType::Int8 i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyInt8& operator=(const prtyInt8& i_Property);
  prtyInt8& operator=(const envType::Int8 i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyInt8& i_Property) const;
  bool operator!=(const prtyInt8& i_Property) const;
  bool operator==(const envType::Int8 i_Value) const;
  bool operator!=(const envType::Int8 i_Value) const;

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator>(const envType::Int8 i_Value) const;
  bool operator>=(const envType::Int8 i_Value) const;
  bool operator<(const envType::Int8 i_Value) const;
  bool operator<=(const envType::Int8 i_Value) const;
  bool operator>(const prtyInt8& i_Value) const;
  bool operator>=(const prtyInt8& i_Value) const;
  bool operator<(const prtyInt8& i_Value) const;
  bool operator<=(const prtyInt8& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
