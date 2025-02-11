/****************************************************************************\
**  fsResourceFinder.hpp
**
**      fsResourceFinder.hpp defines an abstract interface for
**	finding resource files within a directory structure.
**
**
**
\****************************************************************************/

#ifdef FS_RESOURCEFINDER_HPP
#error fsResourceFinder.hpp multiply included
#endif
#define FS_RESOURCEFINDER_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#include <vector>

//============================================================================
//============================================================================
class itString;
class fsLocator;

//============================================================================
//============================================================================
class fsResourceFinder
{
public:
  //----------------------------------------------------------------
  //----------------------------------------------------------------
  virtual ~fsResourceFinder(){};

  //----------------------------------------------------------------
  //	Trying to find resource i_FileName.  If this finder
  //	knows where it is, it should return true and
  //	put the full path to the resource (including filename)
  //	into o_FoundLocator.
  //----------------------------------------------------------------
  virtual bool FindResource(const itString& i_FileName, fsLocator& o_FoundLocator) const = 0;

  //----------------------------------------------------------------
  // This function works like the other FindResource, but this
  //	one provides a locator of the resource that is asking
  //	for the file to be found, which may be important for
  //	some implementations.
  //----------------------------------------------------------------
  virtual bool FindResource(const itString& i_FileName,
                            const fsLocator& i_AskingLocator,
                            fsLocator& o_FoundLocator) const = 0;
};
