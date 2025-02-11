/*****************************************************************************
**  fsXMLParser.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsXMLParser.hpp"

#include "Core/fs/fsFileUtil.hpp"

#include <iostream>
#include <sstream>

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
fsXMLParser::fsXMLParser(const char* i_pBuffer)
{
  m_pBuffer = i_pBuffer;
}

//------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------
fsXMLParser::~fsXMLParser()
{
  free_resources();
}

//------------------------------------------------------------------------
//	open + create the XML file
//------------------------------------------------------------------------
void
fsXMLParser::Open()
{
  this->m_BufferIndex = 0;
}

//------------------------------------------------------------------------
//	finalize + close the XML file
//------------------------------------------------------------------------
void
fsXMLParser::Close()
{
}

//------------------------------------------------------------------------
//	Read the actual values
//------------------------------------------------------------------------
fsXMLData::fs_Node_Type
fsXMLParser::ReadNode(std::string& o_KeyName, std::string& o_Text)
{
  DBG_ASSERT(m_pBuffer != NULL, "cannot have an empty buffer in the Parser");

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
fsXMLParser::Convert(const std::string& i_Text, int& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLParser::Convert(const std::string& i_Text, bool& o_Value)
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
fsXMLParser::Convert(const std::string& i_Text, short& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLParser::Convert(const std::string& i_Text, float& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLParser::Convert(const std::string& i_Text, envType::Int8& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLParser::Convert(const std::string& i_Text, envType::UInt8& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
// void fsXMLParser::Convert(const std::string& i_Text, envType::Int16& o_Value)
//{
//	if (i_Text.length() > 0) o_Value = XmlConvert::ToInt16(new std::string(i_Text.c_str()));
// }
void
fsXMLParser::Convert(const std::string& i_Text, envType::UInt16& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
// void fsXMLParser::Convert(const std::string& i_Text, envType::Int32& o_Value)
//{
//	if (i_Text.length() > 0) o_Value = XmlConvert::ToInt32(new std::string(i_Text.c_str()));
// }
void
fsXMLParser::Convert(const std::string& i_Text, envType::UInt32& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLParser::Convert(const std::string& i_Text, envType::Int64& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLParser::Convert(const std::string& i_Text, envType::UInt64& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
// void fsXMLParser::Convert(const std::string& i_Text, envType::Float32& o_Value)
//{
//	if (i_Text.length() > 0) o_Value = XmlConvert::ToDecimal(new std::string(i_Text.c_str()));
// }
void
fsXMLParser::Convert(const std::string& i_Text, envType::Float64& o_Value)
{
  if (i_Text.length() > 0) {
    std::stringstream ss(i_Text);
    ss >> o_Value;
  }
}
void
fsXMLParser::Convert(const std::string& i_Text, char* o_Value)
{
  if (i_Text.length() > 0)
    strcpy(o_Value, i_Text.c_str());
}
void
fsXMLParser::Convert(const std::string& i_Text, std::string& o_Value)
{
  // if (i_Text.length() > 0)
  o_Value = i_Text;
}
void
fsXMLParser::Convert(const std::string& i_Text, fsLocator& o_Value)
{
  fsFileUtil::ANSIFilenameToLocator(i_Text, o_Value);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::string
fsXMLParser::convert_keyname(std::string& i_KeyName)
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
fsXMLParser::free_resources()
{
  //	this class doesn't own it.
  //
  m_pBuffer = NULL;
}

//------------------------------------------------------------------------
//	Read reads i_NumBytes into the buffer.  If the file is too short
//	to read	i_NumBytes, it will read as many as it can.  The return
//	value is the number actually read into the o_Buffer.  This function
//	also advances the file pointer.
//------------------------------------------------------------------------
int
fsXMLParser::read(envType::Int64 i_NumBytes, void* o_Buffer)
{
  int size = strlen(((char*)m_pBuffer));
  int bytes_to_copy = (((size - m_BufferIndex) > i_NumBytes) ? i_NumBytes : (size - m_BufferIndex));
  strncpy((char*)o_Buffer, &((m_pBuffer)[m_BufferIndex]), bytes_to_copy);
  m_BufferIndex += bytes_to_copy;
  if (m_BufferIndex < size)
    return bytes_to_copy;
  else
    return 0;
}

//------------------------------------------------------------------------
//	read a chunk of data representing a node
//------------------------------------------------------------------------
fsXMLData::fs_Node_Type
fsXMLParser::read_node(std::string& o_Token, int i_MaxChars, char i_bDelimiter, bool i_bComments)
{
  bool bTag = false;
  char c = 0;
  o_Token.resize(0);

  //	Read the token until we get to a break character...
  //
  while (true) {
    if (read(sizeof(c), &c) == 0) {
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
#ifdef FSXMLParser_DEBUG_OUTPUT
      // DBG_LOG("   Comment [" << o_Token.c_str() << "]" );
#endif
      return fsXMLData::e_None;
    } else if (c == '?') // header
    {
      consume_element(o_Token);
#ifdef FSXMLParser_DEBUG_OUTPUT
      DBG_LOG("   Header [" << o_Token.c_str() << "]");
#endif
      return fsXMLData::e_None;
    } else if (c == '/') // end  element
    {
      read_element(o_Token);
#ifdef FSXMLParser_DEBUG_OUTPUT
      DBG_LOG("   End Element [" << o_Token.c_str() << "]");
#endif
      return fsXMLData::e_EndElement;
    } else {
      if (bTag) {
        //	start element
        o_Token.append(sizeof(c), c);
        read_element(o_Token);
#ifdef FSXMLParser_DEBUG_OUTPUT
        DBG_LOG("   Start Element [" << o_Token.c_str() << "]");
#endif
        return fsXMLData::e_Element;
      } else {
        //	content
        o_Token.append(sizeof(c), c);
        read_until_lessthan(o_Token);
#ifdef FSXMLParser_DEBUG_OUTPUT
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
fsXMLParser::consume_element(std::string& o_Token)
{
  char c = 0;

  //	Skip the element until >
  //
  do {
    if (read(sizeof(c), &c) == 0) {
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
fsXMLParser::read_element(std::string& o_Token)
{
  char c = 0;

  //	Skip the white space...
  //
  do {
    if (read(sizeof(c), &c) == 0) {
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
fsXMLParser::read_until_lessthan(std::string& o_Token)
{
  //	Skip the white space...
  //
  char c = 0;
  do {
    if (read(sizeof(c), &c) == 0) {
      return;
    }

    if (c == '<')
      return;

    o_Token.append(1, c);
  } while (true);
}
