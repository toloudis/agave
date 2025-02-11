/*****************************************************************************
**  gfPaths.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/gf/gfPaths.hpp"

#include "Core/dbg/dbgMsg.hpp"
#include "Core/fs/fsLocator.hpp"
#include "Core/gf/gfPackage.hpp"
#include "Core/gf/gfPathsPAC.hpp"

#include <vector>

//============================================================================
//============================================================================
namespace gfPaths {
namespace {
typedef std::vector<fsLocator> PathList;
std::vector<PathList> l_Paths(gfPaths::e_FirstUserDefPath);
const int MAX_PATHS = 50;

// static array based on SubPathIndex, this is always constant
const char* lc_SubPaths[e_NumSubPaths] = { "Data", "Models", "Sounds", "Textures", "Models" }; //"Animation"};

int l_RefCount = 0;
}

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//------------------------------------------------------------------------
// void Init(const char* i_CDVolumeName)
void
Init()
{
  // use ref count to only initialize once
  if (l_RefCount == 0) {
    // gfPathsPAC::InitPaths(l_Paths, i_CDVolumeName);
    gfPathsPAC::InitPaths(l_Paths);
  }
  l_RefCount++;
}

void
CleanUp() throw()
{
  l_RefCount--;

  // use ref count to make sure we only clean up once, when everyone is done
  if (l_RefCount == 0) {
  }
}

//------------------------------------------------------------------------
//	GetNumPathsInList returns how many Paths are listed at the given i_PathIndex
//------------------------------------------------------------------------
int
GetNumPathsInList(int i_PathIndex)
{
  DBG_ASSERT(0 <= i_PathIndex && i_PathIndex < l_Paths.size(),
             "Attempt to access a path out of the current range of app paths, " << gfPaths::e_FirstUserDefPath << " to "
                                                                                << l_Paths.size());
  return l_Paths[i_PathIndex].size();
}

//------------------------------------------------------------------------
//	GetPathList retrieves the entire PathList at i_PathIndex
//------------------------------------------------------------------------
void
GetPathList(int i_PathIndex, std::vector<fsLocator>& o_PathList)
{
  DBG_ASSERT(0 <= i_PathIndex && i_PathIndex < l_Paths.size(),
             "Attempt to access a path out of the current range of app paths, " << gfPaths::e_FirstUserDefPath << " to "
                                                                                << l_Paths.size());
  o_PathList = l_Paths[i_PathIndex];
}

//------------------------------------------------------------------------
//	GetPath retrieves the app path for the supplied index. Returns either
//	the first paths in the list for the index, or the requested path if
//	i_PathListIndex is specified. Can also return the entire PathList
//------------------------------------------------------------------------
const fsLocator&
GetPath(int i_PathIndex, int i_PathListIndex /* = 0*/)
{
  DBG_ASSERT(((0 <= i_PathIndex) && (i_PathIndex < l_Paths.size()) && (0 <= i_PathListIndex) &&
              (i_PathListIndex < l_Paths[i_PathIndex].size())),
             "Attempt to access a path out of the current range of app paths, " << gfPaths::e_FirstUserDefPath << " to "
                                                                                << l_Paths.size());
  return l_Paths[i_PathIndex][i_PathListIndex];
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool
IsPathValid(int i_PathIndex)
{
  if ((i_PathIndex >= 0) && (i_PathIndex < l_Paths.size()) && (l_Paths[i_PathIndex].size() > 0))
    return true;
  return false;
}

//------------------------------------------------------------------------
//	AddPath appends the given path to the Path list at the given index
//------------------------------------------------------------------------
void
AddPath(int i_PathIndex, const fsLocator& i_Path)
{
  DBG_ASSERT(gfPaths::e_FirstUserDefPath <= i_PathIndex && i_PathIndex < MAX_PATHS,
             "Attempt to set a path out of the possible range of app paths, " << gfPaths::e_FirstUserDefPath << " to "
                                                                              << MAX_PATHS);

  //	resize the paths if necessary
  //
  if (l_Paths.size() <= i_PathIndex) {
    l_Paths.resize(i_PathIndex + 1);
  }

  //	append the path to the list
  //
  l_Paths[i_PathIndex].push_back(i_Path);
}

//------------------------------------------------------------------------
//	SetPath appends the given app path to the Path list at the given index.
//------------------------------------------------------------------------
void
SetPath(int i_PathIndex, const fsLocator& i_Path)
{
  DBG_ASSERT(gfPaths::e_FirstUserDefPath <= i_PathIndex && i_PathIndex < MAX_PATHS,
             "Attempt to set a path out of the current range of app paths, " << gfPaths::e_FirstUserDefPath << " to "
                                                                             << MAX_PATHS);

  //	erase the previous paths
  //
  if (l_Paths.size() > i_PathIndex) {
    l_Paths[i_PathIndex].erase(l_Paths[i_PathIndex].begin(), l_Paths[i_PathIndex].end());
  }

  //	add the new path
  //
  AddPath(i_PathIndex, i_Path);
}

//------------------------------------------------------------------------
//	SetAppPath sets the path to the data and art
//------------------------------------------------------------------------
void
SetAppPath(const fsLocator& i_Path)
{
  //	this path index only has one path to it.
  //
  l_Paths[gfPaths::e_AppPath].erase(l_Paths[gfPaths::e_AppPath].begin(), l_Paths[gfPaths::e_AppPath].end());
  l_Paths[gfPaths::e_AppPath].push_back(i_Path);
}

//------------------------------------------------------------------------
//	SetAppPath retrieves the app path for the supplied index.
//------------------------------------------------------------------------
const fsLocator&
GetAppPath()
{
  return l_Paths[gfPaths::e_AppPath][0];
}

//------------------------------------------------------------------------
//	SetCDPath searched drives for i_CDVolumeName and sets the path if found
//------------------------------------------------------------------------
// void SetCDPath( const char* i_CDVolumeName )
//{
//	gfPathsPAC::SetCDPath(l_Paths, i_CDVolumeName);
//}

//------------------------------------------------------------------------
//	Accessor for material library path
//------------------------------------------------------------------------
void
SetMaterialLibraryPath(const fsLocator& i_Path)
{
  PathList& path_list = l_Paths[gfPaths::e_MaterialLibrary];
  path_list.erase(path_list.begin(), path_list.end());
  path_list.push_back(i_Path);
}

//------------------------------------------------------------------------
//	ReIndex attempts to take i_Locator and reindex it with an index that
//	matches the most names in i_Locator, if no matching index can be found
//	o_ReIndexedLocator will be equivalent to i_Locator
//------------------------------------------------------------------------
void
ReIndex(const fsLocator& i_Locator, fsLocator& o_ReIndexedLocator)
{
  int i, j, k, num_names = i_Locator.GetNumNames(), get_name_at_index;
  std::vector<fsLocator> test_dirs;
  if (-1 == i_Locator.GetIndex()) {
    test_dirs.push_back(i_Locator);
  } else {
    GetPathList(i_Locator.GetIndex(), test_dirs);
    for (i = 0; i < test_dirs.size(); ++i) {
      test_dirs[i].Push(i_Locator);
    }
  }

  // only check until we end up back where we started w/ i_Locator
  while (num_names) {
    for (i = 0; i < l_Paths.size(); ++i) {
      for (j = 0; j < l_Paths[i].size(); ++j) {
        for (k = 0; k < test_dirs.size(); ++k) {
          if (l_Paths[i][j] == test_dirs[k]) {
            o_ReIndexedLocator.Clear();
            o_ReIndexedLocator.SetIndex(i);
            for (get_name_at_index = num_names; get_name_at_index < i_Locator.GetNumNames(); ++get_name_at_index) {
              o_ReIndexedLocator.Push(i_Locator.GetName(get_name_at_index));
            }
            return;
          }
        }
      }
    }

    for (i = 0; i < test_dirs.size(); ++i) {
      test_dirs[i].Pop();
    }
    --num_names;
  }

  o_ReIndexedLocator = i_Locator;
}

//------------------------------------------------------------------------
// ResetPath will replace the existing path at i_PathIndex with the
// given locator.  This will assert if the path does not exist.
//------------------------------------------------------------------------
void
ResetPath(int i_PathIndex, const fsLocator& i_Path, int i_PathListIndex /*=0*/)
{
  DBG_ASSERT(0 <= i_PathIndex && i_PathIndex < l_Paths.size() && 0 <= i_PathListIndex &&
               i_PathListIndex < l_Paths[i_PathIndex].size(),
             "Attempt to access a path out of the current range of app paths, " << gfPaths::e_FirstUserDefPath << " to "
                                                                                << l_Paths.size());

  l_Paths[i_PathIndex][i_PathListIndex] = i_Path;
}

//------------------------------------------------------------------------
// GetSubPath will return the const string that is the sub path associated
// with the given index
//------------------------------------------------------------------------
const char*
GetSubPath(SubPaths i_SubPath)
{
  DBG_ASSERT(i_SubPath < e_NumSubPaths, "Invalid sub path index!");

  return lc_SubPaths[i_SubPath];
}

//------------------------------------------------------------------------
//	Set the user data path where the user data will be stored.
//	It could be installed under user/My Documents or all users/Documents
//	on Windows.
//	Under one of these types of folders will be appended the sub-folders
//	passed into this function.
//	AllenInstitute/agave (for example).
//------------------------------------------------------------------------
void
SetUserDataSubPath(fsLocator& i_UDSubPath)
{
  gfPathsPAC::SetUserDataSubPath(i_UDSubPath);
}

}
