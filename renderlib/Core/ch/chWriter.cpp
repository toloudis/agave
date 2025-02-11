/*****************************************************************************
**  chWriter.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ch/chWriter.hpp"

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chWriter::chWriter() {}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
chWriter::~chWriter() {}

//--------------------------------------------------------------------
// This write writes an array of Unicode strings as NULL terminated UInt16
// array below.  It is inlined here and requires no override
//--------------------------------------------------------------------
void
chWriter::Write(const itString& i_String)
{
  DBG_ASSERT(NULL == i_String[i_String.GetLength() - 1], "This must be NULL terminated!");
  this->Write(i_String.GetString());
}