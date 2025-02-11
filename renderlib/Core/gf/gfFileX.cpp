/*****************************************************************************
**  gfFileX.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/gf/gfFileX.hpp"

//----------------------------------------------------------------------------
//	gfInvalidFileBinX is thrown when the header of a File is invalid
//----------------------------------------------------------------------------
gfInvalidFileBinX::gfInvalidFileBinX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//----------------------------------------------------------------------------
// Returns error message string
//----------------------------------------------------------------------------
std::string
gfInvalidFileBinX::GetErrorMessage() const
{
  return "Invalid file format: " + GetLocatorString();
}

//------------------------------------------------------------------------
//	gfInvalidTokenFileTxtX is thrown when errors are found reading
//	tokens from an ascii file.
//------------------------------------------------------------------------
gfInvalidTokenFileTxtX::gfInvalidTokenFileTxtX(const fsLocator& i_Locator)
  : fsFileExceptionX(i_Locator)
{
}

//----------------------------------------------------------------------------
// Returns error message string
//----------------------------------------------------------------------------
std::string
gfInvalidTokenFileTxtX::GetErrorMessage() const
{
  return "Invalid token in file format while parsing: " + GetLocatorString();
}
