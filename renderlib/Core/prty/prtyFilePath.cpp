/****************************************************************************\
**	prtyFilePath.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyFilePath.hpp"

#include "Core/ch/chChunkParserUtil.hpp"
#include "Core/fs/fsFileUtil.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFilePath::prtyFilePath()
  : prtyPropertyTemplate("File Path", fsLocator())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFilePath::prtyFilePath(const std::string& i_Name)
  : prtyPropertyTemplate(i_Name, fsLocator())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFilePath::prtyFilePath(const std::string& i_Name, const fsLocator& i_InitialValue)
  : prtyPropertyTemplate(i_Name, i_InitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyFilePath::GetType()
{
  return "File Path";
}

//--------------------------------------------------------------------
// Set path value by passing single string
//--------------------------------------------------------------------
void
prtyFilePath::SetString(const std::string& i_FilePath, bool i_bDirty) // UndoFlags i_Undoable )
{
  fsLocator path;
  fsFileUtil::ANSIFilenameToLocator(i_FilePath, path);
  SetValue(path, i_bDirty); // i_Undoable);
}

//--------------------------------------------------------------------
// Get path as a single string
//--------------------------------------------------------------------
const std::string
prtyFilePath::GetString() const
{
  std::string path;
  fsFileUtil::LocatorToANSIFilename(m_Value, path);
  return path;
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyFilePath&
prtyFilePath::operator=(const prtyFilePath& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);

  SetValue(i_Property.GetValue());
  return *this;
}
prtyFilePath&
prtyFilePath::operator=(const fsLocator& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyFilePath::operator==(const prtyFilePath& i_Property) const
{
  return (m_Value == i_Property.GetValue());
}
bool
prtyFilePath::operator!=(const prtyFilePath& i_Property) const
{
  return (m_Value != i_Property.GetValue());
}
bool
prtyFilePath::operator==(const fsLocator& i_Value) const
{
  return (m_Value == i_Value);
}
bool
prtyFilePath::operator!=(const fsLocator& i_Value) const
{
  return (m_Value != i_Value);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyFilePath::Read(chReader& io_Reader)
{
  // bga - Please note that I changed this from ANSI to Unicode but that
  //  there is no way to handle versions within the property Read() function.
  //  It is necessary to find all usages of this property in file formats and then
  //  increase the versions of those chunks. I updated all cases that used this that
  //  I could find.
  fsLocator filename;

  // Old ANSI code for reference
  // std::string savefilename;
  // chChunkParserUtil::Read( io_Reader, savefilename );
  // fsFileUtil::ANSIFilenameToLocator( savefilename, filename );

  // Read in a single itString with path separators and then
  // split into locator
  itString savefilename;
  chChunkParserUtil::Read(io_Reader, savefilename);
  fsFileUtil::UnicodeStringToLocator(savefilename, filename);

  SetValue(filename);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyFilePath::Write(chWriter& io_Writer) const
{
  // Switched to unicode
  // std::string savefilename;
  // fsFileUtil::LocatorToANSIFilename( GetValue(), savefilename );
  // chChunkParserUtil::Write( io_Writer, savefilename );

  // Convert array of itStrings to a single itString with
  // path separators in order to write to file.
  itString savefilename;
  fsFileUtil::LocatorToUnicodeString(GetValue(), savefilename);
  chChunkParserUtil::Write(io_Writer, savefilename);
}
