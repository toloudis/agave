/****************************************************************************\
**  envWaitCondition.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/

#include "Core/env/envWaitCondition.hpp"

//--------------------------------------------------------------------
//--------------------------------------------------------------------
envWaitCondition::envWaitCondition()
  : m_bTaskFinished(false)
{
}

//--------------------------------------------------------------------
// Wait for another thread to call "NotifyFinished".
// If i_bResetFlag is true, the m_bTaskFinished tag is set
// to false so that this wait condition can be used again.
//--------------------------------------------------------------------
void
envWaitCondition::WaitForNotify(bool i_bResetFlag)
{
#ifdef ENV_USE_THREADS
  std::unique_lock<std::mutex> lock(m_Mutex);
  while (!m_bTaskFinished) {
    m_Condition.wait(lock);
  }
#endif

  if (i_bResetFlag)
    m_bTaskFinished = false;
}

//--------------------------------------------------------------------
// Mark the task as finished.
//--------------------------------------------------------------------
void
envWaitCondition::NotifyFinished()
{
#ifdef ENV_USE_THREADS
  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_bTaskFinished = true;
  }
  m_Condition.notify_one();
#else
  m_bTaskFinished = true;
#endif
}
