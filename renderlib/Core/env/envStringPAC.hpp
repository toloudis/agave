//****************************************************************************
//  envStringPAC.hpp
//
//	envStringPAC forwards the envString calls to the PAC
//	components
//
//
//
//****************************************************************************
#ifdef ENV_STRINGPAC_HPP
#error envStringPAC.hpp multiply included
#endif
#define ENV_STRINGPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//============================================================================
//	Here is the interface that must be supported by the PAC component:
//
// namespace envStringPAC
//{
//	std::string WideCharToUTF8(const envType::UInt16 *i_String, int i_Length);
// std::wstring UTF8ToWideChar(const char *i_String, int i_Length);
//
//}
//============================================================================

#if ENV_WINDOWS
#include "Core/env/envStringPACWin.hpp"
#else
#error envStringPAC not defined for this platform
#endif
