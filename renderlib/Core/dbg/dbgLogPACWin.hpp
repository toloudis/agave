// #error dbgLogPACWin.hpp is obsolete

//****************************************************************************
//  \namespace dbgLogPACWin.hpp
//
//	dbgLogPACWin is the declaration for the windows
//	version of the debug log PAC.
//
//
//
//****************************************************************************
#ifdef DBG_LOGPACWIN_HPP
#error dbgLogPACWin.hpp multiply included
#endif
#define DBG_LOGPACWIN_HPP

#include <string>

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

//============================================================================
// Debug Log PAC interface
//============================================================================
namespace dbgLogPAC {
//------------------------------------------------------------------------
//	WriteLog writes the given text to the platform debug log.
//
// \param i_Text string to write to the log file
//
// \return void
//------------------------------------------------------------------------
void
WriteLog(const char* i_Text);

//------------------------------------------------------------------------
//	UnicodetoANSI is a convenience function for debug logging that will turn an
//	itString into a std::string.
//
// \param i_String the original string
// \param i_Length the length of the original string
//
// \return the converted string
//------------------------------------------------------------------------
std::string
UnicodetoANSI(const envType::WChar* i_String, int i_Length);

//------------------------------------------------------------------------
//	Don't call Init() yourself; it is called by the package Init().
//
// \return void
//------------------------------------------------------------------------
void
Init();

//------------------------------------------------------------------------
//	Don't call CleanUp() yourself; it is called by the package CleanUp().
//
// \return void
//------------------------------------------------------------------------
void
CleanUp() throw();
}
