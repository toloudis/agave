/*****************************************************************************
**  maPackage.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maPackage.hpp"

#include "Core/dbg/dbgPackage.hpp"
#include "Core/ma/maFunctions.hpp"

//============================================================================
//============================================================================
namespace {
int l_RefCount = 0;
}

//----------------------------------------------------------------------------
//	Init must be called before you use this package.  A good place to
//	do this is in your main function, before you do anything else.
//----------------------------------------------------------------------------
void
maPackage::Init()
{
  // use ref count to only initialize once
  if (l_RefCount == 0) {
    // initialize packages we depend on
    dbgPackage::Init();
  }

  l_RefCount++;
}

//----------------------------------------------------------------------------
//	CleanUp should be called after you are done with this package.
//	A good place to do this is in your main function, after you are done
//	with other deinitialization and cleanup tasks.
//----------------------------------------------------------------------------
void
maPackage::CleanUp()
{
  l_RefCount--;

  // use ref count to make sure we only clean up once, when everyone is done
  if (l_RefCount == 0) {
    // clean up our internal stuff, in reverse order
    //

    // clean up packages we depend on
    dbgPackage::CleanUp();
  }
}
