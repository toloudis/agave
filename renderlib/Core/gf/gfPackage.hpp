/*****************************************************************************
**  gfPackage.hpp
**
**      gfPackage contains the initialization functions
**	for the gf package.  The gf package's package error
**	index is 4.
**
**
**
\****************************************************************************/

#ifdef GF_PACKAGE_HPP
#error gfPackage.hpp multiply included
#endif
#define GF_PACKAGE_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//============================================================================
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
class gfPackage
{
public:
  //--------------------------------------------------------------------
  //	Init must be called before you use the gf package.  A good place to
  //	do this is in your main function, with your other package initializers.
  //	i_BaseTextDir defines the driectory where error code .tsf's can be found
  //--------------------------------------------------------------------
  // static void Init(const char* i_CDVolumeName = NULL, const fsLocator* i_BaseTextDir = NULL);
  static void Init();

  //--------------------------------------------------------------------
  //	CleanUp should be called after you are done with the gf package.
  //	A good place to do this is in your main function, after you are done
  //	with other deinitialization and cleanup tasks.
  //--------------------------------------------------------------------
  static void CleanUp() throw();
};
