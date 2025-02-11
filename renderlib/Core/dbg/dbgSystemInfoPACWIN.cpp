//****************************************************************************
//  dbgSystemInfoPACWin.cpp
//
//      see .hpp
//
//
//
//****************************************************************************
#include <string.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>
// #include <intrin.h>
#include <windows.h>

#include "dbgSystemInfoPACWin.hpp"
#include "Core/Env/envInitX.hpp"
#include "Core/Dbg/dbgMsg.hpp"

//============================================================================
//============================================================================
namespace dbgSystemInfoPAC {
//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getUserName()
{
  TCHAR userID[256];
  DWORD _buff = 256;
  GetUserName(LPWSTR(&userID), &_buff);
  return std::wstring(userID);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getComputerName()
{
  TCHAR compID[256];
  DWORD _buff = 256;
  GetComputerName(LPWSTR(&compID), &_buff);
  return std::wstring(compID);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
getMemory()
{
  MEMORYSTATUS _memStatus;
  GlobalMemoryStatus(&_memStatus);
  return _memStatus.dwTotalVirtual / (1024 * 1024); // returned in MB
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getOS()
{
  // TODO : replace this code with calls from VersionHelpers

  std::wstring _osVersion;
  OSVERSIONINFOEX _osInfo;
  _osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  GetVersionEx((OSVERSIONINFO*)&_osInfo);
  switch (_osInfo.dwMajorVersion) {
    case 6:
      if (_osInfo.dwMinorVersion == 1) {
        if (_osInfo.wProductType == VER_NT_WORKSTATION) {
          _osVersion = L"Windows 7, ";
          _osVersion += _osInfo.szCSDVersion;
        } else {
          _osVersion = L"Windows Server 2008 R2, ";
          _osVersion += _osInfo.szCSDVersion;
        }
      } else if (_osInfo.dwMinorVersion == 0) {
        if (_osInfo.wProductType == VER_NT_WORKSTATION) {
          _osVersion = L"Windows Vista, ";
          _osVersion += _osInfo.szCSDVersion;
        } else {
          _osVersion = L"Windows Server 2008, ";
          _osVersion += _osInfo.szCSDVersion;
        }
      }
      break;
    case 5:
      if (_osInfo.dwMinorVersion == 2) {
        _osVersion = L"Windows Server 2003, ";
        _osVersion += _osInfo.szCSDVersion;
      }
      if (_osInfo.dwMinorVersion == 1) {
        _osVersion = L"Windows XP, ";
        _osVersion += _osInfo.szCSDVersion;
      }
      if (_osInfo.dwMinorVersion == 0) {
        _osVersion = L"Windows 2000, ";
        _osVersion += _osInfo.szCSDVersion;
      }
      break;
    default:
      _osVersion = L"Operation System, ";
      _osVersion += _osInfo.dwMajorVersion + L"." + _osInfo.dwMinorVersion;
      break;
  }
  return _osVersion;
}
}