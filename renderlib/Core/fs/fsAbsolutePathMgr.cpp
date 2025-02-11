/****************************************************************************\
**  fsAbsolutePathMgr.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsAbsolutePathMgr.hpp"

#include "Core/dbg/dbgMsg.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/fs/fsLocator.hpp"
#include "Core/it/itStringUtil.hpp"
#include "Core/Ma/maFunctions.hpp"

#if ENV_WINDOWS
#include <shlobj.h> // for SHGetFolderPath
#endif

#include <map>
#include <set>
#include <string>

//============================================================================
//============================================================================
namespace fsAbsolutePathMgr {
namespace {
const bool c_bDebugMappings = false;
std::map<fsLocator, fsLocator> l_DriveMappings;
std::set<fsLocator> l_LocalDirs;
std::map<fsLocator, fsLocator> l_FileMappings;
std::set<fsLocator> l_BadUNCDirs;
ResolveFunction l_ResolveFunction = NULL;

//--------------------------------------------------------------------
// see if the given locator starts with the given prefix
//--------------------------------------------------------------------
bool
begins_with(const fsLocator& i_Locator, const fsLocator& i_Prefix)
{
  // Special case for finding filenames without any path,
  // There is one directory mapping reserved for this,
  // i_Prefix will be fsLocator(). This should only
  // match when i_Locator is a single filename.
  if ((i_Prefix.GetNumNames() == 0) && (i_Locator.GetNumNames() == 1))
    return true;

  // The Prefix must be shorter than the locator in order to match
  if ((i_Locator.GetNumNames() == 0) || (i_Prefix.GetNumNames() == 0) ||
      (i_Locator.GetNumNames() < i_Prefix.GetNumNames()))
    return false;

  // See if all of the prefix is within the start of
  // the given locator.
  for (int i = 0; i < i_Prefix.GetNumNames(); ++i) {
    if (!itStringUtil::Equal(i_Locator.GetName(i), i_Prefix.GetName(i)))
      return false;
  }
  return true;
}

// bga - Need better way to do this. Problem is that gfPaths appends extra subpaths
//  to the user data directory and I just want the root paths.
#if ENV_WINDOWS
//------------------------------------------------------------------------
//	Get the user's "My Documents" directory for storing data.
//	In Windows XP this is the "Documents and Settings/USER_NAME/My Documents" folder
//	In Windows Vista this is the "\Users\USER_NAME\Documents" folder
//------------------------------------------------------------------------
void
GetUserDocumentsDirectory(fsLocator& o_Locator)
{
  o_Locator.Clear();
  TCHAR path[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);
  fsFileUtil::UnicodeStringToLocator(itString(path), o_Locator);
}

//------------------------------------------------------------------------
//	Get the "All Users" "Shared Documents" directory.
//	In Windows XP this is the "Documents and Settings/All Users/Documents" folder
//	In Windows Vista this is the "\Users\Public\Documents" folder
//------------------------------------------------------------------------
void
GetPublicDocumentsDirectory(fsLocator& o_Locator)
{
  o_Locator.Clear();
  TCHAR path[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path);
  fsFileUtil::UnicodeStringToLocator(itString(path), o_Locator);
}
#endif // ENV_WINDOWS

//------------------------------------------------------------------------
// Look for standard windows "My Documents" directories that
// include a user name or "All Users".
//------------------------------------------------------------------------
bool
check_standard_directories(const fsLocator& i_OrigFilename, fsLocator& o_LocalFilename)
{
  // The standard directories we are looking for here are:
  // Windows XP:
  //	c:\Documents and Settings\All Users\Documents
  //	c:\Documents and Settings\<user>\My Documents
  // Vista and Windows 7:
  //	c:\Users\Public\Documents
  //	c:\Users\<user>\Documents
  //
  // If the original filename has this directory pattern,
  // then look in the local standard directories for the
  // file also. This helps locate material library mappings
  // automatically.
#if ENV_WINDOWS
  if (i_OrigFilename.GetNumNames() > 4) {
    const int c_UserRootIndex = 1; // second string in locator
    const itString c_UserRootXP("Documents and Settings");
    const itString c_UserRootVista("Users");
    const int c_DocumentIndex = 3; // fourth string in locator
    const itString c_MyDocumentsXP("My Documents");
    const itString c_DocumentsXP("Documents");
    const itString c_DocumentsVista("Documents");

    bool bHaveStandardDirectory = false;
    itString user_root = i_OrigFilename.GetName(c_UserRootIndex);
    if (user_root == c_UserRootVista) {
      if (i_OrigFilename.GetName(c_DocumentIndex) == c_DocumentsVista)
        bHaveStandardDirectory = true;
    } else if (user_root == c_UserRootXP) {
      itString docs_dir = i_OrigFilename.GetName(c_DocumentIndex);
      if ((docs_dir == c_MyDocumentsXP) || (docs_dir == c_DocumentsXP))
        bHaveStandardDirectory = true;
    }

    if (bHaveStandardDirectory) {
      fsLocator rest_of_path = i_OrigFilename;
      rest_of_path.RemoveBefore(4);

      fsLocator user_docs, public_docs;

      // Attempt local users directory
      GetUserDocumentsDirectory(user_docs);
      fsLocator attempt_loc = user_docs;
      attempt_loc.Push(rest_of_path);
      if (fsFileUtil::FileExists(attempt_loc)) {
        o_LocalFilename = attempt_loc;
        return true;
      }

      // Attempt public users directory
      GetPublicDocumentsDirectory(public_docs);
      attempt_loc = public_docs;
      attempt_loc.Push(rest_of_path);
      if (fsFileUtil::FileExists(attempt_loc)) {
        o_LocalFilename = attempt_loc;
        return true;
      }
    }
  }
#endif // ENV_WINDOWS

  return false;
}
}

//------------------------------------------------------------------------
// Clear out old mappings
//------------------------------------------------------------------------
void
Clear()
{
  l_DriveMappings.clear();
  l_LocalDirs.clear();
  l_FileMappings.clear();
  l_BadUNCDirs.clear();

  if (c_bDebugMappings) {
    DBG_LOG("Clearing directory mappings");
  }
}

//------------------------------------------------------------------------
// AddDirectoryMapping() - add a mapping between the two given
// absolute directories so that later files using the original path
// can be found in the new location.
//------------------------------------------------------------------------
void
AddDirectoryMapping(const fsLocator& i_OrigDirectory, const fsLocator& i_LocalDirectory)
{
  fsLocator orig_dir(i_OrigDirectory), local_dir(i_LocalDirectory);

  // Always add the full local directory given here to the
  // local directory set. This makes sure that no one has to specify
  // a given local directory more than once, even if the mappings
  // have changed (a flattened hierarchy). And because each file
  // load goes through AddDirectoryMapping() with the local directory
  // set from the original file location, then we get a natural
  // search in the same directory for other assets.
  l_LocalDirs.insert(i_LocalDirectory);

  const int num_to_compare = maFunctions::Lowest(orig_dir.GetNumNames(), local_dir.GetNumNames());
  for (int i = 0; i < num_to_compare; i++) {
    if (itStringUtil::Equal(orig_dir.GetLastName(), local_dir.GetLastName())) {
      // If the paths match at the end, pop off the parts that are shared
      // in order to find the stems that are different
      orig_dir.Pop();
      local_dir.Pop();
    } else
      break;
  }

  // It is okay for orig_dir to be empty now, but there must be a path to map to
  // within the local_dir in order to store it.
  if (local_dir.GetNumNames() > 0) {
    // Found a difference in the absolute paths, add this to the map
    if (c_bDebugMappings) {
      DBG_LOG("Mapping orig dir: " << orig_dir << " to local dir: " << local_dir);
    }

    l_DriveMappings[orig_dir] = local_dir;
  }
}

//------------------------------------------------------------------------
// AddDirMappingFromFiles() - convenience function for creating a mapping
// using two full filenames. The filenames themselves will be
// stripped off and the directories will be given to AddDirectoryMapping.
//------------------------------------------------------------------------
void
AddDirMappingFromFiles(const fsLocator& i_OrigFilename, const fsLocator& i_LocalFilename)
{
  if (i_OrigFilename.GetNumNames() > 0 && i_LocalFilename.GetNumNames() > 0) {
    // Strip off filenames and submit directories
    fsLocator orig_dir(i_OrigFilename), local_dir(i_LocalFilename);
    orig_dir.Pop();
    local_dir.Pop();
    AddDirectoryMapping(orig_dir, local_dir);
  }
}

//------------------------------------------------------------------------
// Store that the given filename was chosen as a replacement for the
// original filename so that we can make the same replacement again
// without asking. Also makes a directory mapping to help resolve
// future files in the same directories.
//------------------------------------------------------------------------
void
AddFileMapping(const fsLocator& i_OrigFilename, const fsLocator& i_LocalFilename)
{
  // Must be at least a filename for the original and
  // the local filename has to have some sort of directory to it.
  if (i_OrigFilename.GetNumNames() > 0 && i_LocalFilename.GetNumNames() > 1) {
    if (!itStringUtil::Equal(i_OrigFilename.GetLastName(), i_LocalFilename.GetLastName())) {
      // Add a mapping for the filename itself so that we can answer
      // the same mapping again without asking the user
      l_FileMappings[i_OrigFilename] = i_LocalFilename;
    }

    // Strip off filenames and submit directories
    fsLocator orig_dir(i_OrigFilename), local_dir(i_LocalFilename);
    orig_dir.Pop();
    local_dir.Pop();
    AddDirectoryMapping(orig_dir, local_dir);
  }
}

//------------------------------------------------------------------------
// ResolvePath() - apply the directory mappings to the given filename
// in order to find the local filename to use. Returns true if the
// file was found.
//------------------------------------------------------------------------
bool
ResolvePath(const fsLocator& i_OrigFilename, fsLocator& o_LocalFilename)
{
  bool bAttemptFileExists = true;
  if ((!l_BadUNCDirs.empty()) && i_OrigFilename.IsUNCPath()) {
    // With UNC files, it can be expensive to look if a file exists.
    // So, if we have had some problems finding a UNC path, keep
    // track of the directories that didn't exist and search them
    // before looking for the actual filename.
    fsLocator unc_dir = i_OrigFilename;
    unc_dir.Pop();

    // If we already looked for this directory and it didn't exist
    // then skip the attempt to find the file by name.
    if (l_BadUNCDirs.find(unc_dir) != l_BadUNCDirs.end())
      bAttemptFileExists = false;
  }

  // If we have passed the UNC directory test, then look for the
  // file itself.
  if (bAttemptFileExists) {
    // First try to find the file in the original location
    if (fsFileUtil::FileExists(i_OrigFilename)) {
      o_LocalFilename = i_OrigFilename;
      return true;
    } else if (i_OrigFilename.IsUNCPath()) {
      // Check to see if the directory exists at all
      fsLocator unc_dir = i_OrigFilename;
      unc_dir.Pop();
      if (!fsFileUtil::DirectoryExists(unc_dir))
        l_BadUNCDirs.insert(unc_dir);
    }
  }

  // First try the specific filename mappings that we have been given
  std::map<fsLocator, fsLocator>::const_iterator it;
  it = l_FileMappings.find(i_OrigFilename);
  if (it != l_FileMappings.end()) {
    o_LocalFilename = it->second;
    if (c_bDebugMappings) {
      DBG_LOG("Mapping filename: " << i_OrigFilename << " to: " << o_LocalFilename);
    }
    return true;
  }

  // Try the directory mappings that we have been given
  for (it = l_DriveMappings.begin(); it != l_DriveMappings.end(); ++it) {
    if (begins_with(i_OrigFilename, it->first)) {
      fsLocator locate_attempt = it->second;
      for (int i = it->first.GetNumNames(); i < i_OrigFilename.GetNumNames(); i++) {
        locate_attempt.Push(i_OrigFilename.GetName(i));
      }

      // Only continue if this mapping found the filename
      if (fsFileUtil::FileExists(locate_attempt)) {
        o_LocalFilename = locate_attempt;
        if (c_bDebugMappings) {
          DBG_LOG("Mapping path: " << i_OrigFilename << " to local file: " << o_LocalFilename);
        }
        return true;
      }
    }
  }

  // Then make a pass through the local directories, looking only for
  // the given filename in the specified directories. This way of searching
  // will handle "flattened" asset directories where all assets are in the
  // same directory as the scene file.
  itString filename = i_OrigFilename.GetLastName();
  std::set<fsLocator>::const_iterator set_it;
  for (set_it = l_LocalDirs.begin(); set_it != l_LocalDirs.end(); ++set_it) {
    fsLocator locate_attempt = (*set_it);
    locate_attempt.Push(filename);

    if (fsFileUtil::FileExists(locate_attempt)) {
      o_LocalFilename = locate_attempt;
      if (c_bDebugMappings) {
        DBG_LOG("Mapping path: " << i_OrigFilename << " to local file: " << o_LocalFilename);
      }
      return true;
    }
  }

  // Look for standard windows "My Documents" directories that
  // include a user name or "All Users".
  if (check_standard_directories(i_OrigFilename, o_LocalFilename)) {
    // Submit the mapping so that we don't have to search standard
    // directories too often.
    AddFileMapping(i_OrigFilename, o_LocalFilename);
    if (c_bDebugMappings) {
      DBG_LOG("Standard directory mapping: " << i_OrigFilename << " to local file: " << o_LocalFilename);
    }
    return true;
  }

  return false;
}

//------------------------------------------------------------------------
// Variation of ResolvePath that alters input if necessary.
// Category allows you to skip assets by type and still warn
// about other missing assets.
// Returns true if the filename was found.
//------------------------------------------------------------------------
bool
ResolvePath(fsLocator& io_FullFilename, const std::string& i_Category)
{

  fsLocator new_name;
  if (ResolvePath(io_FullFilename, new_name)) {
    io_FullFilename = new_name;
    return true;
  }

  if (l_ResolveFunction != NULL) {
    if ((*l_ResolveFunction)(io_FullFilename, new_name, i_Category)) {
      io_FullFilename = new_name;
      return true;
    }
  }

  return false;
}

//------------------------------------------------------------------------
// Set application's function for resolving paths when mappings fail.
// It could, for instance, open a dialog and ask the user for
// a new path.
//------------------------------------------------------------------------
void
SetPathResolveFunction(ResolveFunction i_Function)
{
  l_ResolveFunction = i_Function;
}

//------------------------------------------------------------------------
// Get the Path Resolve Function that currently used
//------------------------------------------------------------------------
ResolveFunction
GetPathResolveFunction()
{
  return l_ResolveFunction;
}

} // end of namespace
