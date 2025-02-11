/****************************************************************************\
**	prtyTime.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyTime.hpp"

#include "Core/ch/chReader.hpp"
#include "Core/ch/chWriter.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTime::prtyTime()
  : prtyPropertyTemplate("Time", maTime())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTime::prtyTime(const std::string& i_Name)
  : prtyPropertyTemplate(i_Name, maTime())
{
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
prtyTime::prtyTime(const std::string& i_Name, const maTime& i_InitialValue)
  : prtyPropertyTemplate(i_Name, i_InitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyTime::GetType()
{
  return "Time";
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyTime&
prtyTime::operator=(const prtyTime& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);

  SetValue(i_Property.GetValue());
  return *this;
}
prtyTime&
prtyTime::operator=(const maTime& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyTime::operator==(const prtyTime& i_Property) const
{
  return (m_Value == i_Property.GetValue());
}
bool
prtyTime::operator!=(const prtyTime& i_Property) const
{
  return (m_Value != i_Property.GetValue());
}
bool
prtyTime::operator==(const maTime& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyTime::operator!=(const maTime& i_Value) const
{
  return (m_Value != i_Value);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyTime::Read(chReader& io_Reader)
{
  // prtyTime is replacing prtyFloat using seconds, so continue to write as float for now.
  envType::Float32 timeInSeconds = 0;
  io_Reader.Read(timeInSeconds);
  SetValue(maTime::FromSeconds(timeInSeconds));
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyTime::Write(chWriter& io_Writer) const
{
  // prtyTime is replacing prtyFloat using seconds, so continue to write as float for now.
  envType::Float32 timeInSeconds = GetValue().AsSeconds();
  io_Writer.Write(timeInSeconds);
}
