/*****************************************************************************
**  fsXMLParser.hpp
**
**      XML Parser for in-memory data
**
**
**
\****************************************************************************/
#ifdef FS_XMLPARSER_HPP
#error fsXMLParser multiply included
#endif
#define FS_XMLPARSER_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif
#ifndef IT_STRING_HPP
#include "Core/it/itString.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
namespace fsXMLData {
enum fs_Node_Type
{
  e_Element = 0,
  e_Text,
  e_EndElement,
  e_None,
  e_EOF
};
}

//============================================================================
//	XML Parser
//============================================================================
class fsXMLParser
{
public:
  //------------------------------------------------------------------------
  //	constructors
  //------------------------------------------------------------------------
  fsXMLParser(const char* i_pBuffer);

  //------------------------------------------------------------------------
  //	destructor
  //------------------------------------------------------------------------
  ~fsXMLParser();

  //------------------------------------------------------------------------
  //	open + create the XML file
  //------------------------------------------------------------------------
  void Open();

  //------------------------------------------------------------------------
  //	finalize + close the XML file
  //------------------------------------------------------------------------
  void Close();

  //------------------------------------------------------------------------
  //	Read the actual values
  //------------------------------------------------------------------------
  fsXMLData::fs_Node_Type ReadNode(std::string& o_KeyName, std::string& o_Text);

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
  void Convert(const std::string& i_Text, int& o_Value);
  void Convert(const std::string& i_Text, bool& o_Value);
  void Convert(const std::string& i_Text, short& o_Value);
  void Convert(const std::string& i_Text, float& o_Value);
  void Convert(const std::string& i_Text, envType::Int8& o_Value);
  void Convert(const std::string& i_Text, envType::UInt8& o_Value);
  // void Convert(const std::string& i_Text, envType::Int16& o_Value);
  void Convert(const std::string& i_Text, envType::UInt16& o_Value);
  // void Convert(const std::string& i_Text, envType::Int32& o_Value);
  void Convert(const std::string& i_Text, envType::UInt32& o_Value);
  void Convert(const std::string& i_Text, envType::Int64& o_Value);
  void Convert(const std::string& i_Text, envType::UInt64& o_Value);
  // void Convert(const std::string& i_Text, envType::Float32& o_Value);
  void Convert(const std::string& i_Text, envType::Float64& o_Value);
  void Convert(const std::string& i_Text, char* o_Value);
  void Convert(const std::string& i_Text, std::string& o_Value);
  void Convert(const std::string& i_Text, fsLocator& o_Value);

private:
  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  std::string convert_keyname(std::string& i_KeyName);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  void free_resources();

  //------------------------------------------------------------------------
  //	Read reads i_NumBytes into the buffer.  If the file is too short
  //	to read	i_NumBytes, it will read as many as it can.  The return
  //	value is the number actually read into the o_Buffer.  This function
  //	also advances the file pointer.
  //------------------------------------------------------------------------
  int read(envType::Int64 i_NumBytes, void* o_Buffer);

  //------------------------------------------------------------------------
  //	read a chunk of data representing a node
  //------------------------------------------------------------------------
  fsXMLData::fs_Node_Type read_node(std::string& o_Token,
                                    int i_MaxChars = 0,
                                    char i_bDelimiter = 0,
                                    bool i_bComments = true);

  //------------------------------------------------------------------------
  //	read until a > is encountered.  This function does NOT clear the
  //	token parameter, it just appends to what is already there.
  //------------------------------------------------------------------------
  void consume_element(std::string& o_Token);

  //------------------------------------------------------------------------
  //	read until a > is encountered.  This function does NOT clear the
  //	token parameter, it just appends to what is already there.
  //------------------------------------------------------------------------
  void read_element(std::string& o_Token);

  //------------------------------------------------------------------------
  //	read until a > is encountered.  This function does NOT clear the
  //	token parameter, it just appends to what is already there.
  //------------------------------------------------------------------------
  void read_until_lessthan(std::string& o_Token);

protected:
  const char* m_pBuffer; // not owned
  int m_BufferIndex;
};
