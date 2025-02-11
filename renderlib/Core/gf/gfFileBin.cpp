/*****************************************************************************
**	gfFileBin.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/Gf/gfFileBin.hpp"

#include "Core/Fs/fsFilePosSaver.hpp"
#include "Core/Gf/gfFileX.hpp"
#include "Core/Gf/gfPaths.hpp"
#include "Core/Gf/gfFileTranslationMgr.hpp"

//------------------------------------------------------------------------
//	Constructor
//------------------------------------------------------------------------
gfFileBin::gfFileBin(const fsLocator& i_Locator, fsFileStream::AccessType i_DesiredAccess, EndianMode i_Mode)
  : gfFile(i_Locator, i_DesiredAccess)
  , m_Mode(i_Mode)
{
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
gfFileBin::~gfFileBin() {}

//------------------------------------------------------------------------
//	Read reads i_NumBytes into the buffer.  If the file is too short
//	to read	i_NumBytes, it will read as many as it can.  The return
//	value is the number actually read into the o_Buffer.  This function
//	also advances the file pointer.
//------------------------------------------------------------------------
int
gfFileBin::Read(envType::Int64 i_NumBytes, void* o_Buffer)
{
  return m_FileStream->Read(i_NumBytes, o_Buffer);
}

//------------------------------------------------------------------------
//	Write writes i_NumBytes from o_Buffer into the file.  The file
//	pointer will also be advanced.
//------------------------------------------------------------------------
void
gfFileBin::Write(envType::Int64 i_NumBytes, const void* i_Buffer)
{
  DBG_ASSERT(m_FileStream != NULL, "Must have a fsFileStream to write");
  m_FileStream->Write(i_NumBytes, i_Buffer);
}

//------------------------------------------------------------------------
//	ReadHeader reads the file's header into the o_Header passed to it. If
//	o_Header is NULL, the file pointer is simply moved past the file header
//	Throws: gfInvalidFileBinX
//------------------------------------------------------------------------
void
gfFileBin::ReadHeader(gfFileBin::Header* o_Header)
{
  // move file pointer past signature, 4 bytes is the size of the signature
  char Sig[4];
  std::string strGigaSig("GWFB");
  std::string strTeraSig("TWFB");
  this->Read(4, &Sig);
  std::string strReadSig(Sig, 4);
  if (strGigaSig.compare(strReadSig) && strTeraSig.compare(strReadSig)) {
    // not a gfFileBin, throw gfInvalidFileBinX
    throw gfInvalidFileBinX(GetLocator());
  }

  // now actually read the header
  if (NULL != o_Header) {
    this->Read(sizeof(gfFileBin::Header), o_Header);
  } else {
    // NULL was passed in, so just move the filepointer
    this->SetFilePos(sizeof(gfFileBin::Header), fsFileStream::e_Current);
  }
}

//------------------------------------------------------------------------
//	WriteHeader writes the given i_Header to the file. gfFileBin::m_Mode
//	is used instead of the value passed in i_Header::m_Mode since a
//	gfFileBin's mode can't altered after construction. The second version of
//	this function is an inline convenience.
//------------------------------------------------------------------------
void
gfFileBin::WriteHeader(const gfFileBin::Header& i_Header)
{
  // write signature
  this->Write(4, "TWFB");

  // set header's mode to this filebin's mode
  gfFileBin::Header Header = i_Header;
  Header.m_Mode = m_Mode;

  // write header
  this->Write(sizeof(gfFileBin::Header), &Header);
  DBG_ASSERT(GetFilePos() == GetHeaderSize(), "Inconsistency in writing header");
}
int
gfFileBin::GetHeaderSize()
{
  return (4 + sizeof(gfFileBin::Header));
}