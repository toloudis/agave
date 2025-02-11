/****************************************************************************\
**	prtyVideo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyVideo.hpp"

#include "Core/ch/chChunkParserUtil.hpp"
#include "Core/fs/fsFileUtil.hpp"

//------------------------------------------------------------------------
//------------------------------------------------------------------------
CVideoData::CVideoData()
  : m_Frame(0)
  , m_Width(0)
  , m_Height(0)
  , m_pVideo(NULL)
  , m_Filename()
{
}

//------------------------------------------------------------------------
//	Equality
//------------------------------------------------------------------------
bool
CVideoData::operator==(const CVideoData& i_Data) const
{
  if ((m_Filename == i_Data.m_Filename) && (m_Frame == i_Data.m_Frame) && (m_Width == i_Data.m_Width) &&
      (m_Height == i_Data.m_Height) && (m_pVideo == i_Data.m_pVideo)) {
    return true;
  }

  return false;
}

//------------------------------------------------------------------------
//	Inequality
//------------------------------------------------------------------------
bool
CVideoData::operator!=(const CVideoData& i_Data) const
{
  return (!(*this == i_Data));
}

//------------------------------------------------------------------------
//	Assignment
//------------------------------------------------------------------------
const CVideoData&
CVideoData::operator=(const CVideoData& i_CopyFrom)
{
  m_Filename = i_CopyFrom.m_Filename;
  m_Frame = i_CopyFrom.m_Frame;
  m_Width = i_CopyFrom.m_Width;
  m_Height = i_CopyFrom.m_Height;
  m_pVideo = i_CopyFrom.m_pVideo;
  return *this;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVideo::prtyVideo()
  : prtyPropertyTemplate("Video", CVideoData())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVideo::prtyVideo(const std::string& i_Name)
  : prtyPropertyTemplate(i_Name, CVideoData())
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyVideo::prtyVideo(const std::string& i_Name, const CVideoData& i_InitialValue)
  : prtyPropertyTemplate(i_Name, i_InitialValue)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyVideo::GetType()
{
  return "Video";
}

//--------------------------------------------------------------------
// Set path value by passing single string
//--------------------------------------------------------------------
void
prtyVideo::SetString(const std::string& i_FilePath, bool i_bDirty) // UndoFlags i_Undoable )
{
  CVideoData data;
  fsFileUtil::ANSIFilenameToLocator(i_FilePath, data.m_Filename);
  SetValue(data, i_bDirty); // i_Undoable);
}

//--------------------------------------------------------------------
// Get path as a single string
//--------------------------------------------------------------------
const std::string
prtyVideo::GetString() const
{
  std::string path;
  fsFileUtil::LocatorToANSIFilename(m_Value.m_Filename, path);
  return path;
}

//--------------------------------------------------------------------
//	operators
//--------------------------------------------------------------------
prtyVideo&
prtyVideo::operator=(const prtyVideo& i_Property)
{
  // copy base data
  prtyProperty::operator=(i_Property);

  SetValue(i_Property.GetValue());
  return *this;
}
prtyVideo&
prtyVideo::operator=(const CVideoData& i_Value)
{
  SetValue(i_Value);
  return *this;
}

//--------------------------------------------------------------------
//	comparison operators
//--------------------------------------------------------------------
bool
prtyVideo::operator==(const prtyVideo& i_Property) const
{
  return (m_Value == i_Property.GetValue());
}
bool
prtyVideo::operator!=(const prtyVideo& i_Property) const
{
  return (m_Value != i_Property.GetValue());
}
/*
bool prtyVideo::operator ==(const fsLocator& i_Value) const
{
        return (m_Value == i_Value);
}
bool prtyVideo::operator !=(const fsLocator& i_Value) const
{
        return (m_Value != i_Value);
}
*/

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyVideo::Read(chReader& io_Reader)
{
  // bga - Please note that I changed this from ANSI to Unicode but that
  //  there is no way to handle versions within the property Read() function.
  //  It is necessary to find all usages of this property in file formats and then
  //  increase the versions of those chunks. I updated all cases that used this that
  //  I could find.
  CVideoData data;

  // Old ANSI code for reference
  // std::string savefilename;
  // chChunkParserUtil::Read( io_Reader, savefilename );
  // fsFileUtil::ANSIFilenameToLocator( savefilename, filename );

  // Read in a single itString with path separators and then
  // split into locator
  itString savefilename;
  chChunkParserUtil::Read(io_Reader, savefilename);
  fsFileUtil::UnicodeStringToLocator(savefilename, data.m_Filename);
  chChunkParserUtil::Read(io_Reader, data.m_Frame);
  chChunkParserUtil::Read(io_Reader, data.m_Width);
  chChunkParserUtil::Read(io_Reader, data.m_Height);

  SetValue(data);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyVideo::Write(chWriter& io_Writer) const
{
  // Switched to unicode
  // std::string savefilename;
  // fsFileUtil::LocatorToANSIFilename( GetValue(), savefilename );
  // chChunkParserUtil::Write( io_Writer, savefilename );

  // Convert array of itStrings to a single itString with
  // path separators in order to write to file.
  itString savefilename;
  fsFileUtil::LocatorToUnicodeString(GetValue().m_Filename, savefilename);
  chChunkParserUtil::Write(io_Writer, savefilename);
  chChunkParserUtil::Write(io_Writer, GetValue().m_Frame);
  chChunkParserUtil::Write(io_Writer, GetValue().m_Width);
  chChunkParserUtil::Write(io_Writer, GetValue().m_Height);
}
