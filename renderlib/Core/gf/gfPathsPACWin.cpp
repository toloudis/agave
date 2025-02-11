/****************************************************************************\
**  gfPathsPACWin.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/gf/gfPathsPACWin.hpp"

#include <stdio.h>
#include <windows.h>
// #include <shlobj.h>

#include "Core/dbg/dbgMsg.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/fs/fsLocator.hpp"
#include "Core/gf/gfPaths.hpp"

#include <shlobj.h> // for SHGetFolderPath

//============================================================================
//============================================================================
namespace gfPathsPAC {
fsLocator l_UserDataSubPath;

namespace {

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// void get_cd_path(std::string& o_Path, const char* i_CDVolumeName)
//{
//	if (!i_CDVolumeName)
//	{
//		return;
//	}
//
//	//	iterate through all cd drives
//	int		i;
//	DWORD	drive_mask = ::GetLogicalDrives();
//
//	// Skip over Drive A: and B: (floppies ?)
//	//
//	drive_mask >>= 2;
//	std::wstring path_name = L"A:\\";
//	for( i = 2 ; i < 32 ; i++ )
//	{
//		DWORD has_drive = drive_mask & 1;
//		drive_mask >>= 1;
//
//		if( has_drive )
//		{
//			path_name[0] = (TCHAR)('A' + i);
//			UINT drive_type = ::GetDriveTypeW(path_name.c_str());
//
//			if( DRIVE_CDROM == drive_type )
//			{
//				TCHAR volume_name_buffer[MAX_PATH];
//				DWORD serial_number, max_component_length, file_system_flags;
//				TCHAR file_system_name[MAX_PATH];
//
//				//	don't display "insert CD in drive" type messages
//				//
//				::SetErrorMode(SEM_FAILCRITICALERRORS);
//
//				BOOL ret_val = ::GetVolumeInformationW(	path_name.c_str(),
//														volume_name_buffer,
//														MAX_PATH,
//														&serial_number,
//														&max_component_length,
//														&file_system_flags,
//														file_system_name,
//														MAX_PATH);
//
//				::SetErrorMode(0);
//
//				if( ret_val )
//				{
//					path_name.resize(2);	//	keep only drive letter and colon
//					if( ::_stricmp(volume_name_buffer, i_CDVolumeName) == 0 )
//					{
//						o_Path = path_name;
//						return;
//					}
//				}
//			}
//		}
//	}
//
//	o_Path.resize(0);
//}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
add_path_to_locator(const std::wstring& i_PathName, fsLocator& o_Locator)
{
  std::wstring::size_type last_pos = 0;
  std::wstring::size_type token_pos = 0;
  std::wstring name;
  itString wide_char_itstring;

  while (token_pos != std::string::npos) {
    token_pos = i_PathName.find('\\', last_pos);

    name = i_PathName.substr(last_pos, token_pos - last_pos);

    if (name.size() > 0) {
      o_Locator.Push(itString(name.c_str()));
      last_pos = token_pos + 1;
    }
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// void add_path_to_locator(const std::string& i_PathName, fsLocator& o_Locator)
//{
//	std::string::size_type last_pos = 0;
//	std::string::size_type token_pos = 0;
//	std::string name;
//	itString wide_char_itstring;
//
//	while( token_pos != std::string::npos )
//	{
//		token_pos = i_PathName.find('\\', last_pos);
//
//		name = i_PathName.substr(last_pos, token_pos - last_pos);
//
//		if( name.size() > 0 )
//		{
//			itString::CharType wide_char_name[512];
//
//			::MultiByteToWideChar(	::GetACP(),
//									0,
//									name.c_str(),
//									-1,
//									wide_char_name,
//									512);
//
//			wide_char_itstring = wide_char_name;
//
//			o_Locator.Push(wide_char_itstring);
//			last_pos = token_pos + 1;
//		}
//	}
//}

void
get_sysdat_path(std::wstring& o_SysdatPath)
{
  //	all users documents folder will be used
  TCHAR path[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
  o_SysdatPath = path;
}

void
get_localsysdat_path(std::wstring& o_SysdatPath)
{
  //	local user documents folder will be used
  TCHAR path[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
  o_SysdatPath = path;
}
}

//----------------------------------------------------------------------------
//	InitPaths handles initializing all the predefined game paths
//
//----------------------------------------------------------------------------
// void InitPaths(std::vector<GamePathList>& o_Vec, const char* i_CDVolumeName)
void
InitPaths(std::vector<GamePathList>& o_Vec)
{
  TCHAR exePath[MAX_PATH];
  TCHAR tempPath[MAX_PATH];
  int PathLen = 0;

  //	executable path
  //
  PathLen = GetModuleFileName(NULL, exePath, MAX_PATH);
  DBG_ASSERT(PathLen, "Retrieval of executable directory path failed.");
  o_Vec[gfPaths::e_ExePath].resize(1);
  add_path_to_locator(exePath, o_Vec[gfPaths::e_ExePath][0]);

  // GetmoduleFileName will leave the filename at the end, so now we pop that off
  o_Vec[gfPaths::e_ExePath][0].Pop();

  //	CDROM path
  //
  //	TODO - support multiple Disc Drives
  //
  // std::wstring cd_path;
  // get_cd_path(cd_path, i_CDVolumeName); // i_CDVolumeName was always NULL
  o_Vec[gfPaths::e_CDROMPath].resize(1);
  // if( cd_path.size() )
  //	add_path_to_locator(cd_path, o_Vec[gfPaths::e_CDROMPath][0]);
  // else
  o_Vec[gfPaths::e_CDROMPath][0].Clear();

  //	system temp path
  //
  PathLen = GetTempPath(MAX_PATH, tempPath);
  DBG_ASSERT(PathLen, "Retrieval of Temp directory path failed.");
  o_Vec[gfPaths::e_SystemTempPath].resize(1);
  add_path_to_locator(tempPath, o_Vec[gfPaths::e_SystemTempPath][0]);

  //	system data path
  //
  std::wstring sysdat_path;
  get_sysdat_path(sysdat_path);
  o_Vec[gfPaths::e_SystemDataPath].resize(1);
  if (sysdat_path.size() > 0)
    add_path_to_locator(sysdat_path.c_str(), o_Vec[gfPaths::e_SystemDataPath][0]);

  std::wstring local_sysdat_path;
  get_localsysdat_path(local_sysdat_path);
  o_Vec[gfPaths::e_LocalSystemDataPath].resize(1);
  if (local_sysdat_path.size() > 0)
    add_path_to_locator(local_sysdat_path.c_str(), o_Vec[gfPaths::e_LocalSystemDataPath][0]);

  //	User Data Path
  //

  //	if the app hasn't set some kind of subpath, then default to the company
  if (l_UserDataSubPath.GetNumNames() == 0) {
    l_UserDataSubPath.Push("AllenInstitute");
  }

  o_Vec[gfPaths::e_UserDataPath].resize(1);
  fsLocator udpath;
  GetUserDataDirectory(udpath);
  if (udpath.GetNumNames() > 0) {
    o_Vec[gfPaths::e_UserDataPath][0] = udpath;
  }

  o_Vec[gfPaths::e_UserDocumentsPath].resize(1);
  GetUserDocumentsDirectory(udpath);
  if (udpath.GetNumNames() > 0) {
    o_Vec[gfPaths::e_UserDocumentsPath][0] = udpath;
  }

  o_Vec[gfPaths::e_CommonDocumentsPath].resize(1);
  GetCommonDocumentsDirectory(udpath);
  if (udpath.GetNumNames() > 0) {
    o_Vec[gfPaths::e_CommonDocumentsPath][0] = udpath;
  }
}

//----------------------------------------------------------------------------
//	returns true if the a CD with the given name is present.  This function
//	can be called without initializing the gf package.
//----------------------------------------------------------------------------
// bool CDInDrive(const char* i_CDVolumeName)
//{
//	std::string cd_path;
//	get_cd_path(cd_path, i_CDVolumeName);
//	return cd_path.size() > 0;
//}

//------------------------------------------------------------------------
//	SetCDPath searched drives for i_CDVolumeName and sets the path if found
//------------------------------------------------------------------------
// void SetCDPath(std::vector<GamePathList>& o_Vec, const char* i_CDVolumeName)
//{
//	//initialize CDROM path
//	std::string cd_path;
//	get_cd_path(cd_path, i_CDVolumeName);
//
//	o_Vec[gfPaths::e_CDROMPath].resize(1);
//	if( cd_path.size() )
//		add_path_to_locator(cd_path.c_str(), o_Vec[gfPaths::e_CDROMPath][0]);
//	else
//		o_Vec[gfPaths::e_CDROMPath][0].Clear();
//}

//------------------------------------------------------------------------
//	Get the user's "home" directory for storing data.
//	In Windows this is
//    "Documents and Settings/USER_NAME/Local Settings/Application Data/AllenInstitute/agave/My Documents"
//  In Linux it is the user's root directory
//
//	We add the "AllenInstitute/agave" directory
//------------------------------------------------------------------------
void
GetUserDataDirectory(fsLocator& o_Locator)
{
  //	check if the user installed apps in
  //	personal or all documents
  //
  //	TODO - if we have two apps and one is installed with user data
  //	and the other with all data, this won't work.  We need to check the
  //	app data folder, but that shouldn't be done here.
  //
  o_Locator.Clear();

  //	find where the user data path is located.
  //
  TCHAR path[MAX_PATH];
  HRESULT result = SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
  fsFileUtil::UnicodeStringToLocator(itString(path), o_Locator);
  o_Locator.Push(l_UserDataSubPath);

  DBG_TRACE("user data folder =" << o_Locator);
}

//  //[Current User]\My Documents
//  0: specialFolder := CSIDL_PERSONAL;
//
//  //All Users\Application Data
//  1: specialFolder := CSIDL_COMMON_APPDATA;
//
//  //[User Specific]\Application Data
//  2: specialFolder := CSIDL_LOCAL_APPDATA;
//
//  //Program Files
//  3: specialFolder := CSIDL_PROGRAM_FILES;
//
//  //All Users\Documents
//  4: specialFolder := CSIDL_COMMON_DOCUMENTS;

//------------------------------------------------------------------------
//	Get the user's "My Documents" directory for storing data.
//	In Windows this is the "Documents and Settings/USER_NAME/My Documents" folder
//	In Linux it is the user's root directory
//------------------------------------------------------------------------
void
GetUserDocumentsDirectory(fsLocator& o_Locator)
{
  //	check if the user installed apps in
  //	personal or all documents
  //
  //	TODO - if we have two apps and one is installed with user data
  //	and the other with all data, this won't work.  We need to check the
  //	app data folder, but that shouldn't be done here.
  //
  o_Locator.Clear();

  //	find where the user data path is located.
  //
  TCHAR path[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);
  fsFileUtil::UnicodeStringToLocator(itString(path), o_Locator);
  o_Locator.Push(l_UserDataSubPath);

  DBG_TRACE("user documents folder =" << o_Locator);
}

//------------------------------------------------------------------------
//	Get the public "My Documents" directory for storing data.
//	In Windows this is the "Users/Public/Documents" folder
//	In Linux it is the user's root directory
//------------------------------------------------------------------------
void
GetCommonDocumentsDirectory(fsLocator& o_Locator)
{
  //	check if the user installed apps in
  //	personal or all documents
  //
  //	TODO - if we have two apps and one is installed with user data
  //	and the other with all data, this won't work.  We need to check the
  //	app data folder, but that shouldn't be done here.
  //
  o_Locator.Clear();

  //	find where the user data path is located.
  //
  TCHAR path[MAX_PATH];
  HRESULT result = SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path);
  fsFileUtil::UnicodeStringToLocator(itString(path), o_Locator);
  o_Locator.Push(l_UserDataSubPath);

  DBG_TRACE("public documents folder =" << o_Locator);
}

//------------------------------------------------------------------------
//	Set the user data path where the user data will be stored.
//	It could be installed under user/My Documents or all users/Documents
//	on Windows.
//	Under one of these types of folders will be appended the sub-folders
//	passed into this function.
//	AllenInstitute/agave (for example).
//------------------------------------------------------------------------
void
SetUserDataSubPath(fsLocator& i_UDSubPath)
{
  l_UserDataSubPath = i_UDSubPath;
}

}
