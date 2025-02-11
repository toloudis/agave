/****************************************************************************\
**	prtyGradient.cpp
**
**		Gradient property
**
**
**
\****************************************************************************/
#include "Core/prty/prtyGradient.hpp"

#include "Core/ch/chChunkParserUtil.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyGradient::prtyGradient()
  : prtyPropertyTemplate("Gradient", maGradient())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyGradient::prtyGradient(const std::string& i_Name)
  : prtyPropertyTemplate(i_Name, maGradient())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyGradient::prtyGradient(const std::string& i_Name, const maGradient& i_InitialValue)
  : prtyPropertyTemplate(i_Name, i_InitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyGradient::GetType()
{
  return "Gradient";
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyGradient&
prtyGradient::operator=(const prtyGradient& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);

  SetValue(i_Property.GetValue());
  return *this;
}

prtyGradient&
prtyGradient::operator=(const maGradient& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyGradient::operator==(const prtyGradient& i_Property) const
{
  return (m_Value == i_Property.GetValue());
}
bool
prtyGradient::operator!=(const prtyGradient& i_Property) const
{
  return (m_Value != i_Property.GetValue());
}
bool
prtyGradient::operator==(const maGradient& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyGradient::operator!=(const maGradient& i_Value) const
{
  return (m_Value != i_Value);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyGradient::Read(chReader& io_Reader)
{
  maGradient temp;
  chChunkParserUtil::Read(io_Reader, temp);
  SetValue(temp);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyGradient::Write(chWriter& io_Writer) const
{
  chChunkParserUtil::Write(io_Writer, GetValue());
}
