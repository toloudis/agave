//****************************************************************************
//	\file dbgStream.hpp
//
// Holds the information for each stream belonging to the dbgMsg system
// Will contain an ID for each stream as well as allow the stream to be active or not
//
//
//
//****************************************************************************
#include "Core/dbg/dbgStream.hpp"

#include <string>
#include <vector>
#include <ostream>

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

//============================================================================
// Debug Stream
//============================================================================

//---------------------------
// assigns a name and stream to the instance
//---------------------------
dbgStream::dbgStream(const std::wstring& i_Name, std::ostream* i_Stream)
  : m_Name(i_Name)
  , m_Stream(i_Stream)
  , m_IsActive(true)
{
  m_Data = new dbgData::StreamData();

  //	Set different flags based on the build type
  //
#if ENV_BUILD == ENV_DEBUGBUILD
  m_Data->m_canLog = true;
  m_Data->m_canWarn = true;
  m_Data->m_canError = true;
  m_Data->m_canTrace = true;
  m_Data->m_writeCSV = false;
  m_Data->m_WriteTime_Date = false;
  m_Data->m_WriteThreadID = false;
  m_Data->m_WriteFileName_Line = true;
  m_Data->m_WriteLevel = true;
  m_Data->m_WriteFilePath = false;
#endif

#if ENV_BUILD == ENV_RELEASEBUILD
  m_Data->m_canLog = true;
  m_Data->m_canWarn = true;
  m_Data->m_canError = true;
  m_Data->m_canTrace = false;
  m_Data->m_writeCSV = false;
  m_Data->m_WriteTime_Date = false;
  m_Data->m_WriteThreadID = false;
  m_Data->m_WriteFileName_Line = false;
  m_Data->m_WriteLevel = true;
  m_Data->m_WriteFilePath = false;
#endif

#if ENV_BUILD == ENV_GOLDBUILD
  m_Data->m_canLog = true;
  m_Data->m_canWarn = true;
  m_Data->m_canError = true;
  m_Data->m_canTrace = false;
  m_Data->m_writeCSV = false;
  m_Data->m_WriteTime_Date = false;
  m_Data->m_WriteThreadID = false;
  m_Data->m_WriteFileName_Line = false;
  m_Data->m_WriteLevel = true;
  m_Data->m_WriteFilePath = false;
#endif
}

//---------------------------
// deconstructor
//---------------------------
dbgStream::~dbgStream()
{
  delete m_Data;
}

//---------------------------
// returns whether or not this stream is active.
//---------------------------
bool
dbgStream::isStreamActive()
{
  return m_IsActive;
}

//---------------------------
// Sets the activation level for the stream
//---------------------------
void
dbgStream::setStreamActive(bool i_IsActive)
{
  m_IsActive = i_IsActive;
}

//---------------------------
// Sets the flag for writing the file and line number
//---------------------------
void
dbgStream::setFileName(bool i_WriteFile)
{
  m_Data->m_WriteFileName_Line = i_WriteFile;
}

//---------------------------
// Sets the flag for writing the date and time
//---------------------------
void
dbgStream::setTimeStamp(bool i_WriteTime)
{
  m_Data->m_WriteTime_Date = i_WriteTime;
}

//---------------------------
// Sets the flag for writing the debug level
//---------------------------
void
dbgStream::setDbgLevel(bool i_WriteLevel)
{
  m_Data->m_WriteLevel = i_WriteLevel;
}

void
dbgStream::setThreadID(bool i_WriteThread)
{
  m_Data->m_WriteThreadID = i_WriteThread;
}

//---------------------------
// Get the can write log flag
//---------------------------
bool
dbgStream::logEnabled()
{
  return m_Data->m_canLog;
}

//---------------------------
// Set the write log flag
//---------------------------
void
dbgStream::enableLog(bool doLog)
{
  m_Data->m_canLog = doLog;
}

//---------------------------
// get the warning flag
//---------------------------
bool
dbgStream::warningEnabled()
{
  return m_Data->m_canWarn;
}

//---------------------------
// set the write warning flag
//---------------------------
void
dbgStream::enableWarning(bool doWarning)
{
  m_Data->m_canWarn = doWarning;
}

//---------------------------
// get the write error flag
//---------------------------
bool
dbgStream::errorEnabled()
{
  return m_Data->m_canError;
}

//---------------------------
// Sets the trace flag for the stream
//---------------------------
void
dbgStream::enableTrace(bool doTrace)
{
  m_Data->m_canTrace = doTrace;
}

//---------------------------
// get the write trace flag
//---------------------------
bool
dbgStream::traceEnabled()
{
  return m_Data->m_canTrace;
}

//---------------------------
// Sets the error flag for the stream
//---------------------------
void
dbgStream::enableError(bool doError)
{
  m_Data->m_canError = doError;
}

bool
dbgStream::csvEnabled()
{
  return m_Data->m_writeCSV;
}

void
dbgStream::enableCSV(bool doCSV)
{
  m_Data->m_writeCSV = doCSV;
}

bool
dbgStream::filePathEnabled()
{
  return m_Data->m_WriteFilePath;
}

void
dbgStream::enableFilePath(bool doWriteFilePath)
{
  m_Data->m_WriteFilePath = doWriteFilePath;
}
//---------------------------
// return the stream belonging to this instance
//---------------------------
std::ostream*
dbgStream::GetStream()
{
  return m_Stream;
}

//---------------------------
// Use the name as an ID for this instance
//---------------------------
std::wstring
dbgStream::GetName()
{
  return m_Name;
}

dbgData::StreamData*
dbgStream::GetData()
{
  return m_Data;
}