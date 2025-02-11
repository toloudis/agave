/*****************************************************************************
**  fsXMLReader.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsXMLReader.hpp"

#include "Core/fs/fsFileX.hpp"
#include "Core/fs/fsFileUtil.hpp"

#include <iostream>
#include <sstream>

//============================================================================
//============================================================================
// #define FSXMLREADER_DEBUG_OUTPUT

//============================================================================
//============================================================================
namespace {
std::string last_element_read;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline bool
is_invalid(char i_Ch)
{
  return (i_Ch < 0);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline bool
is_whitespace(char i_Ch)
{
  // make colon whitespace to make "name:value" pairs easier
  return ((isspace(i_Ch) != 0) /*|| (i_Ch == ':')*/);
}
}

//------------------------------------------------------------------------
//	constructors
//------------------------------------------------------------------------
fsXMLReader::fsXMLReader(const fsLocator& i_Filename)
  : m_Filename(i_Filename)
  , m_pFileStream(NULL)
{
}
fsXMLReader::fsXMLReader(const std::string& i_Filename)
  : m_pFileStream(NULL)
{
  fsFileUtil::ANSIFilenameToLocator(i_Filename, m_Filename);
}
fsXMLReader::fsXMLReader(const char* i_Filename)
  : m_pFileStream(NULL)
{
  fsFileUtil::ANSIFilenameToLocator(std::string(i_Filename), m_Filename);
}

//------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------
fsXMLReader::~fsXMLReader()
{
  free_resources();
}

//------------------------------------------------------------------------
//	open + create the XML file
//------------------------------------------------------------------------
void
fsXMLReader::Open()
{
  //	open the file
  m_pFileStream = new fsFileStream(m_Filename, fsFileStream::e_ReadOnly);

  last_element_read = "";

  //
#ifdef FSXMLREADER_DEBUG_OUTPUT
  std::string outstr;
  fsFileUtil::LocatorToANSIFilename(m_Filename, outstr);
  DBG_LOG("----reading XML file (" << outstr.c_str() << ")");
#endif

  //	skip the XML header
}

//------------------------------------------------------------------------
//	finalize + close the XML file
//------------------------------------------------------------------------
void
fsXMLReader::Close()
{
  //
#ifdef FSXMLREADER_DEBUG_OUTPUT
  std::string outstr;
  fsFileUtil::LocatorToANSIFilename(m_Filename, outstr);
  DBG_LOG("  --closing XML file (" << outstr.c_str() << ")");
#endif

  //
  free_resources();
}

//------------------------------------------------------------------------
//	Read the actual values
//------------------------------------------------------------------------
fsXMLData::fs_Node_Type
fsXMLReader::ReadNode(std::string& o_KeyName, std::string& o_Text)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Reader");

  std::string token;
  fsXMLData::fs_Node_Type node_type = read_node(token);

  if (node_type == fsXMLData::e_EndElement) {
    o_KeyName = token;
    o_Text = "";
  }
  if (node_type == fsXMLData::e_Element) {
    o_KeyName = token;
    o_Text = "";
    last_element_read = token;
  }
  if (node_type == fsXMLData::e_None) {
    o_KeyName = "";
    o_Text = "";
  }
  if (node_type == fsXMLData::e_Text) {
    o_KeyName = last_element_read;
    o_Text = token;
  }

  return node_type;
}

//------------------------------------------------------------------------
//	Convert values
//
//	other possible elements to implement are:
//		maVector3d
//		maRotation
//		maPoint3d
//		maMatrix4x4
//		maFloatRGBA
//		itString
//		fsLocator
//		nameString
//------------------------------------------------------------------------
void
fsXMLReader::Convert(const std::string& i_Text, int& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, bool& o_Value)
{
  if (i_Text.length() > 0) {
    if ((i_Text == "true") || (i_Text == "TRUE") || (i_Text == "True"))
      o_Value = true;
    else if ((i_Text == "false") || (i_Text == "FALSE") || (i_Text == "False"))
      o_Value = false;
    else {
      std::stringstream ss(i_Text);
      ss >> o_Value;
    }
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, short& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, float& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, envType::Int8& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, envType::UInt8& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
// void fsXMLReader::Convert(const std::string& i_Text, envType::Int16& o_Value)
//{
//	if (i_Text.length() > 0) o_Value = XmlConvert::ToInt16(new std::string(i_Text.c_str()));
// }
void
fsXMLReader::Convert(const std::string& i_Text, envType::UInt16& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
// void fsXMLReader::Convert(const std::string& i_Text, envType::Int32& o_Value)
//{
//	if (i_Text.length() > 0) o_Value = XmlConvert::ToInt32(new std::string(i_Text.c_str()));
// }
void
fsXMLReader::Convert(const std::string& i_Text, envType::UInt32& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, envType::Int64& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, envType::UInt64& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
// void fsXMLReader::Convert(const std::string& i_Text, envType::Float32& o_Value)
//{
//	if (i_Text.length() > 0) o_Value = XmlConvert::ToDecimal(new std::string(i_Text.c_str()));
// }
void
fsXMLReader::Convert(const std::string& i_Text, envType::Float64& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLReader::Convert(const std::string& i_Text, char* o_Value)
{
  if (i_Text.length() > 0)
    strcpy(o_Value, i_Text.c_str());
}
void
fsXMLReader::Convert(const std::string& i_Text, std::string& o_Value)
{
  // if (i_Text.length() > 0)
  o_Value = i_Text;
}
void
fsXMLReader::Convert(const std::string& i_Text, fsLocator& o_Value)
{
  fsFileUtil::ANSIFilenameToLocator(i_Text, o_Value);
}

//------------------------------------------------------------------------
//	set_file_pos sets the file pointer to the given value.  It is valid
//	to move the file pointer beyond the end of the file.
//------------------------------------------------------------------------
void
fsXMLReader::set_file_pos(fsFileStream::FilePosType i_Pos, fsFileStream::AccessPointType i_DesiredAccessPoint)
{
  m_pFileStream->SetFilePos(i_Pos, i_DesiredAccessPoint);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::string
fsXMLReader::convert_keyname(std::string& i_KeyName)
{
  //	take out spaces in the element name because it isn't allowed.
  //	the read will reverse this.
  //
  std::string keystr(i_KeyName);

  int idx = 0;
  while ((idx = keystr.find_first_of(',', idx)) >= 0) {
    keystr.replace(idx, 1, "_");
  }
  return keystr;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsXMLReader::free_resources()
{
  if (m_pFileStream != NULL)
    delete m_pFileStream;
  m_pFileStream = NULL;
}

//------------------------------------------------------------------------
//	Read reads i_NumBytes into the buffer.  If the file is too short
//	to read	i_NumBytes, it will read as many as it can.  The return
//	value is the number actually read into the o_Buffer.  This function
//	also advances the file pointer.
//------------------------------------------------------------------------
int
fsXMLReader::read(envType::Int64 i_NumBytes, void* o_Buffer)
{
  return m_pFileStream->Read(i_NumBytes, o_Buffer);
}

//------------------------------------------------------------------------
//	read a chunk of data representing a node
//------------------------------------------------------------------------
fsXMLData::fs_Node_Type
fsXMLReader::read_node(std::string& o_Token, int i_MaxChars, char i_bDelimiter, bool i_bComments)
{
  bool bTag = false;
  char c = 0;
  o_Token.resize(0);

  //	Read the token until we get to a break character...
  //
  while (true) {
    if (!read(sizeof(c), &c)) {
      return fsXMLData::e_EOF;
    }

    if (c == '<') {
      bTag = true;
    }

    if (is_invalid(c) || is_whitespace(c) || (c == '<')) {
      // skip
    } else if (c == '!') // comment
    {
      consume_element(o_Token);
#ifdef FSXMLREADER_DEBUG_OUTPUT
      // DBG_LOG("   Comment [" << o_Token.c_str() << "]" );
#endif
      return fsXMLData::e_None;
    } else if (c == '?') // header
    {
      consume_element(o_Token);
#ifdef FSXMLREADER_DEBUG_OUTPUT
      DBG_LOG("   Header [" << o_Token.c_str() << "]");
#endif
      return fsXMLData::e_None;
    } else if (c == '/') // end  element
    {
      read_element(o_Token);
#ifdef FSXMLREADER_DEBUG_OUTPUT
      DBG_LOG("   End Element [" << o_Token.c_str() << "]");
#endif
      return fsXMLData::e_EndElement;
    } else {
      if (bTag) {
        //	start element
        o_Token.append(sizeof(c), c);
        read_element(o_Token);
#ifdef FSXMLREADER_DEBUG_OUTPUT
        DBG_LOG("   Start Element [" << o_Token.c_str() << "]");
#endif
        return fsXMLData::e_Element;
      } else {
        //	content
        o_Token.append(sizeof(c), c);
        read_until_lessthan(o_Token);
#ifdef FSXMLREADER_DEBUG_OUTPUT
        DBG_LOG("   Content [" << o_Token.c_str() << "]");
#endif
        return fsXMLData::e_Text;
      }
    }
  }
}

//------------------------------------------------------------------------
//	read until a > is encountered.  This function does NOT clear the
//	token parameter, it just appends to what is already there.
//------------------------------------------------------------------------
void
fsXMLReader::consume_element(std::string& o_Token)
{
  char c = 0;

  //	Skip the element until >
  //
  do {
    if (!read(sizeof(c), &c)) {
      return;
    }

    if (c == '>')
      return;

    o_Token.append(1, c);
  } while (true);
}

//------------------------------------------------------------------------
//	read until a > is encountered.  This function does NOT clear the
//	token parameter, it just appends to what is already there.
//------------------------------------------------------------------------
void
fsXMLReader::read_element(std::string& o_Token)
{
  char c = 0;

  //	Skip the white space...
  //
  do {
    if (!read(sizeof(c), &c)) {
      return;
    }

    if (c == '>')
      return;

    if (c == '/')
      return;

    if (c == ' ')
      return;

    o_Token.append(1, c);
  } while (true);
}

//------------------------------------------------------------------------
//	read until a < is encountered.  This function does NOT clear the
//	token parameter, it just appends to what is already there.
//------------------------------------------------------------------------
void
fsXMLReader::read_until_lessthan(std::string& o_Token)
{
  //	Skip the white space...
  //
  char c = 0;
  do {
    if (!read(sizeof(c), &c)) {
      return;
    }

    if (c == '<')
      return;

    o_Token.append(1, c);
  } while (true);
}
