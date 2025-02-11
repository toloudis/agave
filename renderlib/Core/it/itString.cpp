/****************************************************************************\
**  itString.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/It/itString.hpp"

#include "Core/env/envString.hpp"

#include <algorithm>
#include <string.h>

//------------------------------------------------------------------------
//	default and copy constructors
//------------------------------------------------------------------------
itString::itString() {}

itString::itString(const itString& i_CopyFrom)
  : m_String(i_CopyFrom.m_String)
{
}

//------------------------------------------------------------------------
//	This constructor's argument is a single-byte, NULL terminated "C"
//	string.  It will be converted into a Unicode string.
//	API change: This function now interprets multi-byte UTF8 strings.
//------------------------------------------------------------------------
itString::itString(const char* i_CopyFrom)
{
  *this = i_CopyFrom;
}

//------------------------------------------------------------------------
//	This constructor's argument is a unicode, NULL terminated "C"
//	string.  It will be converted into a Unicode string.
//------------------------------------------------------------------------
itString::itString(const CharType* i_CopyFrom)
{
  // simpler with std::wstring
  m_String = i_CopyFrom;

  // int size = 0;
  // while( i_CopyFrom[size] )
  //	size++;
  //
  // m_String.resize(size);
  // if (size > 0)
  //	::memcpy(&(m_String[0]), i_CopyFrom, sizeof(CharType) * size);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
itString::~itString() throw() {}

//------------------------------------------------------------------------
//	operator = copies the input string
//------------------------------------------------------------------------
itString&
itString::operator=(const itString& i_CopyFrom)
{
  m_String = i_CopyFrom.m_String;
  return *this;
}

//------------------------------------------------------------------------
//	this operator = takes a single-byte NULL terminated "C" string, and
//	converts it to a Unicode string.
//	API change: This function now interprets multi-byte UTF8 strings.
//------------------------------------------------------------------------
itString&
itString::operator=(const char* i_CopyFrom)
{
  // size_t len = ::strlen(i_CopyFrom);
  // size_t i;

  // m_String.resize(len);

  // for( i = 0 ; i < len ; i++ )
  //	m_String[i] = static_cast<unsigned char>( i_CopyFrom[i] );

  if (!i_CopyFrom)
    m_String.clear();
  else {
    // In this case strlen will return the number of bytes in the
    // UTF8 string, which is not the number of characters. But,
    // that is what the envString function expects.
    m_String = envString::UTF8ToWideChar(i_CopyFrom, ::strlen(i_CopyFrom));
  }

  return *this;
}

//------------------------------------------------------------------------
//	this operator = takes a NULL terminated Unicode string.
//------------------------------------------------------------------------
itString&
itString::operator=(const CharType* i_CopyFrom)
{
  // simpler with std::wstring
  m_String = i_CopyFrom;

  // int size = 0;
  // while( i_CopyFrom[size] )
  //	size++;
  //
  // m_String.resize(size);

  // if (size > 0)
  //	::memcpy(&(m_String[0]), i_CopyFrom, sizeof(CharType) * size);

  return *this;
}

//------------------------------------------------------------------------
//	operator + concatenates
//------------------------------------------------------------------------
itString
itString::operator+(const itString& i_CopyFrom)
{
  itString OutputString(*this);
  OutputString += i_CopyFrom;
  return OutputString;
}

//------------------------------------------------------------------------
//	This constructor creates the string from a range of characters in
//	the string parameter.  The substring begins at character # i_Begin and
//	lasts for i_Num characters.
//------------------------------------------------------------------------
itString::itString(int i_Begin, int i_Num, const itString& i_String)
{
  // int i;
  // m_String.resize(i_Num);

  DBG_ASSERT(i_Num + i_Begin <= i_String.m_String.size(), "Substring out of range");

  // for( i = 0 ; i < i_Num ; i++ )
  //	m_String[i] = i_String.m_String[i+i_Begin];

  // simpler with std::wstring
  m_String = i_String.m_String.substr(i_Begin, i_Num);
}

//------------------------------------------------------------------------
//	this operator += concatenates a string
//------------------------------------------------------------------------
void
itString::operator+=(const itString& i_String)
{
  //	std::copy(i_String.m_String.begin(), i_String.m_String.end(), std::back_inserter(m_String));

  // simpler with std::wstring
  m_String += i_String.m_String;
}
// API Change: this function is new to allow easier concatenation
void
itString::operator+=(const CharType* i_String)
{
  m_String += i_String;
}

//------------------------------------------------------------------------
//	this operator += concatenates a single character
//------------------------------------------------------------------------
void
itString::operator+=(CharType i_Char)
{
  // m_String.push_back(i_Char);

  // simpler with std::wstring
  m_String += i_Char;
}

//------------------------------------------------------------------------
//	operator == and != test for equality of strings
//------------------------------------------------------------------------
bool
itString::operator==(const itString& i_String) const
{
  return m_String == i_String.m_String;
}
bool
itString::operator!=(const itString& i_String) const
{
  bool result = (*this == i_String);
  return !result;
}

//------------------------------------------------------------------------
//	operator < does a lexicographical comparison.
//------------------------------------------------------------------------
bool
itString::operator<(const itString& i_String) const
{
  return m_String < i_String.m_String;
}

//------------------------------------------------------------------------
//	operator <= does a lexicographical comparison.
//------------------------------------------------------------------------
bool
itString::operator<=(const itString& i_String) const
{
  return m_String <= i_String.m_String;
}

//------------------------------------------------------------------------
//	operator >= does a lexicographical comparison.
//------------------------------------------------------------------------
bool
itString::operator>=(const itString& i_String) const
{
  return m_String >= i_String.m_String;
}

//------------------------------------------------------------------------
//	operator > does a lexicographical comparison.
//------------------------------------------------------------------------
bool
itString::operator>(const itString& i_String) const
{
  return m_String > i_String.m_String;
}

//------------------------------------------------------------------------
//	operator << performs writing to an ostream.
// API Change: This now converts the itString to UTF8 in order to write
//	to the stream.
//------------------------------------------------------------------------
std::ostream&
operator<<(std::ostream& io_os, const itString& i_String)
{
  std::string cur_string;

  // switching to envString so that UTF8 strings are written to iostreams:
  // cur_string = itStringUtil::GetStdString(i_String);
  cur_string = envString::WideCharToUTF8(i_String.m_String);

  io_os << cur_string;
  return io_os;
}
std::wostream&
operator<<(std::wostream& io_os, const itString& i_String)
{
  io_os << i_String.GetString();
  return io_os;
}

//------------------------------------------------------------------------
//	GetString returns a pointer to the beginning of the string
//------------------------------------------------------------------------
const itString::CharType*
itString::GetString() const
{
  // if (m_String.size() > 0)
  //	return &(m_String[0]);
  // else
  //	return 0;

  // by using the std::wstring's c_str() function, we get guaranteed
  // NULL termination. Which also means that GetString() will not return
  // NULL when empty anymore.
  return m_String.c_str();
}

//------------------------------------------------------------------------
//	GetString returns a pointer to the beginning of the string
//------------------------------------------------------------------------
// itString::CharType* itString::GetString()
//{
//	//if (m_String.size() > 0)
//	//	return &(m_String[0]);
//	//else
//	//	return 0;
//
//	return m_String.c_str();
//}

//------------------------------------------------------------------------
//	SetLength sets the length of the string in characters (not bytes)
//------------------------------------------------------------------------
void
itString::SetLength(int i_Length)
{
  DBG_ASSERT(0 <= i_Length, "illegal string length #" << i_Length);

  m_String.resize(i_Length);
}

//------------------------------------------------------------------------
//	GetLength returns the length of the string in characters (not bytes)
//------------------------------------------------------------------------
int
itString::GetLength() const
{
  return int(m_String.size());
}

//------------------------------------------------------------------------
//	Clear empties the string
//------------------------------------------------------------------------
void
itString::Clear()
{
  m_String.clear();
}

//------------------------------------------------------------------------
//	HasSubstring returns true if the string contains the given substring
//	at some point, exactly.
//------------------------------------------------------------------------
bool
itString::HasSubString(const itString& i_SubString) const
{
  // could use wstring::find() here?
  std::basic_string<CharType>::const_iterator it;
  it = std::search(m_String.begin(), m_String.end(), i_SubString.m_String.begin(), i_SubString.m_String.end());
  return (it != m_String.end());
}

//------------------------------------------------------------------------
//	StartsWith returns true if the string contains the given substring
//	at the beginning of the string
//------------------------------------------------------------------------
bool
itString::StartsWith(const itString& i_SubString) const
{
  // could use wstring::find() here?
  return ((i_SubString.GetLength() > 0) && (m_String.length() >= i_SubString.GetLength()) &&
          (std::equal(i_SubString.m_String.begin(), i_SubString.m_String.end(), m_String.begin())));
}

//------------------------------------------------------------------------
//	EndsWith returns true if the string contains the given substring
//	at the end of the string
//------------------------------------------------------------------------
bool
itString::EndsWith(const itString& i_SubString) const
{
  // could use wstring::find() here?
  return ((i_SubString.GetLength() > 0) && (m_String.length() >= i_SubString.GetLength()) &&
          (std::equal(i_SubString.m_String.begin(),
                      i_SubString.m_String.end(),
                      (m_String.end() - i_SubString.m_String.length()))));
}

//------------------------------------------------------------------------
//	InsertCharAt inserts i_Char at i_Index
//------------------------------------------------------------------------
void
itString::InsertCharAt(int i_Index, CharType i_Char)
{
  DBG_ASSERT(0 <= i_Index && i_Index <= m_String.size(), "Invalid Index #" << i_Index);

  m_String.insert(m_String.begin() + i_Index, i_Char);
}

//------------------------------------------------------------------------
//	RemoveCharAt removes the character at i_Index
//------------------------------------------------------------------------
void
itString::RemoveCharAt(int i_Index)
{
  DBG_ASSERT(0 <= i_Index && i_Index <= m_String.size(), "Invalid Index #" << i_Index);

  m_String.erase(m_String.begin() + i_Index);
}

//------------------------------------------------------------------------
//	Add a string to the beginning or end of a string
//------------------------------------------------------------------------
void
itString::Append(itString& i_String)
{
  // std::copy(i_String.m_String.begin(), i_String.m_String.end(), std::back_inserter(m_String));

  // simpler with std::wstring
  m_String += i_String.m_String;
}
void
itString::Prepend(itString& i_String)
{
  m_String.insert(0, i_String.m_String);
}

//------------------------------------------------------------------------
//	ResizeAtNULL resizes the string so that it's length is one less than
//	the first zero in it, if any.
//------------------------------------------------------------------------
void
itString::ResizeAtNULL()
{
  std::basic_string<CharType>::iterator it = std::find(m_String.begin(), m_String.end(), 0);
  if (it != m_String.end())
    m_String.resize(it - m_String.begin());
}

//------------------------------------------------------------------------
//	StripExtension removes all characters including and after any period
//	found in the string.
//------------------------------------------------------------------------
void
itString::StripExtension()
{
  // See Effective STL, by Scott Meyers,
  //   Item 28: "Understand how to use a reverse_iterator's base iterator."
  std::basic_string<CharType>::reverse_iterator ri =
    std::find(m_String.rbegin(), m_String.rend(), itString::CharType('.'));
  if (m_String.rend() != ri)
    m_String.erase((++ri).base(), m_String.end());
}

//------------------------------------------------------------------------
//	ReplaceExtension replace all characters including and after any period
//	found in the string and replace it with a new extension.
//	Note: if no '.' is in the new extension string, one will be added.
//------------------------------------------------------------------------
void
itString::ReplaceExtension(const itString& i_NewExtension)
{
  StripExtension();
  this->operator+=(itString("."));
  this->operator+=(i_NewExtension);
}

//------------------------------------------------------------------------
//	return the extension of the string after the LAST period
//------------------------------------------------------------------------
void
itString::GetExtension(itString& o_Extension) const
{
  //	Find the last occurrence of the '.'
  int i;
  int index = m_String.size();
  for (i = (m_String.size() - 1); i >= 0; --i) {
    if (m_String[i] == '.') {
      index = i;
      break;
    }
  }

  //	if there was a period, grab the values after it.
  //
  if (index != m_String.size()) {
    o_Extension.Clear();
    for (i = index + 1; i < m_String.size(); ++i) {
      o_Extension += m_String[i];
    }
  }
}

//------------------------------------------------------------------------
//	return the base of the string before the LAST period
//------------------------------------------------------------------------
void
itString::GetBase(itString& o_Base) const
{
  int i;
  int index = m_String.size();
  for (i = (m_String.size() - 1); i >= 0; --i) {
    if (m_String[i] == '.') {
      index = i;
      break;
    }
  }

  if (index != m_String.size()) {
    o_Base.Clear();
    for (i = 0; i < index; ++i) {
      o_Base += m_String[i];
    }
  }
}

//------------------------------------------------------------------------
// returns whether or not the string consists of all single byte characters
//------------------------------------------------------------------------
bool
itString::IsStringASCII() const
{
  bool bASCII = true;
  for (int i = 0; i < m_String.size(); ++i) {
    // check if the character is an ascii value
    if ((int)m_String[i] > 127) {
      bASCII = false;
    }
  }

  // return true if all charcters are ascii
  return bASCII;
}