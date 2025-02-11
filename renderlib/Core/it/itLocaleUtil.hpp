/****************************************************************************\
**  itLocaleUtil.hpp
**
**      itLocaleUtil.hpp supplies functions to retrieve information about
**	the current locale.
**
**
**
\****************************************************************************/

#ifdef IT_LOCALEUTIL_HPP
#error itLocaleUtil.hpp multiply included
#endif
#define IT_LOCALEUTIL_HPP

#ifndef IT_LOCALES_HPP
#include "Core/it/itLocales.hpp"
#endif

//============================================================================
//============================================================================
namespace itLocaleUtil {
itLocales::Locale
GetSystemLocale();
}
