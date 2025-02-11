/**********************************************************
**	fsLocator.cpp
**
**		see .hpp
**
**
**
\*********************************************************/
#include "Core/Fs/fsLocator.hpp"

#include "Core/Fs/fsFileUtil.hpp"   // for debug only
#include "Core/It/itStringUtil.hpp" // for debug only

#include <algorithm>
#include <iterator>
#include <string>

//------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------
fsLocator::fsLocator()
  : m_Index(-1)
  , m_bUNCPath(false)
{
}

fsLocator::fsLocator(int i_Index)
  : m_Index(i_Index)
  , m_bUNCPath(false)
{
}

fsLocator::fsLocator(const fsLocator& i_Locator)
  : m_Names(i_Locator.m_Names)
  , m_Index(i_Locator.m_Index)
  , m_bUNCPath(i_Locator.m_bUNCPath)
{
}

fsLocator::fsLocator(const fsLocator& i_Locator, int i_Index)
  : m_Names(i_Locator.m_Names)
  , m_Index(i_Index)
  , m_bUNCPath(i_Locator.m_bUNCPath)
{
}

fsLocator::fsLocator(const itString& i_Name)
  : m_Index(-1)
  , m_Names(1, i_Name)
  , m_bUNCPath(false)
{
}

// This version sets the index and pushes the given name
fsLocator::fsLocator(int i_Index, const itString& i_Name)
  : m_Index(i_Index)
  , m_Names(1, i_Name)
  , m_bUNCPath(false)
{
}

//------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------
fsLocator::~fsLocator() {}

//------------------------------------------------------------------------
//	SetIndex sets the directory index of the locator.  This is the same
//	as using the constructor with the index parameter.
//------------------------------------------------------------------------
void
fsLocator::SetIndex(int i_Index)
{
  if (i_Index < 0)
    m_Index = -1;
  else
    m_Index = i_Index;
}

//------------------------------------------------------------------------
//	GetNumNames() returns the number of names in the path
//------------------------------------------------------------------------
int
fsLocator::GetNumNames() const
{
  return int(m_Names.size());
}

//------------------------------------------------------------------------
//	GetName() returns a particular name.
//------------------------------------------------------------------------
const itString&
fsLocator::GetName(int i_Num) const
{
  DBG_ASSERT(i_Num < m_Names.size(), "Name index out of range - " << i_Num);
  DBG_ASSERT(i_Num >= 0, "Name index out of range - " << i_Num);
  return m_Names[i_Num];
}

//------------------------------------------------------------------------
//	GetLastName() returns the last name in the list
//------------------------------------------------------------------------
const itString&
fsLocator::GetLastName() const
{
  DBG_ASSERT(m_Names.size() > 0, "Must have some names to get last one");
  return *(m_Names.rbegin());
}

//------------------------------------------------------------------------
//	ReplaceLastName() replace the last name in the list
//------------------------------------------------------------------------
void
fsLocator::ReplaceLastName(const itString& i_NewName)
{
  Pop();
  Push(i_NewName);
}

//------------------------------------------------------------------------
//	ReplaceExtension() replace the last name in the list's extension
//	with a new one.
//	Note: if no '.' is in the new extension string, one will be added.
//------------------------------------------------------------------------
void
fsLocator::ReplaceExtension(const itString& i_NewExtension)
{
  (*(m_Names.rbegin())).ReplaceExtension(i_NewExtension);
}
void
fsLocator::ReplaceExtension(const char* i_NewExtension)
{
  ReplaceExtension(itString(i_NewExtension));
}

//------------------------------------------------------------------------
//	Boolean operators.  Two fsLocators are equal if all of their names
//	are equal.
//------------------------------------------------------------------------
bool
fsLocator::operator==(const fsLocator& i_Locator) const
{
  if ((m_Index < 0) != (i_Locator.m_Index < 0))
    return false;
  return (m_Names == i_Locator.m_Names);
}

//------------------------------------------------------------------------
//	operator < performs a lexicographical comparison
//------------------------------------------------------------------------
bool
fsLocator::operator<(const fsLocator& i_Locator) const
{
  return m_Names < i_Locator.m_Names;
}

//------------------------------------------------------------------------
//	operator <= performs a lexicographical comparison
//------------------------------------------------------------------------
bool
fsLocator::operator<=(const fsLocator& i_Locator) const
{
  return m_Names <= i_Locator.m_Names;
}

//------------------------------------------------------------------------
//	operator >= performs a lexicographical comparison
//------------------------------------------------------------------------
bool
fsLocator::operator>=(const fsLocator& i_Locator) const
{
  return m_Names >= i_Locator.m_Names;
}

//------------------------------------------------------------------------
//	operator + concatenates two strings
//------------------------------------------------------------------------
fsLocator
fsLocator::operator+(const fsLocator& i_Locator) const
{
  fsLocator concat;
  concat.Push(*this);
  concat.Push(i_Locator);
  return concat;
}

//------------------------------------------------------------------------
//	operator << performs writing to an ostream
//------------------------------------------------------------------------
std::ostream&
operator<<(std::ostream& io_os, const fsLocator& i_Locator)
{
  std::string cur_string;
  fsFileUtil::LocatorToANSIFilename(i_Locator, cur_string);
  io_os << cur_string;
  return io_os;
}
std::wostream&
operator<<(std::wostream& io_os, const fsLocator& i_Locator)
{
  itString cur_string;
  fsFileUtil::LocatorToUnicodeString(i_Locator, cur_string);
  io_os << cur_string;
  return io_os;
}
//------------------------------------------------------------------------
//	compare two strings case INSENSITIVE
//------------------------------------------------------------------------
bool
fsLocator::Equals(const fsLocator& i_Locator) const
{
  std::string str1, str2;
  fsFileUtil::LocatorToANSIFilename(i_Locator, str1);
  fsFileUtil::LocatorToANSIFilename(*this, str2);

  std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
  std::transform(str2.begin(), str2.end(), str2.begin(), tolower);
  return (str1 == str2);
}

//------------------------------------------------------------------------
//	Assignment
//------------------------------------------------------------------------
fsLocator&
fsLocator::operator=(const fsLocator& i_Locator)
{
  if (this == &i_Locator)
    return (*this);

  if (i_Locator.m_Names.size() > 0) {
    m_Names = i_Locator.m_Names;
  } else {
    m_Names.clear();
  }

  m_bUNCPath = i_Locator.IsUNCPath();
  m_Index = i_Locator.m_Index;

  return *this;
}

//------------------------------------------------------------------------
//	Push() adds a name or locator to the end of the list
//------------------------------------------------------------------------
void
fsLocator::Push(const char* i_Name)
{
  DBG_ASSERT(NULL != i_Name, "Null directories and filenames are not permitted, ever.");
  m_Names.push_back(itString(i_Name));
}

void
fsLocator::Push(const itString& i_Name)
{
  DBG_ASSERT(0 != i_Name.GetLength(), "Emtpy directory and filename strings are not permitted, ever.");
  m_Names.push_back(i_Name);
}

void
fsLocator::Push(const fsLocator& i_Locator)
{
  std::copy(i_Locator.m_Names.begin(), i_Locator.m_Names.end(), std::back_inserter(m_Names));
}

//------------------------------------------------------------------------
//	PopName() removes a name from the end of the list
//------------------------------------------------------------------------
void
fsLocator::Pop()
{
  DBG_ASSERT(0 != m_Names.size(), "Attempt to pop from an empty names list!!");
  m_Names.pop_back();
}

//------------------------------------------------------------------------
//	FindFirst - find the first occurrence of the name.  Returns -1 if
//	name is not found.
//------------------------------------------------------------------------
int
fsLocator::FindFirst(const itString& i_Name) const
{
  int name_size = m_Names.size();

  int i;
  for (i = 0; i < name_size; ++i) {
    //	find the first occurence in the list
    //
    if (i_Name == m_Names[i]) {
      return i;
    }
  }

  return -1;
}

//------------------------------------------------------------------------
//	ReplaceName() changes one of the names in the list
//------------------------------------------------------------------------
void
fsLocator::ReplaceName(const itString& i_Name, int i_Num)
{
  DBG_ASSERT(i_Num < m_Names.size(), "Name index out of range");
  m_Names[i_Num] = i_Name;
}

//------------------------------------------------------------------------
//	Clear() gets rid of all names and empties the locator
//------------------------------------------------------------------------
void
fsLocator::Clear()
{
  m_Names.clear();
  m_Index = -1;
  m_bUNCPath = false;
}

//------------------------------------------------------------------------
//	RemoveAfter() find the first occurence of the string passed in and
//	remove every token AFTER it (but not the token itself).
//
//	returns true if it found the string in the locator
//------------------------------------------------------------------------
bool
fsLocator::RemoveAfter(const itString& i_String)
{
  int startindex = -1;
  int name_size = m_Names.size();

  startindex = this->FindFirst(i_String);
  if (startindex != -1) {
    //	found it
    while (m_Names.size() > (startindex + 1)) {
      m_Names.pop_back();
    }

    return true;
  }

  return false;
}

//------------------------------------------------------------------------
//	RemoveBefore() find the first occurence of the string passed in and
//	remove every token BEFORE it (but not the token itself).
//
//	NOTE: This is useful for removing the first part of a path to get a
//	relative path.
//	If this locator is used as a relative path the user must set the
//	Index to be the path index in the paths list.
//
//	returns true if it found the string in the locator
//------------------------------------------------------------------------
bool
fsLocator::RemoveBefore(const itString& i_String)
{
  int startindex = -1;
  int name_size = m_Names.size();

  int i;
  for (i = 0; i < name_size; ++i) {
    //	find the first occurence in the list
    //
    if (i_String == m_Names[i]) {
      startindex = i;
      break;
    }
  }

  return RemoveBefore(startindex);
}

//------------------------------------------------------------------------
//	RemoveBefore() - remove all the paths BEFORE the index passed in.
//------------------------------------------------------------------------
bool
fsLocator::RemoveBefore(int i_Index)
{
  int name_size = m_Names.size();

  if ((i_Index >= 0) && (i_Index < name_size)) {
    m_Index = 0;

    // remove all of the indexes at the front.
    fsLocator temp;
    int j;
    for (j = i_Index; j < name_size; ++j) {
      temp.Push(m_Names[j]);
    }

    this->Clear();
    this->Push(temp);

    return true;
  }

  return false;
}

//------------------------------------------------------------------------
//	Remove() find the first occurence of the locator passed in and
//	remove it.
//
//	NOTE: This is useful for removing the first part of a path to get a
//	relative path from the App Directory.
//	If this locator is used as a relative path the user must set the
//	Index to be the path index in the paths list.
//
//	returns true if it found the string in the locator
//------------------------------------------------------------------------
bool
fsLocator::Remove(const fsLocator& i_SubDir)
{
  DBG_ASSERT(0 < i_SubDir.GetNumNames(), "Cannot remove an empty locator");
  DBG_ASSERT(0 < m_Names.size(), "Cannot remove a directory from an empty locator");

  int startindex = -1;
  int name_size = m_Names.size();
  int subdir_size = i_SubDir.GetNumNames();

  // DEBUG ONLY
  // std::string filename;
  // fsFileUtil::LocatorToANSIFilename(*this, filename);
  // std::string filename2;
  // fsFileUtil::LocatorToANSIFilename(i_SubDir, filename2);
  // DBG_LOG2("REMOVE (%s) from (%s)", filename2.c_str(), filename.c_str());

  bool bFoundSubDir;
  int i;
  for (i = 0; i < (name_size - subdir_size + 1); ++i) {
    bFoundSubDir = false;

    int j;
    for (j = 0; j < subdir_size; ++j) {
      // DEBUG ONLY
      // std::string str1,str2;
      // str1 = itStringUtil::GetStdString( m_Names[i+j] );
      // str2 = itStringUtil::GetStdString( i_SubDir.GetName(j) );
      // DBG_LOG4("(%02d) (%12s) == (%02d) (%12s)", (i+j),str1.c_str(),j,str2.c_str());

      //	find the first occurence in the list
      //
      if (i_SubDir.GetName(j) == m_Names[i + j]) {
        bFoundSubDir = true;

        if (j == 0) {
          startindex = i;
        }
      } else {
        bFoundSubDir = false;
        break;
      }
    }

    //	if found, remove it
    //
    if (bFoundSubDir) {
      if (startindex >= 0) {
        m_Index = 0;

        // remove all of the indexes at the front.
        //
        fsLocator temp;
        int j;
        for (j = 0; j < name_size; ++j) {
          if ((j < startindex) || (j >= startindex + subdir_size)) {
            temp.Push(m_Names[j]);
          }
        }

        this->Clear();
        this->Push(temp);
      }
      return true;
    }
  }

  return false;
}

//------------------------------------------------------------------------
//	Remove() find the first occurence of the itString passed in and
//	remove it.
//
//	returns true if it found the string in the locator
//------------------------------------------------------------------------
bool
fsLocator::Remove(const itString& i_DirName)
{
  DBG_ASSERT(0 < m_Names.size(), "Cannot remove a directory from an empty locator");

  bool bFoundSubDir = false;
  int startindex = -1;
  int name_size = m_Names.size();
  fsLocator temppath;
  int i;
  for (i = 0; i < name_size; ++i) {
    if (!bFoundSubDir && (m_Names[i] == i_DirName)) {
      bFoundSubDir = true;
    } else {
      temppath.Push(m_Names[i]);
    }
  }

  if (bFoundSubDir) {
    this->Clear();
    this->Push(temppath);
  }
  return bFoundSubDir;
}

//------------------------------------------------------------------------
//	Flag for seeing if path is UNC (uniform naming convention)
//
//	setUNCPath() exists so if directories are being pushed on the
//	code can signal that it is a UNC path.
//------------------------------------------------------------------------
bool
fsLocator::IsUNCPath() const
{
  return m_bUNCPath;
}
void
fsLocator::SetUNCPath(const bool i_bIsUNCPath)
{
  m_bUNCPath = i_bIsUNCPath;
}
