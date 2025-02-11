/****************************************************************************\
**  fsResourceFinderDir.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsResourceFinderDir.hpp"

#include "Core/fs/fsFileUtil.hpp"

//----------------------------------------------------------------
// Searches for resources in the given directory. If strict
//	is set true, then it will check to see if the file really
//	exists in this directory.
//----------------------------------------------------------------
fsResourceFinderDir::fsResourceFinderDir(const fsLocator& i_Directory, bool i_bStrict)
  : m_Directory(i_Directory)
  , m_bStrict(i_bStrict)
{
}

//----------------------------------------------------------------
//	Trying to find resource i_FileName.  If this finder
//	knows where it is, it should return true and
//	put the full path to the resource (including filename)
//	into o_FoundLocator.
//----------------------------------------------------------------
bool
fsResourceFinderDir::FindResource(const itString& i_FileName, fsLocator& o_FoundLocator) const
{
  o_FoundLocator = m_Directory;
  o_FoundLocator.Push(i_FileName);

  if (m_bStrict)
    return fsFileUtil::FileExists(o_FoundLocator);
  else
    return true;
}

//----------------------------------------------------------------
// This function works like the other FindResource, but this
//	one provides a locator of the resource that is asking
//	for the file to be found, which may be important for
//	some implementations.
//----------------------------------------------------------------
bool
fsResourceFinderDir::FindResource(const itString& i_FileName,
                                  const fsLocator& i_AskingLocator,
                                  fsLocator& o_FoundLocator) const
{
  o_FoundLocator = m_Directory;
  o_FoundLocator.Push(i_FileName);

  if (m_bStrict)
    return fsFileUtil::FileExists(o_FoundLocator);
  else
    return true;
}
