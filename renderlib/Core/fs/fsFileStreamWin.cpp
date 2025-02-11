/**********************************************************
**  fsFileStreamWin.cpp
**
**      see .hpp
**
**
**
\*********************************************************/
#include "Core/fs/fsFileStream.hpp"

#include <windows.h>

// Windows unfortunately defines several functions as macros to conditionally
// compile with Unicode or ANSI.  We will undefine them here and call the
// specific versions (which we have to do anyway).
//
#undef CreateFile

//	The windows documentation mentions INVALID_SET_FILE_POINTER,
//	but it is stupidly never defined in a windows header.  So I must
//	define it here.  Normally I would never use a #define but I
//	will try to emulate how Windows would have done it.  sigh...
#define INVALID_SET_FILE_POINTER ((DWORD)-1)

// #include "Core/app/private/appApplicationPAC.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/fs/fsFileX.hpp"
#include "Core/fs/fsResourceTracker.hpp"
#include "Core/fs/fsFileUtilPAC.hpp"
#include "Core/Ma/maFunctions.hpp"

//============================================================================
//============================================================================
const int c_ReadBufferSize = 512;
const int c_WriteBufferSize = 512; // 4096;
typedef envType::UInt8 fsBufferChar;

//============================================================================
//============================================================================
enum BufferFormat
{
  e_BufferRead,
  e_BufferWrite
  // bga - potential for a e_NoBuffering flag here also
};

//============================================================================
//============================================================================
class fsFileStreamImp
{
public:
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  fsFileStreamImp()
    : m_File(NULL)
    , m_FilePointer(0)
    , m_pBuffer(NULL)
    , m_BufferLoc(0)
    , m_BufferValid(0)
    , m_Access(fsFileStream::e_ReadOnly) // e_ReadWrite)
  {
  }

public:
  HANDLE m_File;
  fsFileStream::AccessType m_Access;
  fsFileStream::FilePosType m_FilePointer;
  fsLocator m_Locator;
  // Buffer for reading in chunks of data
  fsBufferChar* m_pBuffer;
  int m_BufferLoc;
  int m_BufferValid;
  BufferFormat m_BufferFormat;
};

//============================================================================
//============================================================================
namespace {
//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
handle_windows_error(const fsLocator& i_Locator)
{
  DWORD error = ::GetLastError();

  switch (error) {
    case ERROR_FILE_NOT_FOUND:
      throw fsFileDoesntExistX(i_Locator);
      break;
    case ERROR_PATH_NOT_FOUND:
      throw fsDirectoryDoesntExistX(i_Locator);
      break;
    case ERROR_INVALID_DRIVE:
      throw fsInvalidLocatorX(i_Locator);
      break;
    case ERROR_SHARING_VIOLATION:
      throw fsFileInUseX(i_Locator);
      break;
    case ERROR_LOCK_VIOLATION:
      throw fsFileInUseX(i_Locator);
      break;
    case ERROR_FILE_EXISTS:
      throw fsFileExistsX(i_Locator);
      break;
    case ERROR_DRIVE_LOCKED:
      throw fsFileInUseX(i_Locator);
      break;
    case ERROR_INVALID_NAME:
      throw fsInvalidLocatorX(i_Locator);
      break;
    case ERROR_DIR_NOT_EMPTY:
      throw fsDirectoryNotEmptyX(i_Locator);
      break;
    case ERROR_PATH_BUSY:
      throw fsFileInUseX(i_Locator);
      break;
    case ERROR_ACCESS_DENIED:
      throw fsReadOnlyX(i_Locator);
      break;
    default:
      throw fsUnknownX(i_Locator);
      break;
  }
}

//------------------------------------------------------------------------
// Attempt to read a chunk of data into the buffer
//------------------------------------------------------------------------
void
read_into_buffer(fsFileStreamImp* io_pImp)
{
  DBG_ASSERT(io_pImp->m_BufferFormat == e_BufferRead, "Needs read buffer format.");
  io_pImp->m_BufferLoc = 0;
  io_pImp->m_BufferValid = 0;
  DWORD num_read = 0;
  BOOL ret_val = ::ReadFile(io_pImp->m_File, io_pImp->m_pBuffer, DWORD(c_ReadBufferSize), &num_read, NULL);
  if (ret_val == 0) {
    // handle failure
    handle_windows_error(io_pImp->m_Locator);
  }
  io_pImp->m_BufferValid = num_read;
}

//------------------------------------------------------------------------
// Attempt to write the contents of the delayed write buffer
// to the file.
//------------------------------------------------------------------------
void
write_out_buffer(fsFileStreamImp* io_pImp)
{
  DBG_ASSERT(io_pImp->m_BufferFormat == e_BufferWrite, "Needs write buffer format.");
  DWORD num_written = 0;
  BOOL ret_val = ::WriteFile(io_pImp->m_File, io_pImp->m_pBuffer, io_pImp->m_BufferValid, &num_written, NULL);
  if (ret_val == 0) {
    // handle failure
    handle_windows_error(io_pImp->m_Locator);
  }
  io_pImp->m_BufferLoc = 0;
  io_pImp->m_BufferValid = 0;
}

//------------------------------------------------------------------------
// If we are writing through a delayed buffer, write the contents
//	of the buffer to the file.
//------------------------------------------------------------------------
void
flush_buffer(fsFileStreamImp* io_pImp)
{
  if ((io_pImp->m_BufferFormat == e_BufferWrite) && (io_pImp->m_BufferValid > 0)) {
    write_out_buffer(io_pImp);
  }
}

}

//------------------------------------------------------------------------
//	Constructor.  This could throw an exception if the file doesn't
//	exist, or something.  So be ready!
//------------------------------------------------------------------------
fsFileStream::fsFileStream(const fsLocator& i_Locator, AccessType i_DesiredAccess)
{
  DWORD access;
  DWORD share_mode;
  DWORD open_flag = OPEN_EXISTING;
  BufferFormat format;

  switch (i_DesiredAccess) {
    case e_ReadOnly:
      // DBG_LOG("      file access: read only");
      access = GENERIC_READ;
      share_mode = FILE_SHARE_READ;
      format = e_BufferRead;
      break;

    case e_WriteOnly:
      // DBG_LOG("      file access: write only");
      access = GENERIC_WRITE;
      share_mode = 0;
      format = e_BufferWrite;
      break;

      // case e_ReadWrite:
      //	//DBG_LOG("      file access: read-write only");
      //	access = GENERIC_READ | GENERIC_WRITE;
      //	share_mode = 0;
      // break;

      // case e_ReadWriteCreate:
      //	//DBG_LOG("      file access: read-write-create only");
      //	access = GENERIC_READ | GENERIC_WRITE;
      //	share_mode = 0;
      //	open_flag = OPEN_ALWAYS | TRUNCATE_EXISTING;
      // break;

    case e_WriteCreate:
      // DBG_LOG("      file access: read-write-create only");
      access = GENERIC_WRITE;
      share_mode = 0;
      open_flag = OPEN_ALWAYS | TRUNCATE_EXISTING;
      format = e_BufferWrite;
      break;

    default:
      DBG_ASSERT(false, "Invalid Access type");
      break;
  }

  //	for DEBUG only
  //
  // if (!fsResourceTracker::IsInList(i_Locator))
  //{
  //	std::string file_path;
  //	fsFileUtil::LocatorToANSIFilename(i_Locator, file_path);
  //	DBG_LOG("File is NOT begin tracked (" << file_path.c_str() << ")" );
  //}

  HANDLE new_file = INVALID_HANDLE_VALUE;

  while (true) {
    //	Create a file
    //
    if (fsFileUtilPAC::MustUseANSIFilenames()) {
      std::string filename;
      fsFileUtilPAC::LocatorToANSIFilename(i_Locator, filename);

      if (filename.size() == 0) {
        fsResourceTracker::Remove(i_Locator); // shouldn't be here?  put where throw is CAUGHT
        throw fsInvalidLocatorX(i_Locator);
      }

      new_file = ::CreateFileA(filename.c_str(), access, share_mode, NULL, open_flag, FILE_ATTRIBUTE_NORMAL, NULL);
    } else {
      itString filename;
      fsFileUtilPAC::LocatorToUnicodeFilename(i_Locator, filename);
      // DBG_LOG("Creating/Opening UNICODE stream  " << filename);

      if (filename.GetLength() == 0) {
        fsResourceTracker::Remove(i_Locator); // shouldn't be here?  put where throw is CAUGHT
        throw fsInvalidLocatorX(i_Locator);
      }

      new_file = ::CreateFileW(filename.GetString(), access, share_mode, NULL, open_flag, FILE_ATTRIBUTE_NORMAL, NULL);
    }

    //	file couldn't be created
    //
    if (new_file == INVALID_HANDLE_VALUE) {
      std::string filename2;
      fsFileUtilPAC::LocatorToANSIFilename(i_Locator, filename2);
      DBG_WARNING("INVALID HANDLE VALUE " << filename2.c_str());

      fsResourceTracker::Remove(i_Locator); // shouldn't be here?  put where throw is CAUGHT

      handle_windows_error(i_Locator); // this will end up throwing, and abort construction
    }

    break;
  }

  // save our work
  m_pImp = new fsFileStreamImp;
  m_pImp->m_File = new_file;
  m_pImp->m_Access = i_DesiredAccess;
  m_pImp->m_FilePointer = 0;
  m_pImp->m_Locator = i_Locator;
  m_pImp->m_pBuffer = NULL;
  m_pImp->m_BufferLoc = 0;
  m_pImp->m_BufferFormat = format;
}

//------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------
fsFileStream::~fsFileStream()
{
  // m_pImp->m_FilePointer
  flush_buffer(m_pImp); // make sure write buffer is flushed to disk before closing
  ::CloseHandle(m_pImp->m_File);
  if (m_pImp->m_pBuffer != NULL)
    delete[] m_pImp->m_pBuffer;
  delete m_pImp;
}

//------------------------------------------------------------------------
//	Read reads i_NumBytes into the buffer.  If the file is too short
//	to read	i_NumBytes, it will read as many as it can.  The return
//	value is the number actually read into the o_Buffer.  This function
//	also advances the file pointer.
//------------------------------------------------------------------------
int
fsFileStream::Read(envType::Int64 i_NumBytes, void* o_Buffer)
{
  DBG_ASSERT(m_pImp->m_Access != e_WriteOnly, "Wrong access type for Read");
  DBG_ASSERT(m_pImp->m_BufferFormat == e_BufferRead, "Needs read buffer format.");

  envType::Int64 num_read = 0;

  // Initialize the buffered reading
  if ((!m_pImp->m_pBuffer) && (c_ReadBufferSize > 0)) {
    m_pImp->m_pBuffer = new fsBufferChar[c_ReadBufferSize];
    m_pImp->m_BufferValid = 0;
    m_pImp->m_BufferLoc = 0;
    read_into_buffer(m_pImp);
  }

  // How much data is left in the buffer
  envType::Int64 num_left_in_buffer = m_pImp->m_BufferValid - m_pImp->m_BufferLoc;
  if (i_NumBytes <= num_left_in_buffer) {
    // We can deliver this request purely with data already in
    // the buffer.
    ::memcpy(o_Buffer, m_pImp->m_pBuffer + m_pImp->m_BufferLoc, i_NumBytes);
    m_pImp->m_BufferLoc += i_NumBytes;
    num_read = i_NumBytes;
  } else {
    envType::Int64 left_to_read = i_NumBytes;
    fsBufferChar* output_buffer = static_cast<fsBufferChar*>(o_Buffer);
    if (num_left_in_buffer > 0) {
      // Put the rest of our buffer into the requesting array
      ::memcpy(o_Buffer, m_pImp->m_pBuffer + m_pImp->m_BufferLoc, num_left_in_buffer);
      m_pImp->m_BufferLoc += num_left_in_buffer;

      left_to_read -= num_left_in_buffer;
      output_buffer += num_left_in_buffer;
      num_read += num_left_in_buffer;
    }

    // If the requester wants more data than can fit in a single buffer
    // read, then just read that directly into the output buffer
    if (left_to_read > c_ReadBufferSize) {
      DWORD num_actual_read = 0;
      BOOL ret_val = ::ReadFile(m_pImp->m_File, output_buffer, DWORD(left_to_read), &num_actual_read, NULL);
      if (ret_val == 0) {
        // handle failure
        handle_windows_error(m_pImp->m_Locator);
      } else {
        num_read += num_actual_read;
      }
    } else if (left_to_read > 0) {
      // If there is still more to read, then fill the buffer again
      read_into_buffer(m_pImp);
      num_left_in_buffer = m_pImp->m_BufferValid;

      if (num_left_in_buffer > 0) {
        // Copy more from our buffer into the requesting array
        int amount_to_copy = maFunctions::Lowest(num_left_in_buffer, left_to_read);
        ::memcpy(output_buffer, m_pImp->m_pBuffer, amount_to_copy);
        m_pImp->m_BufferLoc += amount_to_copy;
        num_read += amount_to_copy;
      }
    }
  }

  // Advance the internal file pointer
  m_pImp->m_FilePointer += num_read;

  return num_read;
}

//------------------------------------------------------------------------
//	Write writes i_NumBytes from o_Buffer into the file.  The file
//	pointer will also be advanced.
//------------------------------------------------------------------------
void
fsFileStream::Write(envType::Int64 i_NumBytes, const void* i_Buffer)
{
  DBG_ASSERT(m_pImp->m_Access != e_ReadOnly, "Wrong access type for Write");
  DBG_ASSERT(m_pImp->m_BufferFormat == e_BufferWrite, "Needs write buffer format.");

  DWORD num_written = 0;

  // If the requester wants to write more data than can fit in a single buffer,
  // then just write that directly into the file
  if (i_NumBytes > c_WriteBufferSize) {
    // Flush anything we had in the buffer
    flush_buffer(m_pImp);

    // Write the full request directly to file
    BOOL ret_val = 0;
    ret_val = ::WriteFile(m_pImp->m_File, i_Buffer, i_NumBytes, &(num_written), NULL);
    if (ret_val == 0) {
      // handle failure
      handle_windows_error(m_pImp->m_Locator);
    }
  } else {
    // Initialize the buffered writing
    if ((!m_pImp->m_pBuffer) && (c_WriteBufferSize > 0)) {
      m_pImp->m_pBuffer = new fsBufferChar[c_WriteBufferSize];
      m_pImp->m_BufferValid = 0;
      m_pImp->m_BufferLoc = 0;
    }

    // How much room is left in the buffer
    envType::Int64 num_left_in_buffer = c_WriteBufferSize - m_pImp->m_BufferLoc;
    if (i_NumBytes > num_left_in_buffer) {
      // Flush anything we had in the buffer
      flush_buffer(m_pImp);
    }

    // Since there was an earlier test for whether the request was
    // larger than the buffer size, we know we can handle this request
    // purely within the memory buffer
    ::memcpy(m_pImp->m_pBuffer + m_pImp->m_BufferLoc, i_Buffer, i_NumBytes);
    m_pImp->m_BufferLoc += i_NumBytes;
    // Note that we might have backed up within the memory buffer,
    // so we only want to increase the valid counter when the
    // current buffer loc has written pass the old valid counter.
    if (m_pImp->m_BufferLoc > m_pImp->m_BufferValid)
      m_pImp->m_BufferValid = m_pImp->m_BufferLoc;
    num_written = i_NumBytes;
  }

  // Advance the internal file pointer
  m_pImp->m_FilePointer += num_written;
}

//------------------------------------------------------------------------
//	GetFilePos returns the current position of the file pointer.
//------------------------------------------------------------------------
fsFileStream::FilePosType
fsFileStream::GetFilePos() const
{
  return m_pImp->m_FilePointer;
}

//------------------------------------------------------------------------
//	GetFileSize returns the size of the file.
//------------------------------------------------------------------------
envType::Int64
fsFileStream::GetFileSize() const
{
  LARGE_INTEGER file_size;
  if (!::GetFileSizeEx(m_pImp->m_File, &file_size)) {
    return 0;
  }
  return file_size.QuadPart;
}

//------------------------------------------------------------------------
//	GetLocator returns this filestream's associated locator
//------------------------------------------------------------------------
const fsLocator&
fsFileStream::GetLocator() const
{
  return m_pImp->m_Locator;
}

//------------------------------------------------------------------------
//	SetFilePos sets the file pointer to the given value.  It is valid
//	to move the file pointer beyond the end of the file.
//------------------------------------------------------------------------
void
fsFileStream::SetFilePos(FilePosType i_Pos, AccessPointType i_DesiredAccessPoint)
{
  DWORD move_method;
  if (i_DesiredAccessPoint == e_Beginning) {
    DBG_ASSERT(i_Pos >= 0, "Negative seek attempted");
    move_method = FILE_BEGIN;
  } else if (i_DesiredAccessPoint == e_Current) {
    DBG_ASSERT(m_pImp->m_FilePointer - i_Pos >= 0, "Negative seek attempted");
    move_method = FILE_CURRENT;
  } else {
    move_method = FILE_END;
  }

  // See if we can resolve this move simply by moving within
  // the buffer we already have in memory.
  bool bNeedFileSeek = true;
  if ((m_pImp->m_pBuffer != NULL) && (move_method != FILE_END)) {
    // Convert both seek methods into a delta from the current posit�on
    envType::Int64 delta = (i_DesiredAccessPoint == e_Beginning) ? (i_Pos - m_pImp->m_FilePointer) : i_Pos;
    if (delta == 0) {
      // Already at the correct position
      bNeedFileSeek = false;
    } else if (m_pImp->m_BufferValid > 0) {
      // static int s_NumAttempts = 0;
      // static int s_NumFails = 0;
      // s_NumAttempts++;

      envType::Int64 new_pos = m_pImp->m_BufferLoc + delta;
      if (new_pos >= 0 && new_pos < m_pImp->m_BufferValid) {
        bNeedFileSeek = false;
        m_pImp->m_BufferLoc = new_pos;
        m_pImp->m_FilePointer += delta;
      }
      // else
      //	 s_NumFails++;
    }
  }

  if (bNeedFileSeek) {
    // If we are writing a delayed buffer and we have to seek outside
    // of the buffer range, then flush what we have written
    flush_buffer(m_pImp);

    // Mark the buffer as invalid
    m_pImp->m_BufferLoc = m_pImp->m_BufferValid = 0;

    // bga - switching this to GetFileSizeEx in order to handle
    //  file sizes greater than 32-bit int can handle.
    //
    // DWORD ret_val = INVALID_SET_FILE_POINTER;
    // ret_val = ::SetFilePointer(	m_pImp->m_File,
    //							i_Pos,
    //							NULL,
    //							move_method );
    // if ( ret_val == INVALID_SET_FILE_POINTER )

    LARGE_INTEGER file_pos, ret_val;
    file_pos.QuadPart = i_Pos;
    if (::SetFilePointerEx(m_pImp->m_File, file_pos, &ret_val, move_method) == 0) // error case is return of zero
    {
      // handle failure
      handle_windows_error(m_pImp->m_Locator);
    } else {
      m_pImp->m_FilePointer = ret_val.QuadPart;
    }
  }
}
