/****************************************************************************\
**	prtyDirectory.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyDirectory.hpp"

#include "Core/ch/chChunkParserUtil.hpp"
#include "Core/fs/fsFileUtil.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyDirectory::prtyDirectory()
  : prtyPropertyTemplate("Directory", fsLocator())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyDirectory::prtyDirectory(const std::string& i_Name)
  : prtyPropertyTemplate(i_Name, fsLocator())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyDirectory::prtyDirectory(const std::string& i_Name, const fsLocator& i_InitialValue)
  : prtyPropertyTemplate(i_Name, i_InitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyDirectory::GetType()
{
  return "Directory";
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
const std::string
prtyDirectory::GetString() const
{
  std::string path;
  fsFileUtil::LocatorToANSIFilename(m_Value, path);
  return path;
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyDirectory&
prtyDirectory::operator=(const prtyDirectory& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);

  SetValue(i_Property.GetValue());
  return *this;
}
prtyDirectory&
prtyDirectory::operator=(const fsLocator& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyDirectory::operator==(const prtyDirectory& i_Property) const
{
  return (m_Value == i_Property.GetValue());
}
bool
prtyDirectory::operator!=(const prtyDirectory& i_Property) const
{
  return (m_Value != i_Property.GetValue());
}
bool
prtyDirectory::operator==(const fsLocator& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyDirectory::operator!=(const fsLocator& i_Value) const
{
  return (m_Value != i_Value);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyDirectory::Read(chReader& io_Reader)
{
  fsLocator filename;
  std::string savefilename;
  chChunkParserUtil::Read(io_Reader, savefilename);
  fsFileUtil::ANSIFilenameToLocator(savefilename, filename);
  SetValue(filename);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyDirectory::Write(chWriter& io_Writer) const
{
  std::string savefilename;
  fsFileUtil::LocatorToANSIFilename(GetValue(), savefilename);
  chChunkParserUtil::Write(io_Writer, savefilename);
}
