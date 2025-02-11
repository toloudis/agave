/****************************************************************************\
**  fsFileUtil.hpp
**
**      fsFileUtil.hpp supplies functions that operate on files.
**
**
**
\****************************************************************************/
#undef CreateDirectory
#ifdef FS_FILEUTIL_HPP
#error fsFileUtil.hpp multiply included
#endif
#define FS_FILEUTIL_HPP

#include <string>

//============================================================================
//============================================================================
class fsLocator;
class itString;

//============================================================================
//============================================================================
namespace fsFileUtil {
//----------------------------------------------------------------------------
//	CreateFile creates a file with the given name.  This must be done before
//	reading or writing with a fsFileStream.
//----------------------------------------------------------------------------
void
CreateFile(const fsLocator& i_Filename);

//----------------------------------------------------------------------------
//	RenameFile renames a file or directory.  This can be used to move a
//	file to different directories, etc.  Known bug: it will not correctly
//	handle moving a directory to a different volume on Windows 98 or 95
//	systems.
//----------------------------------------------------------------------------
void
RenameFile(const fsLocator& i_From, const fsLocator& i_To);

//----------------------------------------------------------------------------
//	DeleteFile deletes a file.  The file must exist, or an exception will be
//	thrown.
//----------------------------------------------------------------------------
void
DeleteFile(const fsLocator& i_Filename);

//----------------------------------------------------------------------------
//	CopyFile copies an entire file from one location to another.  Of course,
//	the file must exist.
//----------------------------------------------------------------------------
void
CopyFile(const fsLocator& i_From, const fsLocator& i_To);

//----------------------------------------------------------------------------
//	CreateDirectory creates a directory with the given name.
//----------------------------------------------------------------------------
void
CreateDirectory(const fsLocator& i_DirectoryName);

//----------------------------------------------------------------------------
//	DeleteDirectory destroys a directory with the given name.  The directory
//	must be empty or a fsDirectoryNotEmpty exception will be thrown.
//----------------------------------------------------------------------------
void
DeleteDirectory(const fsLocator& i_DirectoryName);

//----------------------------------------------------------------------------
//	DeleteDirectoryRecursively deletes the contents of the directory.
//----------------------------------------------------------------------------
bool
DeleteDirectoryRecursively(const fsLocator& i_DirectoryName);

//----------------------------------------------------------------------------
//	CopyDirectory recursively copies the contents of one directory to another.
//	Of course, the from directory must exist
//----------------------------------------------------------------------------
void
CopyDirectory(const fsLocator& i_From, const fsLocator& i_To);

//----------------------------------------------------------------------------
//	FileExists tests if a file exists.
//----------------------------------------------------------------------------
bool
FileExists(const fsLocator& i_Filename);

//----------------------------------------------------------------------------
//	DirectoryExists tests if a directory exists.
//----------------------------------------------------------------------------
bool
DirectoryExists(const fsLocator& i_Filename);

//----------------------------------------------------------------------------
// UnicodeStringToLocator returns a locator from a given itString
// the string is separted by backslashes in the windows style ,this string
// should not be used for pathing, but rather for logging or perhaps
// storing a locator as a fully pathed itString. NOTE: the index will be ignored,
// be sure to call gfFileTranslationMgr::ExpandLocator first if you need to
//----------------------------------------------------------------------------
void
UnicodeStringToLocator(const itString& i_String, fsLocator& o_Locator);

//----------------------------------------------------------------------------
// LocatorToUnicodeString returns an itString from a given locator
// the string is separted by backslashes in the windows style ,this string
// should not be used for pathing, but rather for logging or perhaps
// storing a locator as a fully pathed itString. NOTE: the index will be ignored,
// be sure to call gfFileTranslationMgr::ExpandLocator first if you need to
//----------------------------------------------------------------------------
void
LocatorToUnicodeString(const fsLocator& i_Locator, itString& o_String);

//----------------------------------------------------------------------------
// ANSIFilenameToLocator returns a locator from an ANSI pathname
// API Change: This now interprets the std::string as UTF8
//----------------------------------------------------------------------------
void
ANSIFilenameToLocator(const std::string& i_String, fsLocator& o_Locator);

//----------------------------------------------------------------------------
//	LocatorToANSIFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a ANSI win32 filename (for use in
//	Windows 95 or 98).  It is provided here to assist in error reporting.
// API Change: This now returns the std::string as UTF8.
//----------------------------------------------------------------------------
void
LocatorToANSIFilename(const fsLocator& i_Locator, std::string& o_String, bool i_bUseDoubleSlashes = false);

//----------------------------------------------------------------------------
//	LocatorToANSIFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a ANSI win32 filename (for use in
//	Windows 95 or 98).  It is provided here to assist in error reporting.
// API Change: This now returns the std::string as UTF8.
//----------------------------------------------------------------------------
void
LocatorToANSIFilename(const fsLocator& i_Locator, std::string& o_String, const int i_StartNum, const int i_EndNum);

//----------------------------------------------------------------------------
// Return true if every path in the filename is an ascii string
//----------------------------------------------------------------------------
bool
IsFilenameASCII(const fsLocator& i_Locator);

//----------------------------------------------------------------------------
//	Create a filename string that is unique within the given pathlocator,
//	using the given prefix and extension.
//----------------------------------------------------------------------------
void
GenerateFileName(const fsLocator& i_PathLocator, std::string& i_Prefix, std::string& i_Extension, std::string& o_Name);

//----------------------------------------------------------------------------
//  Check whether filename is absolute or not
//----------------------------------------------------------------------------
bool
IsFileNameAbsolute(const fsLocator& i_Loc);

//----------------------------------------------------------------------------
//	IsReadOnly returns true if the file is read-only, meaning that write
//	operations to the file will fail.
//----------------------------------------------------------------------------
bool
IsReadOnly(const fsLocator& i_Locator);

//----------------------------------------------------------------------------
//	SetReadOnly() - sets the file to be read only if true is passed in.
//----------------------------------------------------------------------------
void
SetReadOnly(const fsLocator& i_Locator, bool i_bSetReadOnly);

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//------------------------------------------------------------------------
void
Init();
void
CleanUp() throw();
}
