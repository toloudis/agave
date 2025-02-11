/****************************************************************************\
**	itStringUtil.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/it/itStringUtil.hpp"

#include "Core/Env/envString.hpp"

#include <cctype> // for toupper
#include <string>
#include <algorithm>
#include <stdio.h>
#include <windows.h>
#include <iomanip>
#include <sstream>

//============================================================================
//============================================================================
namespace itStringUtil {
//------------------------------------------------------------------------
//	GetStdString takes the text in i_String, and returns it in a std::string
//	NOTE: an itString is a unicode structure, whereas a std::string is not,
//	therefore, the results of this operation are unpredictable for any strings
//	that have information in the second byte of the multi-byte character,
//	so this function should only be used on those itStrings that are known
//	to only include ANSI compatible characters, (for example, directory names
//	we write in english)
//------------------------------------------------------------------------
std::string
GetStdString(const itString& i_String)
{
  // OLD VERSION - ANSI
  //
  // int i;
  // std::string ReturnString;
  // ReturnString.resize(i_String.GetLength());
  // for (i = 0; i < i_String.GetLength(); ++i)
  //{
  //	ReturnString[i] = i_String.GetString()[i];
  //}
  // return ReturnString;

  // SECOND PASS - MULTIBYTE WITH ANSI CODE PAGE
  // int num_chars;
  // char work_string[MAX_PATH];
  // std::string ReturnString;
  // num_chars = ::WideCharToMultiByte(	::GetACP(),		// ANSI code page
  //									0,				// no
  //"lo-performance" flags 									i_String.GetString(),
  // i_String.GetLength(), 									work_string, 									MAX_PATH,		// size of
  // target buffer 									NULL, 									NULL);

  //// for some reason, this function doesn't seem to terminate the work_string
  // work_string[num_chars] = 0;

  // ReturnString += work_string;
  // return ReturnString;

  // UTF8 VERSION - PRODUCES 8 BIT MUTIBYTE UNICODE
  return envString::WideCharToUTF8(i_String.GetString(), i_String.GetLength());
}

void
GetStdString(const itString& i_String, char* o_szBuffer, int& io_NumChars)
{
  std::string string = GetStdString(i_String);

  DBG_ASSERT(io_NumChars >= string.size() + 1, "Invalid sized output buffer to receive string!");

  strcpy(o_szBuffer, string.c_str());
  io_NumChars = string.size();
}

//------------------------------------------------------------------------
//	Convert a itString into a double data type.
//------------------------------------------------------------------------
double
GetDouble(const itString& i_String)
{
  // Convert the 16-bit string into an 8-bit string.
  std::string strEightBit(GetStdString(i_String));

  // Convert the 8-bit string into a double.
  return atof(strEightBit.data());
}

//------------------------------------------------------------------------
//	Convert a itString into an int data type.
//------------------------------------------------------------------------
int
GetInt(const itString& i_String)
{
  // Convert the 16-bit string into an 8-bit string.
  std::string strEightBit(GetStdString(i_String));

  // Convert the 8-bit string into an int
  return atoi(strEightBit.data());
}

//------------------------------------------------------------------------
// ToLower will convert an itString to all lower case.  Note:  This
// only works with ASCII text today!
// The return is a reference to the input/output parameter, a convenience for
// quick conversions into function parameters
//------------------------------------------------------------------------
const itString&
ToLower(itString& io_String)
{
  std::string work_string = GetStdString(io_String);
  std::transform(work_string.begin(), work_string.end(), work_string.begin(), tolower);
  io_String = work_string.c_str();
  return io_String;
}

//------------------------------------------------------------------------
//	This ToLower works with std::string.
//------------------------------------------------------------------------
const std::string&
ToLower(std::string& io_String)
{
  int count = io_String.size();
  int i;
  for (i = 0; i < count; ++i) {
    char c = tolower(io_String[i]);
    if (c)
      io_String[i] = c;
  }

  return io_String;
}

//------------------------------------------------------------------------
//	Compare the two strings ignoring case.
//------------------------------------------------------------------------
bool
Equal(itString& i_String1, itString& i_String2)
{
  itString s1 = i_String1;
  ToLower(s1);
  itString s2 = i_String2;
  ToLower(s2);
  return (s1 == s2);
}

//------------------------------------------------------------------------
//	Compare the two strings ignoring case.
//------------------------------------------------------------------------
bool
Equal(const itString& i_String1, const itString& i_String2)
{
  itString s1 = i_String1;
  ToLower(s1);
  itString s2 = i_String2;
  ToLower(s2);
  return (s1 == s2);
}
bool
LessThan(const itString& i_String1, const itString& i_String2)
{
  itString s1 = i_String1;
  ToLower(s1);
  itString s2 = i_String2;
  ToLower(s2);
  return (s1 < s2);
}
bool
LessThanOrEqual(const itString& i_String1, const itString& i_String2)
{
  itString s1 = i_String1;
  ToLower(s1);
  itString s2 = i_String2;
  ToLower(s2);
  return (s1 <= s2);
}
bool
GreaterThan(const itString& i_String1, const itString& i_String2)
{
  itString s1 = i_String1;
  ToLower(s1);
  itString s2 = i_String2;
  ToLower(s2);
  return (s1 > s2);
}
bool
GreaterThanOrEqual(const itString& i_String1, const itString& i_String2)
{
  itString s1 = i_String1;
  ToLower(s1);
  itString s2 = i_String2;
  ToLower(s2);
  return (s1 >= s2);
}

//----------------------------------------------------------------------------
//	Break apart the filename into components.  The assumed format of the
//	filename is TextXXX.ext, where "Text" are letters, "XXX" is a number, and
//	"ext" is the extension.
//
//	1) base name (minus number)
//	2) number
//	3) extension
//
//	return the number of digits in the number part of the filename
//----------------------------------------------------------------------------
int
Breakup_Filename(const itString& i_Filename, itString& o_BaseFilename, itString& o_Ext, int& o_InitialNumber)
{
  if (i_Filename.GetLength() == 0)
    return 0;

  i_Filename.GetExtension(o_Ext);

  //	rework the base to not have the numbers associated.
  i_Filename.GetBase(o_BaseFilename);

  int number_digits = 0;
  int i, index = -1;
  for (i = o_BaseFilename.GetLength() - 1; i >= 0; --i) {
    if (o_BaseFilename[i] < '0' || o_BaseFilename[i] > '9') {
      //	if there are no numbers, just bail out
      //	if there is a number, then mark where it ends
      if (i < (o_BaseFilename.GetLength() - 1))
        index = i;
      break;
    } else {
      number_digits++;
    }
  }

  if (index == -1) {
    o_InitialNumber = 0;
  } else {
    itString numstr(index + 1, (o_BaseFilename.GetLength() - 1) - index, o_BaseFilename);
    o_InitialNumber = itStringUtil::GetInt(numstr);
    o_BaseFilename[index + 1] = 0;
    o_BaseFilename.SetLength(index + 1);
  }

  return number_digits;
}

//	// Look for a stream of bitmap files:
//	int iNumDigits = 0;
//	if ((pszBasename[iPos-1] == '0') || (pszBasename[iPos-1] == '1'))
//	{
//		if (pszBasename[iPos-1] == '0') initial = 0;
//		else initial = 1;
//
//		// Keep going back in string, looking
//		// for zeroes to find base name:
//		iNumDigits=1;
//		const char *pCh;
//		for (pCh = &pszBasename[iPos-2]; pCh >= pszBasename; pCh--)
//		{
//			if ( *pCh == '0' )
//				iNumDigits++;
//			else
//				break;
//		}
//		// Truncate basename base name:
//		pszBasename[iPos - iNumDigits] = '\0';

//----------------------------------------------------------------------------
//	Build a filename based on the parts gathered from Breakup_Filename.
//	The same assumptions about the filename are used.
//----------------------------------------------------------------------------
void
Build_Filename(const itString& i_Basename, const itString& i_Ext, int i_Number, int i_NumDigits, itString& o_FName)
{
  o_FName = i_Basename;
  std::ostringstream num_ss(std::ostringstream::out);
  std::string num;

  num_ss << std::setw(i_NumDigits) << std::setfill('0') << i_Number;
  num = num_ss.str();

  o_FName += itString(&num[0]);
  o_FName += '.';
  o_FName += i_Ext;
}

//----------------------------------------------------------------------------
//	trim the leading and ending spaces from a standard string.
//
//	Note: this would be useful elsewhere but i'm not sure where to put it.  It
//	it currently used for filenames, so here it is.
//----------------------------------------------------------------------------
void
TrimSpaces(std::string& io_String)
{
  std::string::size_type pos = io_String.find_last_not_of(' ');
  if (pos != std::string::npos) {
    io_String.erase(pos + 1);
    pos = io_String.find_first_not_of(' ');

    if (pos != std::string::npos)
      io_String.erase(0, pos);
  } else {
    io_String.erase(io_String.begin(), io_String.end());
  }
}

//----------------------------------------------------------------------------
//	trim the leading and ending spaces from a standard string.
//
//	Note: this would be useful elsewhere but i'm not sure where to put it.  It
//	it currently used for filenames, so here it is.
//----------------------------------------------------------------------------
void
TrimSpaces(itString& io_String)
{
  std::string sstring = GetStdString(io_String);
  TrimSpaces(sstring);
  io_String = sstring.c_str();
}

}
