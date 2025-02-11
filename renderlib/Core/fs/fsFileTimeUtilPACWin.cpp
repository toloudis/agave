/****************************************************************************\
**  fsFileTimeUtilPACWin.cpp
**
**	  fsFileTimeUtilPACWin.cpp defines the file util PAC for windows.
**
**
**
\****************************************************************************/

#include "Core/fs/fsFileTimeUtilPACWin.hpp"

#include "Core/fs/fsFileUtilPACWin.hpp"

#include <stdio.h>
#include <windows.h>

// Windows unfortunately defines several functions as macros to conditionally
// compile with Unicode or ANSI.  We will undefine them here and call the
// specific versions (which we have to do anyway).
#undef CreateFile
#undef DeleteFile
#undef CopyFile
#undef CreateDirectory

#include "Core/dbg/dbgMsg.hpp"
#include "Core/env/envSystemDataPACWin.hpp"
#include "Core/fs/fsFileX.hpp"

#include <iomanip>
#include <sstream>

//============================================================================
//============================================================================
namespace fsFileTimeUtilPAC {

namespace {
//============================================================================
//============================================================================
struct FileCloser
{
  FileCloser(HANDLE i_Handle)
    : m_Handle(i_Handle)
  {
  }
  ~FileCloser() { ::CloseHandle(m_Handle); }
  HANDLE m_Handle;
};

//	This is initialized in the Init() function
//	It is true if we are running on 95 or 98 and have to
//	convert our Unicode strings to ANSI strings before
//	passing them to the Win32 functions
bool l_ANSIFilenames = false;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
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

//----------------------------------------------------------------------------
//	GetHandle creates a file with the given name.  This must be done before
//	reading or writing with a fsFileStream.
//----------------------------------------------------------------------------
HANDLE
GetHandle(const fsLocator& i_Filename)
{
  HANDLE new_file = INVALID_HANDLE_VALUE;

  if (l_ANSIFilenames) {
    std::string filename;
    fsFileUtilPAC::LocatorToANSIFilename(i_Filename, filename);

    if (filename.size() == 0)
      throw fsInvalidLocatorX(i_Filename);

    new_file = ::CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  } else {
    itString filename;
    fsFileUtilPAC::LocatorToUnicodeFilename(i_Filename, filename);

    if (filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_Filename);

    new_file = ::CreateFileW(filename.GetString(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  }

  if (new_file == INVALID_HANDLE_VALUE)
    handle_windows_error(i_Filename); // probably throws

  return new_file; // release file so something else can write to it
}

}

//------------------------------------------------------------------------
//	GetCreationTime returns a double in UTC (Coordinated Universal Time)
//	i.e. the number of seconds since January 1, 1970, 00:00:00 GMT which
//	represents the file's creation time
//------------------------------------------------------------------------
double
GetCreationTime(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftCreate, ftLocal;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, &ftCreate, NULL, NULL))
    return 0;

  // Convert the create time to local time.
  if (!FileTimeToLocalFileTime(&ftCreate, &ftLocal))
    return 0;

  LONGLONG qwResult = (((ULONGLONG)ftLocal.dwHighDateTime) << 32) + ftLocal.dwLowDateTime;

  // subtract out the diff between the year 1970 (the unix and javascript style we're trying to return),
  // and the year 1600 (the time FILETIME structs are built on)
  qwResult -= 116444736000000000;

  // divide to get the value back up to seconds from the FILETIME style of 100-interval nanoseconds
  qwResult /= 10000000;

  return (double)qwResult;
}

//------------------------------------------------------------------------
//	GetLastAccessTime returns a double in UTC (Coordinated Universal Time)
//	i.e. the number of seconds since January 1, 1970, 00:00:00 GMT which
//	represents the file's last access time
//------------------------------------------------------------------------
double
GetLastAccessTime(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftAccess, ftLocal;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, NULL, &ftAccess, NULL))
    return 0;

  // Convert the create time to local time.
  if (!FileTimeToLocalFileTime(&ftAccess, &ftLocal))
    return 0;

  LONGLONG qwResult = (((ULONGLONG)ftLocal.dwHighDateTime) << 32) + ftLocal.dwLowDateTime;

  // subtract out the diff between the year 1970 (the unix and javascript style we're trying to return),
  // and the year 1600 (the time FILETIME structs are built on)
  qwResult -= 116444736000000000;

  // divide to get the value back up to seconds from the FILETIME style of 100-interval nanoseconds
  qwResult /= 10000000;

  return (double)qwResult;
}

//------------------------------------------------------------------------
//	GetLastModifiedTime returns a double in UTC (Coordinated Universal Time)
//	i.e. the number of seconds since January 1, 1970, 00:00:00 GMT which
//	represents the file's last modified time
//------------------------------------------------------------------------
double
GetLastModifiedTime(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftWrite, ftLocal;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, NULL, NULL, &ftWrite))
    return 0;

  // Convert the create time to local time.
  if (!FileTimeToLocalFileTime(&ftWrite, &ftLocal))
    return 0;

  LONGLONG qwResult = (((ULONGLONG)ftLocal.dwHighDateTime) << 32) + ftLocal.dwLowDateTime;

  // subtract out the diff between the year 1970 (the unix and javascript style we're trying to return),
  // and the year 1600 (the time FILETIME structs are built on)
  qwResult -= 116444736000000000;

  // divide to get the value back up to seconds from the FILETIME style of 100-interval nanoseconds
  qwResult /= 10000000;

  return (double)qwResult;
}

//------------------------------------------------------------------------
//	GetCreationString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//------------------------------------------------------------------------
itString
GetCreationString(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftCreate, ftLocal;
  SYSTEMTIME stCreate;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, &ftCreate, NULL, NULL))
    return itString();

  // Convert the create time to local time.
  if (!FileTimeToLocalFileTime(&ftCreate, &ftLocal))
    return itString();

  // Convert the local file time from UTC to system time.
  FileTimeToSystemTime(&ftLocal, &stCreate);

  // Build a string showing the date and time.
  // WCHAR lpszString[20];
  // wsprintf(lpszString, L"%02d/%02d/%d",
  //  stCreate.wMonth, stCreate.wDay, stCreate.wYear);

  std::wostringstream create_ss(std::wostringstream::out);
  create_ss << std::setw(2) << std::setfill(L'0') << stCreate.wMonth << L"/";
  create_ss << std::setw(2) << std::setfill(L'0') << stCreate.wDay << L"/";
  create_ss << stCreate.wYear;

  return itString(create_ss.str().c_str());
}

//------------------------------------------------------------------------
//	GetLastAccessString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//------------------------------------------------------------------------
itString
GetLastAccessString(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftAccess, ftLocal;
  SYSTEMTIME stAccess;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, NULL, &ftAccess, NULL))
    return itString();

  // Convert the last-access time to local time.
  if (!FileTimeToLocalFileTime(&ftAccess, &ftLocal))
    return itString();

  // Convert the local file time from UTC to system time.
  FileTimeToSystemTime(&ftLocal, &stAccess);

  // Build a string showing the date and time.
  // WCHAR lpszString[20];
  // wsprintf(lpszString, L"%02d/%02d/%d",
  //   stAccess.wMonth, stAccess.wDay, stAccess.wYear);

  std::wostringstream access_ss(std::wostringstream::out);

  access_ss << std::setw(2) << std::setfill(L'0') << stAccess.wMonth << L"/";
  access_ss << std::setw(2) << std::setfill(L'0') << stAccess.wDay << L"/";
  access_ss << stAccess.wYear;

  return itString(access_ss.str().c_str());
}

//------------------------------------------------------------------------
//	GetLastModifiedString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//------------------------------------------------------------------------
itString
GetLastModifiedString(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftWrite, ftLocal;
  SYSTEMTIME stWrite;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, NULL, NULL, &ftWrite))
    return itString();

  // Convert the last-write time to local time.
  if (!FileTimeToLocalFileTime(&ftWrite, &ftLocal))
    return itString();

  // Convert the local file time from UTC to system time.
  FileTimeToSystemTime(&ftLocal, &stWrite);

  // Build a string showing the date and time.
  // WCHAR lpszString[20];
  // wsprintf(lpszString, L"%02d/%02d/%d",
  //	stWrite.wMonth, stWrite.wDay, stWrite.wYear);

  std::wostringstream write_ss(std::wostringstream::out);

  write_ss << std::setw(2) << std::setfill(L'0') << stWrite.wMonth << L"/";
  write_ss << std::setw(2) << std::setfill(L'0') << stWrite.wDay << L"/";
  write_ss << stWrite.wYear;

  return itString(write_ss.str().c_str());
}

//------------------------------------------------------------------------
//	GetCreationString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//------------------------------------------------------------------------
itString
GetCreationStringWithTime(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftCreate, ftLocal;
  SYSTEMTIME stCreate;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, &ftCreate, NULL, NULL))
    return itString();

  // Convert the create time to local time.
  if (!FileTimeToLocalFileTime(&ftCreate, &ftLocal))
    return itString();

  // Convert the local file time from UTC to system time.
  FileTimeToSystemTime(&ftLocal, &stCreate);

  // Build a string showing the date and time.
  // WCHAR lpszString[20];
  // wsprintf(lpszString, L"%02d/%02d/%d %02d:%02d",
  //	stCreate.wMonth, stCreate.wDay, stCreate.wYear,
  //   stCreate.wHour, stCreate.wMinute);

  std::wostringstream create_ss(std::wostringstream::out);

  create_ss << std::setw(2) << std::setfill(L'0') << stCreate.wMonth << L"/";
  create_ss << std::setw(2) << std::setfill(L'0') << stCreate.wDay << L"/";
  create_ss << stCreate.wYear << " ";
  create_ss << std::setw(2) << std::setfill(L'0') << stCreate.wHour << ":";
  create_ss << std::setw(2) << std::setfill(L'0') << stCreate.wMinute;

  return itString(create_ss.str().c_str());
}

//------------------------------------------------------------------------
//	GetLastAccessString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//------------------------------------------------------------------------
itString
GetLastAccessStringWithTime(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftAccess, ftLocal;
  SYSTEMTIME stAccess;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, NULL, &ftAccess, NULL))
    return itString();

  // Convert the last-access time to local time.
  if (!FileTimeToLocalFileTime(&ftAccess, &ftLocal))
    return itString();

  // Convert the local file time from UTC to system time.
  FileTimeToSystemTime(&ftLocal, &stAccess);

  // Build a string showing the date and time.
  // WCHAR lpszString[20];
  // wsprintf(lpszString, L"%02d/%02d/%d %02d:%02d",
  //	stAccess.wMonth, stAccess.wDay, stAccess.wYear,
  //	stAccess.wHour, stAccess.wMinute);

  std::wostringstream access_ss(std::wostringstream::out);

  access_ss << std::setw(2) << std::setfill(L'0') << stAccess.wMonth << L"/";
  access_ss << std::setw(2) << std::setfill(L'0') << stAccess.wDay << L"/";
  access_ss << stAccess.wYear << " ";
  access_ss << std::setw(2) << std::setfill(L'0') << stAccess.wHour << ":";
  access_ss << std::setw(2) << std::setfill(L'0') << stAccess.wMinute;

  return itString(access_ss.str().c_str());
}

//------------------------------------------------------------------------
//	GetLastModifiedString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//------------------------------------------------------------------------
itString
GetLastModifiedStringWithTime(const fsLocator& i_Locator)
{
  HANDLE Handle = GetHandle(i_Locator);
  FileCloser closer(Handle);

  FILETIME ftWrite, ftLocal;
  SYSTEMTIME stWrite;

  // Retrieve the file times for the file.
  if (!GetFileTime(Handle, NULL, NULL, &ftWrite))
    return itString();

  // Convert the last-write time to local time.
  if (!FileTimeToLocalFileTime(&ftWrite, &ftLocal))
    return itString();

  // Convert the local file time from UTC to system time.
  FileTimeToSystemTime(&ftLocal, &stWrite);

  // Build a string showing the date and time.
  // WCHAR lpszString[20];
  // wsprintf(lpszString, L"%02d/%02d/%d %02d:%02d",
  //	stWrite.wMonth, stWrite.wDay, stWrite.wYear,
  //	stWrite.wHour, stWrite.wMinute);

  std::wostringstream write_ss(std::wostringstream::out);

  write_ss << std::setw(2) << std::setfill(L'0') << stWrite.wMonth << L"/";
  write_ss << std::setw(2) << std::setfill(L'0') << stWrite.wDay << L"/";
  write_ss << stWrite.wYear << " ";
  write_ss << std::setw(2) << std::setfill(L'0') << stWrite.wHour << ":";
  write_ss << std::setw(2) << std::setfill(L'0') << stWrite.wMinute;

  return itString(write_ss.str().c_str());
}

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//------------------------------------------------------------------------
void
Init()
{
  l_ANSIFilenames = fsFileUtilPAC::MustUseANSIFilenames();
}

void
CleanUp() throw()
{
}

}
