/****************************************************************************\
**  gfFileEnum.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/fs/fsLocator.hpp"
#include "Core/gf/gfFileEnum.hpp"

#include "Core/gf/gfPaths.hpp"

//============================================================================
//============================================================================
namespace gfFileEnum {

//------------------------------------------------------------------------
//	EnumerateFiles lists all of the files in a directory. If i_SubStrings
//	is not empty, only files whose names contain one of the given substrings
//	will be listed. Ensures that i_Locator is properly expanded by the
//	gfFileTranslationMgr and all paths associated w/ i_Locator's index (if any)
//	get enumerated
//------------------------------------------------------------------------
void
EnumerateFiles(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target, bool i_bSearchSubdirectories)
{
  std::vector<itString> no_substrings;
  gfFileEnum::EnumerateFiles(i_Locator, o_Target, no_substrings, i_bSearchSubdirectories);
}
void
EnumerateFiles(const fsLocator& i_Locator,
               fsFileEnum::EnumTarget& o_Target,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories)
{
  if (!i_Locator.HasIndex()) {
    fsFileEnum::EnumerateFiles(i_Locator, o_Target, i_SubStrings, i_bSearchSubdirectories);
  } else {
    std::vector<fsLocator> PathList;
    gfPaths::GetPathList(i_Locator.GetIndex(), PathList);
    int i;
    fsLocator locator;
    for (i = 0; i < PathList.size(); ++i) {
      locator = PathList[i];
      locator.Push(i_Locator);
      if (!fsFileEnum::EnumerateFiles(locator, o_Target, i_SubStrings, i_bSearchSubdirectories)) {
        // false means we should stop enumerating, so return out of this
        return;
      }
    }
  }
}

//------------------------------------------------------------------------
//	EnumerateFiles uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//------------------------------------------------------------------------
void
EnumerateFiles(const fsLocator& i_Locator, fsFileEnum::fsFileList& o_FileList, bool i_bSearchSubdirectories)
{
  std::vector<itString> no_substrings;
  gfFileEnum::EnumerateFiles(i_Locator, o_FileList, no_substrings, i_bSearchSubdirectories);
}
void
EnumerateFiles(const fsLocator& i_Locator,
               fsFileEnum::fsFileList& o_FileList,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories)
{
  fsFileEnum::FileListTarget FileTarget;
  gfFileEnum::EnumerateFiles(i_Locator, FileTarget, i_SubStrings, i_bSearchSubdirectories);
  o_FileList = FileTarget.GetList();
}

//------------------------------------------------------------------------
//	EnumerateDirectories lists all of the subdirectories in a directory.
//	Ensures that i_Locator is properly expanded by the gfFileTranslationMgr
//	and all paths associated w/ i_Locator's index (if any)
//	get enumerated
//------------------------------------------------------------------------
void
EnumerateDirectories(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target)
{
  if (!i_Locator.HasIndex()) {
    fsFileEnum::EnumerateDirectories(i_Locator, o_Target);
  } else {
    std::vector<fsLocator> PathList;
    gfPaths::GetPathList(i_Locator.GetIndex(), PathList);
    int i;
    fsLocator locator;
    for (i = 0; i < PathList.size(); ++i) {
      locator = PathList[i];
      locator.Push(i_Locator);
      if (!fsFileEnum::EnumerateDirectories(locator, o_Target)) {
        // false means we should stop enumerating, so return out of this
        return;
      }
    }
  }
}

//------------------------------------------------------------------------
//	EnumerateDirectories uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//------------------------------------------------------------------------
void
EnumerateDirectories(const fsLocator& i_Locator, fsFileEnum::fsFileList& o_DirList)
{
  fsFileEnum::FileListTarget FileTarget;
  gfFileEnum::EnumerateDirectories(i_Locator, FileTarget);
  o_DirList = FileTarget.GetList();
}

}
