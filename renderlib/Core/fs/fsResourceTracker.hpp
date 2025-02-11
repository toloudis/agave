/****************************************************************************\
**  fsResourceTracker.hpp
**
**      fsResourceTracker.hpp supplies functions that operate on files.
**
**
**
\****************************************************************************/
#ifdef FS_RESOURCETRACKER_HPP
#error fsResourceTracker.hpp multiply included
#endif
#define FS_RESOURCETRACKER_HPP

#ifndef FS_RESOURCETRACKERDATA_HPP
#include "Core/fs/fsResourceTrackerData.hpp"
#endif

//============================================================================
//============================================================================
class fsLocator;
class itString;
class fsResourceTrackerInterest;

//============================================================================
//============================================================================
namespace fsResourceTracker {
//------------------------------------------------------------------------
//	These are called ONCE by the application.
//
//	If Init is not called this tracker will NOT track anything
//------------------------------------------------------------------------
void
Init();
void
CleanUp();

fsResourceTrackerInterest*
GetInterestList();
//------------------------------------------------------------------------
//	Mark where the begin and end of filename access is.  This allows
//	for files to be tracked as "within" another file.  For instance,
//	reading a model file and marking it will allow the textures to be
//	marked as associated with it.
//------------------------------------------------------------------------
void
MarkBegin(const fsLocator& i_Filename);
void
MarkEnd(const fsLocator& i_Filename);

//------------------------------------------------------------------------
//	Remove the file and all of it's sub-files.
//------------------------------------------------------------------------
void
Remove(const fsLocator& i_Filename);
void
Remove(const itString& i_Filename);

//------------------------------------------------------------------------
//	ReplaceFile() - replace JUST the filename (not the sub-files)
//------------------------------------------------------------------------
void
ReplaceFile(const fsLocator& i_OrigFilename, const fsLocator& i_NewFilename);
void
ReplaceFile(const itString& i_OrigFilename, const fsLocator& i_NewFilename);

//------------------------------------------------------------------------
//	After a file (and all its sub-files) has been read, this will give
//	you the list of resource data.
//------------------------------------------------------------------------
fsResourceTrackerData&
GetData();

//------------------------------------------------------------------------
//	Dump the current data and replace it with the new data
//------------------------------------------------------------------------
void
SetData(fsResourceTrackerData& i_NewData);

//------------------------------------------------------------------------
//	Find out if an item is already in the list, if so return the index.
//	A return value of -1 means the file is not in the list.
//------------------------------------------------------------------------
int
GetIndex(const itString& i_Filename);
int
GetIndex(const fsLocator& i_Filename);
bool
IsInList(const fsLocator& i_Filename);

//------------------------------------------------------------------------
//	Try to find the index based only on the base filename, NOT the
//	extension.
//------------------------------------------------------------------------
int
GetIndexFromBaseFileName(const itString& i_Filename);

//------------------------------------------------------------------------
//	Given the resource index passed in, find the last index of all of
//	the sub-resources and return it.
//------------------------------------------------------------------------
int
GetResourceLastIndex(int i_StartIndex);

//------------------------------------------------------------------------
//	GetResourceList - based on the file path, return a list of all the
//	resources associated with that file.
//------------------------------------------------------------------------
void
GetResourceList(const fsLocator& i_FilePath, fsResourceTrackerDataList& io_ResourceList);
void
GetResourceList(const itString& i_FileName, fsResourceTrackerDataList& io_ResourceList);

//------------------------------------------------------------------------
//	Output the list to the debug.log
//------------------------------------------------------------------------
void
Debug_OutputList();

void
Debug_OutputList(fsResourceTrackerData& i_NewData);

}
