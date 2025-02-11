/****************************************************************************\
**  envExceptionX.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/env/envExceptionX.hpp"

#ifdef ENV_EXCEPTION_STACK_TRACE
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

// Get the RtlCaptureContext function at runtime so we can support older SDKs
// and machines without it
typedef VOID(WINAPI* LPRtlCaptureContext)(PCONTEXT ContextRecord);
#endif // ENV_EXCEPTION_STACK_TRACE

//------------------------------------------------------------------------
//------------------------------------------------------------------------
envExceptionX::envExceptionX()
{
#ifdef ENV_EXCEPTION_STACK_TRACE
  RecordStackTrace();
#endif
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
envExceptionX::~envExceptionX() {}

#ifdef ENV_EXCEPTION_STACK_TRACE
void
envExceptionX::RecordStackTrace()
{
  // See if we can get RtlCaptureContext
  LPRtlCaptureContext pfnRtlCaptureContext =
    (LPRtlCaptureContext)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "RtlCaptureContext");
  if (!pfnRtlCaptureContext) {
    // RtlCaptureContext not supported
    return;
  }

  if (!pfnRtlCaptureContext)
    return;

  // Capture context
  static CONTEXT ctx;
  pfnRtlCaptureContext(&ctx);

  // Init the stack frame for this function
  static STACKFRAME64 theStackFrame;
  memset(&theStackFrame, 0, sizeof(theStackFrame));
#ifdef _M_IX86
  DWORD dwMachineType = IMAGE_FILE_MACHINE_I386;
  theStackFrame.AddrPC.Offset = ctx.Eip;
  theStackFrame.AddrPC.Mode = AddrModeFlat;
  theStackFrame.AddrFrame.Offset = ctx.Ebp;
  theStackFrame.AddrFrame.Mode = AddrModeFlat;
  theStackFrame.AddrStack.Offset = ctx.Esp;
  theStackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
  DWORD dwMachineType = IMAGE_FILE_MACHINE_AMD64;
  theStackFrame.AddrPC.Offset = ctx.Rip;
  theStackFrame.AddrPC.Mode = AddrModeFlat;
  theStackFrame.AddrFrame.Offset = ctx.Rsp;
  theStackFrame.AddrFrame.Mode = AddrModeFlat;
  theStackFrame.AddrStack.Offset = ctx.Rsp;
  theStackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
  DWORD dwMachineType = IMAGE_FILE_MACHINE_IA64;
  theStackFrame.AddrPC.Offset = ctx.StIIP;
  theStackFrame.AddrPC.Mode = AddrModeFlat;
  theStackFrame.AddrFrame.Offset = ctx.IntSp;
  theStackFrame.AddrFrame.Mode = AddrModeFlat;
  theStackFrame.AddrBStore.Offset = ctx.RsBSP;
  theStackFrame.AddrBStore.Mode = AddrModeFlat;
  theStackFrame.AddrStack.Offset = ctx.IntSp;
  theStackFrame.AddrStack.Mode = AddrModeFlat;
#else
#error "Platform not supported!"
#endif

  // Walk up the stack
  memset(nPC, 0, sizeof(nPC));
  for (int i = 0; i < cnMaxStackFrames; ++i) {
    nPC[i] = theStackFrame.AddrPC.Offset;
    HANDLE prc = GetCurrentProcess();
    if (!StackWalk64(dwMachineType,
                     prc,
                     GetCurrentThread(),
                     &theStackFrame,
                     &ctx,
                     NULL,
                     SymFunctionTableAccess64,
                     SymGetModuleBase64,
                     NULL)) {
      break;
    }
  }
}
#endif

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
const char*
envExceptionX::GetCallStack() const
{
#ifdef ENV_EXCEPTION_STACK_TRACE
  const size_t cnBufferSize = 512;
  unsigned int nLine;
  static char szBuff[4096];
  static char nBuff[24];

  // Initialise allocation source
  strcpy_s(szBuff, "");
  nLine = 0;

  SymInitialize(GetCurrentProcess(), NULL, TRUE);
  HANDLE prc = GetCurrentProcess();

  // Resolve PC to function names
  DWORD64 PC;
  for (int i = 0; i < cnMaxStackFrames; ++i) {
    // Check for end of stack walk
    PC = nPC[i];
    if (PC == 0)
      break;

    // Get function name
    unsigned char byBuffer[sizeof(SYMBOL_INFO) + cnBufferSize];
    SYMBOL_INFO* pSymbol = (SYMBOL_INFO*)byBuffer;
    DWORD64 dwDisplacement;
    memset(pSymbol, 0, sizeof(SYMBOL_INFO) + cnBufferSize);
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = cnBufferSize;
    if (!SymFromAddr(prc, PC, &dwDisplacement, pSymbol)) {
      strcat_s(szBuff, "??\n");
    } else {
      pSymbol->Name[cnBufferSize - 1] = '\0';
      // See if we need to go further up the stack
      if (strncmp(pSymbol->Name, "envException::", 14) == 0) {
        // In EnvException, keep going...
      } else {
        IMAGEHLP_LINE64 theLine;
        DWORD dwDisplacement;
        memset(&theLine, 0, sizeof(theLine));
        theLine.SizeOfStruct = sizeof(theLine);
        if (!SymGetLineFromAddr64(prc, PC, &dwDisplacement, &theLine)) {
          strcat_s(szBuff, "??");
          nLine = 0;
        } else {
          const char* pszFile = strrchr(theLine.FileName, '\\ ');
          if (!pszFile)
            pszFile = theLine.FileName;
          else
            ++pszFile;
          strncat_s(szBuff, pszFile, cnBufferSize);
          nLine = theLine.LineNumber;
        }
        strcat_s(szBuff, ":");
        _itoa_s(nLine, nBuff, 10);
        strcat_s(szBuff, nBuff);
        strcat_s(szBuff, " (");
        strcat_s(szBuff, pSymbol->Name);
        strcat_s(szBuff, ")\n\r");
      }
      if (strncmp(pSymbol->Name, "WinMain", 7) == 0) {
        break; // don't go higher than winmain in the call stack.
      }
    }
  }

  return szBuff;
#else  // ENV_EXCEPTION_STACK_TRACE
  return "Stack trace unavailable";
#endif // ENV_EXCEPTION_STACK_TRACE
}
