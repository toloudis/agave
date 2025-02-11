/****************************************************************************\
**  fsFileEnumPAC.hpp
**
**      fsFileEnumPAC.hpp forwards calls from the fsFileEnum to the
**	correct PAC component.
**
**
**
\****************************************************************************/

#ifdef FS_FILEENUMPAC_HPP
#error fsFileEnumPAC.hpp multiply included
#endif
#define FS_FILEENUMPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

// PAC components should support the following interface:
//
/*namespace fsFileEnumPAC
{
void EnumerateFiles(const fsLocator& i_Locator,
                                        fsFileEnum::EnumTarget& o_Target,
                                        const itString* i_SubString = NULL);
void EnumerateDirectories(	const fsLocator& i_Locator,
                                                        fsFileEnum::EnumTarget& o_Target);

}*/

#if ENV_WINDOWS
#include "Core/fs/fsFileEnumPACWin.hpp"
#else
#if ENV_OS == ENV_PS2OS
#include "Core/fs/fsFileEnumPACPS2.hpp"
#else
#if ENV_OS == ENV_XBOXOS
#include "Core/fs/fsFileEnumPACXbox.hpp"
#else
#error fsFileEnumPAC not defined for this platform
#endif
#endif
#endif
