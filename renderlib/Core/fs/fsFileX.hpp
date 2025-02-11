/****************************************************************************\
**	fsFileUtil.hpp
**
**		fsFileX.hpp defines the exception classes used by the fs package.
**
**
**
\****************************************************************************/
#ifdef FS_FILEX_HPP
#error fsFileX.hpp multiply included
#endif
#define FS_FILEX_HPP

#ifndef ENV_EXCEPTIONX_HPP
#include "Core/env/envExceptionX.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

//============================================================================
//	fsFileExceptionX is base class for exceptions that refer to a fsLocator.
//============================================================================
class fsFileExceptionX : public envExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsFileExceptionX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual ~fsFileExceptionX() = 0;

  //--------------------------------------------------------------------
  //	GetLocator() returns the offending directory
  //--------------------------------------------------------------------
  const fsLocator& GetLocator() const;

  //--------------------------------------------------------------------
  //	GetLocatorString() returns locator as string for error messages
  //--------------------------------------------------------------------
  std::string GetLocatorString() const;

private:
  fsLocator m_Locator;
};

//============================================================================
//	fsDirectoryDoesntExistX is thrown when the user tries to do something
//	with a directory that doesn't exist.
//============================================================================
class fsDirectoryDoesntExistX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsDirectoryDoesntExistX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsDirectoryExistsX is thrown when the user tries to create a directory
//	that already exists.
//============================================================================
class fsDirectoryExistsX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsDirectoryExistsX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsDirectoryNotEmptyX is thrown when the user tries to delete a directory
//	which contains files or other directories.
//============================================================================
class fsDirectoryNotEmptyX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsDirectoryNotEmptyX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsFileDoesntExistX is thrown when the user tries to do something with
//	a file that doesn't exist.
//============================================================================
class fsFileDoesntExistX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsFileDoesntExistX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsFileExistsX is thrown when the user tries to create a file that already
//	exists.
//============================================================================
class fsFileExistsX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsFileExistsX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//----------------------------------------------------------------------------
//	fsFileInUseX is thrown when the user tries to operate on a file that is
//	already open (possibly by another process).
//----------------------------------------------------------------------------
class fsFileInUseX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsFileInUseX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsInvalidLocatorX is thrown when a locator contains characters that the
//	OS will not allow in a filename, or is flawed in some other way.
//	(too long?)
//============================================================================
class fsInvalidLocatorX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsInvalidLocatorX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//----------------------------------------------------------------------------
//	fsReadOnlyX is thrown when the user tries to write to a file marked as
//	read only.
//----------------------------------------------------------------------------
class fsReadOnlyX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsReadOnlyX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsDiskFullX is thrown when the user tries to write to a file
//	but there is not disk space left or the disk is corrupt
//============================================================================
class fsDiskFullX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsDiskFullX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//----------------------------------------------------------------------------
//	fsUnknownX is thrown when an error is not covered by the above exceptions.
//
//----------------------------------------------------------------------------
class fsUnknownX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsUnknownX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsXMLErrorX is thrown when an error reading an XML file occurs
//============================================================================
class fsXMLErrorX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsXMLErrorX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsNewerVersionAbortX is thrown when the user does not want to read in
//	a file with a newer version than their app supports
//============================================================================
class fsNewerVersionAbortX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsNewerVersionAbortX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsFileNotAllowedX is thrown when the file being read in is not allowed by
//	by the application.
//
//	For instance, if an application only allows files to come from Maya,
//	any files exported by another package would throw this error.
//============================================================================
class fsFileNotAllowedX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsFileNotAllowedX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};

//============================================================================
//	fsUnsupportedVersionX is thrown when the app reads a version of a file
//	that this app doesn't support.  (e.g. MSCore can't read in MSPro files.)
//
//	Note: Could fsFileNotAllowedX + fsUnsupportedVersionX be merged?
//============================================================================
class fsUnsupportedVersionX : public fsFileExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  fsUnsupportedVersionX(const fsLocator& i_Locator);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};
