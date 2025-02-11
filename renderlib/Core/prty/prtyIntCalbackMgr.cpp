/****************************************************************************\
**	prtyIntCallbackMgr.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyIntCallbackMgr.hpp"

#include "Core/dbg/dbgMsg.hpp"

//----------------------------------------------------------------------------
// This whole integer callback thing only is needed for managed controls
// that can't have non-managed callback objects. So, I don't
// want to assert if there is no implmentation in this case.
//----------------------------------------------------------------------------
void
prtyIntCallbackMgr::PropertyChanged(prtyProperty* i_pProperty, int i_Index)
{
  // DBG_ASSERT(sm_pImplementation, "prtyIntCallbackMgr: No implementation");
  if (sm_pImplementation) {
    sm_pImplementation->PropertyChanged(i_pProperty, i_Index);
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
prtyIntCallbackMgr::UpdateControl(prtyPropertyUIInfo* i_pUII)
{
  // DBG_ASSERT(sm_pImplementation, "prtyIntCallbackMgr: No implementation");
  if (sm_pImplementation) {
    sm_pImplementation->UpdateControl(i_pUII);
  }
}
