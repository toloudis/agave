//****************************************************************************
//  dbgAssertPAC.hpp
//
//	dbgAssertPAC forwards the dbgAssert calls to the PAC
//	components.
//
//
//
//****************************************************************************
#ifdef DBG_ASSERTPAC_HPP
#error dbgAssertPAC.hpp multiply included
#endif
#define DBG_ASSERTPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//============================================================================
//	Here is the interface that must be supported by the PAC component:
//
// namespace dbgAssertPAC
//{
//	void OKMessage(const char* i_Text);
//
//	The return value here signifies which button (abort/continue) the
//	user pressed.  true == abort.
//	bool AbortContinueMessage(const char* i_Text);
//
//	void Init();
//	void CleanUp();
//}
//============================================================================
#if ENV_WINDOWS
#include "Core/dbg/dbgAssertPACWin.hpp"
#else
#if ENV_OS == ENV_PS2OS
#include "Core/dbg/dbgAssertPACPS2.hpp"
#else
#if ENV_OS == ENV_XBOXOS
#include "Core/dbg/dbgAssertPACXbox.hpp"
#else
#error dbgAssertPAC not defined for this platform
#endif
#endif
#endif
