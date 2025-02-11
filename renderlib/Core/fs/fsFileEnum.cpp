/****************************************************************************\
**  fsFileEnum.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsFileEnum.hpp"

#include "Core/fs/fsLocator.hpp"
#include "Core/fs/fsFileEnumPAC.hpp"

//============================================================================
//============================================================================
namespace fsFileEnum {
FileListTarget::FileListTarget()
{
  m_FileList.clear();
}
FileListTarget::~FileListTarget() {}
const fsFileList&
FileListTarget::GetList() const
{
  return m_FileList;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool
FileListTarget::Notify(const fsLocator& i_Directory, const fsLocator& i_File)
{
  fsLocator Locator = i_Directory;
  Locator.Push(i_File);
  m_FileList.push_back(Locator);
  return true;
}

//----------------------------------------------------------------------------
//	EnumerateFiles lists all of the files in a directory.  If i_SubString
//	is not NULL, only files whose names contain the given substring
//	will be listed. Returns the last return value from fsEnumTarget::Notify
//----------------------------------------------------------------------------
bool
EnumerateFiles(const fsLocator& i_Locator, EnumTarget& o_Target, bool i_bSearchSubdirectories)
{
  std::vector<itString> no_substrings;
  return EnumerateFiles(i_Locator, o_Target, no_substrings, i_bSearchSubdirectories);
}
bool
EnumerateFiles(const fsLocator& i_Locator,
               EnumTarget& o_Target,
               const itString& i_SubString,
               bool i_bSearchSubdirectories)
{
  std::vector<itString> substrings;
  substrings.push_back(i_SubString);
  return EnumerateFiles(i_Locator, o_Target, substrings, i_bSearchSubdirectories);
}
bool
EnumerateFiles(const fsLocator& i_Locator,
               EnumTarget& o_Target,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories)
{
  // Search files in the root directory first
  bool continue_enum = fsFileEnumPAC::EnumerateFiles(i_Locator, o_Target, i_SubStrings);

  // If we should search subdirectories and the target wants to continue looking,
  // enumerate the directories and then search each of those
  if (i_bSearchSubdirectories && continue_enum) {
    fsFileList dirList;
    EnumerateDirectories(i_Locator, dirList);
    fsFileList::const_iterator it;
    for (it = dirList.begin(); it != dirList.end(); ++it) {
      itString last_name = it->GetLastName();
      if ((last_name != itString(".")) && (last_name != itString(".."))) {
        // std::string filename;
        // fsFileUtil::LocatorToANSIFilename(*it, filename);
        // DBG_LOG("Searching: " << filename.c_str());

        continue_enum = EnumerateFiles(*it, o_Target, i_SubStrings, i_bSearchSubdirectories);
        if (!continue_enum)
          return false;
      }
    }
  }

  return continue_enum;
}

//----------------------------------------------------------------------------
//	EnumerateFiles uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//----------------------------------------------------------------------------
bool
EnumerateFiles(const fsLocator& i_Locator, fsFileList& o_FileList, bool i_bSearchSubdirectories)
{
  std::vector<itString> no_substrings;
  return EnumerateFiles(i_Locator, o_FileList, no_substrings, i_bSearchSubdirectories);
}
bool
EnumerateFiles(const fsLocator& i_Locator,
               fsFileList& o_FileList,
               const itString& i_SubString,
               bool i_bSearchSubdirectories)
{
  std::vector<itString> substrings;
  substrings.push_back(i_SubString);
  return EnumerateFiles(i_Locator, o_FileList, substrings, i_bSearchSubdirectories);
}
bool
EnumerateFiles(const fsLocator& i_Locator,
               fsFileList& o_FileList,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories)
{
  FileListTarget FileTarget;
  bool bResult = EnumerateFiles(i_Locator, FileTarget, i_SubStrings, i_bSearchSubdirectories);

  if (bResult)
    o_FileList = FileTarget.GetList();

  return bResult;
}

//----------------------------------------------------------------------------
//	EnumerateDirectories lists all of the subdirectories in a directory.
//	Returns the last return value from fsEnumTarget::Notify
//----------------------------------------------------------------------------
bool
EnumerateDirectories(const fsLocator& i_Locator, EnumTarget& o_Target)
{
  return fsFileEnumPAC::EnumerateDirectories(i_Locator, o_Target);
}

//----------------------------------------------------------------------------
//	EnumerateDirectories uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//----------------------------------------------------------------------------
bool
EnumerateDirectories(const fsLocator& i_Locator, fsFileList& o_DirList)
{
  FileListTarget FileTarget;
  bool bResult = EnumerateDirectories(i_Locator, FileTarget);

  if (bResult)
    o_DirList = FileTarget.GetList();

  return bResult;
}

}
