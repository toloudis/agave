/****************************************************************************\
**  fsAbsolutePathMgr.hpp
**
**      fsAbsolutePathMgr supplies functions that handle absolute paths
**	on different systems, helping to resolve full paths that may
**	be on different drives.
**
**
**
\****************************************************************************/
#ifdef FS_ABSOLUTEPATHMGR_HPP
#error fsAbsolutePathMgr.hpp multiply included
#endif
#define FS_ABSOLUTEPATHMGR_HPP

#include <functional>
#include <string>

//============================================================================
//============================================================================
class fsLocator;
class itString;

//============================================================================
//============================================================================
namespace fsAbsolutePathMgr {
//------------------------------------------------------------------------
// Clear out old mappings
//------------------------------------------------------------------------
void
Clear();

//------------------------------------------------------------------------
// AddDirectoryMapping() - add a mapping between the two given
// absolute directories so that later files using the original path
// can be found in the new location.
//------------------------------------------------------------------------
void
AddDirectoryMapping(const fsLocator& i_OrigDirectory, const fsLocator& i_LocalDirectory);

//------------------------------------------------------------------------
// AddDirMappingFromFiles() - convenience function for creating a mapping
// using two full filenames. The filenames themselves will be
// stripped off and the directories will be given to AddDirectoryMapping.
//------------------------------------------------------------------------
void
AddDirMappingFromFiles(const fsLocator& i_OrigFilename, const fsLocator& i_LocalFilename);

//------------------------------------------------------------------------
// Store that the given filename was chosen as a replacement for the
// original filename so that we can make the same replacement again
// without asking. Also makes a directory mapping to help resolve
// future files in the same directories.
//------------------------------------------------------------------------
void
AddFileMapping(const fsLocator& i_OrigFilename, const fsLocator& i_LocalFilename);

//------------------------------------------------------------------------
// ResolvePath() - apply the directory mappings to the given filename
// in order to find the local filename to use. Returns true if the
// file was found.
//------------------------------------------------------------------------
bool
ResolvePath(const fsLocator& i_OrigFilename, fsLocator& o_LocalFilename);

//------------------------------------------------------------------------
// Variation of ResolvePath that alters input if necessary.
// This function will also call the application ResolveFunction
// defined below.
// Category allows you to skip assets by type and still warn
// about other missing assets.
// Returns true if the filename was found.
//------------------------------------------------------------------------
bool
ResolvePath(fsLocator& io_FullFilename, const std::string& i_Category);

//------------------------------------------------------------------------
// Typedef for a function provided by application for resolving
// paths when the mappings don't provide the solution.
//------------------------------------------------------------------------
typedef bool (*ResolveFunction)(const fsLocator&, fsLocator&, const std::string&);
//------------------------------------------------------------------------
// Typedef for a different function for path resolving for rendering
//------------------------------------------------------------------------
typedef bool (*ResolveFunction)(const fsLocator&, fsLocator&, const std::string&);

//------------------------------------------------------------------------
// Set application's function for resolving paths when mappings fail.
// It could, for instance, open a dialog and ask the user for
// a new path.
//------------------------------------------------------------------------
void
SetPathResolveFunction(ResolveFunction i_Function);

//------------------------------------------------------------------------
// Get the Path Resolve Function that currently used
//------------------------------------------------------------------------
ResolveFunction
GetPathResolveFunction();
}
