/****************************************************************************\
**  envWaitCondition.hpp
**
**      envWaitCondition.hpp provides a way for one thread to wait for
**	a signal from another thread.
**
**
**
\****************************************************************************/
#ifdef ENV_WAITCONDITION_HPP
#error envWaitCondition.hpp multiply included
#endif
#define ENV_WAITCONDITION_HPP

#ifndef ENV_THREAD_HPP
#include "Core/env/envThread.hpp"
#endif

#ifdef ENV_USE_THREADS
#include <condition_variable>
#endif

//========================================================================
//========================================================================
class envWaitCondition
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  envWaitCondition();

  //--------------------------------------------------------------------
  // Wait for another thread to call "NotifyFinished".
  // If i_bResetFlag is true, the m_bTaskFinished tag is set
  // to false so that this wait condition can be used again.
  //--------------------------------------------------------------------
  void WaitForNotify(bool i_bResetFlag = true);

  //--------------------------------------------------------------------
  // Mark the task as finished.
  //--------------------------------------------------------------------
  void NotifyFinished();

private:
  bool m_bTaskFinished;

#ifdef ENV_USE_THREADS
  envMutex m_Mutex;
  std::condition_variable m_Condition;
#endif
};
