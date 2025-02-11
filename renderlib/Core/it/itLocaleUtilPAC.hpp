/****************************************************************************\
**  itLocaleUtilPAC.hpp
**
**      itLocaleUtilPAC.hpp forwards calls from the itLocaleUtil to the
**	correct PAC component.
**
**
**
\****************************************************************************/

#ifdef IT_LOCALEUTILPAC_HPP
#error itLocaleUtilPAC.hpp multiply included
#endif
#define IT_LOCALEUTILPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

// PAC components should support the following interface:
//
// namespace itLocaleUtilPAC
//{
//	itLocales::Locale GetSystemLocale();
//}

#if ENV_WINDOWS
#include "Core/it/itLocaleUtilPACWin.hpp"
#else
#if ENV_OS == ENV_PS2OS
#include "Core/it/itLocaleUtilPACPS2.hpp"
#else
#if ENV_OS == ENV_XBOXOS
#include "Core/it/itLocaleUtilPACXbox.hpp"
#else
#error itLocaleUtilPACWin not defined for this platform
#endif
#endif
#endif
