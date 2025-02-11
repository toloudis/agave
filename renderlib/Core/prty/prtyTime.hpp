/****************************************************************************\
**	prtyTime.hpp
**
**		Time property using maTime (time units as 6000fps frame number)
**
**
**
\****************************************************************************/
#ifdef PRTY_TIME_HPP
#error prtyTime.hpp multiply included
#endif
#define PRTY_TIME_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif
#ifndef MA_TIME_HPP
#include "Core/Ma/maTime.hpp"
#endif

//============================================================================
//============================================================================
class prtyTime : public prtyPropertyTemplate<maTime, const maTime&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTime();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTime(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTime(const std::string& i_Name, const maTime& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyTime& operator=(const prtyTime& i_Property);
  prtyTime& operator=(const maTime& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyTime& i_Property) const;
  bool operator!=(const prtyTime& i_Property) const;
  bool operator==(const maTime& i_Value) const;
  bool operator!=(const maTime& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
