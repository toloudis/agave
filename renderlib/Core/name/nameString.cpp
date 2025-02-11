/****************************************************************************\
**	nameString.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/name/nameString.hpp"

#include "Core/env/envString.hpp"
#include "Core/it/itStringUtil.hpp"

//--------------------------------------------------------------------
// implicit constructors
//--------------------------------------------------------------------
nameString::nameString()
  : m_UID(e_InvalidUID)
{
  m_String.clear();
}
nameString::nameString(const itString& i_Name)
  : m_UID(e_InvalidUID)
{
  m_String = itStringUtil::GetStdString(i_Name);
}

nameString::nameString(const std::string& i_Name)
  : m_UID(e_InvalidUID)
  , m_String(i_Name) // FIX: -- eventually phase this out.
{
}

nameString::nameString(const std::string& i_Name, const nameUID i_UID)
  : m_UID(i_UID)
  , m_String(i_Name) // FIX: -- eventually phase this out.
{
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
nameString::SetString(const std::string& i_Name)
{
  // nameMgr::SetName( m_UID, i_Name );

  m_String = i_Name; // FIX: -- eventually phase this out.
}

std::string
nameString::GetString() const
{
  // std::string theName;
  // nameMgr::GetNameString( m_UID, theName );
  // return theName;
  return m_String;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
nameUID
nameString::GetUID() const
{
  return m_UID;
}

void
nameString::SetUID(const nameUID i_UID)
{
  m_UID = i_UID;
  // m_String = nameMgr::GetName( m_UID );
}

//--------------------------------------------------------------------
// operator== checks if the UIDs match, or if the strings
//	match if one of the UIDs are invalid
//--------------------------------------------------------------------
bool
nameString::operator==(const nameString& i_Str) const
{
  if ((m_UID == e_InvalidUID) || (i_Str.GetUID() == e_InvalidUID)) {
    return (i_Str.GetString() == m_String);
  } else {
    return (i_Str.m_UID == this->m_UID);
  }
}
bool
nameString::operator==(const std::string& i_Name) const
{
  return (i_Name == m_String);
}
bool
nameString::operator!=(const nameString& i_Str) const
{
  return (!(this->operator==(i_Str)));
}
bool
nameString::operator!=(const std::string& i_Name) const
{
  return (i_Name != m_String);
}

//--------------------------------------------------------------------
// ExactMatch returns true only if the UID and string both match
//--------------------------------------------------------------------
bool
nameString::ExactMatch(const nameString& i_Str) const
{
  return ((m_UID == i_Str.m_UID) && (m_String == i_Str.m_String));
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
nameString::operator=(const nameString& i_Str)
{
  this->m_UID = i_Str.GetUID();

  if (!(i_Str.GetString().empty())) {
    this->SetString(i_Str.GetString());
  } else {
    this->SetString(std::string(""));
  }
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
bool
nameString::operator<(const nameString& i_Str) const
{
  return (this->m_UID < i_Str.m_UID);
}

//--------------------------------------------------------------------
//	check if the string is empty or not
//--------------------------------------------------------------------
bool
nameString::IsEmpty() const
{
  return (this->m_String.empty());
}

//------------------------------------------------------------------------
//	operator << performs writing to an ostream.
//------------------------------------------------------------------------
std::ostream&
operator<<(std::ostream& io_os, const nameString& i_String)
{
  io_os << i_String.GetString();
  return io_os;
}
std::wostream&
operator<<(std::wostream& io_os, const nameString& i_String)
{
  std::wstring cur_string;
  cur_string = envString::UTF8ToWideChar(i_String.m_String);

  io_os << cur_string;
  return io_os;
}
