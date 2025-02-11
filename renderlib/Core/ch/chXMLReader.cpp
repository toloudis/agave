/*****************************************************************************
**  chXMLReader.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/ch/chXMLReader.hpp"

#include "Core/ch/chExceptionX.hpp"
#include "Core/gf/gfFileXML.hpp"

//============================================================================
//============================================================================
namespace {
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
template<class T>
bool
read_int(gfFileXML& i_File, T& o_Val)
{
  std::string tag;
  if (!i_File.ReadToken(tag)) {
    o_Val = 0;
    return false;
  }

  o_Val = atoi(tag.c_str());
  return true;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
template<class T>
bool
read_float(gfFileXML& i_File, T& o_Val)
{
  std::string tag;
  if (!i_File.ReadToken(tag)) {
    o_Val = 0;
    return false;
  }

  o_Val = (float)atof(tag.c_str());
  return true;
}

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
chXMLReader::chXMLReader(gfFileXML& io_File)
  : m_File(io_File)
{
  m_pMap = NULL;
  m_NumContainers = 0;
  m_bCurrentContainer = false;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
chXMLReader::~chXMLReader() {}

//----------------------------------------------------------------------------
//	Ascii Writer and Reader need map from string to chDefs::Name
//  in order to read and write useful chunk names.
//----------------------------------------------------------------------------
void
chXMLReader::SetMap(std::map<std::string, chDefs::Name>* i_pMap)
{
  m_pMap = i_pMap;
}

std::map<std::string, chDefs::Name>*
chXMLReader::GetMap() const
{
  return m_pMap;
}

//----------------------------------------------------------------------------
//	ReadChunkHeader reads a chunk header in the file at the
//	current position.  If there is a problem with the header,
//	it will throw a chInvalidChunkX.  Returns true if more chunks to
//	read, false if no more.
//	The size is the size of the chunk not including the header.
//----------------------------------------------------------------------------
bool
chXMLReader::ReadChunkHeader(chDefs::Name& o_Name, chDefs::Version& o_Version, chDefs::Size& o_Size)
{
  std::string XMLToken;

  fsFileStream::FilePosType cur_pos = m_File.GetFilePos();
  if (!m_File.ReadToken(XMLToken)) {
    // hit end of file
    return false;
  }

  //	strip the XML parts off of the token
  //
  std::string Token;
  for (int c = 0; c < XMLToken.size(); ++c) {
    if ((XMLToken[c] != '>') && (XMLToken[c] != '<') && (XMLToken[c] != '/')) {
      Token.push_back(XMLToken[c]);
    }
  }

  //	if end token return false
  if (XMLToken.find("</") != std::string::npos) {
    // Back up to before the end tag was read, let FinishHeader
    // handle it
    m_File.SetFilePos(cur_pos, fsFileStream::e_Beginning);
    return false;
  }

  //	Convert from Token to header
  int len = Token.length();
  o_Name = chDefs::MakeName(
    (len > 0) ? Token[0] : '\0', (len > 1) ? Token[1] : '\0', (len > 2) ? Token[2] : '\0', (len > 3) ? Token[3] : '\0');
  o_Version = 0;
  o_Size = 0;

  if (m_pMap) {
    // use std::map from string to i_Name
    // to map useful name to chDefs::Name.
    // This is a case-sensitive compare
    //
    std::map<std::string, chDefs::Name>::iterator it = m_pMap->find(Token);
    if (it != m_pMap->end()) {
      o_Name = (*it).second;
    }
  }

  //	See if there is a '>' in the XMLToken, otherwise read the rest of the tag
  int endindex = XMLToken.find(">");
  if (endindex == std::string::npos) {
    bool bReadData;
    std::string TagAttrib;
    cur_pos = m_File.GetFilePos();
    while (bReadData = m_File.ReadToken(TagAttrib)) {
      //	This is an attribute, parse it
      //	example: <CTDP version="1"> 9.960000 </CTDP>
      //
      // if (TagAttrib.find(">") == std::string::npos)
      {
        const char* VERSION_ATTRTIB = "version=";
        int aindex;
        if (aindex = TagAttrib.find(VERSION_ATTRTIB) != std::string::npos) {
          unsigned int ui;
          sscanf(TagAttrib.c_str(), "version=\"%u\"", &ui);
          o_Version = (chDefs::Version)(ui);
          // sscanf(TagAttrib.c_str(),"version=\"%u\"", &o_Version);
        }
      }

      //	jump out, done with attributes
      if (TagAttrib.find(">") != std::string::npos) {
        break;
      }
    }

    if (!bReadData) {
      return false;
    }
  }

  DBG_LOG("ReadChunkHeader: read " << Token.c_str() << " version " << o_Version);

  //	See if this chunk is a container
  cur_pos = m_File.GetFilePos();
  bool bContainer = true;
  if (m_File.ReadToken(Token)) {
    if (Token[0] != '<') {
      // Not a start token, back up to where we were
      m_File.SetFilePos(cur_pos, fsFileStream::e_Beginning);
      bContainer = false;
    } else {
      // Not a start token, back up to where we were
      m_File.SetFilePos(cur_pos, fsFileStream::e_Beginning);
      bContainer = true;
    }
  }

  m_NumContainers++;
  m_bCurrentContainer = bContainer;
  return true;
}

//----------------------------------------------------------------------------
//	FinishChunk will cause the reader to advance to the next position
//	after the end of the current chunk.
//----------------------------------------------------------------------------
void
chXMLReader::FinishChunk()
{
  DBG_ASSERT(m_NumContainers > 0, "Finished a chunk that wasn't started");

  std::string Token;

  fsFileStream::FilePosType cur_pos = m_File.GetFilePos();
  if (!m_File.ReadToken(Token)) {
    // hit end of file
    return;
  }

  if ((Token[0] != '<') && (Token[1] != '/')) {
    // Not an end token, back up to where we were
    m_File.SetFilePos(cur_pos, fsFileStream::e_Beginning);
    // bContainer = false;
  }

  m_NumContainers--;
  m_bCurrentContainer = true;
}

//----------------------------------------------------------------------------
//	Read functions - These will be overridden to read the data
//	given in the appropriate format.
//	It would have been nice to make these template member functions,
//	but I think there would be problems getting them instantiated
//	so that they are part of the abstract base interface.
//----------------------------------------------------------------------------
void
chXMLReader::Read(envType::Int8& o_Val)
{
  read_int<envType::Int8>(m_File, o_Val);
}

void
chXMLReader::Read(envType::UInt8& o_Val)
{
  read_int<envType::UInt8>(m_File, o_Val);
}

void
chXMLReader::Read(envType::Int16& o_Val)
{
  read_int<envType::Int16>(m_File, o_Val);
}

void
chXMLReader::Read(envType::UInt16& o_Val)
{
  read_int<envType::UInt16>(m_File, o_Val);
}

void
chXMLReader::Read(envType::Int32& o_Val)
{
  read_int<envType::Int32>(m_File, o_Val);
}

void
chXMLReader::Read(envType::UInt32& o_Val)
{
  read_int<envType::UInt32>(m_File, o_Val);
}

void
chXMLReader::Read(envType::Int64& o_Val)
{
  read_int<envType::Int64>(m_File, o_Val);
}

void
chXMLReader::Read(envType::UInt64& o_Val)
{
  read_int<envType::UInt64>(m_File, o_Val);
}

void
chXMLReader::Read(envType::Float32& o_Val)
{
  read_float<envType::Float32>(m_File, o_Val);
}

void
chXMLReader::Read(envType::Float64& o_Val)
{
  read_float<envType::Float64>(m_File, o_Val);
}

//----------------------------------------------------------------------------
//	This Read reads a string called either "TRUE" or "FALSE" and
//  converts it into a bool value.
//----------------------------------------------------------------------------
void
chXMLReader::Read(bool& o_Val)
{
  // Read the bool value from the file as a string.
  std::string BoolString;
  Read(BoolString);

  // Convert the string into a bool and return.
  o_Val = (!_stricmp(BoolString.c_str(), "TRUE")) ? true : false;
}

//----------------------------------------------------------------------------
//	This Read reads a NULL-terminated single-byte character string.
//----------------------------------------------------------------------------
void
chXMLReader::Read(std::string& o_String)
{
  m_File.ReadToken(o_String);
}

//----------------------------------------------------------------------------
//	This Read reads a single-byte character string with a length
//	given by i_Length.
//----------------------------------------------------------------------------
void
chXMLReader::Read(std::string& o_String, int i_Length)
{
  m_File.ReadToken(o_String, i_Length);
}

//----------------------------------------------------------------------------
//	This Read reads a NULL-terminated Unicode character string.
//----------------------------------------------------------------------------
void
chXMLReader::Read(itString& o_Val)
{
  std::string str;
  m_File.ReadToken(str);

  int len = str.length();
  o_Val.Clear();

  for (int i = 0; i < len; i++) {
    o_Val += str[i];
  }

  // null-terminating not needed anymore
  // o_Val += '\0';
}

//----------------------------------------------------------------------------
//	This Read Reads a Unicode character string with a length
//	given by i_Length.
//----------------------------------------------------------------------------
void
chXMLReader::Read(itString& o_Val, int i_Length)
{
  std::string str;
  m_File.ReadToken(str, i_Length);

  int len = str.length();
  o_Val.Clear();

  for (int i = 0; i < len; i++) {
    o_Val += str[i];
  }
}

//--------------------------------------------------------------------
//	Read in an envAppVersion structure
//--------------------------------------------------------------------
void
chXMLReader::Read(envAppVersion& o_Val)
{
  DBG_ASSERT(false, "application version read functionality not implemented yet");
}

//----------------------------------------------------------------------------
//	GetLocator returns this file's associated locator
//----------------------------------------------------------------------------
const fsLocator&
chXMLReader::GetLocator() const
{
  return m_File.GetLocator();
}

//----------------------------------------------------------------------------
//	GetFilePos returns the current position of the file pointer.
//----------------------------------------------------------------------------
fsFileStream::FilePosType
chXMLReader::GetFilePos() const
{
  return m_File.GetFilePos();
}
