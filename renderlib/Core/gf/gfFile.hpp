/*****************************************************************************
**  gfFile.hpp
**
**      gfFile is a base class for all types of files.
**
**	Note: this file used to represent either a literal file or a file within
**	a ZIP (PAK) file. The PAK functionality has been removed because of
**	lack of use.
**
**
**
\****************************************************************************/
#ifdef GF_FILE_HPP
#error gfFile.hpp multiply included
#endif
#define GF_FILE_HPP

#ifndef FS_FILESTREAM_HPP
#include "Core/fs/fsFileStream.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

#include <memory>

//============================================================================
//============================================================================
class gfFile
{
public:
  //------------------------------------------------------------------------
  //	Constructor
  //------------------------------------------------------------------------
  gfFile(const fsLocator& i_Locator, fsFileStream::AccessType i_DesiredAccess);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  ~gfFile();

  //------------------------------------------------------------------------
  //	Read reads i_NumBytes into the buffer.  If the file is too short
  //	to read	i_NumBytes, it will read as many as it can.  The return
  //	value is the number actually read into the o_Buffer.  This function
  //	also advances the file pointer.
  //------------------------------------------------------------------------
  virtual int Read(envType::Int64 i_NumBytes, void* o_Buffer) = 0;

  //------------------------------------------------------------------------
  //	Write writes i_NumBytes from o_Buffer into the file.  The file
  //	pointer will also be advanced.
  //------------------------------------------------------------------------
  virtual void Write(envType::Int64 i_NumBytes, const void* i_Buffer) = 0;

  //------------------------------------------------------------------------
  //	GetFilePos returns the current position of the file pointer.
  //------------------------------------------------------------------------
  fsFileStream::FilePosType GetFilePos() const;

  //------------------------------------------------------------------------
  //	GetFileSize returns the size of the file in bytes.
  //------------------------------------------------------------------------
  envType::Int64 GetFileSize() const;

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

protected:
  fsFileStream* m_FileStream;
};
