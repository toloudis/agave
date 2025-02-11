/********************************************************************
**  envSystemDataPACWin.hpp
**
**      envSystemDataPACWin is the declaration for the windows
**	version of the system data PAC.  It supplies, in addition to the
**	Init and CleanUp functions, a function to detect which Windows
**	OS is being used (at runtime).
**
**
**
\*******************************************************************/

#ifdef ENV_SYSTEMDATAPACWIN_HPP
#error envSystemDataPACWin.hpp multiply included
#endif
#define ENV_SYSTEMDATAPACWIN_HPP

//============================================================================
//============================================================================
namespace envSystemDataPAC {
enum WindowsOS
{
  e_Windows95,
  e_Windows98,
  e_WindowsNT40,
  e_Windows2000,
  e_WindowsME,
  e_WindowsCE,
  e_WindowsXP,
  e_WindowsVista,
  e_Windows7,
  e_Unknown
};

//------------------------------------------------------------------------
//	As you might expect, this function finds which version of windows
//	is being used.  The version is cached so it's reasonably fast.
//------------------------------------------------------------------------
WindowsOS
GetWindowsOSType();

//------------------------------------------------------------------------
//	Don't call Init() yourself; it is called by the package Init().
//------------------------------------------------------------------------
void
Init();

//------------------------------------------------------------------------
//	Don't call CleanUp() yourself; it is called by the package CleanUp().
//------------------------------------------------------------------------
void
CleanUp() throw();
}
