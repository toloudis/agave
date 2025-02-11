/****************************************************************************\
**  fsFileUtilPAC.hpp
**
**      fsFileUtilPAC.hpp forwards calls from the fsFileUtil to the
**	correct PAC component.
**
**
**
\****************************************************************************/

#ifdef FS_FILEUTILPAC_HPP
#error fsFileUtilPAC.hpp multiply included
#endif
#define FS_FILEUTILPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

// PAC components should support the following interface:
//
/*namespace fsFileUtilPAC
//{
        void CreateFile(const fsLocator& i_Filename);
        void RenameFile(const fsLocator& i_From, const fsLocator& i_To);
        void DeleteFile(const fsLocator& i_Filename);
        void CopyFile(const fsLocator& i_From, const fsLocator& i_To);
        void CreateDirectory(const fsLocator& i_DirectoryName);
        void DeleteDirectory(const fsLocator& i_DirectoryName);
        bool FileExists(const fsLocator& i_Filename);
        bool DirectoryExists(const fsLocator& i_Filename);
        void LocatorToANSIFilename(const fsLocator& i_Locator, std::string& o_String);
        bool IsReadOnly(const fsLocator& i_Locator);
        void SetReadOnly( const fsLocator& i_Locator, bool i_bSetReadOnly )

        void Init();
        void CleanUp() throw();
}*/

#if ENV_WINDOWS
#include "Core/fs/fsFileUtilPACWin.hpp"
#else
#if ENV_OS == ENV_PS2OS
#include "Core/fs/fsFileUtilPACPS2.hpp"
#else
#if ENV_OS == ENV_XBOXOS
#include "Core/fs/fsFileUtilPACXbox.hpp"
#else
#error fsFileUtilPAC not defined for this platform
#endif
#endif
#endif
