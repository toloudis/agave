/*****************************************************************************
**  fsFileNotify.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/fs/fsFileNotify.hpp"

#include "windows.h"
#include <sys/stat.h>
#include "Core/fs/fsFileUtil.hpp"

fsFileNotify::fsFileNotify(const fsLocator& i_FileToCheck, time_t& io_accessTime)
{

  //
  //	 hEvents = CreateEvent(
  //         NULL,    // default security attribute
  //         TRUE,    // manual-reset event
  //         TRUE,    // initial state = signaled
  //         NULL);   // unnamed event object
  //
  //      if (hEvents == NULL)
  //      {
  //         printf("CreateEvent failed with %d.\n", GetLastError());
  //      }
  //
  //	  overlapped.hEvent = hEvents;
  //
  //
  //	//fsFileUtil::LocatorToANSIFilename(i_Locator,fileName);
  //	HANDLE hDir = CreateFile( L"C:\\Users\\kjagadish\\Documents\\test.txt",
  //                                        /* pointer to the file
  //                                           name */
  //    FILE_LIST_DIRECTORY,                /* access (read-write)
  //                                           mode */
  //    FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,  /* share mode */
  //    NULL,                               /* security descriptor */
  //    OPEN_EXISTING,                      /* how to create */
  //    FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,
  //
  //										/* file attributes */
  //    NULL                                /* file with attributes to
  //                                           copy */
  //  );
  //
  //	if (hDir == INVALID_HANDLE_VALUE)
  //    {
  //        DWORD dwError = GetLastError();
  //       // LPCTSTR errMsg = ErrorMessage(dwError);
  //        DBG_TRACE("Could not open file "<< dwError);
  //        //LocalFree((LPVOID)errMsg);
  //        return;
  //    }
  //
  //	DWORD dw = GetLastError();
  //	//DBG_TRACE("fileopen "<<dw);
  //
  //	DWORD BytesReturned;
  //	DWORD dwBytesRead       = 0;
  //
  //	BOOL readDir = ReadDirectoryChangesW(
  //                                hDir,            /* handle to
  //                                                     directory */
  //                                &Buffer,         /* read results
  //                                                    buffer */
  //                                sizeof(Buffer),  /* length of
  //                                                    buffer */
  //                                TRUE,            /* monitoring
  //                                                    option */
  //                                FILE_NOTIFY_CHANGE_SECURITY|
  //                                FILE_NOTIFY_CHANGE_CREATION|
  //                                FILE_NOTIFY_CHANGE_LAST_ACCESS|
  //                                FILE_NOTIFY_CHANGE_LAST_WRITE|
  //                                FILE_NOTIFY_CHANGE_SIZE|
  //                                FILE_NOTIFY_CHANGE_ATTRIBUTES|
  //                                FILE_NOTIFY_CHANGE_DIR_NAME|
  //                                FILE_NOTIFY_CHANGE_FILE_NAME,
  //                                                 /* filter
  //                                                    conditions */
  //                                &BytesReturned,  /* bytes
  //                                                    returned */
  //                                &overlapped,            /* overlapped
  //                                                    buffer */
  //                                NULL);      /* completion
  //                                                    routine */
  //
  //
  //
  //	dw = GetLastError();
  ////	DBG_TRACE("readDir "<<dw);
  //
  // BOOL hResult = GetOverlappedResult(hDir,
  //									&overlapped,
  //									&dwBytesRead,
  //									FALSE);
  ////DBG_TRACE("hResult "<<hResult);
  //
  ////ResetEvent(overlapped.hEvent);
  //
  // CloseHandle(hDir);
  //
  //
}

fsFileNotify::~fsFileNotify() {}
//------------------------------------------------------------------------
// AddToList - Add the file to the list to probe
//------------------------------------------------------------------------
void
fsFileNotify::AddToList(fsLocator& i_FileToCheck)
{
  /*fileNames.push_back(i_FileToCheck);
  initTimes.push_back(0);*/
}

//------------------------------------------------------------------------
// ReadChangeStatus returns the type of change if any for the file specified
//------------------------------------------------------------------------
void
fsFileNotify::CheckForModification()
{
  /*time_t  newTime;
  std::string fileName;

  for(size_t i = 0; i < fileNames.size(); i++)
  {
          fsFileUtil::LocatorToANSIFilename(fileNames[i], fileName);
          struct stat st;
          stat(fileName.c_str(), &st);
          newTime = st.st_mtime;
          if(newTime != initTimes[i])
          {
                  DBG_TRACE(" Object "<<fileName<<" has been updated");
                  initTimes[i] = newTime;
          }
  }*/
}