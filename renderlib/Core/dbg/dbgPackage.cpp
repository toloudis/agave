//****************************************************************************
//  dbgPackage.cpp
//
//      see .hpp
//
//
//
//****************************************************************************
#include "Core/dbg/dbgPackage.hpp"

#include "Core/dbg/dbgMsg.hpp"
#include "Core/env/envPackage.hpp"
#include "Core/Fs/fsLocator.hpp"
#include "Core/Gf/gfPaths.hpp"

//============================================================================
//============================================================================
namespace {
int l_RefCount = 0;
}

//--------------------------------------------------------------------
//	Init must be called before you use the dbg package.  A good place
//	to do this is in your main function, before you do anything else.
//
// \param none
//
// \return void
//--------------------------------------------------------------------
void
dbgPackage::Init()
{
  // use ref count to only initialize once
  if (l_RefCount == 0) {
    // initialize packages we depend on
    envPackage::Init();
    gfPaths::Init(); // gfPaths needed because of location of the debug log

    // initialize our internal stuff
    dbgMsg::Init();
  }

  l_RefCount++;
}

//--------------------------------------------------------------------
//	CleanUp should be called after you are done with the dbg package.
//	A good place to do this is in your main function, after you are done
//	with other deinitialization and cleanup tasks.
//	The CleanUp function is written with the throw() exception
//	to suggest that it should not throw any exceptions, since typically
//	the caller is in the process of de-initializing and won't be able
//	to do much with them.
//
// \param none
//
// \return void
//--------------------------------------------------------------------
void
dbgPackage::CleanUp() throw()
{
  l_RefCount--;

  // use ref count to make sure we only clean up once, when everyone is done
  if (l_RefCount == 0) {
    // clean up our internal stuff, in reverse order
    dbgMsg::CleanUp();

    // clean up packages we depend on
    gfPaths::CleanUp();
    envPackage::CleanUp();
  }
}
