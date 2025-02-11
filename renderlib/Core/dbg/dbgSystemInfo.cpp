//****************************************************************************
//  dbgSystemInfo.cpp
//
//      see .hpp
//
//
//
//****************************************************************************
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "Core/dbg/dbgMsg.hpp"
#include "Core/dbg/dbgSystemInfo.hpp"
#include "Core/dbg/dbgSystemInfoPAC.hpp"
#include "Core/env/envString.hpp"

#include <vector>

//============================================================================
//============================================================================
namespace dbgSystemInfo {
//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getUserName()
{
  return dbgSystemInfoPAC::getUserName();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getComputerName()
{
  return dbgSystemInfoPAC::getComputerName();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
getMemory()
{
  return dbgSystemInfoPAC::getMemory();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::wstring
getOS()
{
  return dbgSystemInfoPAC::getOS();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
std::string
getCPUInfo()
{
  return dbgSystemInfoPAC::getCPUInfo();
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
logSystemInfo()
{
  std::vector<dbgStream*> _osList = dbgMsg::GetStreamList();
  std::vector<bool> doThreadList(_osList.size());
  std::vector<bool> doFileList(_osList.size());

  for (int i = 0; i < _osList.size(); ++i) {
    doThreadList[i] = _osList[i]->GetData()->m_WriteThreadID;
    doFileList[i] = _osList[i]->GetData()->m_WriteFileName_Line;
    dbgMsg::enableThreadStamp(_osList[i]->GetName(), false);
    dbgMsg::enableFileNameStamp(_osList[i]->GetName(), false);
  }

  DBG_LOG("Username: " << envString::WideCharToUTF8(dbgSystemInfo::getUserName()));
  DBG_LOG("Computer Name: " << envString::WideCharToUTF8(dbgSystemInfo::getComputerName()));
  DBG_LOG("Operating System: " << envString::WideCharToUTF8(dbgSystemInfo::getOS()));
  DBG_LOG("CPU Info: " << dbgSystemInfo::getCPUInfo());
  DBG_LOG("Total System RAM: " << dbgSystemInfo::getMemory() << "MB");

  for (int i = 0; i < _osList.size(); ++i) {
    if (doThreadList[i])
      dbgMsg::enableThreadStamp(_osList[i]->GetName(), true);
    if (doFileList[i])
      dbgMsg::enableFileNameStamp(_osList[i]->GetName(), true);
  }
}

}