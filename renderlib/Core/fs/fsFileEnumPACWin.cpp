/****************************************************************************\
**  fsFileEnum.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsFileEnumPAC.hpp"

#include "Core/fs/fsFileUtilPAC.hpp"
#include "Core/fs/fsLocator.hpp"

#include <windows.h>

//============================================================================
//============================================================================
namespace fsFileEnumPAC {

namespace {
//============================================================================
//============================================================================
struct FindCloser
{
  FindCloser(HANDLE i_Handle)
    : m_Handle(i_Handle)
  {
  }
  ~FindCloser() { ::FindClose(m_Handle); }
  HANDLE m_Handle;
};

//--------------------------------------------------------------------
// returns true if the filename contains one of the substrings.
//--------------------------------------------------------------------
bool
substring_match(const itString& i_Filename, const std::vector<itString>& i_SubStrings)
{
  if (i_SubStrings.empty())
    return true;

  std::vector<itString>::const_iterator it;
  for (it = i_SubStrings.begin(); it != i_SubStrings.end(); ++it) {
    if (i_Filename.HasSubString(*it))
      return true;
  }
  return false;
}

}

//------------------------------------------------------------------------
//	EnumerateFiles lists all of the files in a directory.  If i_SubStrings
//	is not empty, only files whose names contain one of the given substrings
//	will be listed. Returns the last return value from fsEnumTarget::Notify
//------------------------------------------------------------------------
bool
EnumerateFiles(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target, const std::vector<itString>& i_SubStrings)
{
  HANDLE hFind;
  fsLocator ret_val;
  bool continue_enum;

  if (fsFileUtilPAC::MustUseANSIFilenames()) {
    itString filename;
    itString::CharType wide_char_name[512];
    WIN32_FIND_DATAA find_file_data;
    std::string root_dir;

    fsLocator root_dir_locator = i_Locator;
    root_dir_locator.Push(itString("*"));
    fsFileUtilPAC::LocatorToANSIFilename(root_dir_locator, root_dir);

    hFind = ::FindFirstFileA(root_dir.c_str(), &find_file_data);

    if (hFind != INVALID_HANDLE_VALUE) {
      FindCloser closer(hFind);
      if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        ::MultiByteToWideChar(::GetACP(), MB_PRECOMPOSED, find_file_data.cFileName, -1, wide_char_name, 512);

        filename = wide_char_name;

        if (substring_match(filename, i_SubStrings)) {
          ret_val.Push(itString(wide_char_name));
          continue_enum = o_Target.Notify(i_Locator, ret_val);

          if (continue_enum == false)
            return false;

          ret_val.Pop();
        }
      }

      BOOL found = ::FindNextFileA(hFind, &find_file_data);

      while (found) {
        if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
          ::MultiByteToWideChar(::GetACP(), MB_PRECOMPOSED, find_file_data.cFileName, -1, wide_char_name, 512);

          filename = wide_char_name;

          if (substring_match(filename, i_SubStrings)) {
            ret_val.Push(itString(wide_char_name));
            continue_enum = o_Target.Notify(i_Locator, ret_val);

            if (continue_enum == false)
              return false;

            ret_val.Pop();
          }
        }

        found = ::FindNextFileA(hFind, &find_file_data);
      }
    }
  } else {
    itString filename;
    WIN32_FIND_DATAW find_file_data;
    itString root_dir;

    fsLocator locator_with_wildcard = i_Locator;
    locator_with_wildcard.Push(itString("*"));
    fsFileUtilPAC::LocatorToUnicodeFilename(locator_with_wildcard, root_dir);
    // null-terminating not needed anymore
    // root_dir += 0;		//NULL terminate

    hFind = ::FindFirstFileW(root_dir.GetString(), &find_file_data);

    if (hFind != INVALID_HANDLE_VALUE) {
      FindCloser closer(hFind);
      if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        filename = find_file_data.cFileName;

        if (substring_match(filename, i_SubStrings)) {
          ret_val.Push(filename);
          continue_enum = o_Target.Notify(i_Locator, ret_val);
          if (continue_enum == false)
            return false;

          ret_val.Pop();
        }
      }

      BOOL found = ::FindNextFileW(hFind, &find_file_data);

      while (found) {
        if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
          filename = find_file_data.cFileName;

          if (substring_match(filename, i_SubStrings)) {
            ret_val.Push(filename);
            continue_enum = o_Target.Notify(i_Locator, ret_val);
            if (continue_enum == false)
              return false;

            ret_val.Pop();
          }
        }

        found = ::FindNextFileW(hFind, &find_file_data);
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------
//	EnumerateDirectories lists all of the subdirectories in a directory.
//	Returns the last return value from fsEnumTarget::Notify
//------------------------------------------------------------------------
bool
EnumerateDirectories(const fsLocator& i_Locator, fsFileEnum::EnumTarget& o_Target)
{
  HANDLE hFind;
  fsLocator ret_val;
  bool continue_enum;

  if (fsFileUtilPAC::MustUseANSIFilenames()) {
    itString filename;
    itString::CharType wide_char_name[512];
    WIN32_FIND_DATAA find_file_data;
    std::string root_dir;

    fsLocator root_dir_locator = i_Locator;
    root_dir_locator.Push(itString("*"));
    fsFileUtilPAC::LocatorToANSIFilename(root_dir_locator, root_dir);

    hFind = ::FindFirstFileA(root_dir.c_str(), &find_file_data);

    if (hFind != INVALID_HANDLE_VALUE) {
      FindCloser closer(hFind);
      if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        ::MultiByteToWideChar(::GetACP(), MB_PRECOMPOSED, find_file_data.cFileName, -1, wide_char_name, 512);

        filename = wide_char_name;

        ret_val.Push(itString(wide_char_name));
        continue_enum = o_Target.Notify(i_Locator, ret_val);

        if (continue_enum == false)
          return false;

        ret_val.Pop();
      }

      BOOL found = ::FindNextFileA(hFind, &find_file_data);

      while (found) {
        if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
          ::MultiByteToWideChar(::GetACP(), MB_PRECOMPOSED, find_file_data.cFileName, -1, wide_char_name, 512);

          filename = wide_char_name;

          ret_val.Push(itString(wide_char_name));
          continue_enum = o_Target.Notify(i_Locator, ret_val);

          if (continue_enum == false)
            return false;

          ret_val.Pop();
        }

        found = ::FindNextFileA(hFind, &find_file_data);
      }
    }
  } else {
    itString filename;
    WIN32_FIND_DATAW find_file_data;
    itString root_dir;

    fsLocator locator_with_wildcard = i_Locator;
    locator_with_wildcard.Push(itString("*"));
    fsFileUtilPAC::LocatorToUnicodeFilename(locator_with_wildcard, root_dir);
    // null-terminating not needed anymore
    // root_dir += 0;		//NULL terminate

    hFind = ::FindFirstFileW(root_dir.GetString(), &find_file_data);

    if (hFind != INVALID_HANDLE_VALUE) {
      FindCloser closer(hFind);
      if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        filename = find_file_data.cFileName;

        ret_val.Push(filename);
        continue_enum = o_Target.Notify(i_Locator, ret_val);
        if (continue_enum == false)
          return false;

        ret_val.Pop();
      }

      BOOL found = ::FindNextFileW(hFind, &find_file_data);

      while (found) {
        if ((find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
          filename = find_file_data.cFileName;

          ret_val.Push(filename);
          continue_enum = o_Target.Notify(i_Locator, ret_val);
          if (continue_enum == false)
            return false;

          ret_val.Pop();
        }

        found = ::FindNextFileW(hFind, &find_file_data);
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------
//	GetDrives lists all of the drives listed by windows
//------------------------------------------------------------------------
void
GetDrives(std::vector<fsLocator>& o_DirList)
{
  o_DirList.clear();

  unsigned long BufferSize = 128, ActualBufferSize;
  TCHAR* Buffer = new TCHAR[BufferSize];
  ActualBufferSize = GetLogicalDriveStrings(BufferSize, Buffer);
  if (BufferSize < ActualBufferSize) {
    delete Buffer;
    BufferSize = ActualBufferSize;
    Buffer = new TCHAR[BufferSize];
    ActualBufferSize = GetLogicalDriveStrings(BufferSize, Buffer);
  }

  int i;
  itString DriveName;
  for (i = 0; i < ActualBufferSize; ++i) {
    if ('\0' == Buffer[i] && DriveName.GetLength()) {
      fsLocator Loc;
      if ('\\' == DriveName[DriveName.GetLength() - 1]) {
        DriveName.RemoveCharAt(DriveName.GetLength() - 1);
      }
      Loc.Push(DriveName);
      o_DirList.push_back(Loc);
      DriveName.Clear();
    } else {
      DriveName += Buffer[i];
    }
  }

  delete Buffer;
}

}
