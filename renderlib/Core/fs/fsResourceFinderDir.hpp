/****************************************************************************\
**  fsResourceFinderDir.hpp
**
**      fsResourceFinderDir.hpp provides a simple implementation of
**	fsResourceFinder that just assumes everything is in the directory
**	provided in the constructor.
**
**
**
\****************************************************************************/

#ifdef FS_RESOURCEFINDERDIR_HPP
#error fsResourceFinderDir.hpp multiply included
#endif
#define FS_RESOURCEFINDERDIR_HPP

#ifndef FS_RESOURCEFINDER_HPP
#include "Core/fs/fsResourceFinder.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

//============================================================================
//============================================================================
class fsResourceFinderDir : public fsResourceFinder
{
public:
  //----------------------------------------------------------------
  // Searches for resources in the given directory. If strict
  //	is set true, then it will check to see if the file really
  //	exists in this directory.
  //----------------------------------------------------------------
  fsResourceFinderDir(const fsLocator& i_Directory, bool i_bStrict = true);

  //----------------------------------------------------------------
  //	Trying to find resource i_FileName.  If this finder
  //	knows where it is, it should return true and
  //	put the full path to the resource (including filename)
  //	into o_FoundLocator.
  //----------------------------------------------------------------
  virtual bool FindResource(const itString& i_FileName, fsLocator& o_FoundLocator) const;

  //----------------------------------------------------------------
  // This function works like the other FindResource, but this
  //	one provides a locator of the resource that is asking
  //	for the file to be found, which may be important for
  //	some implementations.
  //----------------------------------------------------------------
  virtual bool FindResource(const itString& i_FileName,
                            const fsLocator& i_AskingLocator,
                            fsLocator& o_FoundLocator) const;

private:
  fsLocator m_Directory;
  bool m_bStrict;
};
