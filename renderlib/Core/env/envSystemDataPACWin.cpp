/**********************************************************
**  envSystemDataPACWin.cpp
**
**      envSystemDataPACWin is the definition for the windows
**	version of the system data PAC.
**
**
**
\*********************************************************/

#include <windows.h>

#include "Core/env/envSystemDataPACWin.hpp"

#include "Core/env/envInitX.hpp"

namespace envSystemDataPAC {

namespace {

WindowsOS l_CurWindowsVersion = e_Unknown;

}

//========================================================================
//	As you might expect, this function finds which version of windows
//	is being used.  The version is cached so it's reasonably fast.
//========================================================================
WindowsOS
GetWindowsOSType()
{
  return l_CurWindowsVersion;
}

//========================================================================
//	Don't call Init() yourself; it is called by the package Init().
//========================================================================
void
Init()
{
  // TODO replace with VersionHelper api functions

  // Deduce os version.  This code was copied from the platform SDK documentation
  //
  OSVERSIONINFOEXA os_info_ex;

  // Try calling GetVersionEx using the OSVERSIONINFOEX structure,
  // which is supported on Windows 2000.
  //
  // If that fails, try using the OSVERSIONINFO structure.
  ZeroMemory(&os_info_ex, sizeof(OSVERSIONINFOEXA));
  os_info_ex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

  if (GetVersionExA((OSVERSIONINFOA*)&os_info_ex) == 0) {
    // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
    os_info_ex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

    if (GetVersionExA((OSVERSIONINFOA*)&os_info_ex) == 0)
      throw envInitX("env");
  }

  switch (os_info_ex.dwPlatformId) {
    case VER_PLATFORM_WIN32_NT: {
      if (os_info_ex.dwMajorVersion <= 4)
        l_CurWindowsVersion = e_WindowsNT40;
      else if (os_info_ex.dwMajorVersion == 5)
        l_CurWindowsVersion = e_Windows2000;
      else if (os_info_ex.dwMajorVersion == 6)
        l_CurWindowsVersion = e_WindowsXP; // windows7 shows up as 6 also
      else
        l_CurWindowsVersion = e_Windows7;
    } break;

    case VER_PLATFORM_WIN32_WINDOWS: {
      if ((os_info_ex.dwMajorVersion > 4) || ((os_info_ex.dwMajorVersion == 4) && (os_info_ex.dwMinorVersion > 0))) {
        l_CurWindowsVersion = e_Windows98;
      } else {
        l_CurWindowsVersion = e_Windows95;
      }
    } break;

    default:
      l_CurWindowsVersion = e_Unknown;
      break;
  }
}

//========================================================================
//	Don't call CleanUp() yourself; it is called by the package CleanUp().
//========================================================================
void
CleanUp() throw()
{
}

}
