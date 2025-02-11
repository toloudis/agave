//****************************************************************************
//  envString.cpp
//
//      see .hpp
//
//
//
//****************************************************************************
#include "Core/env/envString.hpp"
#include "Core/env/envStringPAC.hpp"

// #include <vector>
// #include <stdarg.h>
// #include <stdio.h>
// #include <time.h>

//============================================================================
//============================================================================
namespace envString {
//------------------------------------------------------------------------
// Convert from 16-bit wide unicode string to 8-bit multibyte
// UTF8 unicode string.
//------------------------------------------------------------------------
std::string
WideCharToUTF8(const envType::WChar* i_String, int i_Length)
{
  return envStringPAC::WideCharToUTF8(i_String, i_Length);
}

//------------------------------------------------------------------------
// Convert a wide wstring to UTF8 std::string
//------------------------------------------------------------------------
std::string
WideCharToUTF8(const std::wstring& i_String)
{
  return envStringPAC::WideCharToUTF8(i_String.c_str(), i_String.length());
}

//------------------------------------------------------------------------
// Convert from 8-bit UTF8 multibyte unicode string to 16-bit wide
// unicode string.
//------------------------------------------------------------------------
std::wstring
UTF8ToWideChar(const char* i_String, int i_Length)
{
  return envStringPAC::UTF8ToWideChar(i_String, i_Length);
}

//------------------------------------------------------------------------
// Convert a UTF8 std::string to wide wstring
//------------------------------------------------------------------------
std::wstring
UTF8ToWideChar(const std::string& i_String)
{
  return envStringPAC::UTF8ToWideChar(i_String.c_str(), i_String.length());
}
}
