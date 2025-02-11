/*****************************************************************************
**  chXMLReader.hpp
**
**      chXMLReader defines the chXMLReader class, which is a chReader that
**	operates on XML files.
**
**
**
\****************************************************************************/
#ifdef CH_XMLREADER_HPP
#error chXMLReader.hpp multiply included
#endif
#define CH_XMLREADER_HPP

#ifndef CH_READER_HPP
#include "Core/ch/chReader.hpp"
#endif

#include <stack>
#include <vector>
#include <map>

//============================================================================
//	Forward References
//============================================================================
class gfFileXML;

//============================================================================
//============================================================================
class chXMLReader : public chReader
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  chXMLReader(gfFileXML& io_File);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual ~chXMLReader();

  //--------------------------------------------------------------------
  //	Ascii Writer and Reader need map from string to chDefs::Name
  //  in order to read and write useful chunk names.
  //  The map is pointed to, not owned by the chunk reader.
  //--------------------------------------------------------------------
  void SetMap(std::map<std::string, chDefs::Name>* i_pMap);
  std::map<std::string, chDefs::Name>* GetMap() const;

  //--------------------------------------------------------------------
  //	ReadChunkHeader reads a chunk header in the file at the
  //	current position.  If there is a problem with the header,
  //	it will throw a chInvalidChunkX.  Returns true if more chunks to
  //	read, false if no more.
  //	The size is the size of the chunk not including the header.
  //--------------------------------------------------------------------
  virtual bool ReadChunkHeader(chDefs::Name& o_Name, chDefs::Version& o_Version, chDefs::Size& o_Size);

  //--------------------------------------------------------------------
  //	FinishChunk will cause the reader to advance to the next position
  //	after the end of the current chunk.  Every ReadChunkHeader
  //	must be matched by a FinishChunk.
  //--------------------------------------------------------------------
  virtual void FinishChunk();

  //--------------------------------------------------------------------
  //	Read functions - These will be overridden to read the data
  //	given in the appropriate format.
  //	It would have been nice to make these template member functions,
  //	but I think there would be problems getting them instantiated
  //	so that they are part of the abstract base interface.
  //--------------------------------------------------------------------
  virtual void Read(envType::Int8& o_Val);
  virtual void Read(envType::UInt8& o_Val);
  virtual void Read(envType::Int16& o_Val);
  virtual void Read(envType::UInt16& o_Val);
  virtual void Read(envType::Int32& o_Val);
  virtual void Read(envType::UInt32& o_Val);
  virtual void Read(envType::Int64& o_Val);
  virtual void Read(envType::UInt64& o_Val);
  virtual void Read(envType::Float32& o_Val);
  virtual void Read(envType::Float64& o_Val);

  //--------------------------------------------------------------------
  //	This Read reads a string called either "TRUE" or "FALSE" and
  //  converts it into a bool value.
  //--------------------------------------------------------------------
  virtual void Read(bool& o_Val);

  //--------------------------------------------------------------------
  //	This Read reads a NULL-terminated single-byte character string.
  //--------------------------------------------------------------------
  virtual void Read(std::string& o_Val);

  //--------------------------------------------------------------------
  //	This Read Reads a single-byte character string with a length
  //	given by i_Length.
  //--------------------------------------------------------------------
  virtual void Read(std::string& o_Val, int i_Length);

  //--------------------------------------------------------------------
  //	This Read reads a NULL-terminated Unicode character string.
  //--------------------------------------------------------------------
  virtual void Read(itString& o_Val);

  //--------------------------------------------------------------------
  //	This Read Reads a Unicode character string with a length
  //	given by i_Length.
  //--------------------------------------------------------------------
  virtual void Read(itString& o_Val, int i_Length);

  //--------------------------------------------------------------------
  //	Read in an envAppVersion structure
  //--------------------------------------------------------------------
  virtual void Read(envAppVersion& o_Val);

  //--------------------------------------------------------------------
  //	Read chunk of binary data; this is only for
  //	chBinReader, it is not a function of chReader base class
  //--------------------------------------------------------------------
  virtual void Read(void* i_Data, envType::Int64 i_NumBytes){};

  //--------------------------------------------------------------------
  // This function asks if it is safe to read binary data in a
  // single step with this reader. This applies to the implementation
  // in the function above Read(void*,int). Binary file formats
  // should return true and ascii file formats should return false.
  //--------------------------------------------------------------------
  virtual bool CanReadBinaryData() const { return false; }

  //--------------------------------------------------------------------
  //	GetLocator returns this file's associated locator
  //--------------------------------------------------------------------
  const fsLocator& GetLocator() const;

  //--------------------------------------------------------------------
  //	GetFilePos returns the current position of the file pointer.
  //--------------------------------------------------------------------
  fsFileStream::FilePosType GetFilePos() const;

private:
  gfFileXML& m_File;
  int m_NumContainers;
  bool m_bCurrentContainer;
  std::map<std::string, chDefs::Name>* m_pMap;
};
