/*****************************************************************************
**	chWriter.hpp
**
**		chWriter defines the chWriter class, which serves as a base for
**	chunk file writers.
**
**
**
\****************************************************************************/

#ifdef CH_WRITER_HPP
#error chWriter.hpp multiply included
#endif
#define CH_WRITER_HPP

#ifndef CH_DEFS_HPP
#include "Core/ch/chDefs.hpp"
#endif
#ifndef ENV_APPVERSION_HPP
#include "Core/env/envAppVersion.hpp"
#endif
#ifndef IT_STRING_HPP
#include "Core/it/itString.hpp"
#endif

#include <string>

//============================================================================
//	Forward References
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
class chWriter
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  chWriter();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual ~chWriter() = 0;

  //--------------------------------------------------------------------
  //	WriteChunkHeader writes a chunk header in the file at the
  //	current position.
  //--------------------------------------------------------------------
  virtual void WriteChunkHeader(chDefs::Name i_Name, chDefs::Version i_Version, bool i_Container) = 0;

  //--------------------------------------------------------------------
  //	FinishChunk will cause the writer to do whatever it needs to
  //	to finish the chunk (write the binary size in the header, write
  //	a closing brace, etc.)
  //--------------------------------------------------------------------
  virtual void FinishChunk() = 0;

  //--------------------------------------------------------------------
  //	Write functions - These will be overridden to write the data
  //	given in the appropriate format.
  //	It would have been nice to make these template member functions,
  //	but I think there would be problems getting them instantiated
  //	so that they are part of the abstract base interface.
  //--------------------------------------------------------------------
  virtual void Write(envType::Int8 i_Val) = 0;
  virtual void Write(envType::UInt8 i_Val) = 0;
  virtual void Write(envType::Int16 i_Val) = 0;
  virtual void Write(envType::UInt16 i_Val) = 0;
  virtual void Write(envType::Int32 i_Val) = 0;
  virtual void Write(envType::UInt32 i_Val) = 0;
  virtual void Write(envType::Int64 i_Val) = 0;
  virtual void Write(envType::UInt64 i_Val) = 0;
  virtual void Write(envType::Float32 i_Val) = 0;
  virtual void Write(envType::Float64 i_Val) = 0;

  //--------------------------------------------------------------------
  //	This Write writes a string called either "TRUE" or "FALSE" based
  //  on a bool value.
  //--------------------------------------------------------------------
  virtual void Write(bool i_Val) = 0;

  //--------------------------------------------------------------------
  //	This Write writes a NULL-terminated single-byte character string.
  //	It will write the NULL into the file.
  //--------------------------------------------------------------------
  virtual void Write(const char* i_Val) = 0;

  //--------------------------------------------------------------------
  //	This Write writes a single-byte character string with a length
  //	given by i_Length.  It does not write the length into the file;
  //	you must do that yourself.
  //--------------------------------------------------------------------
  virtual void Write(const char* i_Val, int i_Length) = 0;

  //--------------------------------------------------------------------
  //	This Write writes a single-byte character string contained
  //	in the given std::string.  It will write a NULL into the file
  //  to terminate the internal char string.
  // This just turns around and calls this->Write(i_String.c_str() );
  //--------------------------------------------------------------------
  inline void Write(const std::string& i_String);

  //--------------------------------------------------------------------
  // This write writes an array of Unicode strings as NULL terminated UInt16
  // array below.  It is inlined here and requires no override
  //--------------------------------------------------------------------
  void Write(const itString& i_String);

  //--------------------------------------------------------------------
  //	This Write writes a NULL-terminated Unicode character string.  It
  //	will write the NULL into the file.
  //--------------------------------------------------------------------
  virtual void Write(const envType::WChar* i_Val) = 0;

  //--------------------------------------------------------------------
  //	This Write writes a Unicode character string with a length
  //	given by i_Length.  It does not write the length into the file;
  //	you must do that yourself.
  //--------------------------------------------------------------------
  virtual void Write(const envType::WChar* i_Val, int i_Length) = 0;

  //--------------------------------------------------------------------
  //	Write the envAppVersion structure
  //--------------------------------------------------------------------
  virtual void Write(const envAppVersion& i_Val) = 0;

  //--------------------------------------------------------------------
  //	Write chunk of binary data; this is only for
  //	the implementations, it is not a function of chWriter base class
  //--------------------------------------------------------------------
  virtual void Write(const void* i_Data, envType::Int64 i_NumBytes) = 0;

  //------------------------------------------------------------------------
  //	GetLocator returns this file's associated locator
  //------------------------------------------------------------------------
  virtual const fsLocator& GetLocator() const = 0;
};

//--------------------------------------------------------------------
//	This Write writes a single-byte character string contained
//	in the given std::string.  It will write a NULL into the file
//  to terminate the internal char string.
//--------------------------------------------------------------------
// virtual
inline void
chWriter::Write(const std::string& i_String)
{
  this->Write(i_String.c_str());
}
