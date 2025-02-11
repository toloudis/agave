/**********************************************************
**  itPackage.cpp
**
**      see .hpp
**
**
**
\*********************************************************/

#include "Core/it/itPackage.hpp"

#include "Core/dbg/dbgMsg.hpp"
#include "Core/dbg/dbgPackage.hpp"
#include "Core/env/envPackage.hpp"

//============================================================================
//============================================================================
namespace {

int l_RefCount = 0;

}

//------------------------------------------------------------------------
//	Init must be called before you use the dbg package.  A good place to
//	do this is in your main function, before you do anything else.
//------------------------------------------------------------------------
void
itPackage::Init()
{
  // use ref count to only initialize once
  if (l_RefCount == 0) {
    // initialize packages we depend on
    envPackage::Init();
    dbgPackage::Init();

    // initialize our internal stuff
    // (nothing yet)
  }

  l_RefCount++;
}

//------------------------------------------------------------------------
//	CleanUp should be called after you are done with the dbg package.
//	A good place to do this is in your main function, after you are done
//	with other deinitialization and cleanup tasks.
//------------------------------------------------------------------------
void
itPackage::CleanUp() throw()
{
  l_RefCount--;

  // use ref count to make sure we only clean up once, when everyone is done
  if (l_RefCount == 0) {
    // clean up our internal stuff, in reverse order
    // (nothing yet)

    // clean up packages we depend on
    dbgPackage::CleanUp();
    envPackage::CleanUp();
  }
}
