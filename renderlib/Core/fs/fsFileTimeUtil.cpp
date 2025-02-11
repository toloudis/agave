/****************************************************************************\
**  fsFileTimeUtil.cpp
**
**      fsFileTimeUtil.hpp supplies functions that operate on files.
**
**
**
\****************************************************************************/

#include "Core/fs/fsFileTimeUtil.hpp"

#include "Core/fs/fsFileTimeUtilPAC.hpp"
#include "Core/it/itString.hpp"

namespace fsFileTimeUtil {

//========================================================================
//	GetCreationTime returns a double in UTC (Coordinated Universal Time)
//	i.e. the number of seconds since January 1, 1970, 00:00:00 GMT which
//	represents the file's creation time
//========================================================================
double
GetCreationTime(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetCreationTime(i_Locator);
}

//========================================================================
//	GetLastAccessTime returns a double in UTC (Coordinated Universal Time)
//	i.e. the number of seconds since January 1, 1970, 00:00:00 GMT which
//	represents the file's last access time
//========================================================================
double
GetLastAccessTime(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetLastAccessTime(i_Locator);
}

//========================================================================
//	GetLastModifiedTime returns a double in UTC (Coordinated Universal Time)
//	i.e. the number of seconds since January 1, 1970, 00:00:00 GMT which
//	represents the file's last modified time
//========================================================================
double
GetLastModifiedTime(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetLastModifiedTime(i_Locator);
}

//========================================================================
//	GetCreationString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//========================================================================
itString
GetCreationString(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetCreationString(i_Locator);
}

//========================================================================
//	GetLastAccessString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//========================================================================
itString
GetLastAccessString(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetLastAccessString(i_Locator);
}

//========================================================================
//	GetLastModifiedString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//========================================================================
itString
GetLastModifiedString(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetLastModifiedString(i_Locator);
}

//========================================================================
//	GetCreationString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//========================================================================
itString
GetCreationStringWithTime(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetCreationStringWithTime(i_Locator);
}

//========================================================================
//	GetLastAccessString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//========================================================================
itString
GetLastAccessStringWithTime(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetLastAccessStringWithTime(i_Locator);
}

//========================================================================
//	GetLastModifiedString returns the in an itstring of the form
//	mm/dd/yy hh:mm
//========================================================================
itString
GetLastModifiedStringWithTime(const fsLocator& i_Locator)
{
  return fsFileTimeUtilPAC::GetLastModifiedStringWithTime(i_Locator);
}

//========================================================================
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//========================================================================
void
Init()
{
  fsFileTimeUtilPAC::Init();
}

void
CleanUp() throw()
{
  fsFileTimeUtilPAC::CleanUp();
}

}
