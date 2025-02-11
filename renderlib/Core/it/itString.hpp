/****************************************************************************\
**	itString.hpp
**
**		itString.hpp defines the itString class, which is the basic string
**	class for localized text in Terawatt.
**
**		API change: Please notice that this string used to not necessarily be
**	NULL terminated, but it is now. Also, notice that the conversions from
**	char* now assume that the char* string is multi-byte UTF8 format and not
**	just ANSI strings.
**
**
**
\****************************************************************************/
#ifdef IT_STRING_HPP
#error itString.hpp multiply included
#endif
#define IT_STRING_HPP

#ifndef DBG_MSG_HPP
#include "Core/Dbg/dbgMsg.hpp"
#endif
#ifndef ENV_TYPE_HPP
#include "Core/Env/envType.hpp"
#endif

#include <string>
#include <vector>
#include <ostream>

//============================================================================
//============================================================================
class itString
{
public:
  //------------------------------------------------------------------------
  //	Here is the Unicode character type.  It is a 16-bit wide character
  //	on all platforms.  There are some advantages to defining it as a
  //	TCHAR on Microsoft OSs but this is more convenient, and there is
  //	little chance that Microsoft will suddenly develop a non-16 bit
  //	variant of Unicode
  //------------------------------------------------------------------------
  // typedef envType::UInt16 CharType;
  //------------------------------------------------------------------------
  // Now itString is defined to be a "wchar_t" string on all systems.
  // This means it will be different sizes on Unix and Windows OSs
  // and the file format will have to be adjusted.
  //------------------------------------------------------------------------
  typedef envType::WChar CharType;

  //------------------------------------------------------------------------
  //	default and copy constructors
  //------------------------------------------------------------------------
  itString();
  itString(const itString& i_CopyFrom);

  //------------------------------------------------------------------------
  //	This constructor's argument is a single-byte, NULL terminated "C"
  //	string.  It will be converted into a Unicode string.
  //	API change: This function now interprets multi-byte UTF8 strings.
  //------------------------------------------------------------------------
  explicit itString(const char* i_CopyFrom);

  //------------------------------------------------------------------------
  //	This constructor's argument is a unicode, NULL terminated "C"
  //	string.  It will be converted into a Unicode string.
  //------------------------------------------------------------------------
  explicit itString(const CharType* i_CopyFrom);

  //------------------------------------------------------------------------
  //	This constructor creates the string from a range of characters in
  //	the string parameter.  The substring begins at character # i_Begin and
  //	lasts for i_Num characters.
  //------------------------------------------------------------------------
  itString(int i_Begin, int i_Num, const itString& i_String);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  ~itString() throw();

  //------------------------------------------------------------------------
  //	operator = copies the input string
  //------------------------------------------------------------------------
  itString& operator=(const itString& i_CopyFrom);

  //------------------------------------------------------------------------
  //	this operator = takes a NULL terminated Unicode string.
  //------------------------------------------------------------------------
  itString& operator=(const CharType* i_CopyFrom);

  //------------------------------------------------------------------------
  //	this operator = takes a single-byte NULL terminated "C" string, and
  //	converts it to a Unicode string
  //	API change: This function now interprets multi-byte UTF8 strings.
  //------------------------------------------------------------------------
  itString& operator=(const char* i_CopyFrom);

  //------------------------------------------------------------------------
  //	operator + concatenates
  //------------------------------------------------------------------------
  itString operator+(const itString& i_CopyFrom);

  //------------------------------------------------------------------------
  //	this operator += concatenates a string
  //------------------------------------------------------------------------
  void operator+=(const itString& i_String);
  void operator+=(const CharType* i_String);

  //------------------------------------------------------------------------
  //	this operator += concatenates a single character
  //------------------------------------------------------------------------
  void operator+=(CharType i_Char);

  //------------------------------------------------------------------------
  //	operator == and != test for equality of strings
  //------------------------------------------------------------------------
  bool operator==(const itString& i_String) const;
  bool operator!=(const itString& i_String) const;

  //------------------------------------------------------------------------
  //	operator < does a lexicographical comparison.
  //------------------------------------------------------------------------
  bool operator<(const itString& i_String) const;

  //------------------------------------------------------------------------
  //	operator <= does a lexicographical comparison.
  //------------------------------------------------------------------------
  bool operator<=(const itString& i_String) const;

  //------------------------------------------------------------------------
  //	operator > does a lexicographical comparison.
  //------------------------------------------------------------------------
  bool operator>(const itString& i_String) const;

  //------------------------------------------------------------------------
  //	operator >= does a lexicographical comparison.
  //------------------------------------------------------------------------
  bool operator>=(const itString& i_String) const;

  //------------------------------------------------------------------------
  //	operator << writes itString properties to a stream
  // API Change: This now converts the itString to UTF8 in order to write
  //	to the stream.
  //------------------------------------------------------------------------
  friend std::ostream& operator<<(std::ostream& io_os, const itString& i_String);
  friend std::wostream& operator<<(std::wostream& io_os, const itString& i_String);

  //------------------------------------------------------------------------
  //	operator [] returns the CharType at i_Index.
  //------------------------------------------------------------------------
  CharType& operator[](int i_Index);
  CharType operator[](int i_Index) const;

  //------------------------------------------------------------------------
  //	GetString returns a pointer to the beginning of the string.
  // Note that this now behaves like wstring such that an empty
  //	string will return a null-terminated empty string, not a NULL pointer.
  //------------------------------------------------------------------------
  const CharType* GetString() const;
  // CharType* GetString(); // removing non-const version when switching to wstring

  //------------------------------------------------------------------------
  //	SetLength sets the length of the string in characters (not bytes)
  //------------------------------------------------------------------------
  void SetLength(int i_Length);

  //------------------------------------------------------------------------
  //	GetLength returns the length of the string in characters (not bytes)
  //------------------------------------------------------------------------
  int GetLength() const;

  //------------------------------------------------------------------------
  //	Clear empties the string
  //------------------------------------------------------------------------
  void Clear();

  //------------------------------------------------------------------------
  //	HasSubstring returns true if the string contains the given substring
  //	at some point, exactly.
  //------------------------------------------------------------------------
  bool HasSubString(const itString& i_SubString) const;

  //------------------------------------------------------------------------
  //	StartsWith returns true if the string contains the given substring
  //	at the beginning of the string
  //------------------------------------------------------------------------
  bool StartsWith(const itString& i_SubString) const;

  //------------------------------------------------------------------------
  //	EndsWith returns true if the string contains the given substring
  //	at the end of the string
  //------------------------------------------------------------------------
  bool EndsWith(const itString& i_SubString) const;

  //------------------------------------------------------------------------
  //	InsertCharAt inserts i_Char at i_Index
  //------------------------------------------------------------------------
  void InsertCharAt(int i_Index, CharType i_Char);

  //------------------------------------------------------------------------
  //	RemoveCharAt removes the character at i_Index
  //------------------------------------------------------------------------
  void RemoveCharAt(int i_Index);

  //------------------------------------------------------------------------
  //	Add a string to the beginning or end of a string
  //------------------------------------------------------------------------
  void Append(itString& i_String);
  void Prepend(itString& i_String);

  //------------------------------------------------------------------------
  //	ResizeAtNULL resizes the string so that it's length is one less than
  //	the first zero in it, if any.
  //------------------------------------------------------------------------
  void ResizeAtNULL();

  //------------------------------------------------------------------------
  //	StripExtension removes all characters including and after any period
  //	found in the string.
  //------------------------------------------------------------------------
  void StripExtension();

  //------------------------------------------------------------------------
  //	ReplaceExtension replace all characters including and after any period
  //	found in the string and replace it with a new extension.
  //	Note: if no '.' is in the new extension string, one will be added.
  //------------------------------------------------------------------------
  void ReplaceExtension(const itString& i_NewExtension);

  //------------------------------------------------------------------------
  //	return the extension of the string after the LAST period
  //------------------------------------------------------------------------
  void GetExtension(itString& o_Extension) const;

  //------------------------------------------------------------------------
  //	return the base of the string before the LAST period
  //------------------------------------------------------------------------
  void GetBase(itString& o_Base) const;

  //------------------------------------------------------------------------
  // returns whether or not the string consists of all single byte characters
  //------------------------------------------------------------------------
  bool IsStringASCII() const;

private:
  // bga - changing the member variable to help with guaranteeing
  //  NULL-termination, and to try to use STL code to prevent errors.
  //  We may want to actually derive the class from std::wstring instead.
  //
  // std::vector<CharType> m_String;
  std::basic_string<CharType> m_String;
};

//------------------------------------------------------------------------
//	operator [] returns the CharType at i_Index.
//------------------------------------------------------------------------
inline itString::CharType&
itString::operator[](int i_Index)
{
  DBG_ASSERT(0 <= i_Index && i_Index < (int)m_String.size(), "Invalid index #" << i_Index);

  return m_String[i_Index];
}

inline itString::CharType
itString::operator[](int i_Index) const
{
  DBG_ASSERT(0 <= i_Index && i_Index < (int)m_String.size(), "Invalid index #" << i_Index);

  return m_String[i_Index];
}