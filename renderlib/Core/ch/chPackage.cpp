/*****************************************************************************
**  chPackage.cpp
**
**      chPackage contains the initialization functions
**	for the ch package.
**
**
**
\****************************************************************************/

#include "Core/ch/chPackage.hpp"

#include "Core/dbg/dbgPackage.hpp"
#include "Core/env/envPackage.hpp"
#include "Core/fs/fsPackage.hpp"
#include "Core/gf/gfPackage.hpp"
#include "Core/it/itString.hpp"

//============================================================================
//============================================================================
namespace {
int l_RefCount = 0;
}

//----------------------------------------------------------------------------
//	Init must be called before you use the ch package.  A good place to
//	do this is in your main function, before you do anything else.
//----------------------------------------------------------------------------
void
chPackage::Init()
{
  // use ref count to only initialize once
  if (l_RefCount == 0) {
    // initialize packages we depend on
    //
    envPackage::Init();
    dbgPackage::Init();
    fsPackage::Init();
    gfPackage::Init();

    // initialize our internal stuff
    //
  }

  //	increment the ref count
  l_RefCount++;
}

//----------------------------------------------------------------------------
//	CleanUp should be called after you are done with the ch package.
//	A good place to do this is in your main function, after you are done
//	with other deinitialization and cleanup tasks.
//----------------------------------------------------------------------------
void
chPackage::CleanUp() throw()
{
  l_RefCount--;

  // use ref count to make sure we only clean up once, when everyone is done
  if (l_RefCount == 0) {
    // clean up our internal stuff, in reverse order
    //

    // clean up packages we depend on, in reverse order
    //
    gfPackage::CleanUp();
    fsPackage::CleanUp();
    dbgPackage::CleanUp();
    envPackage::CleanUp();
  }
}
