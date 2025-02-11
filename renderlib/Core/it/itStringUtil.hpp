/****************************************************************************\
**	itStringUtil.hpp
**
**		itStringUtil.hpp defines the itStringUtil namespace which provides
**		a set of utility functions for manipulating itStrings
**
**
**
\****************************************************************************/
#ifdef IT_STRINGUTIL_HPP
#error itStringUtil.hpp multiply included
#endif
#define IT_STRINGUTIL_HPP

#ifndef IT_STRING_HPP
#include "Core/it/itString.hpp"
#endif

#include <string>

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
GetStdString(const itString& i_String);

//------------------------------------------------------------------------
// GetStdString, this version sets the output buffer with the ANSI
// chars.  io_NumChars is the count of chars available to write to in
// o_szBuffer, it is set to the number of chars copied on output (not
// inluding the NULL).  The caller must pre-allocate memory
// Easiest way is to allocate i_String.GetLength()+1 bytes.
//------------------------------------------------------------------------
void
GetStdString(const itString& i_String, char* o_szBuffer, int& io_NumChars);

//------------------------------------------------------------------------
//	Convert a itString into a double data type.
//------------------------------------------------------------------------
double
GetDouble(const itString& i_String);

//------------------------------------------------------------------------
//	Convert a itString into an int data type.
//------------------------------------------------------------------------
int
GetInt(const itString& i_String);

//------------------------------------------------------------------------
// ToLower will convert an itString to all lower case.  Note:  This
// only works with ASCII text today!
// The return is a reference to the input/output parameter, a convenience for
// quick conversions into function parameters
//------------------------------------------------------------------------
const itString&
ToLower(itString& io_String);

//------------------------------------------------------------------------
//	This ToLower works with std::string.
//------------------------------------------------------------------------
const std::string&
ToLower(std::string& io_String);

//------------------------------------------------------------------------
//	Compare the two strings ignoring case.
//------------------------------------------------------------------------
bool
Equal(itString& i_String1, itString& i_String2);

//------------------------------------------------------------------------
//	Compare the two strings ignoring case.
//------------------------------------------------------------------------
bool
Equal(const itString& i_String1, const itString& i_String2);
bool
LessThan(const itString& i_String1, const itString& i_String2);
bool
LessThanOrEqual(const itString& i_String1, const itString& i_String2);
bool
GreaterThan(const itString& i_String1, const itString& i_String2);
bool
GreaterThanOrEqual(const itString& i_String1, const itString& i_String2);

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
Breakup_Filename(const itString& i_Filename, itString& o_BaseFilename, itString& o_Ext, int& o_InitialNumber);

//----------------------------------------------------------------------------
//	Build a filename based on the parts gathered from Breakup_Filename.
//	The same assumptions about the filename are used.
//----------------------------------------------------------------------------
void
Build_Filename(const itString& i_Basename, const itString& i_Ext, int i_Number, int i_NumDigits, itString& o_FName);

//----------------------------------------------------------------------------
//	trim the leading and ending spaces from a standard string.
//
//	Note: this would be useful elsewhere but i'm not sure where to put it.  It
//	it currently used for filenames, so here it is.
//----------------------------------------------------------------------------
void
TrimSpaces(std::string& io_String);
void
TrimSpaces(itString& io_String);

}
