/*****************************************************************************
**	chXMLWriter.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/

#include "Core/ch/chXMLWriter.hpp"

#include "Core/fs/fsFilePosSaver.hpp"
#include "Core/gf/gfFileUtil.hpp"
#include "Core/gf/gfFileXML.hpp"

#include <stdio.h>
#include <sstream>
#include <string>

//============================================================================
//============================================================================
namespace {
//--------------------------------------------------------------------
// local variables
//--------------------------------------------------------------------
// temp string for stringstream
std::string l_Buf;
std::ostringstream buf_ss(std::ostringstream::out);
bool l_bLastWriteHeader = true;

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
write_char(gfFileXML& i_File, char i_Ch)
{
  i_File.Write(1, &i_Ch);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
write_newline(gfFileXML& i_File)
{
  const char* nl = "\r\n";
  i_File.Write(2, nl);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
write_string(gfFileXML& i_File, const char* i_pStr)
{
  DBG_ASSERT(i_pStr, "write_string, pointer is null");
  i_File.Write(strlen(i_pStr), (void*)i_pStr);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
write_string(gfFileXML& i_File, const std::string& i_Str)
{
  i_File.Write(i_Str.length(), (void*)i_Str.c_str());
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
write_tabs(gfFileXML& i_File, int i_Indentation)
{
  char c('\t');
  for (int i = 0; i < i_Indentation; i++) {
    i_File.Write(sizeof(c), &c);
  }
}
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chXMLWriter::chXMLWriter(gfFileXML& io_File)
  : m_File(io_File)
{
  m_Indentation = 0;
  m_pMap = NULL;
  m_NumContainers = 0;
  m_bCurrentContainer = false;
  l_bLastWriteHeader = true;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chXMLWriter::~chXMLWriter() {}

//--------------------------------------------------------------------
//	Ascii Writer and Reader need map from string to chDefs::Name
//  in order to read and write useful chunk names.
//  The map is pointed to, not owned by the chunk writer.
//--------------------------------------------------------------------
void
chXMLWriter::SetMap(std::map<std::string, chDefs::Name>* i_pMap)
{
  m_pMap = i_pMap;
}

std::map<std::string, chDefs::Name>*
chXMLWriter::GetMap() const
{
  return m_pMap;
}

//--------------------------------------------------------------------
//	WriteChunkHeader writes a chunk header in the file at the
//	current position.
//--------------------------------------------------------------------
void
chXMLWriter::WriteChunkHeader(chDefs::Name i_Name, chDefs::Version i_Version, bool i_bContainer)
{
  l_bLastWriteHeader = true;
  write_tabs(m_File, m_Indentation);

  // default string is conversion of chDefs::Name
  chunk_data_struct cds;
  cds.m_ChunkName = i_Name;
  cds.m_bChunkIsContainer = i_bContainer;
  m_ChunkNameStack.push(cds);

  std::string tempstr(reinterpret_cast<const char*>(&i_Name), 4);
  buf_ss.str("");
  buf_ss << "<" << tempstr.c_str() << " version=\"" << i_Version << "\">";
  std::string str(buf_ss.str());
  if (m_pMap) {
    // use std::map from i_Name to meaningful string,
    // map's natural direction is used in reading, for
    // writing we have to iterate ourself.
    std::map<std::string, chDefs::Name>::iterator it;
    for (it = m_pMap->begin(); it != m_pMap->end(); ++it) {
      if ((*it).second == i_Name) {
        str = (*it).first;
        break;
      }
    }
  }
  write_string(m_File, str);

  m_NumContainers++;
  m_bCurrentContainer = i_bContainer;
  if (i_bContainer) {
    // if writing a container chunk, add "{"
    write_newline(m_File);
    m_Indentation++;
  } else {
    write_char(m_File, ' ');
  }
}

//--------------------------------------------------------------------
//	FinishChunk will cause the writer to compute the size of the
//	chunk and finish writing the header.
//--------------------------------------------------------------------
void
chXMLWriter::FinishChunk()
{
  DBG_ASSERT(m_NumContainers > 0, "FinishChunk w/o corresponding WriteChunkHeader");

  // default string is conversion of chDefs::Name
  chDefs::Name chunk_name;
  chunk_name = m_ChunkNameStack.top().m_ChunkName;
  bool bContainer = m_ChunkNameStack.top().m_bChunkIsContainer;
  m_ChunkNameStack.pop();

  std::string tempstr(reinterpret_cast<const char*>(&chunk_name), 4);

  buf_ss.str("");
  buf_ss << "</" << tempstr.c_str() << ">";
  std::string str(buf_ss.str());

  if (m_pMap) {
    // use std::map from i_Name to meaningful string,
    // map's natural direction is used in reading, for
    // writing we have to iterate ourself.
    std::map<std::string, chDefs::Name>::iterator it;
    for (it = m_pMap->begin(); it != m_pMap->end(); ++it) {
      if ((*it).second == chunk_name) {
        str = (*it).first;
        break;
      }
    }
  }

  // if currently writing a container chunk, add "}"
  if (bContainer) {
    m_Indentation--;
    write_tabs(m_File, m_Indentation);
  }
  write_string(m_File, str);
  write_newline(m_File);

  m_NumContainers--;
  m_bCurrentContainer = true;
  l_bLastWriteHeader = false;
}

//--------------------------------------------------------------------
//	Write functions - These write the data to the chunk file.
//--------------------------------------------------------------------
void
chXMLWriter::Write(envType::Int8 i_Val)
{

  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::UInt8 i_Val)
{
  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::Int16 i_Val)
{
  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::UInt16 i_Val)
{
  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::Int32 i_Val)
{
  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::UInt32 i_Val)
{
  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::Int64 i_Val)
{
  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::UInt64 i_Val)
{
  buf_ss.str("");
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::Float32 i_Val)
{
  buf_ss.str("");
  buf_ss.setf(0, std::ios::floatfield);
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

void
chXMLWriter::Write(envType::Float64 i_Val)
{
  buf_ss.str("");
  buf_ss.setf(0, std::ios::floatfield);
  buf_ss << i_Val << " ";
  l_Buf = buf_ss.str();

  m_File.Write(l_Buf.length(), l_Buf.c_str());
  l_bLastWriteHeader = false;
}

//--------------------------------------------------------------------
//	This Write writes a string called either "TRUE" or "FALSE" based
//  on a bool value.
//--------------------------------------------------------------------
void
chXMLWriter::Write(bool i_Val)
{
  if (i_Val) {
    Write("TRUE");
  } else {
    Write("FALSE");
  }
  l_bLastWriteHeader = false;
}

//--------------------------------------------------------------------
//	This Write writes a NULL-terminated single-byte character string
//--------------------------------------------------------------------
void
chXMLWriter::Write(const char* i_Val)
{
  std::string temp = i_Val;

  m_File.WriteToken(temp);
  write_char(m_File, ' ');
  l_bLastWriteHeader = false;
}

//--------------------------------------------------------------------
//	This Write writes a single-byte character string with a length
//	given by i_Length.
//--------------------------------------------------------------------
void
chXMLWriter::Write(const char* i_Val, int i_Length)
{
  std::string str;
  str.resize(i_Length);
  for (int i = 0; i < i_Length; i++) {
    str[i] = i_Val[i];
  }

  m_File.WriteToken(str);
  write_char(m_File, ' ');
  l_bLastWriteHeader = false;
}

//--------------------------------------------------------------------
//	This Write writes a NULL-terminated Unicode character string
//--------------------------------------------------------------------
void
chXMLWriter::Write(const envType::WChar* i_Val)
{
  // convert to ascii, UNICODE shouldn't be in data files
  // anyway. it should be in gfTextSetFile only.
  //
  const envType::WChar* ptr = i_Val;
  char ch;
  std::string str;
  while (*ptr) {
    ch = (char)(*ptr);
    DBG_ASSERT((((*ptr) & 0xff00) == 0), "Can't write true unicode into ascii files.");
    str += ch;
    ptr++;
  }

  m_File.WriteToken(str);
  write_char(m_File, ' ');
  l_bLastWriteHeader = false;
}

//--------------------------------------------------------------------
//	This Write writes a Unicode character string with a length
//	given by i_Length.
//--------------------------------------------------------------------
void
chXMLWriter::Write(const envType::WChar* i_Val, int i_Length)
{
  // convert to ascii, UNICODE shouldn't be in data files
  // anyway. it should be in gfTextSetFile only.
  //
  char ch;
  std::string str;
  for (int i = 0; i < i_Length; i++) {
    ch = (char)i_Val[i];
    DBG_ASSERT(((ch & 0xff00) == 0), "Can't write true unicode into ascii files.");
    str += ch;
  }

  m_File.WriteToken(str);
  write_char(m_File, ' ');
  l_bLastWriteHeader = false;
}

//--------------------------------------------------------------------
//	Write the envAppVersion structure
//--------------------------------------------------------------------
void
chXMLWriter::Write(const envAppVersion& i_Val)
{
  DBG_ASSERT(false, "application version write functionality not implemented yet");
}

//------------------------------------------------------------------------
//	GetLocator returns this file's associated locator
//------------------------------------------------------------------------
const fsLocator&
chXMLWriter::GetLocator() const
{
  return m_File.GetLocator();
}
