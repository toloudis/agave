/**********************************************************
**  envPackage.cpp
**
**      see .hpp
**
**
**
\*********************************************************/
#include "Core/env/envPackage.hpp"

#include "Core/env/envSystemData.hpp"

//============================================================================
//============================================================================
namespace {

int l_RefCount = 0;

}

//------------------------------------------------------------------------
//	Init must be called before you use the env package.  A good place to
//	do this is in your main function, before you do anything else.
//------------------------------------------------------------------------
void
envPackage::Init()
{
  if (l_RefCount == 0) {
    envSystemData::Init();
  }

  l_RefCount++;
}

//------------------------------------------------------------------------
//	CleanUp should be called after you are done with the env package.
//	A good place to do this is in your main function, after you are done
//	with other deinitialization and cleanup tasks.
//------------------------------------------------------------------------
void
envPackage::CleanUp() throw()
{
  l_RefCount--;

  if (l_RefCount == 0) {
    envSystemData::CleanUp();
  }
}
