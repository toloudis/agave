/*****************************************************************************
**  chBinWriter.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/ch/chBinWriter.hpp"

#include "Core/ch/chExceptionX.hpp"
#include "Core/fs/fsFilePosSaver.hpp"
#include "Core/gf/gfFileUtil.hpp"

//============================================================================
//============================================================================
namespace {
// Since chunk size is written to file as an unsigned 32-bit integer,
// this limits the maximum size of an individual chunk.
const envType::Int64 c_MaxChunkSize = UINT_MAX; // 4294967295 (0xffffffff)
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chBinWriter::chBinWriter(gfFileBin& io_File)
  : m_File(io_File)
{
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chBinWriter::~chBinWriter() {}

//--------------------------------------------------------------------
//	WriteChunkHeader writes a chunk header in the file at the
//	current position.
//--------------------------------------------------------------------
void
chBinWriter::WriteChunkHeader(chDefs::Name i_Name, chDefs::Version i_Version, bool i_Container)
{
  fsFileStream::FilePosType header_pos = m_File.GetFilePos();
  m_HeaderLocs.push(header_pos);
  gfFileUtil::Write(m_File, i_Name);
  gfFileUtil::Write(m_File, chDefs::Size(-1)); // don't know the size yet, write a obviously wrong value
  gfFileUtil::Write(m_File, i_Version);
}

//--------------------------------------------------------------------
//	FinishChunk will cause the writer to compute the size of the
//	chunk and finish writing the header.
//--------------------------------------------------------------------
void
chBinWriter::FinishChunk()
{
  DBG_ASSERT(m_HeaderLocs.size() > 0, "FinishChunk w/o corresponding WriteChunkHeader");

  fsFilePosSaver<gfFileBin> saver(m_File);
  fsFileStream::FilePosType header_pos = m_HeaderLocs.top();
  fsFileStream::FilePosType chunk_size =
    saver.GetPos() - (header_pos + sizeof(chDefs::Name) + sizeof(chDefs::Size) + sizeof(chDefs::Version));

  m_File.SetFilePos(header_pos + sizeof(chDefs::Name)); // skip past the name
  DBG_ASSERT(chunk_size >= 0, "Negative chunk size - how did that happen?");

  // Check for chunk size too large
  if (chunk_size > c_MaxChunkSize) {
    DBG_ERROR("Invalid chunk -- chunk size exceeding maximum size.");
    throw chInvalidChunkX();
  } else
    gfFileUtil::Write(m_File, chDefs::Size(chunk_size));

  m_HeaderLocs.pop();
}

//--------------------------------------------------------------------
//	Write functions - These write the data to the chunk file.
//--------------------------------------------------------------------
void
chBinWriter::Write(envType::Int8 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::UInt8 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::Int16 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::UInt16 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::Int32 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::UInt32 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::Int64 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::UInt64 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::Float32 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

void
chBinWriter::Write(envType::Float64 i_Val)
{
  gfFileUtil::Write(m_File, i_Val);
}

//--------------------------------------------------------------------
//	This Write writes a string called either "TRUE" or "FALSE" based
//  on a bool value.
//--------------------------------------------------------------------
void
chBinWriter::Write(bool i_Val)
{
  gfFileUtil::Write(m_File, static_cast<envType::Int8>(i_Val));
}

//--------------------------------------------------------------------
//	This Write writes a NULL-terminated single-byte character string
//--------------------------------------------------------------------
void
chBinWriter::Write(const char* i_Val)
{
  int num_to_write = ::strlen(i_Val);
  m_File.Write(num_to_write + 1, i_Val); // write NULL also
}

//--------------------------------------------------------------------
//	This Write writes a single-byte character string with a length
//	given by i_Length.
//--------------------------------------------------------------------
void
chBinWriter::Write(const char* i_Val, int i_Length)
{
  m_File.Write(i_Length, i_Val);
}

//--------------------------------------------------------------------
//	This Write writes a NULL-terminated Unicode character string
//--------------------------------------------------------------------
void
chBinWriter::Write(const envType::WChar* i_Val)
{
#if (WCHART_SIZE == 2)
  int num_to_write = 0;

  while (i_Val[num_to_write])
    num_to_write++;

  m_File.Write(2 * (num_to_write + 1), i_Val); // write NULL also
#else
#error "UTF16 file format needs to be converted to wchar_t"
#endif
}

//--------------------------------------------------------------------
//	This Write writes a Unicode character string with a length
//	given by i_Length.
//--------------------------------------------------------------------
void
chBinWriter::Write(const envType::WChar* i_Val, int i_Length)
{
#if (WCHART_SIZE == 2)
  m_File.Write(i_Length * 2, i_Val);
#else
#error "UTF16 file format needs to be converted to wchar_t"
#endif
}

//--------------------------------------------------------------------
//	Write the envAppVersion structure
//--------------------------------------------------------------------
void
chBinWriter::Write(const envAppVersion& i_Val)
{
  gfFileUtil::Write(m_File, i_Val.GetMajor());
  gfFileUtil::Write(m_File, i_Val.GetMinor());
  gfFileUtil::Write(m_File, i_Val.GetRevision());
  gfFileUtil::Write(m_File, i_Val.GetBuild());
}

//--------------------------------------------------------------------
//	Write chunk of binary data; this is only for
//	chBinWriter, it is not a function of chWriter base class
//--------------------------------------------------------------------
void
chBinWriter::Write(const void* i_Data, envType::Int64 i_NumBytes)
{
  m_File.Write(i_NumBytes, i_Data);
}

//------------------------------------------------------------------------
//	GetLocator returns this file's associated locator
//------------------------------------------------------------------------
const fsLocator&
chBinWriter::GetLocator() const
{
  return m_File.GetLocator();
}
