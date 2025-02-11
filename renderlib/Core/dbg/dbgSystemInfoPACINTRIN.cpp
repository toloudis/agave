//****************************************************************************
//  dbgSystemInfoPACIntrin.cpp
//
//      see .hpp
//
//
//
//****************************************************************************

#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include <sstream>
#include "dbgSystemInfoPACINTRIN.hpp"
#include "Core/Env/envInitX.hpp"
#include "Core/Dbg/dbgMsg.hpp"

//============================================================================
//============================================================================
namespace dbgSystemInfoPAC {
std::string
getCPUInfo()
{
  std::string processorInfo;

  char CPUBrandString[64];
  int CPUInfo[4] = { -1 };
  int nCacheSizeK = 0;

  unsigned int nExIds;

  // Calling __cpuid with 0x80000000 as the InfoType argument
  // gets the number of valid extended IDs.
  __cpuid(CPUInfo, 0x80000000);
  nExIds = CPUInfo[0];
  memset(CPUBrandString, 0, sizeof(CPUBrandString));

  // Get the information associated with each extended ID.
  for (unsigned int i = 0x80000000; i <= nExIds; ++i) {
    __cpuid(CPUInfo, i);
    // Interpret CPU brand string and cache information.
    if (i == 0x80000002)
      memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
    else if (i == 0x80000003)
      memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
    else if (i == 0x80000004)
      memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    else if (i == 0x80000006) {
      nCacheSizeK = (CPUInfo[2] >> 16) & 0xffff;
    }
  }

  if (nExIds >= 0x80000004) {
    processorInfo = std::string(CPUBrandString);
  }

  if (nExIds >= 0x80000006) {
    std::ostringstream cacheStream(std::ostringstream::out);
    cacheStream << nCacheSizeK;
    processorInfo += ",  Cache Size: " + cacheStream.str();
  }

  return processorInfo;
}
}