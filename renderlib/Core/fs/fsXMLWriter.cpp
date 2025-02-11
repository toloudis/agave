/*****************************************************************************
**  fsXMLWriter.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsXMLWriter.hpp"

#include "Core/it/itStringUtil.hpp"

#include <iostream>
#include <sstream>
#include <tchar.h>

//============================================================================
//============================================================================
// #define FSXMLWRITER_DEBUG_OUTPUT

//============================================================================
//============================================================================
namespace {
const char* nl = "\r\n";
const char* tab = "  "; //"\t";
int l_XML_write_depth = 0;
}

//------------------------------------------------------------------------
//	constructors
//------------------------------------------------------------------------
fsXMLWriter::fsXMLWriter(const fsLocator& i_Filename)
  : m_Filename(i_Filename)
{
}
fsXMLWriter::fsXMLWriter(const std::string& i_Filename)
{
  fsFileUtil::ANSIFilenameToLocator(i_Filename, m_Filename);
}
fsXMLWriter::fsXMLWriter(const char* i_Filename)
{
  fsFileUtil::ANSIFilenameToLocator(std::string(i_Filename), m_Filename);
}

//------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------
fsXMLWriter::~fsXMLWriter()
{
  free_resources();
}

//------------------------------------------------------------------------
//	open + create the XML Text file
//------------------------------------------------------------------------
void
fsXMLWriter::Open()
{
  // Create the file if it doesn't exist
  if (!fsFileUtil::FileExists(m_Filename))
    fsFileUtil::CreateFile(m_Filename);

  //	open the file
  m_pFileStream = new fsFileStream(m_Filename, fsFileStream::e_WriteCreate);

  //	write the XML header
  WriteHeader();

  l_XML_write_depth = 0;
}

//------------------------------------------------------------------------
//	finalize + close the XML file
//------------------------------------------------------------------------
void
fsXMLWriter::Close()
{
  free_resources();
}

//------------------------------------------------------------------------
//	Every XML file should have a header.  This happens BEFORE anything
//------------------------------------------------------------------------
void
fsXMLWriter::WriteHeader()
{
  std::stringstream ss;
  ss << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << nl;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: header (" << ss.str().c_str() << ")");
#endif
}

//------------------------------------------------------------------------
//	Every XML file needs one start element.  This happens BEFORE any
//	elements are written.  At the end of the elements call the matching
//	WriteEndElement.
//------------------------------------------------------------------------
void
fsXMLWriter::WriteStartElement(std::string& i_Element)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create the element
  m_ElementName.push(i_Element);
  write_start_element(i_Element, true);

  l_XML_write_depth++;
}

//------------------------------------------------------------------------
//	Every Start element needs a matching end element.
//------------------------------------------------------------------------
void
fsXMLWriter::WriteEndElement()
{
  DBG_ASSERT((m_pFileStream != NULL), "Must Open() the Writer");
  DBG_ASSERT((l_XML_write_depth == m_ElementName.size()),
             "Number of XML write start elements not equal to the number of end elements");

  l_XML_write_depth--;

  //	create the element
  write_end_element(m_ElementName.top(), true);
  m_ElementName.pop();
}

//------------------------------------------------------------------------
//	write the actual values
//
//	other possible elements to implement are:
//		maVector3d
//		maRotation
//		maPoint3d
//		maMatrix4x4
//		maFloatRGBA
//		itString
//		nameString
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, int i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  //	create and write the element
  //	char buffer[128];
  //	sprintf(buffer, "%s", i_KeyName.c_str());
  //	m_pFileStream->Write(sizeof(buffer), buffer);
  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content int (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, bool i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << (i_Value ? "true" : "false");
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content bool (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, short i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content short (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, float i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content float (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, envType::Int8 i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  envType::Int32 val32 = i_Value; // bga - promote the int8 so it doesn't write as a character
  ss << val32;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content int8 (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, envType::UInt8 i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  envType::UInt32 val32 = i_Value; // bga - promote the uint8 so it doesn't write as a character
  ss << val32;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content uint8 (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
// void fsXMLWriter::WriteElement(std::string& i_KeyName, envType::Int16 i_Value)
//{
//	DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");
//	m_pFileStream->WriteElementString(convert_keyname(i_KeyName), XmlConvert::ToString(i_Value));
//}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, envType::UInt16 i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content uint16 (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
// void fsXMLWriter::WriteElement(std::string& i_KeyName, envType::Int32 i_Value)
//{
//	DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");
//	m_pFileStream->WriteElementString(convert_keyname(i_KeyName), XmlConvert::ToString(i_Value));
//}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, envType::UInt32 i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content uint32 (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, envType::Int64 i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content int64 (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, envType::UInt64 i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content uint64 (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
// void fsXMLWriter::WriteElement(std::string& i_KeyName, envType::Float32 i_Value)
//{
//	DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");
//	m_pFileStream->WriteElementString(convert_keyname(i_KeyName), XmlConvert::ToString(i_Value));
//}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, envType::Float64 i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content float64 (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, const char* i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(std::string(i_KeyName));

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());
  //	m_pFileStream->Write(_strncnt(i_Value,strlen(i_Value)), i_Value);

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG2("WriteXML: content charstr (%s)[%s]", i_KeyName.c_str(), ss.str().c_str());
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, const std::string& i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG2("WriteXML: content str0 (%s)[%s]", i_KeyName.c_str(), i_Value.c_str());
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, std::string& i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG2("WriteXML: content str1 (%s)[%s]", i_KeyName.c_str(), i_Value.c_str());
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, const fsLocator& i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  //
  std::string outstr;
  fsFileUtil::LocatorToANSIFilename(i_Value, outstr);
  std::stringstream ss;
  ss << outstr;
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG2("WriteXML: content str2 (%s)[%s]", i_KeyName.c_str(), outstr.c_str());
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, const itString& i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  //
  std::string outstr;
  outstr = itStringUtil::GetStdString(i_Value);
  m_pFileStream->Write(sizeof(outstr.c_str()), outstr.c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG2("WriteXML: content str3 (%s)[%s]", i_KeyName.c_str(), outstr.c_str());
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}
//------------------------------------------------------------------------
void
fsXMLWriter::WriteElement(std::string& i_KeyName, const maFloatRGBA& i_Value)
{
  DBG_ASSERT(m_pFileStream != NULL, "Must Open() the Writer");

  //	create and write the element
  write_start_element(i_KeyName);

  std::stringstream ss;
  ss << i_Value.GetRed();
  ss << ',';
  ss << i_Value.GetGreen();
  ss << ',';
  ss << i_Value.GetBlue();
  ss << ',';
  ss << i_Value.GetAlpha();
  m_pFileStream->Write(ss.str().size(), ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG("WriteXML: content float (" << i_KeyName.c_str() << ")");
#endif

  //	create and write the element
  write_end_element(i_KeyName);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::string
fsXMLWriter::convert_keyname(const std::string& i_KeyName) const
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
//	write the start value
//------------------------------------------------------------------------
void
fsXMLWriter::write_start_element(const std::string& i_KeyName, bool i_bWriteNewLine)
{
  std::ostringstream buff_ss(std::ostringstream::out);

  if (i_bWriteNewLine)
    buff_ss << get_tabs_string().c_str() << "<" << i_KeyName.c_str() << ">" << nl;
  else
    buff_ss << get_tabs_string().c_str() << "<" << i_KeyName.c_str() << ">";

  //	write the preferences
  m_pFileStream->Write(buff_ss.str().length(), buff_ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG2("WriteXML: start (%s)[%s]", i_KeyName.c_str(), buff_ss.str().c_str());
#endif
}

//------------------------------------------------------------------------
//	write the end value
//------------------------------------------------------------------------
void
fsXMLWriter::write_end_element(const std::string& i_KeyName, bool i_bWriteTabs)
{
  std::ostringstream buff_ss(std::ostringstream::out);

  if (i_bWriteTabs)
    buff_ss << get_tabs_string().c_str() << "</" << i_KeyName.c_str() << ">" << nl;
  else
    buff_ss << "</" << i_KeyName.c_str() << ">" << nl;

  //	write the preferences
  m_pFileStream->Write(buff_ss.str().length(), buff_ss.str().c_str());

#ifdef FSXMLWRITER_DEBUG_OUTPUT
  DBG_LOG2("WriteXML: end (%s)[%s]", i_KeyName.c_str(), buff_ss.str().c_str());
#endif
}

//------------------------------------------------------------------------
//	write the end value
//------------------------------------------------------------------------
std::string
fsXMLWriter::get_tabs_string()
{
  std::string tabs;
  for (int i = 0; i < l_XML_write_depth; ++i) {
    tabs += tab;
  }
  return tabs;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsXMLWriter::free_resources()
{
  if (m_pFileStream != NULL)
    delete m_pFileStream;
  m_pFileStream = NULL;
}
