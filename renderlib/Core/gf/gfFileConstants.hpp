/****************************************************************************\
**  gfFileConstants.hpp
**
**      gfFileConstants.hpp defines constants for the gf package.
**
**
**
\****************************************************************************/

#ifdef GF_FILECONSTANTS_HPP
#error gfFileConstants.hpp multiply included
#endif
#define GF_FILECONSTANTS_HPP

//============================================================================
//============================================================================
namespace gfFileConstants {
//------------------------------------------------------------------------
//	gfFile type constants
//------------------------------------------------------------------------
enum gfWriteFormats
{
  eFileBinary = 0,
  eFileXML = 1,
  eFileASCII = 2, // not used -- phase out
  eFileUserDefined,
};
};
