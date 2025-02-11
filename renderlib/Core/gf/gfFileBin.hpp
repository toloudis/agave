/*****************************************************************************
**	gfFileBin.hpp
**
**		gfFileBin is a class which inherits from fsFileStream and contains
**		a data member tracking endian mode, along with accessor methods for it,
**		as well as a method for writing binary headers
**
**
**
\****************************************************************************/
#ifdef GF_FILEBIN_HPP
#error gfFileBin.hpp multiply included
#endif
#define GF_FILEBIN_HPP

#ifndef ENV_EXCEPTIONX_HPP
#include "Core/Env/envExceptionX.hpp"
#endif
#ifndef ENV_PLATFORM_HPP
#include "Core/Env/envPlatform.hpp"
#endif
#ifndef FS_FILESTREAM_HPP
#include "Core/Fs/fsFileStream.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/Fs/fsLocator.hpp"
#endif
#ifndef GF_FILE_HPP
#include "gfFile.hpp"
#endif

#include <memory>

//============================================================================
//============================================================================
class gfFileBin : public gfFile
{
public:
  //------------------------------------------------------------------------
  //	enum
  //------------------------------------------------------------------------
  enum EndianMode
  {
    e_NativeEndian = 0,
    e_BigEndian,
    e_LittleEndian,
  };

  //------------------------------------------------------------------------
  //	structs
  //------------------------------------------------------------------------

  // this header is preceded in the file by the four-byte signature "TWFB"
  struct Header
  {
    char m_Mode;
    char m_Major;
    char m_Minor;
    char m_Rev;
  };

  //------------------------------------------------------------------------
  //	Constructor
  //------------------------------------------------------------------------
  gfFileBin(const fsLocator& i_Locator, fsFileStream::AccessType i_DesiredAccess, EndianMode i_Mode = e_NativeEndian);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  ~gfFileBin();

  //------------------------------------------------------------------------
  //	GetEndianMode returns this gfFileBin's EndianMode
  //------------------------------------------------------------------------
  EndianMode GetEndianMode() const;

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
  //	ReadHeader reads the file's header into the o_Header passed to it. If
  //	o_Header is NULL, the file pointer is simply moved past the file header
  //	Throws: gfInvalidFileBinX
  //------------------------------------------------------------------------
  void ReadHeader(Header* o_Header = NULL);

  //------------------------------------------------------------------------
  //	WriteHeader writes the given i_Header to the file. gfFileBin::m_Mode
  //	is used instead of the value passed in i_Header::m_Mode since a
  //	gfFileBin's mode can't altered after construction. The second version of
  //	this function is an inline convenience.
  //------------------------------------------------------------------------
  void WriteHeader(const Header& i_Header);
  void WriteHeader(int i_Major = 0, int i_Minor = 0, int i_Rev = 0);

  static int GetHeaderSize();

private:
  EndianMode m_Mode;
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline gfFileBin::EndianMode
gfFileBin::GetEndianMode() const
{
  return m_Mode;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void
gfFileBin::WriteHeader(int i_Major, int i_Minor, int i_Rev)
{
  Header h;
  h.m_Major = i_Major;
  h.m_Minor = i_Minor;
  h.m_Rev = i_Rev;
  WriteHeader(h);
}
