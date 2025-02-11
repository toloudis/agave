/*****************************************************************************
**	fsFileUtil.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsFileX.hpp"

#include "Core/fs/fsFileUtil.hpp"

//----------------------------------------------------------------------------
//	fsFileExceptionX is base class for exceptions that refer to
//	a fsLocator.
//----------------------------------------------------------------------------
fsFileExceptionX::fsFileExceptionX(const fsLocator& i_Locator)
  : m_Locator(i_Locator)
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
fsFileExceptionX::~fsFileExceptionX() {}

//----------------------------------------------------------------------------
//	GetLocator() returns the offending directory
//----------------------------------------------------------------------------
const fsLocator&
fsFileExceptionX::GetLocator() const
{
  return m_Locator;
}

//--------------------------------------------------------------------
//	GetLocatorString() returns locator as string for error messages
//--------------------------------------------------------------------
std::string
fsFileExceptionX::GetLocatorString() const
{
  std::string file_str;
  fsFileUtil::LocatorToANSIFilename(m_Locator, file_str);
  return file_str;
}

//----------------------------------------------------------------------------
//	fsDirectoryDoesntExistX is thrown when the user tries to do something
//	with a directory that doesn't exist.
//----------------------------------------------------------------------------
fsDirectoryDoesntExistX::fsDirectoryDoesntExistX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsDirectoryDoesntExistX::GetErrorMessage() const
{
  return "Directory does not exist: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsDirectoryNotEmptyX is thrown when the user tries to delete a directory
//	which contains files or other directories.
//----------------------------------------------------------------------------
fsDirectoryNotEmptyX::fsDirectoryNotEmptyX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsDirectoryNotEmptyX::GetErrorMessage() const
{
  return "Directory not empty: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsFileDoesntExistX is thrown when the user tries to do something with
//	a file that doesn't exist.
//----------------------------------------------------------------------------
fsFileDoesntExistX::fsFileDoesntExistX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsFileDoesntExistX::GetErrorMessage() const
{
  return "File does not exist: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsFileExistsX is thrown when the user tries to create a file that already
//	exists.
//----------------------------------------------------------------------------
fsFileExistsX::fsFileExistsX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsFileExistsX::GetErrorMessage() const
{
  return "File already exists: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsFileInUseX is thrown when the user tries to operate on a file that is
//	already open (possibly by another process).
//----------------------------------------------------------------------------
fsFileInUseX::fsFileInUseX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsFileInUseX::GetErrorMessage() const
{
  return "File is currently in use: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsInvalidLocatorX is thrown when a locator contains characters that the
//	OS will not allow in a filename, or is flawed in some other way.
//	(too long?)
//----------------------------------------------------------------------------
fsInvalidLocatorX::fsInvalidLocatorX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsInvalidLocatorX::GetErrorMessage() const
{
  return "Invalid filename " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsReadOnlyX is thrown when the user tries to write to a file marked as
//	read only.
//----------------------------------------------------------------------------
fsReadOnlyX::fsReadOnlyX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsReadOnlyX::GetErrorMessage() const
{
  return "File is read only: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsDiskFullX is thrown when the user tries to write to a file
//	but there is not disk space left or the disk is corrupt
//----------------------------------------------------------------------------
fsDiskFullX::fsDiskFullX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsDiskFullX::GetErrorMessage() const
{
  return "Out of disk space or the disk is corrupt. Could not write  " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsUnknownX is thrown when an error is not covered by the above exceptions.
//----------------------------------------------------------------------------
fsUnknownX::fsUnknownX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsUnknownX::GetErrorMessage() const
{
  return "Cannot read file: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsXMLErrorX is thrown when an error reading an XML file occurs
//----------------------------------------------------------------------------
fsXMLErrorX::fsXMLErrorX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsXMLErrorX::GetErrorMessage() const
{
  return "Error reading XML file: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsNewerVersionAbortX is thrown when an error reading an XML file occurs
//----------------------------------------------------------------------------
fsNewerVersionAbortX::fsNewerVersionAbortX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsNewerVersionAbortX::GetErrorMessage() const
{
  return "User aborted reading in file because version is newer than application supports: " + GetLocatorString();
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsFileNotAllowedX is thrown when the file being read in is not allowed by
//	by the application.
//----------------------------------------------------------------------------
fsFileNotAllowedX::fsFileNotAllowedX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsFileNotAllowedX::GetErrorMessage() const
{
  return "The file " + GetLocatorString() + " was created in software not allowed in this version of the application.";
}

//****************************************************************************
//----------------------------------------------------------------------------
//	fsUnsupportedVersionX is thrown when the app reads a version of a file
//	that this app doesn't support.  (e.g. MSCore can't read in MSPro files.)
//----------------------------------------------------------------------------
fsUnsupportedVersionX::fsUnsupportedVersionX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
fsUnsupportedVersionX::GetErrorMessage() const
{
  return "File " + GetLocatorString() + " was created in an application that isn't supported.";
}
