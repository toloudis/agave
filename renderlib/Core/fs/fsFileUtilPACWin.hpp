/****************************************************************************\
**  fsFileUtilPACWin.hpp
**
**      fsFileUtilPACWin.hpp defines the file util PAC for windows.
**
**
**
\****************************************************************************/

#ifdef FS_FILEUTILPACWIN_HPP
#error fsFileUtilPACWin.hpp multiply included
#endif
#define FS_FILEUTILPACWIN_HPP

#include <string>

//	windows foolishly defines these things as macros
#undef CreateFile
#undef DeleteFile
#undef CreateDirectory
#undef CopyFile

//============================================================================
//============================================================================
class itString;
class fsLocator;

//============================================================================
//============================================================================
namespace fsFileUtilPAC {
//----------------------------------------------------------------------------
//	CreateFile creates a file with the given name.  This must be done before
//	reading or writing with a fsFileStream.
//----------------------------------------------------------------------------
void
CreateFile(const fsLocator& i_Filename);

//----------------------------------------------------------------------------
//	RenameFile renames a file.  This can be used to move a file to different
//	directories, etc.
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
//	CopyDirectory copies a directory with the given name and all sub-dirs.
//----------------------------------------------------------------------------
void
CopyDirectory(const fsLocator& i_DirectoryName);

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
//	FileExists tests if a file exists.
//----------------------------------------------------------------------------
bool
FileExists(const fsLocator& i_Filename);

//----------------------------------------------------------------------------
//	DirectoryExists tests if a directory exists.
//----------------------------------------------------------------------------
bool
DirectoryExists(const fsLocator& i_Filename);

//------------------------------------------------------------------------
// ANSIFilenameToLocator returns a locator from an ANSI pathname
// API Change: This now interprets the std::string as UTF8
//------------------------------------------------------------------------
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
//	LocatorToUnicodeFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a Unicode filename (for use in
//	Windows NT or 2000).  This function is not part of the fsFileUtil
//	interface.
//----------------------------------------------------------------------------
void
LocatorToUnicodeFilename(const fsLocator& i_Locator, itString& o_String);

//----------------------------------------------------------------------------
//	LocatorToUnicodeFilename is used internally by the win32 PACs in the fs
//	package.  It converts a fsLocator to a Unicode filename (for use in
//	Windows NT or 2000).  This function is not part of the fsFileUtil
//	interface.
//----------------------------------------------------------------------------
void
LocatorToUnicodeFilename(const fsLocator& i_Locator, itString& o_String, const int i_StartNum, const int i_EndNum);

//----------------------------------------------------------------------------
//	Create a filename string that is unique within the given pathlocator,
//	using the given prefix and extension.
//----------------------------------------------------------------------------
void
GenerateFileName(const fsLocator& i_PathLocator, std::string& i_Prefix, std::string& i_Extension, std::string& o_Name);

//----------------------------------------------------------------------------
//	MustUseANSIFilenames() returns true for Windows OSs that don't have
//	full Unicode support.  This function is not part of the fsFileUtil
//	interface.
//----------------------------------------------------------------------------
bool
MustUseANSIFilenames();

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
