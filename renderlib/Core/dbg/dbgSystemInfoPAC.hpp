//****************************************************************************
//  dbgSystemInfoPAC.hpp
//
//	dbgSystemInfoPAC forwards the dbgSystemInfo calls to the PAC
//	components
//
//
//
//****************************************************************************
#ifdef DBG_SYSTEMINFOPAC_HPP
#error dbgSystemInfoPAC.hpp multiply included
#endif
#define DBG_SYSTEMINFOPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//============================================================================
// Debug System Info General PAC interface
//============================================================================

#if ENV_WINDOWS
#include "Core/dbg/dbgSystemInfoPACWin.hpp"
#include "Core/dbg/dbgSystemInfoPACIntrin.hpp"
#else
#error dbgSystemInfoPAC not defined for this platform
#endif
