/*****************************************************************************
**  gfFileTranslationMgr.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/gf/gfFileTranslationMgr.hpp"

#include "Core/fs/fsFileX.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/gf/gfPaths.hpp"

//============================================================================
//============================================================================
namespace gfFileTranslationMgr {

namespace {
gfFileTranslator* l_FileTranslator = NULL;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
gfFileTranslator::gfFileTranslator() {}

gfFileTranslator::~gfFileTranslator() {}

//------------------------------------------------------------------------
//	*** NOTE: this also checks gfPakSearcher::FileExists as a convenience ***
//	FileExists returns true if the file can be found in the correct
//	location (either pak files or on disk, depending on the decision
//	of the PakLocator).
//------------------------------------------------------------------------
bool
FileExists(const fsLocator& i_Locator)
{
  if (i_Locator.HasIndex()) {
    if (l_FileTranslator) {
      try {
        fsLocator TranslatedFile;
        l_FileTranslator->TranslateFile(i_Locator, TranslatedFile);
        return true;
      } catch (const fsFileDoesntExistX& i_Ex) {
        i_Ex;
      } catch (const fsDirectoryDoesntExistX& i_Ex) {
        i_Ex;
      }
    } else {
      std::vector<fsLocator> PathList;
      gfPaths::GetPathList(i_Locator.GetIndex(), PathList);
      int i;
      fsLocator Tester;

      for (i = 0; i < PathList.size(); ++i) {
        Tester = PathList[i];
        Tester.Push(i_Locator);
        try {
          if (fsFileUtil::FileExists(Tester)) {
            return true;
          }
        } catch (const fsDirectoryDoesntExistX& i_Ex) {
          // catch the missing directory exception, don't have to do anything, just let for loop go on
          i_Ex;
        }
      }
    }
  }

  return fsFileUtil::FileExists(i_Locator);
}

//------------------------------------------------------------------------
//	DirectoryExists returns true if the directory can be found among the
//	possible paths based on the index (if any)
//------------------------------------------------------------------------
bool
DirectoryExists(const fsLocator& i_Locator)
{
  if (!i_Locator.HasIndex()) {
    return fsFileUtil::DirectoryExists(i_Locator);
  } else if (l_FileTranslator) {
    try {
      fsLocator TranslatedFile;
      l_FileTranslator->TranslateFile(i_Locator, TranslatedFile);
      return true;
    } catch (const fsFileDoesntExistX& i_Ex) {
      i_Ex;
    } catch (const fsDirectoryDoesntExistX& i_Ex) {
      i_Ex;
    }

    return false;
  } else {
    std::vector<fsLocator> PathList;
    gfPaths::GetPathList(i_Locator.GetIndex(), PathList);
    int i;
    fsLocator Tester;

    for (i = 0; i < PathList.size(); ++i) {
      Tester = PathList[i];
      Tester.Push(i_Locator);
      try {
        if (fsFileUtil::DirectoryExists(Tester)) {
          return true;
        }
      } catch (const fsDirectoryDoesntExistX& i_Ex) {
        // catch the missing directory exception, don't have to do anything, just let for loop go on
        i_Ex;
      }
    }

    return false;
  }
}

//------------------------------------------------------------------------
//	ExpandLocator causes the given locator to be transformed to it's
//	full path representation (removes the index).
//------------------------------------------------------------------------
void
ExpandLocator(fsLocator& io_Locator)
{
  if (io_Locator.HasIndex()) {
    fsLocator Tester;

    if (l_FileTranslator) {
      try {
        l_FileTranslator->TranslateFile(io_Locator, Tester);
        io_Locator = Tester;
        return;
      } catch (const fsFileDoesntExistX& i_Ex) {
        i_Ex;
      } catch (const fsDirectoryDoesntExistX& i_Ex) {
        i_Ex;
      }
    } else {
      std::vector<fsLocator> PathList;
      gfPaths::GetPathList(io_Locator.GetIndex(), PathList);
      int i;

      for (i = 0; i < PathList.size(); ++i) {
        Tester = PathList[i];
        Tester.Push(io_Locator);
        try {
          if (fsFileUtil::FileExists(Tester)) {
            io_Locator = Tester;
            return;
          }
        } catch (const fsDirectoryDoesntExistX& i_Ex) {
          // catch the missing directory exception, don't have to do anything, just let for loop go on
          i_Ex;
        }
      }
    }

    // if we couldn't find a path on the list that the file exists at, just dump the first one in
    Tester = gfPaths::GetPath(io_Locator.GetIndex());
    Tester.Push(io_Locator);
    io_Locator = Tester;
  }
}

//------------------------------------------------------------------------
//	SetFileTranslator sets the FileTranslator object which is used by the
//	gfFileTranslationMgr to translate locators.  The gfFileTranslationMgr
//	does not own the FileTranslator; you must ensure that it is destroyed yourself.
//------------------------------------------------------------------------
void
SetFileTranslator(gfFileTranslator* i_FileTranslator)
{
  l_FileTranslator = i_FileTranslator;
}

}
