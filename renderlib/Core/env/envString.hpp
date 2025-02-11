//****************************************************************************
//	\file envString.hpp
//
// Conversion routines for converting between wide UTF-16 strings and
// char based UTF-8 strings.
//
//
//
//****************************************************************************
#ifdef ENV_STRING_HPP
#error envString.hpp multiply included
#endif
#define ENV_STRING_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

#include <string>

//============================================================================
// envString interface
//============================================================================
namespace envString {
//------------------------------------------------------------------------
// Convert from 16-bit wide unicode string to 8-bit multibyte
// UTF8 unicode string.
//------------------------------------------------------------------------
std::string
WideCharToUTF8(const envType::WChar* i_String, int i_Length);

//------------------------------------------------------------------------
// Convert a wide wstring to UTF8 std::string
//------------------------------------------------------------------------
std::string
WideCharToUTF8(const std::wstring& i_String);

//------------------------------------------------------------------------
// Convert from 8-bit UTF8 multibyte unicode string to 16-bit wide
// unicode string.
//------------------------------------------------------------------------
std::wstring
UTF8ToWideChar(const char* i_String, int i_Length);

//------------------------------------------------------------------------
// Convert a UTF8 std::string to wide wstring
//------------------------------------------------------------------------
std::wstring
UTF8ToWideChar(const std::string& i_String);

}
