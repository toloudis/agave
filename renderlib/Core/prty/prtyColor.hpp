/****************************************************************************\
**	prtyColor.hpp
**
**		Color property
**
**
**
\****************************************************************************/
#ifdef PRTY_COLOR_HPP
#error prtyColor.hpp multiply included
#endif
#define PRTY_COLOR_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef MA_FLOATRGBA_HPP
#include "Core/ma/maFloatRGBA.hpp"
#endif

//============================================================================
//============================================================================
class prtyColor : public prtyPropertyTemplate<maFloatRGBA, const maFloatRGBA&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyColor();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyColor(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyColor(const std::string& i_Name, const maFloatRGBA& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyColor& operator=(const prtyColor& i_Property);
  prtyColor& operator=(const maFloatRGBA& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyColor& i_Property) const;
  bool operator!=(const prtyColor& i_Property) const;
  bool operator==(const maFloatRGBA& i_Value) const;
  bool operator!=(const maFloatRGBA& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
