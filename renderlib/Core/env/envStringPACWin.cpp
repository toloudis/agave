//****************************************************************************
//  envStringPACWin.cpp
//
//      see .hpp
//
//
//
//****************************************************************************
#include "envStringPACWin.hpp"

#include <windows.h>

//============================================================================
//============================================================================
namespace envStringPAC {
//------------------------------------------------------------------------
// Convert from 16-bit wide unicode string to 8-bit multibyte
// UTF8 unicode string.
//------------------------------------------------------------------------
std::string
WideCharToUTF8(const envType::WChar* i_String, int i_Length)
{
  std::string res;
  if (i_String != NULL) {
    int nOutputStrLen = WideCharToMultiByte(CP_UTF8, 0, i_String, i_Length, NULL, 0, 0, 0);

    // bga -  An example on the web had double NULL termination, why would that be needed?

    // NULL Termination, make sure that buffer length is larger than
    // the value given to WideCharToMultiByte below to handle boundary case.
    char* buffer = new char[nOutputStrLen + 1];
    int nResult = WideCharToMultiByte(CP_UTF8, 0, i_String, i_Length, buffer, nOutputStrLen, 0, 0);
    buffer[nResult] = 0x00;
    res = buffer;
    delete[] buffer;
  }
  return res;
}

//------------------------------------------------------------------------
// Convert from 8-bit UTF8 multibyte unicode string to 16-bit wide
// unicode string.
//------------------------------------------------------------------------
std::wstring
UTF8ToWideChar(const char* i_String, int i_Length)
{
  std::wstring res;
  if (i_String != NULL) {
    int nOutputStrLen = MultiByteToWideChar(CP_UTF8, 0, i_String, i_Length, NULL, 0);

    // NULL Termination, make sure that buffer length is larger than
    // the value given to WideCharToMultiByte below to handle boundary case.
    wchar_t* buffer = new wchar_t[nOutputStrLen + 1];
    int nResult = MultiByteToWideChar(CP_UTF8, 0, i_String, i_Length, buffer, nOutputStrLen);
    buffer[nResult] = 0x0000;
    res = buffer;
    delete[] buffer;
  }
  return res;
}
}
