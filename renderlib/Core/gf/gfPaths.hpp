/*****************************************************************************
**  gfPaths.hpp
**
**      gfPaths is a namespace resonsible for storing and retrieving useful
**		paths as a series of vectors of fsLocators.
**
**
**
\****************************************************************************/

#ifdef GF_PATHS_HPP
#error gfPaths.hpp multiply included
#endif
#define GF_PATHS_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#include <vector>

//============================================================================
//	Forward references
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
namespace gfPaths {
//------------------------------------------------------------------------
//	Enumeration of standardized app paths available. The final Path,
//	e_FirstUserDefPath, is a convenience for the client to use as a base for
//	their own indices for retrieving their own
//	Paths.
//------------------------------------------------------------------------
enum Paths
{
  e_ExePath = 0, // where the EXE is launched from
  e_CDROMPath,
  e_SystemTempPath,
  e_LocalSystemDataPath, /// app data in the local user directory
  e_SystemDataPath,      // app data root directory
  e_AppPath,             // where the app (the "game/app/movie/etc.")
  e_MaterialLibrary,     // root directory of material library
  e_UserDataPath,        // (local settings/app data/...)
  e_UserDocumentsPath,   // "my documents" or "shared documents"
  e_CommonDocumentsPath, // "public documents"
  // e_UserLocalDocumentsPath,	// "my documents"
  e_FirstUserDefPath,
  e_MabPath // path for mab file
};

//------------------------------------------------------------------------
// Enumeration of standardized subpaths for objects / props / etc. that are
// loaded in a game.  These are always under a type directory such as
// PopsStat\Common, for example.
//------------------------------------------------------------------------
enum SubPaths
{
  e_Data = 0,
  e_Models,
  e_Sounds,
  e_Textures,
  e_Animation,
  e_NumSubPaths
};

//------------------------------------------------------------------------
//	GetNumPathsInList returns how many Paths are listed at the given i_PathIndex
//------------------------------------------------------------------------
int
GetNumPathsInList(int i_PathIndex);

//------------------------------------------------------------------------
//	GetPathList retrieves the entire PathList at i_PathIndex
//------------------------------------------------------------------------
void
GetPathList(int i_PathIndex, std::vector<fsLocator>& o_PathList);

//------------------------------------------------------------------------
//	GetPath retrieves the path for the supplied index. Returns either
//	the first paths in the list for the index, or the requested path if
//	i_PathListIndex is specified. Can also return the entire PathList
//------------------------------------------------------------------------
const fsLocator&
GetPath(int i_PathIndex, int i_PathListIndex = 0);
bool
IsPathValid(int i_PathIndex);

//------------------------------------------------------------------------
//	AddPath appends the given path to the Path list at the given index
//------------------------------------------------------------------------
void
AddPath(int i_PathIndex, const fsLocator& i_Path);

//------------------------------------------------------------------------
//	SetPath sets the given path to the Path list at the given index.
//	it will replace all other paths at that index.
//------------------------------------------------------------------------
void
SetPath(int i_PathIndex, const fsLocator& i_Path);

//------------------------------------------------------------------------
//	Specific accessors for specific paths
//------------------------------------------------------------------------
void
SetAppPath(const fsLocator& i_Path);
const fsLocator&
GetAppPath();

//------------------------------------------------------------------------
//	SetCDPath searched drives for i_CDVolumeName and sets the path if found
//------------------------------------------------------------------------
// void SetCDPath( const char* i_CDVolumeName );

//------------------------------------------------------------------------
//	Accessor for material library path
//------------------------------------------------------------------------
void
SetMaterialLibraryPath(const fsLocator& i_Path);

//------------------------------------------------------------------------
//	ReIndex attempts to take i_Locator and reindex it with an index that
//	matches the most names in i_Locator, if no matching index can be found
//	o_ReIndexedLocator will be equivalent to i_Locator
//------------------------------------------------------------------------
void
ReIndex(const fsLocator& i_Locator, fsLocator& o_ReIndexedLocator);

//------------------------------------------------------------------------
// ResetPath will replace the existing path at i_PathIndex with the
// given locator.  This will assert if the path does not exist.
//------------------------------------------------------------------------
void
ResetPath(int i_PathIndex, const fsLocator& i_Path, int i_PathListIndex = 0);

//------------------------------------------------------------------------
// GetSubPath will return the const string that is the sub path associated
// with the given index
//------------------------------------------------------------------------
const char*
GetSubPath(SubPaths i_SubPath);

//------------------------------------------------------------------------
//	Set the user data path where the user data will be stored.
//	It could be installed under user/My Documents or all users/Documents
//	on Windows.
//	Under one of these types of folders will be appended the sub-folders
//	passed into this function.
//	AllenInstitute/agave (for example).
//------------------------------------------------------------------------
void
SetUserDataSubPath(fsLocator& i_UDSubPath);

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//------------------------------------------------------------------------
// void Init(const char* i_CDVolumeName = NULL);
void
Init();
void
CleanUp() throw();
}
