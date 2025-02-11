/*****************************************************************************
**	fsFileNotifyMgr.hpp
**
**	Manages the file updates .
**
**
**
\****************************************************************************/
#ifdef FS_FILENOTIFYMGR_HPP
#error fsFileNotifyMgr.hpp multiply included
#endif
#define FS_FILENOTIFYMGR_HPP

#ifndef FS_FILERESOURCETRACKERINTEREST_HPP
#include "Core/fs/fsResourceTrackerInterest.hpp"
#endif

//============================================================================
//============================================================================
class fsFileNotifyMgr : public fsResourceTrackerInterest
{
public:
  //------------------------------------------------------------------------
  /// Initialize
  //------------------------------------------------------------------------
  static void Intialize();

  //------------------------------------------------------------------------
  /// DeInitialize
  //------------------------------------------------------------------------
  static void DeInitialize();

  //--------------------------------------------------------------------
  /// Check if a file has been modified.
  //--------------------------------------------------------------------
  static void Check(unsigned short time);

  //--------------------------------------------------------------------
  /// Check for new objects being created
  //--------------------------------------------------------------------
  static void CheckForNewObjects();

  //--------------------------------------------------------------------
  ///	FileAdded - a file has been
  ///		added to the system
  //--------------------------------------------------------------------
  void FileAdded(const fsLocator& i_FileToCheck);

  //--------------------------------------------------------------------
  ///	FileRemoved - a file has been
  ///	removed from the system
  //--------------------------------------------------------------------
  void FileRemoved(const fsLocator& i_FileToCheck);

  //--------------------------------------------------------------------
  ///	FileRenamed - a file has been renamed
  //--------------------------------------------------------------------
  void FileRenamed(const fsLocator& i_FileToRename, const fsLocator& i_RenamedFile);

  //--------------------------------------------------------------------
  /// Function pointer for ObjectReload function
  //--------------------------------------------------------------------
  void ObjectReloadFunction(void (*i_ObjectReloadFunction)(const fsLocator& i_Path, const std::string i_SystemName));

  void TextureReloadFunction(void (*i_TextureReloadFunction)(const fsLocator& i_TextureName, bool i_bIsMipMap));

  void ObjectAddFunction(void (*i_AddObjectsFunction)(const itString& i_FileName, const fsLocator& i_Path));
};

namespace FileUpdated {
/*void FileAdded( const fsLocator& i_FileToCheck);
void FileRemoved(const fsLocator& i_FileToCheck);
void FileRenamed(const fsLocator& i_FileToRename, const fsLocator& i_RenamedFile);
bool FileExists(const fsLocator& i_FileToCheck);*/
void
SetAutoUpdate(const bool& i_EnableAutoUpdate);
void
SetEnableRevisionFlag(const bool& i_EnableAutoUpdate);
bool
GetEnableRevisionFlag();
}
