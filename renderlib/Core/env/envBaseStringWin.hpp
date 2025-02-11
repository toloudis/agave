/****************************************************************************\
**  envBaseStringWin.hpp
**
**      envBaseStringWin.hpp defines some indices into the Win32 string table
**	of the application.  These refer to strings which must be localized but
**	which for some reason are not convenient to put in a *.tsf file (for
**	example, they must be used by a package which does not know about the gf
**	package).
**
**
**
\****************************************************************************/

#ifdef ENV_BASESTRINGWIN_HPP
#error envBaseStringWin.hpp multiply included
#endif
#define ENV_BASESTRINGWIN_HPP

//============================================================================
//============================================================================
namespace envBaseString {
const int c_InstallCorrupt = 1;
const int c_InsertCD = 2;
const int c_ReInsertCD = 3;
const int c_No3DAccel = 4;
const int c_MissingCDTitle = 5;
const int c_InitFailureTitle = 6;
const int c_InstallCorruptTitle = 7;
}
