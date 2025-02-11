/*****************************************************************************
**  gfPackage.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/gf/gfPackage.hpp"

#include "Core/dbg/dbgPackage.hpp"
#include "Core/env/envPackage.hpp"
#include "Core/fs/fsPackage.hpp"
#include "Core/gf/gfPaths.hpp"
#include "Core/it/itPackage.hpp"

//============================================================================
//============================================================================
namespace {
int l_RefCount = 0;
}

//----------------------------------------------------------------------------
//	Init must be called before you use the gf package.  A good place to
//	do this is in your main function, before you do anything else.
//	i_BaseTextDir defines the driectory where error code .tsf's can be found
//----------------------------------------------------------------------------
// void gfPackage::Init(const char* i_CDVolumeName, const fsLocator* i_BaseTextDir)
void
gfPackage::Init()
{
  // use ref count to only initialize once
  if (l_RefCount == 0) {
    //	set this up so the dbg package has the paths it needs to write out the logs
    // gfPaths::Init(i_CDVolumeName);
    gfPaths::Init();

    // initialize packages we depend on
    //
    envPackage::Init();
    dbgPackage::Init();
    fsPackage::Init();
    itPackage::Init();

    // initialize our internal stuff
    //
  }

  l_RefCount++;
}

//----------------------------------------------------------------------------
//	CleanUp should be called after you are done with the gf package.
//	A good place to do this is in your main function, after you are done
//	with other deinitialization and cleanup tasks.
//----------------------------------------------------------------------------
void
gfPackage::CleanUp() throw()
{
  l_RefCount--;

  // use ref count to make sure we only clean up once, when everyone is done
  if (l_RefCount == 0) {
    // clean up our internal stuff, in reverse order
    //
    gfPaths::CleanUp();

    // clean up packages we depend on, in reverse order
    //
    itPackage::CleanUp();
    fsPackage::CleanUp();
    dbgPackage::CleanUp();
    envPackage::CleanUp();
  }
}
