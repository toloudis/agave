/****************************************************************************\
**	prtyGradient.hpp
**
**		Gradient property
**
**
**
\****************************************************************************/
#ifdef PRTY_GRADIENT_HPP
#error prtyGradient.hpp multiply included
#endif
#define PRTY_GRADIENT_HPP

#ifndef PRTY_PROPERTYTEMPLATE_HPP
#include "Core/prty/prtyPropertyTemplate.hpp"
#endif

#ifndef MA_GRADIENT_HPP
#include "Core/ma/maGradient.hpp"
#endif

//============================================================================
// Todo: implement the gradient property
// right now it's the same with point3d property
//============================================================================
class prtyGradient : public prtyPropertyTemplate<maGradient, const maGradient&>
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyGradient();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyGradient(const std::string& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyGradient(const std::string& i_Name, const maGradient& i_InitialValue);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  //	operators
  //--------------------------------------------------------------------
  prtyGradient& operator=(const prtyGradient& i_Value);
  prtyGradient& operator=(const maGradient& i_Value);

  //--------------------------------------------------------------------
  //	comparison operators
  //--------------------------------------------------------------------
  bool operator==(const prtyGradient& i_Property) const;
  bool operator!=(const prtyGradient& i_Property) const;
  bool operator==(const maGradient& i_Value) const;
  bool operator!=(const maGradient& i_Value) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
