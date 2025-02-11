/**********************************************************
**  gfDirectoryCategories.cpp
**
**		see .hpp
**
**
**
\*********************************************************/
#include "Core/gf/gfDirectoryCategories.hpp"
#include "Core/Fs/fsLocator.hpp"

#include <map>

//========================================================================
//========================================================================
namespace gfDirectoryCategories {
namespace {
std::map<std::string, fsLocator> l_DirectoryCategories;
}

//----------------------------------------------------------------
// Store the current directory for a named category
//----------------------------------------------------------------
void
SetCurDirectory(const std::string& i_Category, const fsLocator& i_Directory)
{
  l_DirectoryCategories[i_Category] = i_Directory;
}

//----------------------------------------------------------------
// Query the current directory for a named category,
// returns false if no current directory has been set yet.
//----------------------------------------------------------------
bool
GetCurDirectory(const std::string& i_Category, fsLocator& o_Directory)
{
  std::map<std::string, fsLocator>::const_iterator it;
  it = l_DirectoryCategories.find(i_Category);
  if (it != l_DirectoryCategories.end()) {
    o_Directory = it->second;
    return true;
  }
  return false;
}

//----------------------------------------------------------------
// Clear out all categories of directories in order to start over
//----------------------------------------------------------------
void
ClearAllCategories()
{
  l_DirectoryCategories.clear();
}

} // end of namespace
