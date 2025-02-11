/*****************************************************************************
**  gfFileUtil.hpp
**
**      gfFileUtil is a namespace containing several functions for reading and
**		writing simple data types such as envType::UInt16
**
**
**
\****************************************************************************/

#ifdef GF_FILEUTIL_HPP
#error gfFileUtil.hpp multiply included
#endif
#define GF_FILEUTIL_HPP

#ifndef ENV_TYPE_HPP
#include "Core/Env/envType.hpp"
#endif

#ifndef GF_FILEBIN_HPP
#include "Core/Gf/gfFileBin.hpp"
#endif

//============================================================================
//============================================================================
namespace gfFileUtil {
//--------------------------------------------------------------------------------
//	Read is a template function that receives a simple primtive value such as
//	envType::UInt16 and returns it byteswapped if the EndianMode is different than
//	the platform the game was compiled for.
//--------------------------------------------------------------------------------
template<class T>
inline void
Read(gfFile& i_File, T& i_Val)
{
  i_File.Read(sizeof(i_Val), &i_Val);

  if (dynamic_cast<gfFileBin*>(&i_File) != NULL) {
#if ENV_ENDIAN == ENV_LITTLEENDIAN
    if (gfFileBin::e_BigEndian == (dynamic_cast<gfFileBin*>(&i_File))->GetEndianMode()) {
#else
    if (gfFileBin::e_LittleEndian == (dynamic_cast<gfFileBin*>(&i_File))->GetEndianMode()) {
#endif
      int Len = sizeof(i_Val);
      int LoopLen = Len / 2; // loop times half the # of bytes
      char* Swap = (char*)&i_Val;
      char TempByte;
      for (int i = 0; i < LoopLen; i++) {
        TempByte = Swap[i];
        Swap[i] = Swap[Len - 1 - i];
        Swap[Len - 1 - i] = TempByte;
      }
    }
  }
}

//--------------------------------------------------------------------------------
//	Write is a template function that receives a simple primtive value such as
//	envType::UInt16 and byteswaps it before writing it if the EndianMode is
//	different than the platform the game was compiled for.
//--------------------------------------------------------------------------------
template<class T>
inline void
Write(gfFile& i_File, T i_Val)
{
  if ((dynamic_cast<gfFileBin*>(&i_File)) != NULL) {
#if ENV_ENDIAN == ENV_LITTLEENDIAN
    if (gfFileBin::e_BigEndian == (dynamic_cast<gfFileBin*>(&i_File))->GetEndianMode()) {
#else
    if (gfFileBin::e_LittleEndian == (dynamic_cast<gfFileBin*>(&i_File))->GetEndianMode()) {
#endif
      int Len = sizeof(i_Val);
      int LoopLen = Len / 2; // loop times half the # of bytes
      char* Swap = (char*)&i_Val;
      char TempByte;
      for (int i = 0; i < LoopLen; i++) {
        TempByte = Swap[i];
        Swap[i] = Swap[Len - 1 - i];
        Swap[Len - 1 - i] = TempByte;
      }
    }
  }

  i_File.Write(sizeof(i_Val), &i_Val);
}

}
