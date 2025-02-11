/*****************************************************************************
**  fsPackage.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/fs/fsPackage.hpp"

#include "Core/dbg/dbgPackage.hpp"
#include "Core/env/envPackage.hpp"
#include "Core/fs/fsFileUtil.hpp"
#include "Core/fs/fsFileTimeUtil.hpp"
#include "Core/it/itPackage.hpp"

//============================================================================
//============================================================================
namespace {

int l_RefCount = 0;

}

//----------------------------------------------------------------------------
//	Init must be called before you use the fs package.  A good place to
//	do this is in your main function, before you do anything else.
//----------------------------------------------------------------------------
void
fsPackage::Init()
{
  // use ref count to only initialize once
  if (l_RefCount == 0) {
    // initialize packages we depend on
    envPackage::Init();
    dbgPackage::Init();
    itPackage::Init();

    // initialize our internal stuff
    //
    fsFileUtil::Init();
    fsFileTimeUtil::Init();
  }

  l_RefCount++;
}

//----------------------------------------------------------------------------
//	CleanUp should be called after you are done with the fs package.
//	A good place to do this is in your main function, after you are done
//	with other deinitialization and cleanup tasks.
//----------------------------------------------------------------------------
void
fsPackage::CleanUp() throw()
{
  l_RefCount--;

  // use ref count to make sure we only clean up once, when everyone is done
  if (l_RefCount == 0) {
    // clean up our internal stuff, in reverse order
    //
    fsFileTimeUtil::CleanUp();
    fsFileUtil::CleanUp();

    // clean up packages we depend on
    itPackage::CleanUp();
    dbgPackage::CleanUp();
    envPackage::CleanUp();
  }
}
