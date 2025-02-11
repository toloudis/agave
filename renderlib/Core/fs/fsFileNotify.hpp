/****************************************************************************\
**  fsFileNotify.hpp
**
**      fsFileNotify.hpp is used to check for changes to the specified file
**
**
**
\****************************************************************************/

#ifdef FS_FILENOTIFY_HPP
#error fsFileNotify.hpp multiply included
#endif
#define FS_FILENOTIFY_HPP

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

// #include "windows.h"

//============================================================================
//============================================================================
class fsFileNotify
{
public:
  //------------------------------------------------------------------------
  // Constructors
  //------------------------------------------------------------------------
  fsFileNotify(const fsLocator& i_FileToCheck, time_t& io_accessTime);
  // fsFileNotify(const fsLocator& i_Locator);

  //------------------------------------------------------------------------
  // Constructors
  //------------------------------------------------------------------------
  ~fsFileNotify();

  //------------------------------------------------------------------------
  // ReadChangeStatus returns the type of change if any for the file specified
  //------------------------------------------------------------------------
  void CheckForModification();

  //------------------------------------------------------------------------
  // AddToList - Add the file to the list to probe
  //------------------------------------------------------------------------
  void AddToList(fsLocator& i_FileToCheck);

private:
};