//****************************************************************************
//	dbgMsg.cpp
//
//		see .hpp
//
//
//
//****************************************************************************
#include "Core/Dbg/dbgMsg.hpp"
#include "Core/Dbg/dbgStringStream.hpp"
#include "Core/Dbg/dbgAssertPAC.hpp"
#include "Core/Dbg/dbgLogPAC.hpp"
#include "Core/Env/envSTLHelpers.hpp"
#include "Core/Env/envThreadGroup.hpp"
#include "Core/fs/fsLocator.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/gf/gfPaths.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

//============================================================================
//============================================================================
// mutex for allowing debug messages to be written from multiple concurrent threads.
envMutex g_DbgMutex;

//============================================================================
//============================================================================
namespace dbgMsg {
namespace {
// vector that will hold all of the streams this logging system will write to.
std::vector<dbgStream*> m_StreamList;
std::stringstream m_AssertStream;

// hacks to remove memory leaks
std::ofstream* l_ofs = NULL;
std::ostream* l_couts = NULL;
dbgStreamBuf* l_buf = NULL;
dbgStringStream* l_dbgs = NULL;
std::ofstream* l_csvfs = NULL;

//--------------------------------------------------------------------
// Get the executable name and format it to become the default log name
//--------------------------------------------------------------------
std::wstring
getDefaultStreamName()
{
  TCHAR exepath[MAX_PATH];
  GetModuleFileName(0, exepath, MAX_PATH);
  std::wstring fileString(exepath);

  // get just the exe file
  int fileIndex = fileString.rfind(L"\\");
  std::wstring exeFile = fileString.substr(fileIndex + 1);

  // extract the .exe to get only the name
  int exeIndex = exeFile.rfind(L".exe");
  std::wstring fileName = exeFile.substr(0, exeIndex);
  //::OSVERSIONINFO

  return fileName; // envString::WideCharToUTF8(fileName);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void
AddDefaultStream()
{
  // get the name of the executable, if it can't be found, just use 'Application'
  std::wstring defaultName = L"Application";
  std::wstring appName = L"";
  appName = getDefaultStreamName();

  if (appName != L"")
    defaultName = appName;

  fsLocator streamfile = gfPaths::GetPath(gfPaths::e_UserDataPath);
  itString fname(defaultName.c_str());
  fname += L".log";
  streamfile.Push(fname);
  fsFileUtil::LocatorToUnicodeString(streamfile, fname);

  l_ofs = new std::ofstream(fname.GetString());
  addStream(std::wstring(L"default"), l_ofs);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
void
AddDebugWindowStream()
{
  l_buf = new dbgStreamBuf();
  l_dbgs = new dbgStringStream(l_buf);

  addStream(std::wstring(L"debugWindow"), l_dbgs);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
void
AddSTDStream()
{
  addStream(std::wstring(L"stdCout"), &std::cout);
  addStream(std::wstring(L"stdCerr"), &std::cerr);

  enableStreamLog(std::wstring(L"stdCerr"), false);
  enableStreamWarning(std::wstring(L"stdCerr"), true);
  enableStreamError(std::wstring(L"stdCerr"), true);
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
void
AddCSVStream()
{
  // get the name of the executable, if it can't be found, just use 'Application'
  std::wstring defaultName = L"Application";
  std::wstring appName = L"";
  appName = getDefaultStreamName();

  if (appName != L"")
    defaultName = appName;

  std::wstring fileName = defaultName + L".csv";
  l_csvfs = new std::ofstream(fileName.c_str());
  addCSVStream(std::wstring(L"defaultCSV"), l_csvfs);
}

//--------------------------------------------------------------------
// Remove a specified stream from our stream list
//--------------------------------------------------------------------
void
RemoveStreamByName(const std::wstring& i_StreamName)
{
  for (unsigned int i = 0; i < m_StreamList.size(); i++) {
    if (m_StreamList[i]->GetName() == i_StreamName) {
      delete m_StreamList[i]; // fixes memory leak in debug console
      m_StreamList.erase(m_StreamList.begin() + i);
    }
  }
}

//--------------------------------------------------------------------
// Given a file path return the name of the file only
//--------------------------------------------------------------------
std::wstring
FormatFilePath(std::wstring& io_File)
{
  int found = io_File.find_last_of(L"/\\");
  if (found != std::string::npos) {
    return io_File.substr(found + 1);
  }
  return std::wstring(L"");
}
}

//------------------------------------------------------------------------
//	This write function writes all the flagged information first;
//  time/date/Filename/line number/ message level
//
//
// return void
//------------------------------------------------------------------------
void
Write(std::wstring i_File, int i_Line, dbgStream* io_Stream, const std::wstring& i_DbgLevel)
{
  std::ostream* cur_os = io_Stream->GetStream();
  dbgData::StreamData* stream_data = io_Stream->GetData();
  bool doMsgColon = false;

  if (!stream_data->m_WriteFilePath)
    i_File = FormatFilePath(i_File);

  if (stream_data->m_writeCSV) {
    WriteCSV(i_File, i_Line, io_Stream, i_DbgLevel);
    return;
  }

  const char* sep = " ";
  if (stream_data->m_WriteTime_Date) {
    //	date + time wide because in some languages things like "AM" and "PM" are multi-byte
    wchar_t date_string[64];
    _wstrdate_s(date_string);
    *(cur_os) << envString::WideCharToUTF8(date_string) << sep;
    wchar_t time_string[64];
    _wstrtime_s(time_string);
    *(cur_os) << envString::WideCharToUTF8(time_string) << sep;
    doMsgColon = true;
  }

  // Write Thread
  // bga - turning off the thread id reporting
  // if (stream_data->m_WriteThreadID)
  //{
  //	*(cur_os) << "(" << boost::this_thread::get_id() << ")" << sep;
  //	//*(cur_os) << "(" <<  << ")" << sep;
  // }

  if (stream_data->m_WriteFileName_Line) {
    *(cur_os) << envString::WideCharToUTF8(i_File);
    *(cur_os) << "(" << i_Line << ")" << sep;

    doMsgColon = true;
  }

  if (stream_data->m_WriteLevel) {
    if (i_DbgLevel.length() > 0) {
      *(cur_os) << "****" << envString::WideCharToUTF8(i_DbgLevel) << "****" << sep;
      doMsgColon = true;
    }
  }
  if (doMsgColon)
    *(cur_os) << ": ";
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
WriteCSV(std::wstring i_File, int i_Line, dbgStream* io_Stream, const std::wstring& i_DbgLevel)
{
  std::ostream* cur_os = io_Stream->GetStream();
  dbgData::StreamData* stream_data = io_Stream->GetData();
  if (!stream_data->m_WriteFilePath)
    i_File = FormatFilePath(i_File);

  wchar_t sep = L',';

  // first write the date
  if (stream_data->m_WriteTime_Date) {
    wchar_t date_string[64];
    _wstrdate_s(date_string);
    *(cur_os) << date_string;
  }
  // add seperator after every entry to still have an empty cell if
  // that information wasn't written
  *(cur_os) << sep;

  // now write the time
  if (stream_data->m_WriteTime_Date) {
    wchar_t time_string[64];
    _wstrtime_s(time_string);
    *(cur_os) << time_string;
  }
  *(cur_os) << sep;

  // Write Thread
  // if (stream_data->m_WriteThreadID)
  //{
  //	//*(cur_os) << boost::this_thread::get_id();
  // }
  //*(cur_os) << sep;

  if (stream_data->m_WriteFileName_Line) {
    *(cur_os) << envString::WideCharToUTF8(i_File);
  }
  *(cur_os) << sep;

  if (stream_data->m_WriteFileName_Line) {
    *(cur_os) << i_Line;
  }
  *(cur_os) << sep;

  if (stream_data->m_WriteLevel) {
    if (i_DbgLevel.length() > 0) {
      *(cur_os) << envString::WideCharToUTF8(i_DbgLevel);
    }
  }
  *(cur_os) << sep;
}

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//	They always use single-byte ANSI characters.
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteLog(const std::wstring& i_File, int i_Line, dbgStream* io_Stream)
{
  Write(i_File, i_Line, io_Stream, std::wstring(L""));

  return io_Stream->GetStream();
}

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//	They always use single-byte ANSI characters.
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteWarning(const std::wstring& i_File, int i_Line, dbgStream* io_Stream)
{
  Write(i_File, i_Line, io_Stream, std::wstring(L"WARNING"));

  return io_Stream->GetStream();
}

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//	They always use single-byte ANSI characters.
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteError(const std::wstring& i_File, int i_Line, dbgStream* io_Stream)
{
  Write(i_File, i_Line, io_Stream, std::wstring(L"ERROR"));
  return io_Stream->GetStream();
}

//------------------------------------------------------------------------
//	"Write" functions are used by the debug logging macros below.
//	They always use single-byte ANSI characters.
//
// \return ostream*
//------------------------------------------------------------------------
std::ostream*
WriteTrace(const std::wstring& i_File, int i_Line, dbgStream* io_Stream)
{
  Write(i_File, i_Line, io_Stream, std::wstring(L""));
  return io_Stream->GetStream();
}

//------------------------------------------------------------------------
//  If an assertion call is passed, run this function
//
//------------------------------------------------------------------------
void
AssertMsg(std::wstring& i_File, int i_Line)
{
  std::ostringstream dbg_line(std::ostringstream::out);
  std::string assert_msg = m_AssertStream.str();

  dbg_line << "assertion failure in " << envString::WideCharToUTF8(i_File.c_str()) << "(" << i_Line << "):\n"
           << assert_msg;

  std::vector<dbgStream*> _osList = GetStreamList();
  for (int i = 0; i < _osList.size(); ++i) {
    if (_osList[i]->isStreamActive())
      if (_osList[i]->logEnabled())
        *(dbgMsg::WriteLog(i_File, i_Line, _osList[i])) << dbg_line.str() << std::endl;
  }

  if (dbgAssertPAC::AbortContinueMessage(dbg_line.str().c_str())) {
    // true means abort
    exit(1);
  }
}

//------------------------------------------------------------------------
//  Return the stream that will be used to read in the assertion message
//
//------------------------------------------------------------------------
std::stringstream&
GetAssertStream()
{
  return m_AssertStream;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableTimeStamp(std::wstring& i_StreamName, bool doTimeStamp)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;
  curStream->setTimeStamp(doTimeStamp);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableFileNameStamp(const std::wstring& i_StreamName, bool doFileNameStamp)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;
  curStream->setFileName(doFileNameStamp);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableFilePathStamp(std::wstring& i_StreamName, bool doFilePathStamp)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;
  curStream->enableFilePath(doFilePathStamp);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableLevelStamp(std::wstring& i_StreamName, bool doLevelStamp)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  curStream->setDbgLevel(doLevelStamp);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableThreadStamp(const std::wstring& i_StreamName, bool doThreadStamp)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  curStream->setThreadID(doThreadStamp);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableDbgStream(const std::wstring& i_StreamName, bool enableStream)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  curStream->setStreamActive(enableStream);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableStreamLog(const std::wstring& i_StreamName, bool i_enableStream)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  curStream->enableLog(i_enableStream);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableStreamWarning(const std::wstring& i_StreamName, bool i_enableStream)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  curStream->enableWarning(i_enableStream);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableStreamError(const std::wstring& i_StreamName, bool i_enableStream)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  curStream->enableError(i_enableStream);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
enableCSVFormat(std::wstring& i_StreamName, bool i_enableCSV)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  curStream->enableCSV(i_enableCSV);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
addStream(const std::wstring& i_StreamName, std::ostream* i_NewStream)
{
  // create and add the new dbgStream to the stream list
  dbgStream* curStream = new dbgStream(i_StreamName, i_NewStream);
  m_StreamList.push_back(curStream);

  // now log the start time for this stream
  curStream->setTimeStamp(true);
  bool file_enabled = curStream->GetData()->m_WriteFileName_Line;
  if (file_enabled) {
    curStream->setFileName(false);
  }
  *(WriteLog(std::wstring(L""), 0, curStream)) << "Application Log initialized." << std::endl;

  // reset defaults
  curStream->setTimeStamp(false);
  if (file_enabled)
    curStream->setFileName(true);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
addCSVStream(const std::wstring& i_StreamName, std::ostream* i_NewStream)
{
  // create and add the new dbgStream to the stream list
  dbgStream* curStream = new dbgStream(i_StreamName, i_NewStream);
  m_StreamList.push_back(curStream);

  // setup the attributes for CSV streams
  curStream->setTimeStamp(true);
  curStream->setFileName(false);
  curStream->enableCSV(true);

  //*(i_NewStream) << "Date,Time,ThreadID,FileName,Line,Type,Message" << std::endl;
  *(WriteLog(std::wstring(L""), 0, curStream)) << "Application Log initialized." << std::endl;
  curStream->setFileName(true);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
removeStream(const std::wstring& i_StreamName)
{
  dbgStream* curStream = GetStreamByName(i_StreamName);
  if (curStream == NULL)
    return;

  // now log the start time for this stream
  curStream->setTimeStamp(true);
  bool file_enabled = curStream->GetData()->m_WriteFileName_Line;
  if (file_enabled) {
    curStream->setFileName(false);
  }
  *(WriteLog(std::wstring(L""), 0, curStream)) << "Application Log has been terminated." << std::endl;

  // reset defaults
  curStream->setTimeStamp(false);
  if (file_enabled)
    curStream->setFileName(true);

  // now remove the stream
  RemoveStreamByName(i_StreamName);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::vector<dbgStream*>
GetStreamList()
{
  return m_StreamList;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
dbgStream*
GetStreamByName(const std::wstring& i_StreamName)
{
  for (unsigned int i = 0; i < m_StreamList.size(); i++) {
    if (m_StreamList[i]->GetName() == i_StreamName)
      return m_StreamList[i];
  }
  return NULL;
}

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//
// \param none
//
// \return void
//------------------------------------------------------------------------
void
Init()
{
  dbgLogPAC::Init();

  AddDefaultStream();
  AddDebugWindowStream();
  AddSTDStream();
}

//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//
// \param none
//
// \return void
//------------------------------------------------------------------------
void
CleanUp() throw()
{
  for (unsigned int i = 0; i < m_StreamList.size(); i++) {
    removeStream(m_StreamList[i]->GetName());
  }

  envSTLHelpers::DeleteContainer(m_StreamList);

  // delete dangling pointers
  delete l_ofs;
  delete l_buf;
  delete l_dbgs;
  delete l_csvfs;

  dbgLogPAC::CleanUp();
}

//------------------------------------------------------------------------
//	Write a Warning string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Warning(std::wstring& i_File, int i_Line, const std::string& i_Msg)
{
  envScopedLock lock(g_DbgMutex);

  std::vector<dbgStream*> _osList = dbgMsg::GetStreamList();
  if (_osList.size() > 0)
    for (int debug_iterator = 0; debug_iterator < _osList.size(); ++debug_iterator) {
      if (_osList[debug_iterator]->isStreamActive()) {
        if (_osList[debug_iterator]->warningEnabled()) {
          *(dbgMsg::WriteWarning(i_File, i_Line, _osList[debug_iterator])) << i_Msg << std::endl;
        }
      }
    }
}

//------------------------------------------------------------------------
//	Write a Log string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Log(std::wstring& i_File, int i_Line, const std::string& i_Msg)
{
  envScopedLock lock(g_DbgMutex);

  std::vector<dbgStream*> _osList = dbgMsg::GetStreamList();
  if (_osList.size() > 0)
    for (int debug_iterator = 0; debug_iterator < _osList.size(); ++debug_iterator) {
      if (_osList[debug_iterator]->isStreamActive()) {
        if (_osList[debug_iterator]->logEnabled()) {
          *(dbgMsg::WriteLog(i_File, i_Line, _osList[debug_iterator])) << i_Msg << std::endl;
        }
      }
    }
}

//------------------------------------------------------------------------
//	Write a Text string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Text(std::string& i_Msg)
{
  envScopedLock lock(g_DbgMutex);

  std::vector<dbgStream*> _osList = dbgMsg::GetStreamList();
  if (_osList.size() > 0)
    for (int debug_iterator = 0; debug_iterator < _osList.size(); ++debug_iterator) {
      if (_osList[debug_iterator]->isStreamActive()) {
        if (_osList[debug_iterator]->logEnabled()) {
          *(_osList[debug_iterator]->GetStream()) << i_Msg << std::endl;
        }
      }
    }
}

//------------------------------------------------------------------------
//	Write a Error string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Error(std::wstring& i_File, int i_Line, std::string& i_Msg)
{
  envScopedLock lock(g_DbgMutex);

  std::vector<dbgStream*> _osList = dbgMsg::GetStreamList();
  if (_osList.size() > 0)
    for (int debug_iterator = 0; debug_iterator < _osList.size(); ++debug_iterator) {
      if (_osList[debug_iterator]->isStreamActive()) {
        if (_osList[debug_iterator]->errorEnabled()) {
          *(dbgMsg::WriteError(i_File, i_Line, _osList[debug_iterator])) << i_Msg << std::endl;
        }
      }
    }
}

void
Error(std::wstring& i_File, int i_Line, const std::string& i_Msg)
{
  envScopedLock lock(g_DbgMutex);

  std::vector<dbgStream*> _osList = dbgMsg::GetStreamList();
  if (_osList.size() > 0)
    for (int debug_iterator = 0; debug_iterator < _osList.size(); ++debug_iterator) {
      if (_osList[debug_iterator]->isStreamActive()) {
        if (_osList[debug_iterator]->errorEnabled()) {
          *(dbgMsg::WriteError(i_File, i_Line, _osList[debug_iterator])) << i_Msg << std::endl;
        }
      }
    }
}

//------------------------------------------------------------------------
//	Write a Trace string to all log streams, thread-safe.
//
// \return void
//------------------------------------------------------------------------
void
Trace(std::wstring& i_File, int i_Line, const std::string& i_Msg)
{
  envScopedLock lock(g_DbgMutex);

  std::vector<dbgStream*> _osList = dbgMsg::GetStreamList();
  if (_osList.size() > 0)
    for (int debug_iterator = 0; debug_iterator < _osList.size(); ++debug_iterator) {
      if (_osList[debug_iterator]->isStreamActive()) {
        if (_osList[debug_iterator]->traceEnabled()) {
          *(dbgMsg::WriteTrace(i_File, i_Line, _osList[debug_iterator])) << i_Msg << std::endl;
        }
      }
    }
}

}
