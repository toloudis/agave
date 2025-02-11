/*****************************************************************************
**	chTxtWriter.hpp
**
**		chTxtWriter is a chWriter which writes ascii chunk files.
**
**
**
\****************************************************************************/
#ifdef CH_TXTWRITER_HPP
#error chTxtWriter.hpp multiply included
#endif
#define CH_TXTWRITER_HPP

#ifndef CH_WRITER_HPP
#include "Core/ch/chWriter.hpp"
#endif

#include <stack>
#include <vector>
#include <map>

//============================================================================
//============================================================================
class gfFileTxt;

//============================================================================
//============================================================================
class chTxtWriter : public chWriter
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  chTxtWriter(gfFileTxt& io_File);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual ~chTxtWriter();

  //--------------------------------------------------------------------
  //	Ascii Writer and Reader need map from string to chDefs::Name
  //  in order to read and write useful chunk names.
  //  The map is pointed to, not owned by the chunk writer.
  //--------------------------------------------------------------------
  void SetMap(std::map<std::string, chDefs::Name>* i_pMap);
  std::map<std::string, chDefs::Name>* GetMap() const;

  //--------------------------------------------------------------------
  //	WriteChunkHeader writes a chunk header in the file at the
  //	current position.
  //--------------------------------------------------------------------
  virtual void WriteChunkHeader(chDefs::Name i_Name, chDefs::Version i_Version, bool i_Container);

  //--------------------------------------------------------------------
  //	FinishChunk will cause the writer to compute the size of the
  //	chunk and finish writing the header.
  //--------------------------------------------------------------------
  virtual void FinishChunk();

  //--------------------------------------------------------------------
  //	Write functions - These write the data to the chunk file.
  //--------------------------------------------------------------------
  virtual void Write(envType::Int8 i_Val);
  virtual void Write(envType::UInt8 i_Val);
  virtual void Write(envType::Int16 i_Val);
  virtual void Write(envType::UInt16 i_Val);
  virtual void Write(envType::Int32 i_Val);
  virtual void Write(envType::UInt32 i_Val);
  virtual void Write(envType::Int64 i_Val);
  virtual void Write(envType::UInt64 i_Val);
  virtual void Write(envType::Float32 i_Val);
  virtual void Write(envType::Float64 i_Val);

  //--------------------------------------------------------------------
  //	This Write writes a string called either "TRUE" or "FALSE" based
  //  on a bool value.
  //--------------------------------------------------------------------
  virtual void Write(bool i_Val);

  //--------------------------------------------------------------------
  //	This Write writes a NULL-terminated single-byte character string.
  //	It will write the NULL into the file.
  //--------------------------------------------------------------------
  virtual void Write(const char* i_Val);

  //--------------------------------------------------------------------
  //	This Write writes a single-byte character string with a length
  //	given by i_Length.  It does not write the length into the file;
  //	you must do that yourself.
  //--------------------------------------------------------------------
  virtual void Write(const char* i_Val, int i_Length);

  //--------------------------------------------------------------------
  //	This Write writes a NULL-terminated Unicode character string.  It
  //	will write the NULL into the file.
  //--------------------------------------------------------------------
  virtual void Write(const envType::WChar* i_Val);

  //--------------------------------------------------------------------
  //	This Write writes a Unicode character string with a length
  //	given by i_Length.  It does not write the length into the file;
  //	you must do that yourself.
  //--------------------------------------------------------------------
  virtual void Write(const envType::WChar* i_Val, int i_Length);

  //--------------------------------------------------------------------
  //	Write the envAppVersion structure
  //--------------------------------------------------------------------
  virtual void Write(const envAppVersion& i_Val);

  //--------------------------------------------------------------------
  //	Write chunk of binary data; this is only for
  //	the implementations, it is not a function of chWriter base class
  //--------------------------------------------------------------------
  virtual void Write(const void* i_Data, envType::Int64 i_NumBytes){};

  //------------------------------------------------------------------------
  //	GetLocator returns this file's associated locator
  //------------------------------------------------------------------------
  const fsLocator& GetLocator() const;

private:
  gfFileTxt& m_File;
  int m_NumContainers;
  bool m_bCurrentContainer;
  int m_Indentation;
  std::map<std::string, chDefs::Name>* m_pMap;
};
