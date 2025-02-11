// #error dbgLogPAC.hpp is obsolete

//****************************************************************************
//  dbgLogPAC.hpp
//
//	dbgLogPAC forwards the dbgLog calls to the PAC
//	components
//
//
//
//****************************************************************************
#ifdef DBG_LOGPAC_HPP
#error dbgLogPAC.hpp multiply included
#endif
#define DBG_LOGPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//============================================================================
//	Here is the interface that must be supported by the PAC component:
//
// namespace dbgLogPAC
//{
//	void WriteLog(const char* i_Text);
//
//	void Init();
//	void CleanUp();
//}
//============================================================================

#if ENV_WINDOWS
#include "Core/dbg/dbgLogPACWin.hpp"
#else
#if ENV_OS == ENV_PS2OS
#include "Core/dbg/dbgLogPACPS2.hpp"
#else
#if ENV_OS == ENV_XBOXOS
#include "Core/dbg/dbgLogPACXbox.hpp"
#else
#error dbgLogPAC not defined for this platform
#endif
#endif
#endif
