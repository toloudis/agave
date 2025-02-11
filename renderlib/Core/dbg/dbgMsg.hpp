//****************************************************************************
//	\file dbgMsg.hpp
//
// writes out strings to a log file for debugging.
// dbgLog contains debug logging (file and debugger) functions and
//	macros.  These write a message, possibly with sprintf style arguments,
//	to a debug log.  On Windows the message is written as a line in a file
//	"debug.log" in the same directory as the executable, and also to the
//	Visual Studio debugger.
//
// Here are some Examples of the syntax and operations for the new debug system.
//
// DBG_LOG("This is" << " a log");
// DBG_WARNING("This is a warning");
// DBG_ERROR("This is an error");
// DBG_TRACE("This is a trace");
// DBG_TEXT("This text won't show file, line or anyother information...just the text");
//
// dbgMsg::enableTimeStamp(true);
// dbgMsg::enableFileNameStamp(false);
// dbgMsg::enableLevelStamp(false);
//
// dbgMsg::addStream("Log2", new std::ofstream("Log2.log"));
// dbgMsg::enableDbgStream("Log2", false);
// dbgMsg::enableStreamLog("Log2", true);
// dbgMsg::enableStreamWarning("Log2", false);
// dbgMsg::enableStreamError("Log2", false);
// dbgMsg::enableStreamTrace("Log2", true);
// dbgMsg::removeStream("Log2");
//
// Macros will write to all active streams.
//
// Currently can write to:
// any ostream derived class - ofstream, std::cout, etc.
//
//
//
//****************************************************************************
#pragma once
#ifdef DBG_MSG_HPP
#error dbgMsg.hpp multiply included
#endif
#define DBG_MSG_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

#ifndef DBG_STREAM_HPP
#include "Core/dbg/dbgStream.hpp"
#endif

#include <string>
#include <sstream>
#include <vector>
#include <tchar.h>

// Macros that get the unicode (wchar_t) version of a string
// always, irrespective of whether UNICODE is defined.
// as opposed to the _T( ) macro in tchar.h.
// A double layer of macro-s is needed,
// because some of the arguments are macro-s itself (eg: __FILE__)
#define SGPU_L_TEMP(a) L##a
#define SGPU_L(a) SGPU_L_TEMP(a)

//============================================================================
//============================================================================
class fsLocator;

//============================================================================
// Debug Log interface
//============================================================================
namespace dbgMsg {
//------------------------------------------------------------------------
//	Write a Warning string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Warning(std::wstring& i_File, int i_Line, const std::string& i_Msg);

//------------------------------------------------------------------------
//	Write a Trace string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Trace(std::wstring& i_File, int i_Line, const std::string& i_Msg);

//------------------------------------------------------------------------
//	Write a Error string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Error(std::wstring& i_File, int i_Line, std::string& i_Msg);
void
Error(std::wstring& i_File, int i_Line, const std::string& i_Msg);

//------------------------------------------------------------------------
//	Write a Log string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Log(std::wstring& i_File, int i_Line, const std::string& i_Msg);

//------------------------------------------------------------------------
//	Write a Text string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Text(std::string& i_Msg);

//------------------------------------------------------------------------
//	This write function is not used by any macros; It does the main write
//	functions for each type of log.
//
//
// \return void
//------------------------------------------------------------------------
void
Write(std::wstring i_File, int i_Line, dbgStream* io_Stream, const std::wstring& i_DbgLevel);

//------------------------------------------------------------------------
//	This write function is not used by any macros; It does the main write
//	functions for each type of log.  It formats all output in CSV format
//
//
// \return void
//------------------------------------------------------------------------
void
WriteCSV(std::wstring i_File, int i_Line, dbgStream* io_Stream, const std::wstring& i_DbgLevel);

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteLog(const std::wstring& i_File, int i_Line, dbgStream* io_Stream);

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//	They always use single-byte ANSI characters.
//
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteWarning(const std::wstring& i_File, int i_Line, dbgStream* io_Stream);

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//	They always use single-byte ANSI characters.
//
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteError(const std::wstring& i_File, int i_Line, dbgStream* io_Stream);

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//	They always use single-byte ANSI characters.
//
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteTrace(const std::wstring& i_File, int i_Line, dbgStream* io_Stream);

//------------------------------------------------------------------------
//  If an assertion call is passed, run this function
//
//------------------------------------------------------------------------
void
AssertMsg(std::wstring& i_File, int i_Line);

//------------------------------------------------------------------------
//  Return the stream that will be used to read in the assertion message
//
//------------------------------------------------------------------------
std::stringstream&
GetAssertStream();

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableTimeStamp(std::wstring& i_StreamName, bool doTimeStamp);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableFileNameStamp(const std::wstring& i_StreamName, bool doFileNameStamp);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableFilePathStamp(std::wstring& i_StreamName, bool doFilePathStamp);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableLevelStamp(std::wstring& i_StreamName, bool doLevelStamp);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableThreadStamp(const std::wstring& i_StreamName, bool doThreadStamp);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableDbgStream(std::wstring& i_StreamName, bool enableStream);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableStreamLog(const std::wstring& i_StreamName, bool i_enableStream);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableStreamWarning(const std::wstring& i_StreamName, bool i_enableStream);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableStreamError(const std::wstring& i_StreamName, bool i_enableStream);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableCSVFormat(std::wstring& i_StreamName, bool i_enableCSV);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
addStream(const std::wstring& i_StreamName, std::ostream* i_NewStream);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
addCSVStream(const std::wstring& i_StreamName, std::ostream* i_NewStream);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
removeStream(const std::wstring& i_StreamName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::vector<dbgStream*>
GetStreamList();

//------------------------------------------------------------------------
//------------------------------------------------------------------------
dbgStream*
GetStreamByName(const std::wstring& i_StreamName);

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//
// \return void
//------------------------------------------------------------------------
void
Init();

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//
// \return void
//------------------------------------------------------------------------
void
CleanUp() throw();
}

//----------------------------------------------------------------------------
//	Debug logging macros exist only in debug builds.
//	Errors and Warnings exist in debug, release, and gold.
//	usage example: DBG_LOG("Inner Loop");
//	usage example: DBG_LOG("Character Name " << pCharacter->GetName());
//
//	NOTE: The Do-While loop is there so there will only be a single line for the macro
//
//----------------------------------------------------------------------------
#if ENV_BUILD == ENV_DEBUGBUILD
#define DBG_TEXT(msg)                                                                                                  \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    dbgMsg::Text(strmsg.str());                                                                                        \
  } while (false)

#define DBG_LOG(msg)                                                                                                   \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Log(_dbgsrcfile, __LINE__, strmsg.str());                                                                  \
  } while (false)

#define DBG_WARNING(msg)                                                                                               \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Warning(_dbgsrcfile, __LINE__, strmsg.str());                                                              \
  } while (false)

#define DBG_ERROR(msg)                                                                                                 \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Error(_dbgsrcfile, __LINE__, strmsg.str());                                                                \
  } while (false)

#define DBG_TRACE(msg)                                                                                                 \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Trace(_dbgsrcfile, __LINE__, strmsg.str());                                                                \
  } while (false)

#define DBG_ASSERT(condition, msg)                                                                                     \
  do {                                                                                                                 \
    if (!(condition)) {                                                                                                \
      dbgMsg::GetAssertStream() << msg << std::endl;                                                                   \
      std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                      \
      dbgMsg::AssertMsg(_dbgsrcfile, __LINE__);                                                                        \
    }                                                                                                                  \
  } while (false)

#endif

#if ENV_BUILD == ENV_RELEASEBUILD

#define DBG_TEXT(msg)                                                                                                  \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    dbgMsg::Text(strmsg.str());                                                                                        \
  } while (false)

#define DBG_LOG(msg)                                                                                                   \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Log(_dbgsrcfile, __LINE__, strmsg.str());                                                                  \
  } while (false)

#define DBG_WARNING(msg)                                                                                               \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Warning(_dbgsrcfile, __LINE__, strmsg.str());                                                              \
  } while (false)

#define DBG_ERROR(msg)                                                                                                 \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Error(_dbgsrcfile, __LINE__, strmsg.str());                                                                \
  } while (false)

#define DBG_TRACE(msg)                                                                                                 \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Trace(_dbgsrcfile, __LINE__, strmsg.str());                                                                \
  } while (false)

#define DBG_ASSERT(condition, msg)                                                                                     \
  {                                                                                                                    \
  }
#endif

#if ENV_BUILD == ENV_GOLDBUILD

#define DBG_TEXT(msg)                                                                                                  \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    dbgMsg::Text(strmsg.str());                                                                                        \
  } while (false)

#define DBG_LOG(msg)                                                                                                   \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Log(_dbgsrcfile, __LINE__, strmsg.str());                                                                  \
  } while (false)

#define DBG_WARNING(msg)                                                                                               \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Warning(_dbgsrcfile, __LINE__, strmsg.str());                                                              \
  } while (false)

#define DBG_ERROR(msg)                                                                                                 \
  do {                                                                                                                 \
    std::ostringstream strmsg;                                                                                         \
    strmsg << msg;                                                                                                     \
    std::wstring _dbgsrcfile(SGPU_L(__FILE__));                                                                        \
    dbgMsg::Error(_dbgsrcfile, __LINE__, strmsg.str());                                                                \
  } while (false)

#define DBG_TRACE(msg)                                                                                                 \
  {                                                                                                                    \
  }

#define DBG_ASSERT(condition, msg)                                                                                     \
  {                                                                                                                    \
  }
#endif
