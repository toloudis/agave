/****************************************************************************\
**  fsFileEnumPACWin.hpp
**
**      fsFileEnumPACWin.hpp is the interface to the windows pac for the
**	fsFileEnum component.
**
**
**
\****************************************************************************/

#ifdef FS_FILEENUMPACWIN_HPP
#error fsFileEnum.hpp multiply included
#endif
#define FS_FILEENUMPACWIN_HPP

#ifndef FS_FILEENUM_HPP
#include "Core/fs/fsFileEnum.hpp"
#endif

//============================================================================
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
namespace fsFileEnumPAC {

//------------------------------------------------------------------------
//	EnumerateFiles lists all of the files in a directory.  If i_SubStrings
//	is not empty, only files whose names contain one of the given substrings
//	will be listed. Returns the last return value from fsEnumTarget::Notify
//------------------------------------------------------------------------
bool
EnumerateFiles(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target, const std::vector<itString>& i_SubStrings);

//------------------------------------------------------------------------
//	EnumerateDirectories lists all of the subdirectories in a directory.
//	Returns the last return value from fsEnumTarget::Notify
//------------------------------------------------------------------------
bool
EnumerateDirectories(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target);

//------------------------------------------------------------------------
//	GetDrives lists all of the drives listed by windows
//------------------------------------------------------------------------
void
GetDrives(std::vector<fsLocator>& o_DirList);

}
