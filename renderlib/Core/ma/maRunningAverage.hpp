/*****************************************************************************
**  maRunningAverage.hpp
**
**      The maRunningAverage averages the last "n" values given to it.
**
**
**
\****************************************************************************/
#ifdef MA_RUNNINGAVERAGE_HPP
#error maRunningAverage.hpp multiply included
#endif
#define MA_RUNNINGAVERAGE_HPP

#include <vector>

//============================================================================
//============================================================================
class maRunningAverage
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  maRunningAverage(int i_HistorySize = 600);

  //--------------------------------------------------------------------
  //	Push adds a value to the history and also causes the oldest value
  //	to be popped.
  //--------------------------------------------------------------------
  void Push(float i_Val);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  float GetAverage() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetHistorySize(int i_HistorySize);

private:
  std::vector<float> m_History;
  int m_HistorySize;
  int m_NextToReplace;
  float m_Average;
};
