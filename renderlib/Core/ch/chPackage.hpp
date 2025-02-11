/*****************************************************************************
**  chPackage.hpp
**
**      chPackage contains the initialization functions
**	for the ch package.
**
**
**
\****************************************************************************/

#ifdef CH_PACKAGE_HPP
#error chPackage.hpp multiply included
#endif
#define CH_PACKAGE_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//============================================================================
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
class chPackage
{
public:
  //------------------------------------------------------------------------
  //	Init must be called before you use the ch package.  A good place to
  //	do this is in your main function, with your other package initializers.
  //------------------------------------------------------------------------
  static void Init();

  //------------------------------------------------------------------------
  //	CleanUp should be called after you are done with the ch package.
  //	A good place to do this is in your main function, after you are done
  //	with other deinitialization and cleanup tasks.
  //------------------------------------------------------------------------
  static void CleanUp() throw();
};
