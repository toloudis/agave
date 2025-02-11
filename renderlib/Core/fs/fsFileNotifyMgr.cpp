
/*****************************************************************************
**  fsFileNotifyMgr.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/fs/fsFileNotifyMgr.hpp"
#ifndef FS_FILEUTIL_HPP
#include "Core/fs/fsFileUtil.hpp"
#endif

#include "Core/it/itStringUtil.hpp"
#include "Core/fs/fsXMLReader.hpp"
#include "Core/fs/fsXMLWriter.hpp"
#include "Core/gf/gfPaths.hpp"
#include "Core/name/nameMgr.hpp"
#include "Core/name/nameObject.hpp"

// #include "Tool/gui/guiMessageBox.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

namespace FileUpdated {
bool l_PromptBeforeUpdate = false;
bool l_Revision = false;
//--------------------------------------------------------------------
// Set autoupdate flag on/off
//--------------------------------------------------------------------
void
SetAutoUpdate(const bool& i_PromptBeforeUpdate)
{
  l_PromptBeforeUpdate = i_PromptBeforeUpdate;
}

//--------------------------------------------------------------------
// Set revision flag on/off
//--------------------------------------------------------------------
void
SetEnableRevisionFlag(const bool& i_Revision)
{
  l_Revision = i_Revision;
}

bool
GetEnableRevisionFlag()
{
  return l_Revision;
}
}

namespace {
std::vector<fsLocator> fileNames;
std::vector<time_t> initTimes;
std::vector<bool> need_update(true);

unsigned short m_prevTime = 0.0;

int l_count = 0;
size_t start = 0.0, finish = 0.0, end = 0.0;
void (*l_ReloadObjectsFunction)(const fsLocator& i_Path, const std::string i_SystemName);
void (*l_ReloadTextureFunction)(const fsLocator& i_TextureName, bool i_bIsMipMap);
void (*l_AddObjectsFunction)(const itString& i_FileName, const fsLocator& i_Path);

//--------------------------------------------------------------------
//	FileAdded - a file has been
//		added to the system
//--------------------------------------------------------------------
void
file_added(const fsLocator& i_FileToCheck)
{
  std::string fileNameStr;
  for (int i = 0; i < fileNames.size(); i++) {
    if (i_FileToCheck == fileNames[i])
      return;
  }
  fsFileUtil::LocatorToANSIFilename(i_FileToCheck, fileNameStr);
  struct stat st;
  stat(fileNameStr.c_str(), &st);
  fileNames.push_back(i_FileToCheck);
  DBG_TRACE(" Object " << fileNameStr << " has been added");
  initTimes.push_back(st.st_mtime);
  need_update.push_back(true);
}

//--------------------------------------------------------------------
//	FileRemoved - a file has been
//		removed from the system
//--------------------------------------------------------------------
void
file_removed(const fsLocator& i_FileToCheck)
{
  for (int i = 0; i < fileNames.size(); i++) {
    if (i_FileToCheck == fileNames[i]) {
      std::string fileNameStr;
      fsFileUtil::LocatorToANSIFilename(fileNames[i], fileNameStr);
      DBG_TRACE(" Object " << fileNameStr << " has been deleted");
      fileNames.erase(fileNames.begin() + i);
      return;
    }
  }
}

//--------------------------------------------------------------------
//	FileRenamed - a file has been renamed
//--------------------------------------------------------------------
void
file_renamed(const fsLocator& i_FileToRename, const fsLocator& i_RenamedFile)
{
  file_removed(i_FileToRename);
  file_added(i_RenamedFile);
}

//--------------------------------------------------------------------
// check if a file already exists in the scene
//--------------------------------------------------------------------
bool
file_exists(const fsLocator& i_FileToCheck)
{
  for (int i = 0; i < fileNames.size(); i++) {
    if (i_FileToCheck == fileNames[i]) {
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------
// Checks if a file needs to be updated.
// Reloads Objects and textures if needed
//--------------------------------------------------------------------
void
file_check(const size_t start, const size_t end)
{
  std::string fileName, i_fileName;
  itString t_FileName, o_Extension;
  time_t newTime;
  bool UpdateObject = false;

  for (size_t i = start; i < end; i++) {
    fsFileUtil::LocatorToUnicodeString(fileNames[i], t_FileName);
    fsFileUtil::LocatorToANSIFilename(fileNames[i], fileName);
    fsFileUtil::LocatorToANSIFilename(fileNames[i].GetLastName(), i_fileName);

    struct _stat64i32 st;
    //_wstat(t_FileName, &st);
    _wstat(t_FileName.GetString(), &st);
    newTime = st.st_mtime;
    // Check whether the object has been updated
    if (newTime != initTimes[i] && newTime != -1 && need_update[i]) {
      int length = t_FileName.GetLength();
      // Break out extension
      o_Extension = "";
      int pos = length - 1;
      if (t_FileName.HasSubString(itString("."))) {
        t_FileName.GetExtension(o_Extension);
        o_Extension = itStringUtil::ToLower(o_Extension);
      }
      DBG_TRACE(" Object " << fileName << " has been updated");

      // If the dialog needs to be shown
      std::string message = " Do you want to update the file " + fileName + "?";
      if (FileUpdated::l_PromptBeforeUpdate) {
        int retVal = 0; // guiMessageBox::Show(message.c_str(),"File Updated", guiMessageBox::e_YesNo);

        if (retVal == 1) {
          UpdateObject = false;
          need_update[i] = false;

          return;
        } else
          UpdateObject = true;
      }
      // If the object needs to be updated

      if (UpdateObject || !FileUpdated::l_PromptBeforeUpdate) {
        // Check if the file that needs to be updated is an object
        if (l_ReloadObjectsFunction != NULL && o_Extension.HasSubString(itString("gxb"))) {
          if (fsFileUtil::FileExists(fileNames[i])) {
            l_ReloadObjectsFunction(fileNames[i], "Objects");
            file_added(fileNames[i]);
          }

          else
            DBG_TRACE("Object " << fileNames[i] << " has been renamed or deleted");
        }

        // Reload Textures
        else if (l_ReloadTextureFunction != NULL &&
                 (o_Extension.HasSubString(itString("bmp")) || o_Extension.HasSubString(itString("png")) ||
                  o_Extension.HasSubString(itString("dds")) || o_Extension.HasSubString(itString("tga")) ||
                  o_Extension.HasSubString(itString("jpg")) || o_Extension.HasSubString(itString("hdr")) ||
                  o_Extension.HasSubString(itString("tif")))) {
          if (fsFileUtil::FileExists(fileNames[i])) {
            l_ReloadTextureFunction(fileNames[i], true);
            file_added(fileNames[i]);
          }
        }
      } // end of checking if object needs update
      initTimes[i] = newTime;
    } // end of checking if the object has updated
  }   // end of for loop
} // end of Check function
} // end of namespace

//------------------------------------------------------------------------
// Initialize
//------------------------------------------------------------------------

void
fsFileNotifyMgr::Intialize()
{
  fileNames.clear();
  initTimes.clear();
}

//------------------------------------------------------------------------
// DeInitialize
//------------------------------------------------------------------------
void
fsFileNotifyMgr::DeInitialize()
{
  fileNames.clear();
  initTimes.clear();
}

//--------------------------------------------------------------------
// Check if a file has been modified.
//--------------------------------------------------------------------
void
fsFileNotifyMgr::Check(unsigned short time)
{

  float waitTime = 5.0;                       // This is the time interval in seconds between each check
  int num_of_resources = fileNames.size();    // Get the number of resources to check
  int num_of_counts = num_of_resources / 100; // Strip them in counts of 100
  int remaining = num_of_resources % 100;
  bool fromFirstSet = false;

  if (l_count != num_of_counts && num_of_counts > 0) // If there are more than 100 elements
  {
    start = 100 * l_count;
    finish = start + 100;
    end = finish;
    if (time > m_prevTime + waitTime) {
      file_check(start, finish);
      m_prevTime = time;
    }
    l_count++;
    fromFirstSet = true;
  } else if (l_count == num_of_counts && !fromFirstSet) // When the resources are less than 100
  {
    start = end;
    finish = remaining;
    if (time > m_prevTime + waitTime) {
      file_check(start, finish);
      m_prevTime = time;
    }

    l_count = 0;
  }
}

//--------------------------------------------------------------------
// Check if a new file has been exported
//--------------------------------------------------------------------
void
fsFileNotifyMgr::CheckForNewObjects()
{
  fsLocator path_loc;
  fsLocator full_path;
  fsLocator textfilePath = gfPaths::GetPath(gfPaths::e_LocalSystemDataPath);

  textfilePath.Push("Temp");
  textfilePath.Push("allenExport_Rhino.txt");

  std::string cfgpath;
  fsFileUtil::LocatorToANSIFilename(textfilePath, cfgpath);
  if (fsFileUtil::FileExists(textfilePath)) {
    fsXMLReader pXMLReader(cfgpath);
    pXMLReader.Open();
    fsXMLData::fs_Node_Type node_type;
    std::string keyname, strvalue;
    while ((node_type = pXMLReader.ReadNode(keyname, strvalue)) != fsXMLData::e_EOF)

    {
      if ((node_type == fsXMLData::e_EndElement) && (strcmp(keyname.c_str(), "export_files") == 0)) {
        continue;
      }
      if (strcmp(keyname.c_str(), "version") == 0)
        continue;
      else if ((node_type == fsXMLData::e_Text) && (keyname.length() > 0)) {
        fsFileUtil::ANSIFilenameToLocator(strvalue.c_str(), path_loc);
        full_path.Push("Objects");
        full_path.Push(path_loc);

        if (!file_exists(path_loc)) {
          file_added(path_loc);
          l_AddObjectsFunction(full_path.GetLastName(), full_path);
        }
        full_path.Clear();
        DBG_TRACE(" KeyName " << keyname << "StrValue :" << strvalue);
      }
    }
    pXMLReader.Close();
    fsFileUtil::DeleteFile(textfilePath);
  }
}

//--------------------------------------------------------------------
// Function pointer for ObjectReload function
//--------------------------------------------------------------------
void
fsFileNotifyMgr::ObjectReloadFunction(void (*i_ObjectReloadFunction)(const fsLocator& i_Path,
                                                                     const std::string i_SystemName))
{
  l_ReloadObjectsFunction = i_ObjectReloadFunction;
}

void
fsFileNotifyMgr::ObjectAddFunction(void (*i_ObjectAddFunction)(const itString& i_Name, const fsLocator& i_Path))
{
  l_AddObjectsFunction = i_ObjectAddFunction;
}

void
fsFileNotifyMgr::TextureReloadFunction(void (*i_TextureReloadFunction)(const fsLocator& i_TextureName,
                                                                       bool i_bIsMipMap))
{
  l_ReloadTextureFunction = i_TextureReloadFunction;
}

void
fsFileNotifyMgr::FileAdded(const fsLocator& i_FileToCheck)
{
  file_added(i_FileToCheck);
}

void
fsFileNotifyMgr::FileRenamed(const fsLocator& i_FileToRename, const fsLocator& i_RenamedFile)
{
  file_renamed(i_FileToRename, i_RenamedFile);
}

void
fsFileNotifyMgr::FileRemoved(const fsLocator& i_FileToCheck)
{
  file_removed(i_FileToCheck);
}
