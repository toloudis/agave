//****************************************************************************
//	\file dbgStringStream.cpp
//
//  see .hpp
//
//
//
//****************************************************************************
#include "Core/dbg/dbgStringStream.hpp"

#include <Windows.h>

//============================================================================
// Debug String Stream
//============================================================================

//----------------------------------------------------------------------------
// default constructor
//----------------------------------------------------------------------------
dbgStreamBuf::dbgStreamBuf()
  : std::stringbuf()
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
dbgStreamBuf::~dbgStreamBuf()
{
  sync();
}

//----------------------------------------------------------------------------
// The sync function takes care of the
// streams buffer and places it into the debug window
//----------------------------------------------------------------------------
int
dbgStreamBuf::sync()
{
  // bga - no need to check for UNICODE here. We have a UTF 8-bit char stream here
  //  so we can only use the "W" variation, even in ANSI builds. But, we have to
  //  promote the multi-byte string to a wide string to output.
  std::wstring wide_str = envString::UTF8ToWideChar(str());
  ::OutputDebugStringW(wide_str.c_str());

  str(std::basic_string<char>()); // Clear the string buffer

  return 0;
}
