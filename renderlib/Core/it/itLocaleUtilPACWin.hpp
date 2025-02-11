/****************************************************************************\
**  itLocaleUtilPACWin.hpp
**
**      itLocaleUtilPACWin.hpp defines the locale util PAC for windows.
**
**
**
\****************************************************************************/

#ifdef IT_LOCALEUTILPACWIN_HPP
#error itLocaleUtilPACWin.hpp multiply included
#endif
#define IT_LOCALEUTILPACWIN_HPP

#ifndef IT_LOCALES_HPP
#include "Core/it/itLocales.hpp"
#endif

//============================================================================
//============================================================================
namespace itLocaleUtilPAC {
itLocales::Locale
GetSystemLocale();
}
