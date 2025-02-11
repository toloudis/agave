/*****************************************************************************
**  chChunkParserUtil.hpp
**
**      chChunkParserUtil contains utility functions for
**		for reading and writing data
**
**
**
\****************************************************************************/

#ifdef CH_CHUNKPARSERUTIL_HPP
#error chChunkParserUtil.hpp multiply included
#endif
#define CH_CHUNKPARSERUTIL_HPP

#ifndef CH_DEFS_HPP
#include "Core/Ch/chDefs.hpp"
#endif
#ifndef CH_READER_HPP
#include "Core/Ch/chReader.hpp"
#endif
#ifndef CH_WRITER_HPP
#include "Core/Ch/chWriter.hpp"
#endif
#ifndef MA_FLOATRGBA_HPP
#include "Core/Ma/maFloatRGBA.hpp"
#endif
#ifndef MA_GRADIENT_HPP
#include "Core/Ma/maGradient.hpp"
#endif
#ifndef MA_INTPOINT2D_HPP
#include "Core/Ma/maIntPoint2d.hpp"
#endif
#ifndef MA_POINT2D_HPP
#include "Core/Ma/maPoint2d.hpp"
#endif
#ifndef MA_POINT3D_HPP
#include "Core/Ma/maPoint3d.hpp"
#endif
#ifndef MA_ROTATION_HPP
#include "Core/Ma/maRotation.hpp"
#endif
#ifndef MA_VECTOR4D_HPP
#include "Core/Ma/maVector4d.hpp"
#endif

#include <string>
#include <vector>

//============================================================================
//============================================================================
class itString;
class maIntPoint2d;

//============================================================================
//============================================================================
namespace chChunkParserUtil {
//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, bool& o_bValue);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, float& o_fValue);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, int& o_nValue);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, unsigned int& o_nValue);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maPoint3d& o_Point);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maPoint2d& o_Point);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maIntPoint2d& o_Point);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maRotation& o_Orientation);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maVector4d& o_Point);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maFloatRGBA& o_Color);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maGradient& o_Gradient);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, itString& o_Name);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, std::string& o_Name);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Read(chReader& i_Reader, maMatrix4x4& o_Matrix);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const itString& i_Name, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const std::string& i_Name, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maFloatRGBA& i_Color, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maGradient& i_Gradient, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maPoint3d& i_Point, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maIntPoint2d& i_Point, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maRotation& i_Orientation, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maVector4d& i_Point, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, bool i_bValue, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, int i_Value, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, float i_fValue, chDefs::Name i_ChunkName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maMatrix4x4& i_Matrix);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const maGradient& i_Gradient);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const itString& i_Name);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Write(chWriter& i_Writer, const std::string& i_Name);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, const maVector4d& i_Vec)
{
  i_Writer.Write(envType::Float32(i_Vec.GetX()));
  i_Writer.Write(envType::Float32(i_Vec.GetY()));
  i_Writer.Write(envType::Float32(i_Vec.GetZ()));
  i_Writer.Write(envType::Float32(i_Vec.GetW()));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, const maPoint3d& i_Point)
{
  i_Writer.Write(envType::Float32(i_Point.GetX()));
  i_Writer.Write(envType::Float32(i_Point.GetY()));
  i_Writer.Write(envType::Float32(i_Point.GetZ()));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, const maPoint2d& i_Point)
{
  i_Writer.Write(envType::Float32(i_Point.GetX()));
  i_Writer.Write(envType::Float32(i_Point.GetY()));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, const maIntPoint2d& i_Point)
{
  i_Writer.Write(envType::Int32(i_Point.GetX()));
  i_Writer.Write(envType::Int32(i_Point.GetY()));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, const maFloatRGBA& i_Color)
{
  i_Writer.Write(envType::Float32(i_Color.GetAlpha()));
  i_Writer.Write(envType::Float32(i_Color.GetRed()));
  i_Writer.Write(envType::Float32(i_Color.GetGreen()));
  i_Writer.Write(envType::Float32(i_Color.GetBlue()));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, bool i_bValue)
{
  i_Writer.Write(envType::UInt8(i_bValue));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, unsigned int i_nValue)
{
  i_Writer.Write(envType::UInt32(i_nValue));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, int i_nValue)
{
  i_Writer.Write(envType::Int32(i_nValue));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, float i_fValue)
{
  i_Writer.Write(envType::Float32(i_fValue));
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
Write(chWriter& i_Writer, const maRotation& i_Orientation)
{
  maVector3d axis;
  float angle;
  i_Orientation.GetValue(axis, angle);
  Write(i_Writer, axis);
  i_Writer.Write(envType::Float32(angle));
}

//------------------------------------------------------------------------
// If possible, read arrays in one step.
//------------------------------------------------------------------------
template<class T>
void
ReadArray(chReader& i_Reader, std::vector<T>& o_Array, envType::UInt32 i_Num)
{
  o_Array.resize(i_Num);
  if (i_Reader.CanReadBinaryData()) {
    if (i_Num > 0) {
      // Read the whole array at once.
      i_Reader.Read(&o_Array[0], i_Num * sizeof(T));
    }
  } else {
    // Individual vector read
    envType::UInt32 cur;
    for (cur = 0; cur < i_Num; cur++)
      chChunkParserUtil::Read(i_Reader, o_Array[cur]);
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
template<class C>
void
Read(chReader& i_Reader, std::vector<C>& o_Vector)
{
  // read in the size
  envType::UInt32 size;
  i_Reader.Read(size);
  ReadArray(i_Reader, o_Vector, size);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
template<class C>
void
Write(chWriter& i_Writer, const std::vector<C>& i_Vector, chDefs::Name i_ChunkName)
{
  i_Writer.WriteChunkHeader(i_ChunkName, 0, true);

  // Write out the size
  i_Writer.Write(envType::UInt32(i_Vector.size()));

  typename std::vector<C>::const_iterator it, end;
  it = i_Vector.begin();
  end = i_Vector.end();
  for (; it != end; ++it) {
    Write(i_Writer, (*it));
  }

  i_Writer.FinishChunk();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
template<class C>
void
Write(chWriter& i_Writer, const std::vector<C>& i_Vector)
{
  // Write out the size
  i_Writer.Write(envType::UInt32(i_Vector.size()));

  typename std::vector<C>::const_iterator it, end;
  it = i_Vector.begin();
  end = i_Vector.end();
  for (; it != end; ++it) {
    Write(i_Writer, (*it));
  }
}

//------------------------------------------------------------------------
//	Convert the chunk "name" to a string for output to the debug log
//------------------------------------------------------------------------
void
DebugDisplayChunkName(chDefs::Name i_ChunkName);
};
