/********************************************************************************************\
**  prtyTextureDataParser.cpp
**
**		see .hpp
**
**
**
\********************************************************************************************/
#include "Core/prty/prtyTextureDataParser.hpp"

#include "Core/prty/prtyObject.hpp"
#include "Core/prty/prtyTextureFileName.hpp"
#include "Core/prty/prtyTextureFileChooserUIInfo.hpp"
#include "Core/ch/chChunkParserUtil.hpp"
#include "Core/ch/chExceptionX.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/fs/fsFileX.hpp"

//============================================================================
//============================================================================
namespace prtyTextureDataParser {

namespace {
//------------------------------------------------------------------------
//------------------------------------------------------------------------
const chDefs::Name c_TXFD = chDefs::MakeName('T', 'X', 'F', 'D'); //   +-- texture file data
const chDefs::Name c_TXDT = chDefs::MakeName('T', 'X', 'D', 'T'); // +-- texture data type
const chDefs::Name c_FILE = chDefs::MakeName('F', 'I', 'L', 'E'); //   +-- file data
const chDefs::Name c_PNTG = chDefs::MakeName('P', 'N', 'T', 'G'); // +-- paint list
const chDefs::Name c_RAMP = chDefs::MakeName('R', 'A', 'M', 'P'); // +-- ramp data

//------------------------------------------------------------------------
//  Function ptr, callback when something is written to the buffer
//------------------------------------------------------------------------
prtyObject* (*l_prtyRampReadFunction)(chReader& i_Reader);
void (*l_prtyRampWriteFunction)(chWriter& o_Writer, prtyObject* i_RampData);

//------------------------------------------------------------------------
//   ReadTextureTypeData
//------------------------------------------------------------------------
void
ReadTextureTypeData(chReader& io_Reader, chDefs::Version i_Version, prtyTextureFileData& o_TextureData)
{
  std::string callback;
  chChunkParserUtil::Read(io_Reader, callback);
  o_TextureData.m_CurrentCallback = callback;
}

//------------------------------------------------------------------------
//   WriteTextureTypeData
//------------------------------------------------------------------------
void
WriteTextureTypeData(chWriter& o_Writer, const prtyTextureFileData& i_TextureData)
{
  const int l_cTXDT_VERSION = 1;
  o_Writer.WriteChunkHeader(c_TXDT, l_cTXDT_VERSION, true);

  // write out the current callback value of the texture
  chChunkParserUtil::Write(o_Writer, i_TextureData.m_CurrentCallback);

  o_Writer.FinishChunk();
}

//------------------------------------------------------------------------
//   ReadTextureFile
//------------------------------------------------------------------------
void
ReadTextureFile(chReader& io_Reader, chDefs::Version i_Version, prtyTextureFileData& o_TextureData)
{
  itString tex_filename;
  chChunkParserUtil::Read(io_Reader, tex_filename);
  fsFileUtil::UnicodeStringToLocator(tex_filename, o_TextureData.m_TextureLocator);
}

//------------------------------------------------------------------------
//   WriteTextureFile
//------------------------------------------------------------------------
void
WriteTextureFile(chWriter& o_Writer, const prtyTextureFileData& i_TextureData)
{
  const int l_cFILE_VERSION = 1;
  o_Writer.WriteChunkHeader(c_FILE, l_cFILE_VERSION, true);

  // write out the current texture file
  itString tex_filename;
  fsFileUtil::LocatorToUnicodeString(i_TextureData.m_TextureLocator, tex_filename);
  chChunkParserUtil::Write(o_Writer, tex_filename);

  o_Writer.FinishChunk();
}

//------------------------------------------------------------------------
//   ReadTexturePaint
//------------------------------------------------------------------------
void
ReadTexturePaint(chReader& io_Reader, chDefs::Version i_Version, prtyTextureFileData& o_TextureData)
{
  //**Currently paint has no special parameters, so just read the texture location
  itString tex_filename;
  chChunkParserUtil::Read(io_Reader, tex_filename);
  fsFileUtil::UnicodeStringToLocator(tex_filename, o_TextureData.m_TextureLocator);
}

//------------------------------------------------------------------------
//   WriteTexturePaint
//------------------------------------------------------------------------
void
WriteTexturePaint(chWriter& o_Writer, const prtyTextureFileData& i_TextureData)
{
  const int l_cPNTG_VERSION = 1;
  o_Writer.WriteChunkHeader(c_PNTG, l_cPNTG_VERSION, true);

  //**Currently paint has no special parameters, so just write the texture location
  itString tex_filename;
  fsFileUtil::LocatorToUnicodeString(i_TextureData.m_TextureLocator, tex_filename);
  chChunkParserUtil::Write(o_Writer, tex_filename);

  o_Writer.FinishChunk();
}

//------------------------------------------------------------------------
//   ReadTextureRamp
//------------------------------------------------------------------------
void
ReadTextureRamp(chReader& io_Reader, chDefs::Version i_Version, prtyTextureFileData& o_TextureData)
{
  if (l_prtyRampReadFunction != NULL) {
    prtyObject* pRampObj = NULL;
    pRampObj = l_prtyRampReadFunction(io_Reader);
    o_TextureData.m_RampObject = (shared_ptr<prtyObject>)(pRampObj);
  }
}

//------------------------------------------------------------------------
//   WriteTextureRamp
//------------------------------------------------------------------------
void
WriteTextureRamp(chWriter& o_Writer, const prtyTextureFileData& i_TextureData)
{
  const int l_cRAMP_VERSION = 1;
  o_Writer.WriteChunkHeader(c_RAMP, l_cRAMP_VERSION, true);

  if (l_prtyRampWriteFunction != NULL) {
    l_prtyRampWriteFunction(o_Writer, i_TextureData.m_RampObject.get());
  }

  o_Writer.FinishChunk();
}

} // end namespace

//------------------------------------------------------------------------
//  returns chunk name for this data type
//------------------------------------------------------------------------
chDefs::Name
GetChunkName()
{
  return c_TXFD;
}

////------------------------------------------------------------------------
////   Write each piece of data for the current layer
////------------------------------------------------------------------------
void
WriteAllTextureData(chWriter& o_Writer, const prtyTextureFileData& i_Data)
{
  // write the current type
  WriteTextureTypeData(o_Writer, i_Data);

  // depending on the current type, write the next chunk of data
  std::string callback = i_Data.m_CurrentCallback;
  int callbackID = prtyTextureFileChooserUIInfo::GetTextureTypeID(callback);
  switch (callbackID) {
    case prtyTextureFileChooserUIInfo::e_Texture:
      WriteTextureFile(o_Writer, i_Data);
      break;
    case prtyTextureFileChooserUIInfo::e_Paint:
      WriteTexturePaint(o_Writer, i_Data);
      break;
    case prtyTextureFileChooserUIInfo::e_Ramp:
      WriteTextureRamp(o_Writer, i_Data);
      break;
    default:
      WriteTextureFile(o_Writer, i_Data); // if no category, write the file data if any
      break;
  }
}

////------------------------------------------------------------------------
////   Read each piece of data for the current layer
////------------------------------------------------------------------------
void
ReadAllTextureData(chReader& i_Reader, chDefs::Version i_Version, chDefs::Size i_Size, prtyTextureFileData& o_Data)
{
  chDefs::Name name;
  chDefs::Version version;
  chDefs::Size size;

  while (i_Reader.ReadChunkHeader(name, version, size)) {

    if (name == c_TXDT) {
      ReadTextureTypeData(i_Reader, version, o_Data);
    } else if (name == c_FILE) {
      ReadTextureFile(i_Reader, version, o_Data);
    } else if (name == c_PNTG) {
      ReadTexturePaint(i_Reader, version, o_Data);
    } else if (name == c_RAMP) {
      ReadTextureRamp(i_Reader, version, o_Data);
    } else {
      // DBG_ASSERT(false, "invalid chunk header");
      DBG_TRACE("invalid chunk header" << name);
    }
    i_Reader.FinishChunk();
  }
}

//------------------------------------------------------------------------
//   ReadData
//------------------------------------------------------------------------
void
ReadData(chReader& i_Reader, prtyTextureFileName& o_TextureData)
{
  chDefs::Name name;
  chDefs::Version version;
  chDefs::Size size;

  prtyTextureFileData cur_data;
  while (i_Reader.ReadChunkHeader(name, version, size)) {
    if (name == c_TXFD) {
      ReadAllTextureData(i_Reader, version, size, cur_data);
      o_TextureData.SetValue(cur_data);

      i_Reader.FinishChunk();
    } else {
      // DBG_ASSERT(false, "invalid chunk header");
      DBG_TRACE("invalid chunk header" << name);
    }
  }
}

//------------------------------------------------------------------------
//   WriteData
//------------------------------------------------------------------------
void
WriteData(chWriter& o_Writer, const prtyTextureFileName& i_TextureData)
{
  const int l_cTXFD_VERSION = 1;
  o_Writer.WriteChunkHeader(c_TXFD, l_cTXFD_VERSION, true);

  WriteAllTextureData(o_Writer, i_TextureData.GetFullValue());

  o_Writer.FinishChunk();
}

//------------------------------------------------------------------------
// Set the appropriate read and write functions for ramps
// The ramp code is inaccessible to this core class, so we should create
// a function pointer to the proper functions
//------------------------------------------------------------------------
void
SetRampReadFunction(prtyObject* (*i_RampReadFunction)(chReader& i_Reader))
{
  l_prtyRampReadFunction = i_RampReadFunction;
}
void
SetRampWriteFunction(void (*i_RampWriteFunction)(chWriter& o_Writer, prtyObject* i_RampData))
{
  l_prtyRampWriteFunction = i_RampWriteFunction;
}

} // end of namespace
