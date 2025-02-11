/*****************************************************************************
**	prtyTextureDataParser.hpp
**
**	Parse the prtyTextureFileName data
**
**
**
\****************************************************************************/
#ifdef PRTY_TEXTURE_DATAPARSER_HPP
#error prtyTextureDataParser.hpp multiply included
#endif
#define PRTY_TEXTURE_DATAPARSER_HPP

#ifndef CH_DEFS_HPP
#include "Core/ch/chDefs.hpp"
#endif

#ifndef ENV_BOOST_HPP
#include "Core/Env/envBoost.hpp"
#endif

//============================================================================
//============================================================================
class chReader;
class chWriter;
class fsLocator;
class prtyObject;
class prtyTextureFileData;
class prtyTextureFileName;

//============================================================================
//============================================================================
namespace prtyTextureDataParser {
//------------------------------------------------------------------------
//  returns chunk name for this data type
//------------------------------------------------------------------------
chDefs::Name
GetChunkName();

//------------------------------------------------------------------------
//   Write each piece of data for the current texture
//------------------------------------------------------------------------
void
WriteAllTextureData(chWriter& o_Writer, const prtyTextureFileData& i_Data);

//------------------------------------------------------------------------
//   Read each piece of data for the current layer
//------------------------------------------------------------------------
void
ReadAllTextureData(chReader& i_Reader, chDefs::Version i_Version, chDefs::Size i_Size, prtyTextureFileData& o_Data);

//------------------------------------------------------------------------
//   ReadData
//------------------------------------------------------------------------
void
ReadData(chReader& i_Reader, prtyTextureFileName& o_TextureData);

//------------------------------------------------------------------------
//   WriteData
//------------------------------------------------------------------------
void
WriteData(chWriter& o_Writer, const prtyTextureFileName& i_TextureData);

//------------------------------------------------------------------------
// Set the appropriate read and write functions for ramps
// The ramp code is inaccessible to this core class, so we should create
// a function pointer to the proper functions
//------------------------------------------------------------------------
void
SetRampReadFunction(prtyObject* (*i_RampReadFunction)(chReader& i_Reader));
void
SetRampWriteFunction(void (*i_RampWriteFunction)(chWriter& o_Writer, prtyObject* i_RampData));
}