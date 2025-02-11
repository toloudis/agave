/****************************************************************************\
**  envThreadGroup.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/

#include "Core/env/envThreadGroup.hpp"

//====================================================================
// static member variables
//====================================================================
bool envThreadGroup::sm_bEnableThreading = false;

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
envThreadGroup::SetThreadingEnabled(bool i_bEnabled)
{
  envThreadGroup::sm_bEnableThreading = i_bEnabled;
}
bool
envThreadGroup::GetThreadingEnabled()
{
  return envThreadGroup::sm_bEnableThreading;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
envThreadGroup::envThreadGroup() {}

//--------------------------------------------------------------------
// Add a task to the group to be managed. The argument could be
//	a function pointer or a function object.
//--------------------------------------------------------------------
void
envThreadGroup::AddThread(const ThreadFunc& i_ThreadFunc)
{
#ifdef ENV_USE_THREADS
  if (envThreadGroup::sm_bEnableThreading)
    m_ThreadGroup.emplace_back(i_ThreadFunc);
  // m_ThreadGroup.create_thread(i_ThreadFunc);
  else
    i_ThreadFunc();
#else
  // If not using threads, execute function immediately
  i_ThreadFunc();
#endif
}

//--------------------------------------------------------------------
// Wait for all tasks to finish.
//--------------------------------------------------------------------
void
envThreadGroup::WaitForAll()
{
#ifdef ENV_USE_THREADS
  for (auto& thread : m_ThreadGroup)
    thread.join();
#endif
}
