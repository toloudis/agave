/*****************************************************************************
**  chExceptionX.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ch/chExceptionX.hpp"

//--------------------------------------------------------------------
// Returns error message string
//--------------------------------------------------------------------
std::string
chInvalidChunkX::GetErrorMessage() const
{
  return "Invalid Chunk while reading file";
}
