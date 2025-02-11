/****************************************************************************\
**  envThreadGroup.hpp
**
**      envThreadGroup.hpp provides a way to launch a group of threads
**	to handle a set of tasks and then wait for the threads to finish.
**	If single-threaded, the tasks will complete in order.
**	If multithreaded, the tasks will run on separate processors.
**
**
**
\****************************************************************************/
#ifdef ENV_THREADGROUP_HPP
#error envThreadGroup.hpp multiply included
#endif
#define ENV_THREADGROUP_HPP

#ifndef ENV_THREAD_HPP
#include "Core/env/envThread.hpp"
#endif

#include <functional>
#include <thread>
#include <vector>

//========================================================================
// envThreadGroup is a manager for a set of tasks that can be
//	run concurrently.
//========================================================================
class envThreadGroup
{
public:
  typedef std::function<void()> ThreadFunc;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  static void SetThreadingEnabled(bool i_bEnabled);
  static bool GetThreadingEnabled();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  envThreadGroup();

  //--------------------------------------------------------------------
  // Add a task to the group to be managed. The argument could be
  //	a function pointer or a function object.
  //--------------------------------------------------------------------
  void AddThread(const ThreadFunc& i_ThreadFunc);

  //--------------------------------------------------------------------
  // Wait for all tasks to finish.
  //--------------------------------------------------------------------
  void WaitForAll();

private:
#ifdef ENV_USE_THREADS
  std::vector<std::thread> m_ThreadGroup;
#endif
  static bool sm_bEnableThreading;
};
