/*****************************************************************************
**	chBinReader.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/ch/chBinReader.hpp"

#include "Core/ch/chExceptionX.hpp"
#include "Core/fs/fsFilePosSaver.hpp"
#include "Core/gf/gfFileBin.hpp"
#include "Core/gf/gfFileUtil.hpp"

//============================================================================
//============================================================================
namespace {
const int c_HeaderLength = sizeof(chDefs::Name) + sizeof(chDefs::Size) + sizeof(chDefs::Version);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chBinReader::chBinReader(gfFileBin& io_File)
  : m_File(io_File)
{
  m_FileLength = m_File.GetFileSize();
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chBinReader::~chBinReader() {}

//--------------------------------------------------------------------
//	ReadChunkHeader reads a chunk header in the file at the
//	current position.  If there is a problem with the header,
//	it will throw a chInvalidChunkX.  Returns true if more chunks to
//	read, false if no more.
//	The size is the size of the chunk not including the header.
//--------------------------------------------------------------------
bool
chBinReader::ReadChunkHeader(chDefs::Name& o_Name, chDefs::Version& o_Version, chDefs::Size& o_Size)
{
  fsFileStream::FilePosType cur_pos = m_File.GetFilePos();

  //	past the end of the current chunk
  //
  if ((m_ChunkEnds.size() > 0) && (cur_pos >= m_ChunkEnds.top()))
    return false;

  //	no more data in file
  //
  if (cur_pos >= m_FileLength)
    return false;

  //	not enough space for a header
  //
  if (cur_pos > (m_FileLength - c_HeaderLength)) {
    DBG_ERROR("Invalid chunk -- not enough space in file to read in header");
    throw chInvalidChunkX();
  }

  //	ok, now maybe we can read the header
  //
  gfFileUtil::Read(m_File, o_Name);
  gfFileUtil::Read(m_File, o_Size);
  gfFileUtil::Read(m_File, o_Version);

  //	figure the end of the current chunk
  //
  fsFileStream::FilePosType cur_end = cur_pos + c_HeaderLength + o_Size;

  //	chunk can't be longer than the file
  //
  if (cur_end > m_FileLength) {
    DBG_ERROR("Invalid chunk -- not enough space in file to read in chunk");
    throw chInvalidChunkX();
  }

  m_ChunkEnds.push(cur_end);
  return true;
}

//--------------------------------------------------------------------
//	FinishChunk will cause the reader to advance to the next position
//	after the end of the current chunk.
//--------------------------------------------------------------------
void
chBinReader::FinishChunk()
{
  DBG_ASSERT(m_ChunkEnds.size() > 0, "Finished a chunk that wasn't started");
  m_File.SetFilePos(m_ChunkEnds.top());
  m_ChunkEnds.pop();
}

//--------------------------------------------------------------------
//	Read functions - These will be overridden to read the data
//	given in the appropriate format.
//	It would have been nice to make these template member functions,
//	but I think there would be problems getting them instantiated
//	so that they are part of the abstract base interface.
//--------------------------------------------------------------------
void
chBinReader::Read(envType::Int8& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::UInt8& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::Int16& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::UInt16& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::Int32& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::UInt32& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::Int64& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::UInt64& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::Float32& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

void
chBinReader::Read(envType::Float64& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");
  gfFileUtil::Read(m_File, o_Val);
}

//--------------------------------------------------------------------
//	This Read reads a string called either "TRUE" or "FALSE" and
//  converts it into a bool value.
//--------------------------------------------------------------------
void
chBinReader::Read(bool& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");

  envType::Int8 int8_value;
  gfFileUtil::Read(m_File, int8_value);
  o_Val = (int8_value == 0) ? false : true;
}

//--------------------------------------------------------------------
//	This Read reads a NULL-terminated single-byte character string.
//--------------------------------------------------------------------
void
chBinReader::Read(std::string& o_String)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");

  o_String.resize(0);

  envType::Int8 character;

  do {
    m_File.Read(1, &character);
    if (character)
      o_String += character;
  } while (character);
}

//--------------------------------------------------------------------
//	This Read reads a single-byte character string with a length
//	given by i_Length.
//--------------------------------------------------------------------
void
chBinReader::Read(std::string& o_Val, int i_Length)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");

  o_Val.resize(i_Length);

  static std::vector<envType::Int8> characters;
  characters.resize(i_Length);

  //	we can read all in one block because each character is one byte - no
  //	endian problems
  //
  m_File.Read(i_Length, &(characters[0]));

  int i;
  for (i = 0; i < i_Length; i++)
    o_Val[i] = characters[i];
}

//--------------------------------------------------------------------
//	This Read reads a NULL-terminated Unicode character string.
//--------------------------------------------------------------------
void
chBinReader::Read(itString& o_Val)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");

  o_Val.Clear();

  envType::Int16 character;

  do {
    gfFileUtil::Read(m_File, character);
    // o_Val += character;	// add the NULL
    if (character)
      o_Val += character; // now, do not add the NULL
  } while (character);
}

//--------------------------------------------------------------------
//	This Read Reads a Unicode character string with a length
//	given by i_Length.
//--------------------------------------------------------------------
void
chBinReader::Read(itString& o_Val, int i_Length)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");

  o_Val.Clear();

  envType::Int16 character;

  int i;
  for (i = 0; i < i_Length; i++) {
    gfFileUtil::Read(m_File, character);
    o_Val += character;
  }
}

//--------------------------------------------------------------------
//	Read in an envAppVersion structure
//--------------------------------------------------------------------
void
chBinReader::Read(envAppVersion& o_Val)
{
  envType::Int32 major, minor, rev, build;
  gfFileUtil::Read(m_File, major);
  gfFileUtil::Read(m_File, minor);
  gfFileUtil::Read(m_File, rev);
  gfFileUtil::Read(m_File, build);
  o_Val.Set(major, minor, rev, build);
}

//--------------------------------------------------------------------
//	Read chunk of binary data; this is only for
//	chBinReader, it is not a function of chReader base class
//--------------------------------------------------------------------
void
chBinReader::Read(void* i_Data, envType::Int64 i_NumBytes)
{
  DBG_ASSERT(m_File.GetFilePos() < m_ChunkEnds.top(), "Read past end of chunk");

  m_File.Read(i_NumBytes, i_Data);
}

//------------------------------------------------------------------------
//	GetLocator returns this file's associated locator
//------------------------------------------------------------------------
const fsLocator&
chBinReader::GetLocator() const
{
  return m_File.GetLocator();
}

//------------------------------------------------------------------------
//	GetFilePos returns the current position of the file pointer.
//------------------------------------------------------------------------
fsFileStream::FilePosType
chBinReader::GetFilePos() const
{
  return m_File.GetFilePos();
}
