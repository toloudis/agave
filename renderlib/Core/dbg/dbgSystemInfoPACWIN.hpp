//****************************************************************************
//  \namespace dbgSystemInfoPACWin.hpp
//
//	dbgSystemInfoPACWin is the declaration for the windows
//	version of the debug sytem info PAC.
//
//
//
//****************************************************************************
#ifdef DBG_SYSTEMINFOPACWIN_HPP
#error dbgSystemInfoPACWin.hpp multiply included
#endif
#define DBG_SYSTEMINFOPACWIN_HPP

#include <string>

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

//============================================================================
// Debug System Info PAC interface for Windows
//============================================================================
namespace dbgSystemInfoPAC {
//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getUserName();

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getComputerName();

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
getMemory();

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getOS();

}