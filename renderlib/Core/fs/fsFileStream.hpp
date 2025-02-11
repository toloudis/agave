/**********************************************************
**  fsFileStream.hpp
**
**      fsFileStream is the object used to read and write
**	data from a file.
**
**
**
\*********************************************************/

#ifdef FS_FILESTREAM_HPP
#error fsFileStream.hpp multiply included
#endif
#define FS_FILESTREAM_HPP

#ifndef ENV_TYPE_HPP
#include "Core/Env/envType.hpp"
#endif

//================================================================================
//	Forward references
//================================================================================
class fsLocator;
class fsFileStreamImp;

//================================================================================
//================================================================================
class fsFileStream
{
public:
  //--------------------------------------------------------------------------------
  // Expanding file position type to 64 bit, but keeping it signed because
  // the SetPos() function can use negative numbers to move backwards.
  //--------------------------------------------------------------------------------
  typedef envType::Int64 FilePosType;

  //--------------------------------------------------------------------------------
  //	enum
  //--------------------------------------------------------------------------------
  enum AccessPointType
  {
    e_Beginning,
    e_Current,
    e_End,
  };

  enum AccessType
  {
    e_ReadOnly,
    e_WriteOnly,
    // bga - removing these file formats because they complicate buffering.
    //  If needed, then they could be handled without a buffer.
    // e_ReadWrite,
    // e_ReadWriteCreate	// force create if not existing
    e_WriteCreate // force create if not existing
  };

  //========================================================================
  //	Constructor.  This could throw an exception if the file doesn't
  //	exist, or something.  So be ready! knowon exceptiosn thrown:
  //	fsInvalidLocatorX, fsFileDoesntExistX
  //========================================================================
  fsFileStream(const fsLocator& i_Locator, AccessType i_DesiredAccess);

  //========================================================================
  //	Destructor
  //========================================================================
  ~fsFileStream();

  //========================================================================
  //	Read reads i_NumBytes into the buffer.  If the file is too short
  //	to read	i_NumBytes, it will read as many as it can.  The return
  //	value is the number actually read into the o_Buffer.  This function
  //	also advances the file pointer.
  //========================================================================
  int Read(envType::Int64 i_NumBytes, void* o_Buffer);

  //========================================================================
  //	Write writes i_NumBytes from o_Buffer into the file.  The file
  //	pointer will also be advanced.
  //========================================================================
  void Write(envType::Int64 i_NumBytes, const void* i_Buffer);

  //========================================================================
  //	GetFilePos returns the current position of the file pointer.
  //========================================================================
  FilePosType GetFilePos() const;

  //========================================================================
  //	GetFileSize returns the size of the file.
  //========================================================================
  envType::Int64 GetFileSize() const;

  //========================================================================
  //	SetFilePos sets the file pointer to the given value.  It is valid
  //	to move the file pointer beyond the end of the file.
  //========================================================================
  void SetFilePos(FilePosType i_Pos, AccessPointType i_DesiredAccessPoint = e_Beginning);

  //========================================================================
  //	GetLocator returns this filestream's associated locator
  //========================================================================
  const fsLocator& GetLocator() const;

private:
  fsFileStreamImp* m_pImp;
};
