//****************************************************************************
//  dbgAssertPACWin.cpp
//
//      see .hpp
//
//
//
//****************************************************************************
#include <windows.h>

#include "Core/dbg/dbgAssertPACWin.hpp"

//============================================================================
//============================================================================
namespace dbgAssertPAC {
//------------------------------------------------------------------------
// Pops up a window with the text and an OK button.
//
// \param i_Text the string to display
//
// \return void
//------------------------------------------------------------------------
void
OKMessage(const char* i_Text)
{
  ::SetForegroundWindow(::GetDesktopWindow());
  ::MessageBoxA(NULL, i_Text, "Application Alert", MB_OK | MB_SETFOREGROUND);
}
void
OKMessage(const wchar_t* i_Text)
{
  ::SetForegroundWindow(::GetDesktopWindow());
  ::MessageBoxW(NULL, i_Text, L"Application Alert", MB_OK | MB_SETFOREGROUND);
}

//------------------------------------------------------------------------
// Pops up a window the the text and abort and continue
// buttons.
//
// \param i_Text the string to display
//
// \return the button pressed (true == abort, false == continue)
//------------------------------------------------------------------------
bool
AbortContinueMessage(const char* i_Text)
{
  ::SetForegroundWindow(::GetDesktopWindow());
  int ret_val = ::MessageBoxA(NULL, i_Text, "Application Alert", MB_ABORTRETRYIGNORE | MB_SETFOREGROUND);
  if (ret_val == IDABORT)
    return true;
  else if (ret_val == IDRETRY) {
    __debugbreak();
    return true;
  } else
    return false;
}
bool
AbortContinueMessage(const wchar_t* i_Text)
{
  ::SetForegroundWindow(::GetDesktopWindow());
  int ret_val = ::MessageBoxW(NULL, i_Text, L"Application Alert", MB_ABORTRETRYIGNORE | MB_SETFOREGROUND);
  if (ret_val == IDABORT)
    return true;
  else if (ret_val == IDRETRY) {
    ::DebugBreak();
    return true;
  } else
    return false;
}

//------------------------------------------------------------------------
//	Init is called by the package initializer.  Don't call it yourself.
//
// \param none
//
// \return void
//------------------------------------------------------------------------
void
Init()
{
  // no operations needed for now
}

//------------------------------------------------------------------------
//	CleanUp is called by the package initializer.  Don't call it yourself.
//
// \param none
//
// \return void
//------------------------------------------------------------------------
void
CleanUp() throw()
{
  // no operations needed for now
}

}
