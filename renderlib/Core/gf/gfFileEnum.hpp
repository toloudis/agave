/****************************************************************************\
**  gfFileEnum.hpp
**
**      gfFileEnum.hpp supplies functions that layer the existing functionality
**		of fsFileEnum to better supported the automated use of fsLocator's index
**		and the multiple paths that may be associated w/ it
**
**
**
\****************************************************************************/

#ifdef GF_FILEENUM_HPP
#error gfFileEnum.hpp multiply included
#endif
#define GF_FILEENUM_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

#ifndef FS_FILEENUM_HPP
#include "Core/fs/fsFileEnum.hpp"
#endif

//============================================================================
//============================================================================
namespace gfFileEnum {

//------------------------------------------------------------------------
//	EnumerateFiles lists all of the files in a directory.  If i_SubStrings
//	is not empty, only files whose names contain one of the given substrings
//	will be listed. Ensures that i_Locator is properly expanded by the
//	gfFileTranslationMgr and all paths associated w/ i_Locator's index (if any)
//	get enumerated
//------------------------------------------------------------------------
void
EnumerateFiles(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target, bool i_bSearchSubdirectories = false);
void
EnumerateFiles(const fsLocator& i_Locator,
               fsFileEnum::EnumTarget& o_Target,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories = false);

//------------------------------------------------------------------------
//	EnumerateFiles uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//------------------------------------------------------------------------
void
EnumerateFiles(const fsLocator& i_Locator, fsFileEnum::fsFileList& o_FileList, bool i_bSearchSubdirectories = false);
void
EnumerateFiles(const fsLocator& i_Locator,
               fsFileEnum::fsFileList& o_FileList,
               const std::vector<itString>& i_SubStrings,
               bool i_bSearchSubdirectories = false);

//------------------------------------------------------------------------
//	EnumerateDirectories lists all of the subdirectories in a directory.
//	Ensures that i_Locator is properly expanded by the gfFileTranslationMgr
//	and all paths associated w/ i_Locator's index (if any)
//	get enumerated
//------------------------------------------------------------------------
void
EnumerateDirectories(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target);

//------------------------------------------------------------------------
//	EnumerateDirectories uses a fsFileEnum::fsFileList instead of a
//  fsFileEnum::EnumTarget.
//------------------------------------------------------------------------
void
EnumerateDirectories(const fsLocator& i_Locator, fsFileEnum::fsFileList& o_DirList);

}
