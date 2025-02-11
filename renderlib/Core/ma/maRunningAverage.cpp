/*****************************************************************************
**  maRunningAverage.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/ma/maRunningAverage.hpp"

#include "Core/dbg/dbgMsg.hpp"
#include "Core/ma/maFunctions.hpp"

//--------------------------------------------------------------------
//--------------------------------------------------------------------
maRunningAverage::maRunningAverage(int i_HistorySize)
  : m_HistorySize(i_HistorySize)
  , m_NextToReplace(0)
  , m_Average(0.0f)
{
  m_History.resize(i_HistorySize);
}

//--------------------------------------------------------------------
//	Push adds a value to the history and also causes the oldest value
//	to be popped.
//--------------------------------------------------------------------
void
maRunningAverage::Push(float i_Val)
{
  if (m_History.size() < m_HistorySize) {
    m_History.push_back(i_Val);
  } else {
    m_History[m_NextToReplace++] = i_Val;
    if (m_NextToReplace >= m_History.size())
      m_NextToReplace = 0;
  }

  int num = maFunctions::Lowest(int(m_HistorySize), int(m_History.size()));
  m_Average = 0.0f;

  int i;
  for (i = 0; i < num; ++i) {
    m_Average += m_History[i];
  }

  m_Average /= float(num);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
float
maRunningAverage::GetAverage() const
{
  return m_Average;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void
maRunningAverage::SetHistorySize(int i_HistorySize)
{
  DBG_ASSERT(i_HistorySize > 0, "Running Average history size must be greater than zero.");

  if (m_HistorySize > 0) {
    m_HistorySize = i_HistorySize;

    m_History.resize(m_HistorySize);
  }
}
