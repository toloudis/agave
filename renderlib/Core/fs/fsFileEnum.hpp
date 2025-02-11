/****************************************************************************\
**  fsFileEnum.hpp
**
**      fsFileEnum.hpp supplies functions that enumerate files and
**	directories.
**
**
**
\****************************************************************************/

#ifdef FS_FILEENUM_HPP
#error fsFileEnum.hpp multiply included
#endif
#define FS_FILEENUM_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#include <vector>

//============================================================================
//============================================================================
class itString;
class fsLocator;

//============================================================================
//============================================================================
namespace fsFileEnum {

typedef std::vector<fsLocator> fsFileList;

//========================================================================
//	EnumTarget should be inherited from by classes which wish to have
//	the files or subdirectories in a directory enumerated to them.
//========================================================================
class EnumTarget
{
public:
  //----------------------------------------------------------------
  //	Child classes should override the Notify function to receive
  //	the list of files or directories.  The listing is not
  //	recursive.  The locator "i_File" is a relative locator
  //	which should have the input directory prepended to it to
  //	form the full locator of the file or directory being
  //	enumerated.  The object should return false if it wants
  //	to stop the enumeration, otherwise it should return true.
  //	i_Directory is the the directory being enumerated over
  //----------------------------------------------------------------
  virtual bool Notify(const fsLocator& i_Directory, const fsLocator& i_File) = 0;
};

//============================================================================
//	FileEnumTarget is a fsLocator in a list of files.  This is meant to be a
//	helper class for the EnumerateFiles function.
//============================================================================
class FileListTarget : public fsFileEnum::EnumTarget
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  FileListTarget();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  ~FileListTarget();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  const fsFileList& GetList() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual bool Notify(const fsLocator& i_Directory, const fsLocator& i_File);

private:
  fsFileList m_FileList;
};

//========================================================================
//	EnumerateFiles lists all of the files in a directory.  If i_SubStrings
//	is not empty, only files whose names contain one of the given substrings
//	will be listed. Returns the last return value from fsEnumTarget::Notify
//========================================================================
bool
EnumerateFiles(const fsLocator& i_Locator, EnumTarget& o_Target, bool i_bSearchSubdirectories = false);
bool
EnumerateFiles(const fsLocator& i_Locator,
               EnumTarget& o_Target,
               const itString& i_SubString,
               bool i_bSearchSubdirectories = false);
bool
EnumerateFiles(const fsLocator& i_Locator,
               EnumTarget& o_Target,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories = false);

//========================================================================
//	EnumerateFiles uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//========================================================================
bool
EnumerateFiles(const fsLocator& i_Locator, fsFileList& o_FileList, bool i_bSearchSubdirectories = false);
bool
EnumerateFiles(const fsLocator& i_Locator,
               fsFileList& o_FileList,
               const itString& i_SubString,
               bool i_bSearchSubdirectories = false);
bool
EnumerateFiles(const fsLocator& i_Locator,
               fsFileList& o_FileList,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories = false);

//========================================================================
//	EnumerateDirectories lists all of the subdirectories in a directory.
//	Returns the last return value from fsEnumTarget::Notify
//========================================================================
bool
EnumerateDirectories(const fsLocator& i_Locator, EnumTarget& o_Target);

//========================================================================
//	EnumerateDirectories uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//========================================================================
bool
EnumerateDirectories(const fsLocator& i_Locator, fsFileList& o_DirList);

}
