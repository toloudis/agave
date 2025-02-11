/*****************************************************************************
**  gfFileTxt.hpp
**
**      gfFileTxt is a class which inherits from fsFileStream
**  to represent text files.  The current intention is to
**  only handle ascii files, but if unicode text files are
**  later also supported, this class could be changed to support
**	the difference.
**
**
**
\****************************************************************************/

#ifdef GF_FILETXT_HPP
#error gfFileTxt.hpp multiply included
#endif
#define GF_FILETXT_HPP

#ifndef ENV_EXCEPTIONX_HPP
#include "Core/Env/envExceptionX.hpp"
#endif

#ifndef ENV_PLATFORM_HPP
#include "Core/Env/envPlatform.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/Fs/fsLocator.hpp"
#endif
#ifndef FS_FILESTREAM_HPP
#include "Core/Fs/fsFileStream.hpp"
#endif
#ifndef GF_FILE_HPP
#include "gfFile.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class gfFileTxt : public gfFile
{
public:
  //------------------------------------------------------------------------
  //	Constructor
  //------------------------------------------------------------------------
  gfFileTxt(const fsLocator& i_Locator, fsFileStream::AccessType i_DesiredAccess);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  ~gfFileTxt();

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

private:
  int m_Indentation;
};
