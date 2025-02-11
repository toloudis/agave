/****************************************************************************\
**  itLocaleUtilPACWin.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/it/itLocaleUtilPACWin.hpp"

#include <windows.h>

//============================================================================
//============================================================================
namespace itLocaleUtilPAC {

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
itLocales::Locale
GetSystemLocale()
{
  LCID locale_id = ::GetUserDefaultLCID();
  WORD composite_lang_id = LANGIDFROMLCID(locale_id);
  WORD lang_id = PRIMARYLANGID(composite_lang_id);

  switch (lang_id) {
    case LANG_AFRIKAANS:
      return itLocales::e_Afrikaans;
      break;
    case LANG_KONKANI:
      return itLocales::e_Konkani;
      break;
    case LANG_ALBANIAN:
      return itLocales::e_Albanian;
      break;
    case LANG_KOREAN:
      return itLocales::e_Korean;
      break;
    case LANG_ARABIC:
      return itLocales::e_Arabic;
      break;
    case LANG_LATVIAN:
      return itLocales::e_Latvian;
      break;
    case LANG_ARMENIAN:
      return itLocales::e_Armenian;
      break;
    case LANG_LITHUANIAN:
      return itLocales::e_Lithuanian;
      break;
    case LANG_ASSAMESE:
      return itLocales::e_Assamese;
      break;
    case LANG_MACEDONIAN:
      return itLocales::e_Macedonian;
      break;
    case LANG_AZERI:
      return itLocales::e_Azeri;
      break;
    case LANG_MALAY:
      return itLocales::e_Malay;
      break;
    case LANG_BASQUE:
      return itLocales::e_Basque;
      break;
    case LANG_MALAYALAM:
      return itLocales::e_Malayalam;
      break;
    case LANG_BELARUSIAN:
      return itLocales::e_Belarusian;
      break;
    case LANG_MANIPURI:
      return itLocales::e_Manipuri;
      break;
    case LANG_BENGALI:
      return itLocales::e_Bengali;
      break;
    case LANG_MARATHI:
      return itLocales::e_Marathi;
      break;
    case LANG_BULGARIAN:
      return itLocales::e_Bulgarian;
      break;
    case LANG_NEPALI:
      return itLocales::e_Nepali;
      break;
    case LANG_CATALAN:
      return itLocales::e_Catalan;
      break;
    case LANG_NEUTRAL:
      return itLocales::e_Neutral;
      break;
    case LANG_CHINESE:
      return itLocales::e_Chinese;
      break;
    case LANG_NORWEGIAN:
      return itLocales::e_Norwegian;
      break;
    case LANG_CROATIAN:
      return itLocales::e_Croatian;
      break;
    case LANG_ORIYA:
      return itLocales::e_Oriya;
      break;
    case LANG_CZECH:
      return itLocales::e_Czech;
      break;
    case LANG_POLISH:
      return itLocales::e_Polish;
      break;
    case LANG_DANISH:
      return itLocales::e_Danish;
      break;
    case LANG_PORTUGUESE:
      return itLocales::e_Portuguese;
      break;
    case LANG_DUTCH:
      return itLocales::e_Dutch;
      break;
    case LANG_PUNJABI:
      return itLocales::e_Punjabi;
      break;
    case LANG_ENGLISH:
      return itLocales::e_English;
      break;
    case LANG_ROMANIAN:
      return itLocales::e_Romanian;
      break;
    case LANG_ESTONIAN:
      return itLocales::e_Estonian;
      break;
    case LANG_RUSSIAN:
      return itLocales::e_Russian;
      break;
    case LANG_FAEROESE:
      return itLocales::e_Faeroese;
      break;
    case LANG_SANSKRIT:
      return itLocales::e_Sanskrit;
      break;
    case LANG_FARSI:
      return itLocales::e_Farsi;
      break;
    // case LANG_SERBIAN:	// apparently same as croatian
    //	return itLocales::e_Serbian;
    // break;
    case LANG_FINNISH:
      return itLocales::e_Finnish;
      break;
    case LANG_SINDHI:
      return itLocales::e_Sindhi;
      break;
    case LANG_FRENCH:
      return itLocales::e_French;
      break;
    case LANG_SLOVAK:
      return itLocales::e_Slovak;
      break;
    case LANG_GEORGIAN:
      return itLocales::e_Georgian;
      break;
    case LANG_SLOVENIAN:
      return itLocales::e_Slovenian;
      break;
    case LANG_GERMAN:
      return itLocales::e_German;
      break;
    case LANG_SPANISH:
      return itLocales::e_Spanish;
      break;
    case LANG_GREEK:
      return itLocales::e_Greek;
      break;
    case LANG_SWAHILI:
      return itLocales::e_Swahili;
      break;
    case LANG_GUJARATI:
      return itLocales::e_Gujarati;
      break;
    case LANG_SWEDISH:
      return itLocales::e_Swedish;
      break;
    case LANG_HEBREW:
      return itLocales::e_Hebrew;
      break;
    case LANG_TAMIL:
      return itLocales::e_Tamil;
      break;
    case LANG_HINDI:
      return itLocales::e_Hindi;
      break;
    case LANG_TATAR:
      return itLocales::e_Tatar;
      break;
    case LANG_HUNGARIAN:
      return itLocales::e_Hungarian;
      break;
    case LANG_TELUGU:
      return itLocales::e_Telugu;
      break;
    case LANG_ICELANDIC:
      return itLocales::e_Icelandic;
      break;
    case LANG_THAI:
      return itLocales::e_Thai;
      break;
    case LANG_INDONESIAN:
      return itLocales::e_Indonesian;
      break;
    case LANG_TURKISH:
      return itLocales::e_Turkish;
      break;
    case LANG_ITALIAN:
      return itLocales::e_Italian;
      break;
    case LANG_JAPANESE:
      return itLocales::e_Japanese;
      break;
    case LANG_URDU:
      return itLocales::e_Urdu;
      break;
    case LANG_KANNADA:
      return itLocales::e_Kannada;
      break;
    case LANG_UZBEK:
      return itLocales::e_Uzbek;
      break;
    case LANG_KASHMIRI:
      return itLocales::e_Kashmiri;
      break;
    case LANG_VIETNAMESE:
      return itLocales::e_Vietnamese;
      break;
    case LANG_KAZAK:
      return itLocales::e_Kazak;
      break;

    default:
      return itLocales::e_Unknown;
      break;
  }
}

}
