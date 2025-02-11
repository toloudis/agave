//****************************************************************************
//  \namespace envStringPACWin.hpp
//
//	envStringPACWin is the declaration for the windows
//	version of the debug log PAC.
//
//
//
//****************************************************************************
#ifdef ENV_STRINGPACWIN_HPP
#error envStringPACWin.hpp multiply included
#endif
#define ENV_STRINGPACWIN_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

#include <string>

//============================================================================
// Debug Log PAC interface
//============================================================================
namespace envStringPAC {
//------------------------------------------------------------------------
// Convert from 16-bit wide unicode string to 8-bit multibyte
// UTF8 unicode string.
//------------------------------------------------------------------------
std::string
WideCharToUTF8(const envType::WChar* i_String, int i_Length);

//------------------------------------------------------------------------
// Convert from 8-bit UTF8 multibyte unicode string to 16-bit wide
// unicode string.
//------------------------------------------------------------------------
std::wstring
UTF8ToWideChar(const char* i_String, int i_Length);
}
