/****************************************************************************\
**  fsResourceTrackerData.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsResourceTrackerData.hpp"

//
//	fsResourceTrackerFileData
//

//------------------------------------------------------------------------
//------------------------------------------------------------------------
fsResourceTrackerFileData::fsResourceTrackerFileData()
  : m_Depth(0)
  , m_ReferenceCount(0)
  , m_bOpen(false)
  , m_bObjectName(false)
{
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::OnOpen(const fsLocator& i_File)
{
  m_File = i_File;
  m_bOpen = true;
  m_Depth = 0;
  ++m_ReferenceCount;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::OnClose()
{
  m_bOpen = false;
  --m_ReferenceCount;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::IncrementDepth()
{
  m_Depth++;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::DecrementDepth()
{
  m_Depth--;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::SetDepth(int i_Depth)
{
  m_Depth = i_Depth;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
fsResourceTrackerFileData::GetDepth() const
{
  return m_Depth;
}

//------------------------------------------------------------------------
//	Increment the reference count and return the new count
//------------------------------------------------------------------------
int
fsResourceTrackerFileData::IncrementReferenceCount()
{
  ++m_ReferenceCount;
  return m_ReferenceCount;
}

//------------------------------------------------------------------------
//	decrement the reference count and return the new count
//------------------------------------------------------------------------
int
fsResourceTrackerFileData::DecrementReferenceCount()
{
  --m_ReferenceCount;
  if (m_ReferenceCount < 0)
    m_ReferenceCount = 0;
  return m_ReferenceCount;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
fsResourceTrackerFileData::GetReferenceCount() const
{
  return m_ReferenceCount;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::SetFilePath(const fsLocator& i_FileName)
{
  m_File = i_FileName;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::SetFilePath(const itString& i_FileName)
{
  m_File.Clear();
  if (i_FileName.GetLength() > 0)
    m_File.Push(i_FileName);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
const fsLocator&
fsResourceTrackerFileData::GetFilePath() const
{
  return m_File;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
const itString&
fsResourceTrackerFileData::GetFileName() const
{
  return m_File.GetLastName();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool
fsResourceTrackerFileData::IsOpen()
{
  return m_bOpen;
}

//------------------------------------------------------------------------
//	Set the name of this resource.  If there is not a file, but only a
//	name, it is an category or object name.
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::SetName(const nameString& i_Name)
{
  m_Name = i_Name;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
nameString
fsResourceTrackerFileData::GetName() const
{
  return m_Name;
}

//------------------------------------------------------------------------
//	Set the flag to signify the name is the name of the object (as
//	opposed to a material, fragment, etc).
//------------------------------------------------------------------------
void
fsResourceTrackerFileData::SetObjectName(bool i_bObjectName)
{
  m_bObjectName = i_bObjectName;
}
bool
fsResourceTrackerFileData::IsObjectName() const
{
  return m_bObjectName;
}

//
//	fsResourceTrackerData
//

//------------------------------------------------------------------------
//------------------------------------------------------------------------
fsResourceTrackerDataList&
fsResourceTrackerData::GetList()
{
  return m_Resources;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerData::Add(const fsLocator& i_FilePath)
{
  if (i_FilePath.GetNumNames() > 0) {
    int size = m_Resources.size();
    m_Resources.resize(size + 1);
    m_Resources[size].SetFilePath(i_FilePath);
    m_Resources[size].SetDepth(m_Depth); // copy the "global" depth

    DBG_TRACE("RSTK Add file - " << m_Depth << " " << i_FilePath);
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerData::Add(const nameString& i_Name, bool i_bIsObjectName /*= false*/)
{
  int size = m_Resources.size();
  m_Resources.resize(size + 1);
  m_Resources[size].SetName(i_Name);
  m_Resources[size].SetDepth(m_Depth); // copy the "global" depth

  DBG_TRACE("RSTK Add name - " << m_Depth << " " << i_Name);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerData::operator=(const fsResourceTrackerData& i_Data)
{
  //	copy the main data
  m_Depth = i_Data.GetCurrentDepth();

  //	copy the list
  m_Resources.resize(i_Data.m_Resources.size());

  for (int i = 0; i < i_Data.m_Resources.size(); ++i) {
    m_Resources[i] = i_Data.m_Resources[i];
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerData::operator+=(const fsResourceTrackerData& i_Data)
{
  //	copy the main data
  m_Depth = i_Data.GetCurrentDepth();

  //	copy the list
  int size = m_Resources.size();
  m_Resources.resize(size + i_Data.m_Resources.size());

  for (int i = 0; i < i_Data.m_Resources.size(); ++i) {
    m_Resources[size + i] = i_Data.m_Resources[i];
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerData::DecrementCurrentDepth()
{
  --m_Depth;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerData::IncrementCurrentDepth()
{
  ++m_Depth;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
fsResourceTrackerData::SetCurrentDepth(int i_Depth)
{
  m_Depth = i_Depth;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
fsResourceTrackerData::GetCurrentDepth() const
{
  return m_Depth;
}
