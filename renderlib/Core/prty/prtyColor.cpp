/****************************************************************************\
**	prtyColor.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyColor.hpp"

#include "Core/ch/chChunkParserUtil.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyColor::prtyColor()
  : prtyPropertyTemplate("Color", maFloatRGBA(0, 0, 0, 0))
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyColor::prtyColor(const std::string& i_Name)
  : prtyPropertyTemplate(i_Name, maFloatRGBA(0, 0, 0, 0))
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyColor::prtyColor(const std::string& i_Name, const maFloatRGBA& i_InitialValue)
  : prtyPropertyTemplate(i_Name, i_InitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyColor::GetType()
{
  return "Color";
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyColor&
prtyColor::operator=(const prtyColor& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);

  SetValue(i_Property.GetValue());
  return *this;
}
prtyColor&
prtyColor::operator=(const maFloatRGBA& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyColor::operator==(const prtyColor& i_Property) const
{
  return (m_Value == i_Property.GetValue());
}
bool
prtyColor::operator!=(const prtyColor& i_Property) const
{
  return (m_Value != i_Property.GetValue());
}
bool
prtyColor::operator==(const maFloatRGBA& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyColor::operator!=(const maFloatRGBA& i_Value) const
{
  return (m_Value != i_Value);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyColor::Read(chReader& io_Reader)
{
  maFloatRGBA temp;
  chChunkParserUtil::Read(io_Reader, temp);
  SetValue(temp);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyColor::Write(chWriter& io_Writer) const
{
  chChunkParserUtil::Write(io_Writer, GetValue());
}
