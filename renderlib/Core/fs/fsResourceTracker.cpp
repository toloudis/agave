/****************************************************************************\
**  fsResourceTracker.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/env/envSTLHelpers.hpp"
#include "Core/dbg/dbgMsg.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/fs/fsResourceTracker.hpp"
#include "Core/it/itStringUtil.hpp"

#include "Core/fs/fsFileNotifyMgr.hpp"
#include <string>
#include <iomanip>

//============================================================================
//============================================================================
namespace fsResourceTracker {
fsResourceTrackerData m_ResourceData;

fsResourceTrackerInterest* l_InterestList = new fsFileNotifyMgr;

int m_Depth;
bool m_bTracking = false;

void
notify_interests_added(const fsLocator& i_FileToAdd)
{
  l_InterestList->FileAdded(i_FileToAdd);
}

void
notify_interests_remove(const fsLocator& i_FileToRemove)
{
  l_InterestList->FileRemoved(i_FileToRemove);
}

void
notify_interests_renamed(const fsLocator& i_FileToRename, const fsLocator& i_RenamedFile)
{
  l_InterestList->FileRenamed(i_FileToRename, i_RenamedFile);
}

fsResourceTrackerInterest*
GetInterestList()
{
  return l_InterestList;
}
//------------------------------------------------------------------------
//	These are called ONCE by the application.
//
//	If Init is not called this tracker will NOT track anything
//------------------------------------------------------------------------
void
Init()
{
  m_ResourceData.m_Resources.clear();
  m_ResourceData.m_Resources.resize(0);
  m_Depth = 0;
  m_bTracking = true;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
CleanUp()
{
  //	m_ResourceData.m_Resources.clear();
  //	m_ResourceData.m_Resources.resize(0);
  m_bTracking = false;

  // clean up mem. note this was not allocated in Init (could it be?)
  delete l_InterestList;
}

//------------------------------------------------------------------------
//	Mark where the begin and end of filename access is.  This allows
//	for files to be tracked as "within" another file.  For instance,
//	reading a model file and marking it will allow the textures to be
//	marked as associated with it.
//------------------------------------------------------------------------
void
MarkBegin(const fsLocator& i_Filename)
{
  if (!m_bTracking)
    return;

  int index = GetIndex(i_Filename);
  if (index == -1) {
    index = m_ResourceData.m_Resources.size();
    m_ResourceData.m_Resources.resize(m_ResourceData.m_Resources.size() + 1);
  }

  m_ResourceData.m_Resources[index].OnOpen(i_Filename);
  m_ResourceData.m_Resources[index].SetDepth(m_Depth);

  // std::string file_path;
  // fsFileUtil::LocatorToANSIFilename(i_Filename, file_path);
  // DBG_TRACE(">>>>>Begin (%d)[%d] (%s)", m_Depth, m_ResourceData.m_Resources[index].GetResourceCount(),
  // file_path.c_str());
  notify_interests_added(i_Filename);
  ++m_Depth;
}

void
MarkEnd(const fsLocator& i_Filename)
{
  if (!m_bTracking)
    return;

  int index = GetIndex(i_Filename);
  if (index != -1) {
    --m_Depth;

    // std::string file_path;
    // fsFileUtil::LocatorToANSIFilename(i_Filename, file_path);
    // DBG_TRACE(">>>>>End   (%d) (%s)", m_Depth, file_path.c_str());
  } else {
    std::string file_path;
    fsFileUtil::LocatorToANSIFilename(i_Filename, file_path);
    DBG_WARNING("<<<<<End  duplicate (" << m_Depth << ") (index " << index << ") (" << file_path.c_str() << ")");
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
remove_files_at_index(int i_Index)
{
  if (i_Index != -1) {
    //	DEBUG
    // Debug_OutputList();
    DBG_ASSERT(m_ResourceData.m_Resources.size() >= i_Index + 1, "bad index");
    std::vector<fsResourceTrackerFileData>::iterator it = m_ResourceData.m_Resources.begin() + i_Index;
    std::vector<fsResourceTrackerFileData>::iterator last_it = m_ResourceData.m_Resources.begin() + i_Index;

    //	find the end of the resource chunk to remove
    //
    int depth = m_ResourceData.m_Resources[i_Index].GetDepth();
    while (++(it) != m_ResourceData.m_Resources.end()) {
      last_it = it;
      if ((*it).GetDepth() == depth) {
        break;
      }
    }

    //	if at the end, decrease the depth
    //
    if (it == m_ResourceData.m_Resources.end()) {
      last_it = m_ResourceData.m_Resources.end();
      if (m_Depth > 0)
        m_Depth--;
    }

    //	remove the resources, checking for the resource count
    //
    std::vector<fsResourceTrackerFileData>::iterator first_it;
    first_it = m_ResourceData.m_Resources.begin() + i_Index;
    it = last_it;
    int index = i_Index;
    std::vector<int> indicesToRemove;
    while (it != first_it) {
      --it;

      // std::string filepath;
      // fsFileUtil::LocatorToANSIFilename((*it).GetFilePath(), filepath);
      // DBG_TRACE("removing..." << filepath.c_str() );

      if ((*it).DecrementReferenceCount() == 0) {
        // DBG_TRACE("...erasing");

        notify_interests_remove((*it).GetFilePath());
        // FIXME erasing during list iteration; verify the correctness here
        // erase returns the next item.
        indicesToRemove.push_back(it - m_ResourceData.m_Resources.begin());
        //					it = m_ResourceData.m_Resources.erase(it);

        // if we erased the iterator corresponding to first_it, then first_it is invalidated!
        //                    if (m_ResourceData.m_Resources.size() == 0) {
        //                        break;
        //                    }
      }
    };
    for (int i = 0; i < indicesToRemove.size(); ++i) {
      m_ResourceData.m_Resources.erase(m_ResourceData.m_Resources.begin() + indicesToRemove[i]);
    }

    // m_ResourceData.m_Resources.erase( m_ResourceData.m_Resources.begin()+(i_Index), last_it );
  }

  //	notify_interests_removed();
}

//------------------------------------------------------------------------
//	Remove the file and all of it's sub-files.
//------------------------------------------------------------------------
void
Remove(const fsLocator& i_Filename)
{
  if (!m_bTracking)
    return;

  int index = GetIndex(i_Filename);
  remove_files_at_index(index);
  notify_interests_remove(i_Filename);

  //	notify_interests_removed();
}

//------------------------------------------------------------------------
//	Remove the file and all of it's sub-files.
//------------------------------------------------------------------------
void
Remove(const itString& i_Filename)
{
  if (!m_bTracking)
    return;

  int index = GetIndex(i_Filename);
  remove_files_at_index(index);

  //	notify_interests_removed();
}

//------------------------------------------------------------------------
//	ReplaceFile() - replace JUST the filename (not the sub-files)
//------------------------------------------------------------------------
void
ReplaceFile(const fsLocator& i_OrigFilename, const fsLocator& i_NewFilename)
{
  if (!m_bTracking)
    return;

  int index = GetIndex(i_OrigFilename);
  if (index != -1) {
    //	if the reference count is one then we can just replace the name
    //	if it is greater than one then we need to decrement the count
    //	and insert a new texturename.
    if (m_ResourceData.m_Resources[index].GetReferenceCount() == 1) {
      m_ResourceData.m_Resources[index].SetFilePath(i_NewFilename);

    } else {
      m_ResourceData.m_Resources[index].DecrementReferenceCount();
      int tempindex = GetIndex(i_NewFilename);
      if (tempindex != -1) {
        //	already exists
        m_ResourceData.m_Resources[tempindex].IncrementReferenceCount();
      } else {
        //	doesn't exist
        m_ResourceData.m_Resources.insert(m_ResourceData.m_Resources.begin() + index,
                                          m_ResourceData.m_Resources[index]);
        m_ResourceData.m_Resources[index].SetFilePath(i_NewFilename);
      }
    }
  }
  // notify_interests_renamed(i_OrigFilename, i_NewFilename);
}

//------------------------------------------------------------------------
//	ReplaceFile() - replace JUST the filename (not the sub-files)
//------------------------------------------------------------------------
void
ReplaceFile(const itString& i_OrigFilename, const fsLocator& i_NewFilename)
{
  if (!m_bTracking)
    return;

  int index = GetIndex(i_OrigFilename);
  if (index != -1) {
    //	if the reference count is one then we can just replace the name
    //	if it is greater than one then we need to decrement the count
    //	and insert a new texturename.
    if (m_ResourceData.m_Resources[index].GetReferenceCount() == 1) {
      m_ResourceData.m_Resources[index].SetFilePath(i_NewFilename);
    } else {
      m_ResourceData.m_Resources[index].DecrementReferenceCount();
      int tempindex = GetIndex(i_NewFilename);
      if (tempindex != -1) {
        //	already exists
        m_ResourceData.m_Resources[tempindex].IncrementReferenceCount();
      } else {
        //	doesn't exist
        m_ResourceData.m_Resources.insert(m_ResourceData.m_Resources.begin() + index,
                                          m_ResourceData.m_Resources[index]);
        m_ResourceData.m_Resources[index].SetFilePath(i_NewFilename);
      }
    }
  }
  //	notify_interests_renamed();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
fsResourceTrackerData&
GetData()
{
  return m_ResourceData;
}

//------------------------------------------------------------------------
//	Dump the current data and replace it with the new data
//------------------------------------------------------------------------
void
SetData(fsResourceTrackerData& i_NewData)
{
  m_ResourceData.m_Resources.clear();
  m_ResourceData.m_Resources.resize(i_NewData.m_Resources.size());

  for (int i = 0; i < i_NewData.m_Resources.size(); ++i) {
    m_ResourceData.m_Resources[i] = i_NewData.m_Resources[i];
  }
}

//------------------------------------------------------------------------
//	Find out if an item is already in the list, if so return the index.
//	A return value of -1 means the file is not in the list.
//------------------------------------------------------------------------
int
GetIndex(const itString& i_Filename)
{
  for (int i = 0; i < m_ResourceData.m_Resources.size(); ++i) {
    //	DEBUG only
    // std::string strpath2;
    // fsFileUtil::LocatorToANSIFilename( m_ResourceData.m_Resources[i].GetFilePath(), strpath2 );
    // DBG_TRACE("GI+    %03d. %s", i, itStringUtil::GetStdString(i_Filename).c_str());
    // DBG_TRACE("            " << strpath2.c_str());

    //
    if (itStringUtil::Equal(i_Filename, m_ResourceData.m_Resources[i].GetFilePath().GetLastName())) {
      // DBG_TRACE("----MATCH!----");
      return i;
    }
  }
  return -1;
}
int
GetIndex(const fsLocator& i_Filename)
{
  for (int i = 0; i < m_ResourceData.m_Resources.size(); ++i) {
    //	DEBUG only
    // std::string strpath;
    // std::string strpath2;
    // fsFileUtil::LocatorToANSIFilename( i_Filename, strpath );
    // fsFileUtil::LocatorToANSIFilename( m_ResourceData.m_Resources[i].GetFilePath(), strpath2 );
    // DBG_TRACE("GI     %03d. %s", i, strpath.c_str());
    // DBG_TRACE("            " << strpath2.c_str());

    //
    if (i_Filename.Equals(m_ResourceData.m_Resources[i].GetFilePath())) {
      // DBG_TRACE("----MATCH!----");
      return i;
    }
  }
  return -1;
}
bool
IsInList(const fsLocator& i_Filename)
{
  return (GetIndex(i_Filename) >= 0);
}

//------------------------------------------------------------------------
//	Try to find the index based only on the base filename, NOT the
//	extension.
//------------------------------------------------------------------------
int
GetIndexFromBaseFileName(const itString& i_Filename)
{
  for (int i = 0; i < m_ResourceData.m_Resources.size(); ++i) {
    //	DEBUG only
    // std::string strpath2;
    // fsFileUtil::LocatorToANSIFilename( m_ResourceData.m_Resources[i].GetFilePath(), strpath2 );
    // DBG_TRACE("GI+    %03d. %s", i, itStringUtil::GetStdString(i_Filename).c_str());
    // DBG_TRACE("            " << strpath2.c_str());

    //
    itString str1, str2;
    str1 = i_Filename;
    str1.StripExtension();
    str2 = m_ResourceData.m_Resources[i].GetFilePath().GetLastName();
    str2.StripExtension();
    if (itStringUtil::Equal(str1, str2)) {
      // DBG_TRACE("----MATCH!---- base filename");
      return i;
    }
  }
  return -1;
}

//------------------------------------------------------------------------
//	Given the resource index passed in, find the last index of all of
//	the sub-resources and return it.
//------------------------------------------------------------------------
int
GetResourceLastIndex(int i_StartIndex)
{
  int depth = m_ResourceData.m_Resources[i_StartIndex].GetDepth();

  for (int i = i_StartIndex + 1; i < m_ResourceData.m_Resources.size(); ++i) {
    if (depth >= m_ResourceData.m_Resources[i].GetDepth()) {
      return (i - 1);
    }
  }
  return m_ResourceData.m_Resources.size() - 1;
}

//------------------------------------------------------------------------
//	GetResourceList - based on the file path, return a list of all the
//	resources associated with that file.
//------------------------------------------------------------------------
void
GetResourceList(fsLocator& i_FilePath, fsResourceTrackerDataList& io_ResourceList)
{
  int index = GetIndex(i_FilePath);
  if (index != -1) {
    int end_index = GetResourceLastIndex(index);
    int size = io_ResourceList.size();
    io_ResourceList.resize(size + (end_index - index) + 1);
    for (int i = index; i <= end_index; ++i) {
      io_ResourceList[size + (i - index)] = m_ResourceData.m_Resources[i];

      //	DEBUG only
      // std::string strpath;
      // fsFileUtil::LocatorToANSIFilename( m_ResourceData.m_Resources[i].GetFilePath(), strpath );
      // DBG_TRACE("    %03d. %s", i, strpath.c_str());
    }
  } else {
    //	if it wasn't found add it anyway.
    //
    int size = io_ResourceList.size();
    io_ResourceList.resize(size + 1);
    io_ResourceList[size].SetFilePath(i_FilePath);

    //	DEBUG only
    // std::string strpath;
    // fsFileUtil::LocatorToANSIFilename( io_ResourceList[size].GetFilePath(), strpath );
    // DBG_TRACE("    00A. " << strpath.c_str());
  }
}

//------------------------------------------------------------------------
//	GetResourceList - based on the file name, return a list of all the
//	resources associated with that file.
//------------------------------------------------------------------------
void
GetResourceList(const itString& i_FileName, fsResourceTrackerDataList& io_ResourceList)
{
  int index = GetIndex(i_FileName);
  if (index != -1) {
    int end_index = GetResourceLastIndex(index);
    int size = io_ResourceList.size();
    io_ResourceList.resize(size + (end_index - index) + 1);
    for (int i = index; i <= end_index; ++i) {
      io_ResourceList[size + (i - index)] = m_ResourceData.m_Resources[i];

      //	DEBUG only
      // std::string strpath;
      // fsFileUtil::LocatorToANSIFilename( m_ResourceData.m_Resources[i].GetFilePath(), strpath );
      // DBG_TRACE("    %03d. %s", i, strpath.c_str());
    }
  } else {
    index = GetIndexFromBaseFileName(i_FileName);
    if (index != -1) {
      //	if it wasn't found add it anyway.
      //	TODO - should we add a range of indices (like the 'then' part above?) [rjk]
      //
      int size = io_ResourceList.size();
      io_ResourceList.resize(size + 1);
      io_ResourceList[size] = m_ResourceData.m_Resources[index];

      //	DEBUG only
      // std::string strpath;
      // fsFileUtil::LocatorToANSIFilename( m_ResourceData.m_Resources[index].GetFilePath(), strpath );
      // DBG_TRACE("    0B1. " << strpath.c_str());
    } else {
      //	if it wasn't found add it anyway.
      //
      int size = io_ResourceList.size();
      io_ResourceList.resize(size + 1);
      io_ResourceList[size].SetFilePath(i_FileName);

      //	DEBUG only
      // std::string strpath;
      // strpath = itStringUtil::GetStdString( i_FileName );
      // DBG_TRACE("    0B2. " << strpath.c_str());
    }
  }
}

//------------------------------------------------------------------------
//	Output the list to the debug.log
//------------------------------------------------------------------------
void
Debug_OutputList()
{
  Debug_OutputList(m_ResourceData);
}

//------------------------------------------------------------------------
//	Output the list to the debug.log
//------------------------------------------------------------------------
void
Debug_OutputList(fsResourceTrackerData& i_Data)
{
  DBG_TRACE("=-- RESOURCES --=");
  for (int i = 0; i < i_Data.m_Resources.size(); ++i) {
    std::string fullpath;
    std::string space;
    fsFileUtil::LocatorToANSIFilename(i_Data.m_Resources[i].GetFilePath(), fullpath);
    int depth = i_Data.m_Resources[i].GetDepth();
    for (int j = 0; j < depth; ++j)
      space += "--";

    if (fullpath.size() > 0) {
      DBG_TRACE(std::setw(2) << i << "-" << depth << " " << space.c_str() << " " << i_Data.m_Resources[i].GetName()
                             << " = " << fullpath.c_str() << " (" << i_Data.m_Resources[i].GetReferenceCount() << ")");
    } else {
      DBG_TRACE(std::setw(2) << i << "-" << depth << " " << space.c_str() << " " << i_Data.m_Resources[i].GetName());
    }
  }
}

}
