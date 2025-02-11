//****************************************************************************
//  \namespace dbgSystemInfoPACWin.hpp
//
//	dbgSystemInfoPACIntrin is the declaration for windows intrinsic modules
// that cannot exist in the same level as windows.h
//
//
//
//****************************************************************************
#ifdef DBG_SYSTEMINFOPACINTRIN_HPP
#error dbgSystemInfoPACWin.hpp multiply included
#endif
#define DBG_SYSTEMINFOPACINTRIN_HPP

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
std::string
getCPUInfo();
}