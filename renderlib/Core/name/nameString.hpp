/****************************************************************************\
**	nameString.hpp
**
**		This class encapsulates our system of naming.  For now it is just
**	a std::string, but it could be extended to use a itString or hash ids or
**	some other method that could save memory.
**
**
**
\****************************************************************************/
#ifdef NAME_STRING_HPP
#error nameString.hpp multiply included
#endif
#define NAME_STRING_HPP

#ifndef IT_STRING_HPP
#include "Core/it/itString.hpp"
#endif

#include <ostream>
#include <string>

//============================================================================
//============================================================================
typedef int nameUID;

//============================================================================
//============================================================================
class nameString
{
public:
  enum nameUIDvalue
  {
    e_InvalidUID = -1
  };

  //--------------------------------------------------------------------
  // implicit constructors
  //--------------------------------------------------------------------
  nameString();
  nameString(const itString& i_Name);
  nameString(const std::string& i_Name);
  nameString(const std::string& i_Name, const nameUID i_UID);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetString(const std::string& i_Name);
  std::string GetString() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  nameUID GetUID() const;
  void SetUID(const nameUID i_UID);

  //--------------------------------------------------------------------
  // operator== checks if the UIDs match, or if the strings
  //	match if one of the UIDs are invalid
  //--------------------------------------------------------------------
  bool operator==(const nameString& i_Str) const;
  bool operator==(const std::string& i_Name) const;
  bool operator!=(const nameString& i_Str) const;
  bool operator!=(const std::string& i_Name) const;

  //--------------------------------------------------------------------
  // ExactMatch returns true only if the UID and string both match
  //--------------------------------------------------------------------
  bool ExactMatch(const nameString& i_Str) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void operator=(const nameString& i_Str);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  bool operator<(const nameString& i_Str) const;

  //--------------------------------------------------------------------
  //	check if the string is empty or not
  //--------------------------------------------------------------------
  bool IsEmpty() const;

  //------------------------------------------------------------------------
  //	operator << writes itString properties to a stream
  // API Change: This now converts the itString to UTF8 in order to write
  //	to the stream.
  //------------------------------------------------------------------------
  friend std::ostream& operator<<(std::ostream& io_os, const nameString& i_String);
  friend std::wostream& operator<<(std::wostream& io_os, const nameString& i_String);

private:
  std::string m_String;

  nameUID m_UID;
};
