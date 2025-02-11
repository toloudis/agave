/*****************************************************************************
**  gfFileXML.hpp
**
**      gfFileXML is a class which inherits from gfFileTxt
**  to represent XML files.  The current intention is to
**  only handle ascii files, but if unicode text files are
**  later also supported, this class could be changed to support
**	the difference.
**
**
**
\****************************************************************************/

#ifdef GF_FILEXML_HPP
#error gfFileXML.hpp multiply included
#endif
#define GF_FILEXML_HPP

#ifndef ENV_EXCEPTIONX_HPP
#include "Core/env/envExceptionX.hpp"
#endif
#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif
#ifndef FS_FILESTREAM_HPP
#include "Core/fs/fsFileStream.hpp"
#endif
#ifndef GF_FILE_HPP
#include "Core/gf/gfFile.hpp"
#endif

#include <string>

//============================================================================
// header is <?xml version="1.0" encoding="utf-8"?>
//============================================================================
class gfFileXML : public gfFile
{
public:
  //------------------------------------------------------------------------
  //	structs
  //------------------------------------------------------------------------
  struct Header
  {
    char m_Version[32];
    char m_Encoding[32];
  };

public:
  //------------------------------------------------------------------------
  //	Constructor
  //------------------------------------------------------------------------
  gfFileXML(const fsLocator& i_Locator, fsFileStream::AccessType i_DesiredAccess);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  ~gfFileXML();

  //------------------------------------------------------------------------
  //	ReadToken
  //
  //		ReadToken reads characters until whitespace or delimiter
  //  is found.  Comments are lines that begin with "#"
  //  Quotes (") can allow strings which contain spaces,
  //  quotes within quoted strings should be escaped '\'.
  //		Returns true if characters were read.
  //------------------------------------------------------------------------
  bool ReadToken(std::string& o_Token, int i_MaxChars = 0, char i_bDelimiter = 0, bool i_bComments = true);

  //------------------------------------------------------------------------
  //	WriteToken
  //
  //		WriteToken writes string so that it can be read with
  //	later call to ReadToken.  If string contains spaces or
  //	quotes, the whole string will be quoted.  Internal quotes
  //  will be escaped.
  //------------------------------------------------------------------------
  void WriteToken(std::string& i_Token);

  //--------------------------------------------------------------------
  //	IncrementIndentation
  //--------------------------------------------------------------------
  void IncrementIndentation();

  //--------------------------------------------------------------------
  //	DecrementIndentation
  //--------------------------------------------------------------------
  void DecrementIndentation();

  //--------------------------------------------------------------------
  //	GetIndentation
  //--------------------------------------------------------------------
  int GetIndentation();

  //--------------------------------------------------------------------
  //	WriteIndentation
  //--------------------------------------------------------------------
  void WriteIndentation();

  //------------------------------------------------------------------------
  //	ReadLine
  //
  //		ReadLine reads characters until an end of line is reached
  //	(or end of file).
  //
  //		Returns true if characters were read.
  //------------------------------------------------------------------------
  bool ReadLine(std::string& o_Line, int i_MaxChars = 0);

  //------------------------------------------------------------------------
  //	WriteLine
  //
  //		WriteLine writes a string.
  //------------------------------------------------------------------------
  void WriteLine(const std::string& i_Line);

  //------------------------------------------------------------------------
  //	ReadHeader reads the file's header into the o_Header passed to it. If
  //	o_Header is NULL, the file pointer is simply moved past the file header
  //	Throws: gfInvalidFileXMLX
  //------------------------------------------------------------------------
  void ReadHeader(Header* o_Header = NULL);

  //------------------------------------------------------------------------
  //	WriteHeader writes the given i_Header to the file. gfFileBin::m_Mode
  //	is used instead of the value passed in i_Header::m_Mode since a
  //	gfFileBin's mode can't altered after construction. The second version of
  //	this function is an inline convenience.
  //
  //	This function also writes out a root-level tag
  //------------------------------------------------------------------------
  void WriteHeader(const Header& i_Header);
  void WriteHeader(const char* i_Version = 0, const char* i_Encoding = 0);

  //------------------------------------------------------------------------
  //	Write a tag to close out the header
  //------------------------------------------------------------------------
  void WriteFooter();

  //------------------------------------------------------------------------
  //	Read reads i_NumBytes into the buffer.  If the file is too short
  //	to read	i_NumBytes, it will read as many as it can.  The return
  //	value is the number actually read into the o_Buffer.  This function
  //	also advances the file pointer.
  //------------------------------------------------------------------------
  int Read(envType::Int64 i_NumBytes, void* o_Buffer);

  //------------------------------------------------------------------------
  //	Write writes i_NumBytes from o_Buffer into the file.  The file
  //	pointer will also be advanced.
  //------------------------------------------------------------------------
  void Write(envType::Int64 i_NumBytes, const void* i_Buffer);

  //------------------------------------------------------------------------
  //	GetFilePos returns the current position of the file pointer.
  //------------------------------------------------------------------------
  fsFileStream::FilePosType GetFilePos() const;

  //------------------------------------------------------------------------
  //	SetFilePos sets the file pointer to the given value.  It is valid
  //	to move the file pointer beyond the end of the file.
  //------------------------------------------------------------------------
  void SetFilePos(fsFileStream::FilePosType i_Pos,
                  fsFileStream::AccessPointType i_DesiredAccessPoint = fsFileStream::e_Beginning);

  //------------------------------------------------------------------------
  //	GetLocator returns this file's associated locator
  //------------------------------------------------------------------------
  const fsLocator& GetLocator() const;
};
