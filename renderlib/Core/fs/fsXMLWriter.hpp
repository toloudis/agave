/*****************************************************************************
**  fsXMLWriter.hpp
**
**      XML file writer
**
**
**
\****************************************************************************/
#ifdef FS_XMLWRITER_HPP
#error fsXMLWriter multiply included
#endif
#define FS_XMLWRITER_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif
#ifndef FS_FILESTREAM_HPP
#include "Core/fs/fsFileStream.hpp"
#endif
#ifndef FS_FILEUTIL_HPP
#include "Core/fs/fsFileUtil.hpp"
#endif
#ifndef MA_FLOATRGBA_HPP
#include "Core/Ma/maFloatRGBA.hpp"
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <stack>

//============================================================================
//	XML writer
//============================================================================
class fsXMLWriter
{
public:
  //------------------------------------------------------------------------
  //	constructor
  //------------------------------------------------------------------------
  fsXMLWriter(const fsLocator& i_Filename);
  fsXMLWriter(const std::string& i_Filename);
  fsXMLWriter(const char* i_Filename);

  //------------------------------------------------------------------------
  //	destructor
  //------------------------------------------------------------------------
  ~fsXMLWriter();

  //------------------------------------------------------------------------
  //	open + create the XML Text file
  //------------------------------------------------------------------------
  void Open();

  //------------------------------------------------------------------------
  //	finalize + close the XML file
  //------------------------------------------------------------------------
  void Close();

  //------------------------------------------------------------------------
  //	Every XML file should have a header.  This happens BEFORE anything
  //------------------------------------------------------------------------
  void WriteHeader();

  //------------------------------------------------------------------------
  //	Every XML file needs one start element.  This happens BEFORE any
  //	elements are written.  At the end of the elements call the matching
  //	WriteEndElement.
  //------------------------------------------------------------------------
  void WriteStartElement(std::string& i_Element);

  //------------------------------------------------------------------------
  //	Every Start element needs a matching end element.
  //------------------------------------------------------------------------
  void WriteEndElement();

  //------------------------------------------------------------------------
  //	write the actual values
  //
  //	other possible elements to implement are:
  //		maVector3d
  //		maRotation
  //		maPoint3d
  //		maMatrix4x4
  //		itString
  //		nameString
  //------------------------------------------------------------------------
  void WriteElement(std::string& i_KeyName, int i_Value);
  void WriteElement(std::string& i_KeyName, bool i_Value);
  void WriteElement(std::string& i_KeyName, short i_Value);
  void WriteElement(std::string& i_KeyName, float i_Value);
  void WriteElement(std::string& i_KeyName, envType::Int8 i_Value);
  void WriteElement(std::string& i_KeyName, envType::UInt8 i_Value);
  // void WriteElement(std::string& i_KeyName, envType::Int16 i_Value);
  void WriteElement(std::string& i_KeyName, envType::UInt16 i_Value);
  // void WriteElement(std::string& i_KeyName, envType::Int32 i_Value);
  void WriteElement(std::string& i_KeyName, envType::UInt32 i_Value);
  void WriteElement(std::string& i_KeyName, envType::Int64 i_Value);
  void WriteElement(std::string& i_KeyName, envType::UInt64 i_Value);
  // void WriteElement(std::string& i_KeyName, envType::Float32 i_Value);
  void WriteElement(std::string& i_KeyName, envType::Float64 i_Value);
  void WriteElement(std::string& i_KeyName, const char* i_Value);
  void WriteElement(std::string& i_KeyName, const std::string& i_Value);
  void WriteElement(std::string& i_KeyName, std::string& i_Value);
  void WriteElement(std::string& i_KeyName, const fsLocator& i_Value);
  void WriteElement(std::string& i_KeyName, const itString& i_Value);
  void WriteElement(std::string& i_KeyName, const maFloatRGBA& i_Value);

private:
  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  std::string convert_keyname(const std::string& i_KeyName) const;

  //------------------------------------------------------------------------
  //	write the start and end values
  //------------------------------------------------------------------------
  void write_start_element(const std::string& i_KeyName, bool i_bWriteNewLine = false);
  void write_end_element(const std::string& i_KeyName, bool i_bWriteTabs = false);

  //------------------------------------------------------------------------
  //	write the end value
  //------------------------------------------------------------------------
  std::string get_tabs_string();

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  void free_resources();

protected:
  //
  //	member variables
  //
  fsLocator m_Filename;
  std::stack<std::string> m_ElementName;
  fsFileStream* m_pFileStream;
};
