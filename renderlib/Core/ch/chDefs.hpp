/*****************************************************************************
**  chDefs.hpp
**
**      chDefs contains several definitions global to the ch package,
**	and a useful function.
**
**
**
\****************************************************************************/

#ifdef CH_DEFS_HPP
#error chDefs.hpp multiply included
#endif
#define CH_DEFS_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

//============================================================================
//============================================================================
namespace chDefs {
typedef envType::UInt32 Name;
typedef envType::UInt32 Size;
typedef envType::UInt16 Version;

//------------------------------------------------------------------------
//	MakeName returns a chDefs::Name made from the 4 given characters.
//	The characters will appear in the order given on
//	little-endian machines.
//------------------------------------------------------------------------
Name
MakeName(char i_1, char i_2, char i_3, char i_4);

//----------------------------------------------------------------------------
//	Implementation
//----------------------------------------------------------------------------

inline Name
MakeName(char i_1, char i_2, char i_3, char i_4)
{
  Name ret_val = i_4;

  ret_val <<= 8;
  ret_val |= i_3;

  ret_val <<= 8;
  ret_val |= i_2;

  ret_val <<= 8;
  ret_val |= i_1;

  return ret_val;
}

}
