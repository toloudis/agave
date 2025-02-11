/****************************************************************************\
**  fsFileUtilPACWin.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
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
#include "Core/Env/envString.hpp"
#include "Core/env/envSystemDataPACWin.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/fs/fsFileX.hpp"
#include "Core/it/itStringUtil.hpp"

//============================================================================
//============================================================================
namespace fsFileUtilPAC {

namespace {
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

    case ERROR_DISK_CORRUPT:
    case ERROR_DISK_FULL:
      throw fsDiskFullX(i_Locator);
      break;

    default:
      throw fsUnknownX(i_Locator);
      break;
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
handle_windows_multifile_error(const fsLocator& i_From, const fsLocator& i_To)
{
  DWORD error = ::GetLastError();

  switch (error) {
    case ERROR_FILE_NOT_FOUND:
      throw fsFileDoesntExistX(i_From);
      break;

    case ERROR_PATH_NOT_FOUND:
      throw fsDirectoryDoesntExistX(i_From);
      break;

    case ERROR_INVALID_DRIVE:
      throw fsInvalidLocatorX(i_From);
      break;

    case ERROR_SHARING_VIOLATION:
      throw fsFileInUseX(i_From);
      break;

    case ERROR_LOCK_VIOLATION:
      throw fsFileInUseX(i_From);
      break;

    case ERROR_FILE_EXISTS:
    case ERROR_ALREADY_EXISTS:
      throw fsFileExistsX(i_To);
      break;

    case ERROR_DRIVE_LOCKED:
      throw fsFileInUseX(i_From);
      break;

    case ERROR_INVALID_NAME:
      throw fsInvalidLocatorX(i_From);
      break;

    case ERROR_PATH_BUSY:
      throw fsFileInUseX(i_From);
      break;

    case ERROR_ACCESS_DENIED:
      throw fsReadOnlyX(i_To);
      break;

    case ERROR_DISK_CORRUPT:
    case ERROR_DISK_FULL:
      throw fsDiskFullX(i_To);
      break;

    default:
      throw fsUnknownX(i_From);
      break;
  }
}

}

//----------------------------------------------------------------------------
//	CreateFile creates a file with the given name.  This must be done before
//	reading or writing with a fsFileStream.
//----------------------------------------------------------------------------
void
CreateFile(const fsLocator& i_Filename)
{
  HANDLE new_file = INVALID_HANDLE_VALUE;

  if (l_ANSIFilenames) {
    // DBG_LOG("Creating ANSI file");

    std::string filename;
    LocatorToANSIFilename(i_Filename, filename);

    if (filename.size() == 0)
      throw fsInvalidLocatorX(i_Filename);

    new_file = ::CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
  } else {
    // DBG_LOG("Creating/Opening UNICODE file  " << i_Filename);

    itString filename;
    LocatorToUnicodeFilename(i_Filename, filename);

    if (filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_Filename);

    new_file = ::CreateFileW(filename.GetString(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
  }

  if (new_file == INVALID_HANDLE_VALUE)
    handle_windows_error(i_Filename); // probably throws
  else
    ::CloseHandle(new_file); // release file so something else can write to it
}

//----------------------------------------------------------------------------
//	RenameFile renames a file.  This can be used to move a file to different
//	directories, etc.
//----------------------------------------------------------------------------
void
RenameFile(const fsLocator& i_From, const fsLocator& i_To)
{
  BOOL ret_val = 0;

  envSystemDataPAC::WindowsOS os_type = envSystemDataPAC::GetWindowsOSType();

  bool has_move_file_ex = (os_type == envSystemDataPAC::e_Windows2000) || (os_type == envSystemDataPAC::e_WindowsXP) ||
                          (os_type == envSystemDataPAC::e_WindowsVista) || (os_type == envSystemDataPAC::e_Windows7);

  if (l_ANSIFilenames) {
    std::string from_filename, to_filename;

    LocatorToANSIFilename(i_From, from_filename);
    LocatorToANSIFilename(i_To, to_filename);

    if (from_filename.size() == 0)
      throw fsInvalidLocatorX(i_From);

    if (to_filename.size() == 0)
      throw fsInvalidLocatorX(i_To);

    if (has_move_file_ex)
      ret_val = ::MoveFileExA(from_filename.c_str(), to_filename.c_str(), MOVEFILE_COPY_ALLOWED);
    else
      ret_val = ::MoveFileA(from_filename.c_str(), to_filename.c_str());
  } else {
    itString from_filename, to_filename;

    LocatorToUnicodeFilename(i_From, from_filename);
    LocatorToUnicodeFilename(i_To, to_filename);

    if (from_filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_From);

    if (to_filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_To);

    if (has_move_file_ex)
      ret_val = ::MoveFileExW(from_filename.GetString(), to_filename.GetString(), MOVEFILE_COPY_ALLOWED);
    else
      ret_val = ::MoveFileW(from_filename.GetString(), to_filename.GetString());
  }

  if (ret_val == 0)
    handle_windows_multifile_error(i_From, i_To);
}

//----------------------------------------------------------------------------
//	DeleteFile deletes a file.  The file must exist, or an exception will be
//	thrown.
//----------------------------------------------------------------------------
void
DeleteFile(const fsLocator& i_Filename)
{
  BOOL ret_val = 0;

  if (l_ANSIFilenames) {
    std::string filename;

    LocatorToANSIFilename(i_Filename, filename);

    if (filename.size() == 0)
      throw fsInvalidLocatorX(i_Filename);

    ret_val = ::DeleteFileA(filename.c_str());
  } else {
    itString filename;

    LocatorToUnicodeFilename(i_Filename, filename);

    if (filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_Filename);

    ret_val = ::DeleteFileW(filename.GetString());
  }

  if (ret_val == 0)
    handle_windows_error(i_Filename);
}

//----------------------------------------------------------------------------
//	CopyFile copies an entire file from one location to another.  Of course,
//	the file must exist.
//----------------------------------------------------------------------------
void
CopyFile(const fsLocator& i_From, const fsLocator& i_To)
{
  BOOL ret_val = 0;

  if (l_ANSIFilenames) {
    std::string from_filename, to_filename;

    LocatorToANSIFilename(i_From, from_filename);
    LocatorToANSIFilename(i_To, to_filename);

    if (from_filename.size() == 0)
      throw fsInvalidLocatorX(i_From);

    if (to_filename.size() == 0)
      throw fsInvalidLocatorX(i_To);

    ret_val = ::CopyFileA(from_filename.c_str(), to_filename.c_str(), true);
  } else {
    itString from_filename, to_filename;

    LocatorToUnicodeFilename(i_From, from_filename);
    LocatorToUnicodeFilename(i_To, to_filename);

    if (from_filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_From);

    if (to_filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_To);

    ret_val = ::CopyFileW(from_filename.GetString(), to_filename.GetString(), true);
  }

  if (ret_val == 0)
    handle_windows_multifile_error(i_From, i_To);
}

//----------------------------------------------------------------------------
//	CopyDirectory copies a directory with the given name and all sub-dirs.
//----------------------------------------------------------------------------
void
CopyDirectory(const fsLocator& i_DirectoryName)
{
  DBG_ASSERT(false, "please implement this function.");
}

//----------------------------------------------------------------------------
//	CreateDirectory creates a directory with the given name.
//----------------------------------------------------------------------------
void
CreateDirectoryOneDir(const fsLocator& i_DirectoryName)
{
  BOOL ret_val = 0;

  // DBG_LOG("Create One Dir= " << i_DirectoryName);

  if (l_ANSIFilenames) {
    std::string filename;

    LocatorToANSIFilename(i_DirectoryName, filename);

    if (filename.size() == 0)
      throw fsInvalidLocatorX(i_DirectoryName);

    itStringUtil::TrimSpaces(filename);
    ret_val = ::CreateDirectoryA(filename.c_str(), NULL);
    //	return errors ERROR_PATH_NOT_FOUND or ERROR_ALREADY_EXISTS
  } else {
    itString filename;

    LocatorToUnicodeFilename(i_DirectoryName, filename);

    if (filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_DirectoryName);

    itStringUtil::TrimSpaces(filename);
    ret_val = ::CreateDirectoryW(filename.GetString(), NULL);
  }

  if (ret_val == 0)
    handle_windows_error(i_DirectoryName);
}

//----------------------------------------------------------------------------
//	CreateDirectory creates a directory with the given name.
//----------------------------------------------------------------------------
void
CreateDirectoryRecursively(const fsLocator& i_DirectoryName)
{
  if (!DirectoryExists(i_DirectoryName)) {
    if (i_DirectoryName.GetNumNames() == 0)
      throw fsDirectoryDoesntExistX(i_DirectoryName);

    fsLocator newdir(i_DirectoryName);
    itString lastdir = newdir.GetLastName();
    newdir.Pop();

    CreateDirectoryRecursively(newdir);

    newdir.Push(lastdir);
    CreateDirectoryOneDir(newdir);
  }
}

//----------------------------------------------------------------------------
//	CreateDirectory creates a directory with the given name.
//----------------------------------------------------------------------------
void
CreateDirectory(const fsLocator& i_DirectoryName)
{
  CreateDirectoryRecursively(i_DirectoryName);
}

//----------------------------------------------------------------------------
//	DeleteDirectory destroys a directory with the given name.  The directory
//	must be empty or a fsDirectoryNotEmpty exception will be thrown.
//----------------------------------------------------------------------------
void
DeleteDirectory(const fsLocator& i_DirectoryName)
{
  BOOL ret_val = 0;

  if (l_ANSIFilenames) {
    std::string filename;

    LocatorToANSIFilename(i_DirectoryName, filename);

    if (filename.size() == 0)
      throw fsInvalidLocatorX(i_DirectoryName);

    ret_val = ::RemoveDirectoryA(filename.c_str());
  } else {
    itString filename;

    LocatorToUnicodeFilename(i_DirectoryName, filename);

    if (filename.GetLength() == 0)
      throw fsInvalidLocatorX(i_DirectoryName);

    ret_val = ::RemoveDirectoryW(filename.GetString());
  }

  if (ret_val == 0)
    handle_windows_error(i_DirectoryName);
}

//----------------------------------------------------------------------------
//	FileExists tests if a file exists.
//
//----------------------------------------------------------------------------
bool
FileExists(const fsLocator& i_Filename)
{
  DWORD ret_val = -1;

  if (l_ANSIFilenames) {
    std::string filename;
    LocatorToANSIFilename(i_Filename, filename);

    if (filename.size() == 0)
      return false;

    ret_val = ::GetFileAttributesA(filename.c_str());
  } else {
    itString filename;
    LocatorToUnicodeFilename(i_Filename, filename);

    if (filename.GetLength() == 0)
      return false;

    ret_val = ::GetFileAttributesW(filename.GetString());
  }

  if ((ret_val == -1) || (ret_val & FILE_ATTRIBUTE_DIRECTORY))
    return false;
  else
    return true;
}

//----------------------------------------------------------------------------
//	DirectoryExists tests if a directory exists.
//
//----------------------------------------------------------------------------
bool
DirectoryExists(const fsLocator& i_Filename)
{
  DWORD ret_val = -1;

  if (l_ANSIFilenames) {
    std::string filename;
    LocatorToANSIFilename(i_Filename, filename);

    if (filename.size() == 0)
      return false;

    ret_val = ::GetFileAttributesA(filename.c_str());
  } else {
    itString filename;
    LocatorToUnicodeFilename(i_Filename, filename);

    if (filename.GetLength() == 0)
      return false;

    ret_val = ::GetFileAttributesW(filename.GetString());
  }

  if ((ret_val == -1) || ((ret_val & FILE_ATTRIBUTE_DIRECTORY) == 0))
    return false;
  else
    return true;
}

//------------------------------------------------------------------------
// ANSIFilenameToLocator returns a locator from an ANSI pathname
// API Change: This now interprets the std::string as UTF8
//------------------------------------------------------------------------
void
ANSIFilenameToLocator(const std::string& i_String, fsLocator& o_Locator)
{
  o_Locator.Clear();

  int k;
  int nBegin;
  itString itName(i_String.c_str()); // this line here assumes UTF8 now
  itString Piece;
  nBegin = 0;
  for (k = 0; k < itName.GetLength(); k++) {
    if ((itName[k] == itString::CharType('\\')) || (itName[k] == itString::CharType('/'))) {
      Piece = itString(nBegin, (k - nBegin), itName);
      nBegin = k + 1;
      if (Piece.GetLength()) {
        o_Locator.Push(Piece);
      } else {
        //	check for UNC Path
        if (k == 0) {
          o_Locator.SetUNCPath(true);
        }
      }
    }
  }

  Piece = itString(nBegin, (itName.GetLength() - nBegin), itName);
  if (Piece.GetLength()) {
    o_Locator.Push(Piece);
  }
}

//----------------------------------------------------------------------------
//	LocatorToANSIFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a ANSI win32 filename (for use in
//	Windows 95 or 98).
// API Change: This now returns the std::string as UTF8.
//----------------------------------------------------------------------------
void
LocatorToANSIFilename(const fsLocator& i_Locator, std::string& o_String, bool i_bUseDoubleSlashes)
{
  std::string work_string;
  // char work_string[MAX_PATH];
  int num_names = i_Locator.GetNumNames();
  // int num_chars;

  o_String.clear();

  if (num_names <= 0)
    return;

  --num_names;

  //	if UNC Path then prepend \\ at the beginning
  //
  if (i_Locator.IsUNCPath()) {
    o_String += '\\';
    o_String += '\\';
  }

  // iterate and add each name to the path
  //
  for (int i = 0; i < num_names; i++) {
    const itString& cur_string = i_Locator.GetName(i);

    // New UTF8 version:
    work_string = envString::WideCharToUTF8(cur_string.GetString(), cur_string.GetLength());

    // Old ANSI version
    // num_chars = ::WideCharToMultiByte(	::GetACP(),	// ANSI code page
    //									0,			// no "lo-performance"
    // flags 									cur_string.GetString(), 									cur_string.GetLength(),
    // work_string, MAX_PATH,
    // // size of target buffer 									NULL,
    // NULL);

    //// for some reason, this function doesn't seem to terminate the
    //// work_string
    // work_string[num_chars] = 0;

    o_String += work_string;
    o_String += '\\'; // official directory separator character

    // add a second slash seperator - this is a fix for appending
    //"My Documents" directory to python sys.path
    if (i_bUseDoubleSlashes)
      o_String += '\\';
  }

  const itString& last_string = i_Locator.GetLastName();

  // New UTF8 version:
  work_string = envString::WideCharToUTF8(last_string.GetString(), last_string.GetLength());

  // Old ANSI version
  // num_chars = ::WideCharToMultiByte(	::GetACP(),		// ANSI code page
  //									0,			// no "lo-performance"
  // flags 									last_string.GetString(),
  // last_string.GetLength(),
  // work_string, 									MAX_PATH,	// size of
  // target buffer NULL, 									NULL);

  //// for some reason, this function doesn't seem to terminate the
  //// work_string
  // work_string[num_chars] = 0;

  o_String += work_string;

  //	o_String += char(0);
}

//----------------------------------------------------------------------------
//	LocatorToANSIFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a ANSI win32 filename (for use in
//	Windows 95 or 98).
// API Change: This now returns the std::string as UTF8.
//----------------------------------------------------------------------------
void
LocatorToANSIFilename(const fsLocator& i_Locator, std::string& o_String, const int i_StartNum, const int i_EndNum)
{
  // char work_string[MAX_PATH];
  int num_names = i_Locator.GetNumNames();
  // int num_chars;

  o_String.clear();

  if (num_names <= 0)
    return;

  --num_names;

  DBG_ASSERT(i_StartNum >= 0, "Locator start index out of range - " << i_StartNum);
  DBG_ASSERT(i_EndNum <= num_names, "Locator end index out of range - " << i_EndNum << " > " << num_names);

  //	if UNC Path then prepend \\ at the beginning
  //
  if (i_Locator.IsUNCPath() && i_StartNum == 0) {
    o_String += '\\';
    o_String += '\\';
  }

  // iterate and add each name to the path
  //
  for (int i = i_StartNum; i < i_EndNum; i++) {
    const itString& cur_string = i_Locator.GetName(i);

    // New UTF8 version:
    std::string work_string = envString::WideCharToUTF8(cur_string.GetString(), cur_string.GetLength());

    // Old ANSI version
    // num_chars = ::WideCharToMultiByte(	::GetACP(),		// ANSI code page
    //									0,			// no "lo-performance"
    // flags 									cur_string.GetString(), 									cur_string.GetLength(),
    // work_string, MAX_PATH,
    // // size of target buffer 									NULL,
    // NULL);

    //// for some reason, this function doesn't seem to terminate the
    //// work_string
    // work_string[num_chars] = 0;

    o_String += work_string;
    o_String += '\\'; // official directory separator character
  }

  const itString& last_string = i_Locator.GetLastName();

  // New UTF8 version:
  std::string work_string = envString::WideCharToUTF8(last_string.GetString(), last_string.GetLength());

  // Old ANSI version
  // num_chars = ::WideCharToMultiByte(	::GetACP(),		// ANSI code page
  //									0,			// no "lo-performance"
  // flags 									last_string.GetString(),
  // last_string.GetLength(),
  // work_string, 									MAX_PATH,	// size of
  // target buffer NULL, 									NULL);

  //// for some reason, this function doesn't seem to terminate the
  //// work_string
  // work_string[num_chars] = 0;

  o_String += work_string;
  //	o_String += char(0);
}

//----------------------------------------------------------------------------
//	LocatorToUnicodeFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a Unicode filename (for use in
//	Windows NT or 2000).
//----------------------------------------------------------------------------
void
LocatorToUnicodeFilename(const fsLocator& i_Locator, itString& o_String)
{
  int num_names = i_Locator.GetNumNames();
  int i;

  o_String.Clear();

  if (num_names <= 0)
    return;

  --num_names;

  //	if UNC Path then prepend \\ at the beginning
  //
  if (i_Locator.IsUNCPath()) {
    o_String += '\\';
    o_String += '\\';
  }

  // iterate and add each name to the path
  //
  for (i = 0; i < num_names; i++) {
    o_String += i_Locator.GetName(i);

    // a harmless assumption here:
    // Unicode "backslash" character is the same numeric
    // value as ANSI backslash
    o_String += itString::CharType('\\');
  }

  o_String += i_Locator.GetLastName();
  // null-terminator implied in itString now
  // o_String += itString::CharType(0);
}

//----------------------------------------------------------------------------
//	LocatorToUnicodeFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a Unicode filename (for use in
//	Windows NT or 2000).
//----------------------------------------------------------------------------
void
LocatorToUnicodeFilename(const fsLocator& i_Locator, itString& o_String, const int i_StartNum, const int i_EndNum)
{
  int num_names = i_Locator.GetNumNames();
  int i;

  o_String.Clear();

  if (num_names <= 0)
    return;

  --num_names;

  DBG_ASSERT(i_StartNum >= 0, "Locator start index out of range - " << i_StartNum);
  DBG_ASSERT(i_EndNum <= num_names, "Locator end index out of range - " << i_EndNum << " > " << num_names);

  //	if UNC Path then prepend \\ at the beginning
  //
  if (i_Locator.IsUNCPath() && i_StartNum == 0) {
    o_String += '\\';
    o_String += '\\';
  }

  // iterate and add each name to the path
  //
  for (i = i_StartNum; i < i_EndNum; i++) {
    o_String += i_Locator.GetName(i);

    // a harmless assumption here:
    // Unicode "backslash" character is the same numeric
    // value as ANSI backslash
    o_String += itString::CharType('\\');
  }

  // bga - This GetLastName() call is actually false, it should be the "EndNum" which makes me think that this
  //  function isn't used anywhere. I am tempted to comment it out completely to see. Fewer functions are
  //  easier to maintain.
  // o_String += i_Locator.GetLastName();
  o_String += i_Locator.GetName(i_EndNum);

  // null-terminator implied in itString now
  // o_String += itString::CharType(0);
}

//----------------------------------------------------------------------------
//	MustUseANSIFilenames() returns true for Windows OSs that don't have
//	full Unicode support.
//----------------------------------------------------------------------------
bool
MustUseANSIFilenames()
{
  return l_ANSIFilenames;
}

//=============================================================================
//  Check whether filename is absolute or not
//=============================================================================
bool
IsFileNameAbsolute(const fsLocator& i_Loc)
{
  if (i_Loc.IsUNCPath()) {
    return true;
  }

  if (i_Loc.GetNumNames() > 0) {
    const itString& sRootDir = i_Loc.GetName(0);

    if (sRootDir.GetLength() >= 2 && sRootDir[1] == L':') {
      return true;
    }
  }
  return false;
}
//----------------------------------------------------------------------------
//	IsReadOnly returns true if the file is read-only, meaning that write
//	operations to the file will fail.
//----------------------------------------------------------------------------
bool
IsReadOnly(const fsLocator& i_Locator)
{
  DWORD attribute = -1;

  if (l_ANSIFilenames) {
    std::string filename;
    LocatorToANSIFilename(i_Locator, filename);

    if (filename.size() == 0)
      return false;

    attribute = ::GetFileAttributesA(filename.c_str());
  } else {
    itString filename;
    LocatorToUnicodeFilename(i_Locator, filename);

    if (filename.GetLength() == 0)
      return false;

    attribute = ::GetFileAttributesW(filename.GetString());
  }

  if ((attribute == -1) || ((attribute & FILE_ATTRIBUTE_DIRECTORY) != 0))
    throw fsFileDoesntExistX(i_Locator);

  return (attribute & FILE_ATTRIBUTE_READONLY) != 0;
}

//----------------------------------------------------------------------------
//	SetReadOnly() - sets the file to be read only if true is passed in.
//----------------------------------------------------------------------------
void
SetReadOnly(const fsLocator& i_Locator, bool i_bSetReadOnly)
{
  if (!FileExists(i_Locator))
    return;

  //	create the filename string
  itString fileName;
  LocatorToUnicodeFilename(i_Locator, fileName);

  //	check the attributes and determine what to set (if anything)
  //
  DWORD attribs = GetFileAttributesW(fileName.GetString());
  if (i_bSetReadOnly) {
    if (!(attribs & FILE_ATTRIBUTE_READONLY)) {
      SetFileAttributesW(fileName.GetString(), attribs | FILE_ATTRIBUTE_READONLY);
    }
  } else {
    if ((attribs & FILE_ATTRIBUTE_READONLY)) {
      SetFileAttributesW(fileName.GetString(), attribs ^ FILE_ATTRIBUTE_READONLY);
    }
  }
}

//----------------------------------------------------------------------------
//	Create a filename string that is unique within the given pathlocator,
//	using the given prefix and extension.
//----------------------------------------------------------------------------
void
GenerateFileName(const fsLocator& i_PathLocator, std::string& i_Prefix, std::string& i_Extension, std::string& o_Name)
{
  // maybe some asserts for valid inputs?
  // extension ends in .???
  // prefix not empty

  std::string sPath;
  fsLocator loc;

  // build filename into loc.
  loc = i_PathLocator;

  // initally, try for just prefix+extension.
  std::string name = i_Prefix;
  name.append(i_Extension);
  loc.Push(name.c_str());

  int index = 0;
  while (FileExists(loc)) {
    loc.Pop();

    name = i_Prefix;
    char sIndex[10]; // 9 digits better be enough.
    _itoa(index++, sIndex, 10);
    name.append(sIndex);
    name.append(i_Extension);
    loc.Push(name.c_str());
  }
  // isn't name the name?
  o_Name = name;
  // strip off path portion; +1 for slash
  //	LocatorToANSIFilename(loc, sPath);
  //	std::string filePath;
  //	LocatorToANSIFilename(i_PathLocator, filePath);
  //	o_Name = sPath.substr(filePath.length()+1, sPath.length()-(filePath.length()+1));
}

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//------------------------------------------------------------------------
void
Init()
{
  envSystemDataPAC::WindowsOS os_type = envSystemDataPAC::GetWindowsOSType();

  if ((os_type == envSystemDataPAC::e_Windows95) || (os_type == envSystemDataPAC::e_Windows98) ||
      (os_type == envSystemDataPAC::e_WindowsME)) {
    l_ANSIFilenames = true;
  } else {
    l_ANSIFilenames = false;
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
CleanUp() throw()
{
}

}
