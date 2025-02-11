/*****************************************************************************
**  fsResourceTrackerInterest.hpp
**
**	Callback for when Environment data changes
**
**
**
\****************************************************************************/

#ifdef FS_RESOURCETRACKERINTEREST_HPP
#error fsResourceTrackerInterest.hpp multiply included
#endif
#define FS_RESOURCETRACKERINTEREST_HPP

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

class nameString;

//============================================================================
//============================================================================
class fsResourceTrackerInterest
{
public:
  //--------------------------------------------------------------------
  //	FileAdded - a file has been
  //		added to the system
  //--------------------------------------------------------------------
  virtual void FileAdded(const fsLocator& i_FileToCheck) = 0;

  //--------------------------------------------------------------------
  //	FileRemoved - a file has been
  //		removed from the system
  //--------------------------------------------------------------------
  virtual void FileRemoved(const fsLocator& i_FileToCheck) = 0;

  //--------------------------------------------------------------------
  //	FileRenamed - a file has been renamed
  //--------------------------------------------------------------------
  virtual void FileRenamed(const fsLocator& i_FileToRename, const fsLocator& i_RenamedFile) = 0;

  virtual void ObjectReloadFunction(void (*i_ObjectReloadFunction)(const fsLocator& i_Path,
                                                                   const std::string i_SystemName)) = 0;

  virtual void TextureReloadFunction(void (*i_TextureReloadFunction)(const fsLocator& i_TextureName,
                                                                     bool i_bIsMipMap)) = 0;

  virtual void ObjectAddFunction(void (*i_AddObjectsFunction)(const itString& i_FileName, const fsLocator& i_Path)) = 0;
};