/*****************************************************************************
**  gfFile.cpp
**
**      see .hpp
**
**	Note: this file used to represent either a literal file or a file within
**	a ZIP (PAK) file. The PAK functionality has been removed because of
**	lack of use.
**
**
**
\****************************************************************************/
#include "Core/gf/gfFile.hpp"

#include "Core/fs/fsFilePosSaver.hpp"
#include "Core/gf/gfFileTranslationMgr.hpp"

//------------------------------------------------------------------------
//	Constructor
//------------------------------------------------------------------------
gfFile::gfFile(const fsLocator& i_Locator, fsFileStream::AccessType i_DesiredAccess)
  : m_FileStream(NULL)
{
  if (i_Locator.HasIndex()) {
    fsLocator full_path(i_Locator);
    gfFileTranslationMgr::ExpandLocator(full_path);
    m_FileStream = new fsFileStream(full_path, i_DesiredAccess);
  } else {
    m_FileStream = new fsFileStream(i_Locator, i_DesiredAccess);
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
gfFile::~gfFile()
{
  delete m_FileStream;
}

//------------------------------------------------------------------------
//	GetFilePos returns the current position of the file pointer.
//------------------------------------------------------------------------
fsFileStream::FilePosType
gfFile::GetFilePos() const
{
  return m_FileStream->GetFilePos();
}

//------------------------------------------------------------------------
//	GetFileSize returns the size of the file in bytes.
//------------------------------------------------------------------------
envType::Int64
gfFile::GetFileSize() const
{
  // Old way was to seek to end of file and determine offset from there
  // fsFilePosSaver<fsFileStream> saver(*m_FileStream);
  // m_FileStream->SetFilePos(0, fsFileStream::e_End);
  // return m_FileStream->GetFilePos();

  // Trying to just use the windows API function now
  return m_FileStream->GetFileSize();
}

//------------------------------------------------------------------------
//	SetFilePos sets the file pointer to the given value.  It is valid
//	to move the file pointer beyond the end of the file.
//------------------------------------------------------------------------
void
gfFile::SetFilePos(fsFileStream::FilePosType i_Pos, fsFileStream::AccessPointType i_DesiredAccessPoint)
{
  m_FileStream->SetFilePos(i_Pos, i_DesiredAccessPoint);
}

//------------------------------------------------------------------------
//	GetLocator returns this file's associated locator
//------------------------------------------------------------------------
const fsLocator&
gfFile::GetLocator() const
{
  return m_FileStream->GetLocator();
}
