/*****************************************************************************
**  maTime.hpp
**
**      The maTime represents a time value using an integer
**	for a frame within a 6000fps timeline. This frame value can then
**	be converted to frame numbers in other frame rates or to a
**	floating point value in seconds. The underlying integer
**	representation makes equality comparisons easier and removes
**	some precision errors in floating point math.
**
**
**
\****************************************************************************/

#ifdef MA_TIME_HPP
#error maTime.hpp multiply included
#endif
#define MA_TIME_HPP

#ifndef ENV_TYPE_HPP
#include "Core/Env/envType.hpp"
#endif

//============================================================================
//============================================================================
class maTime
{
public:
  //--------------------------------------------------------------------
  // In order to avoid unintended implicit conversions, there is only
  // the default constructor and instead construction functions are
  // provided as static functions with distinct names.
  //--------------------------------------------------------------------
  static maTime FromSeconds(float i_Seconds);
  static maTime FromFrame(int i_Frame, int i_FrameRate);
  static maTime FromUnits(envType::Int64 i_TimeValue);

  //--------------------------------------------------------------------
  // To make comparisons with zero easier, define a static constant
  //--------------------------------------------------------------------
  static const maTime c_ZeroTime;

  //--------------------------------------------------------------------
  // default constructor, time == 0
  //--------------------------------------------------------------------
  maTime();

  //--------------------------------------------------------------------
  // Get and Set value in its internal units (6000fps frame rate)
  //--------------------------------------------------------------------
  envType::Int64 GetValue() const;
  void SetValue(envType::Int64 i_TimeValue);

  //--------------------------------------------------------------------
  // Set time value as a frame within a given frame rate.
  // The frame rate should be some factor of 6000 or else
  // precision will be lost.
  //--------------------------------------------------------------------
  void SetFrame(int i_Frame, int i_FrameRate);

  //--------------------------------------------------------------------
  //	Set time value from a floating point value in seconds.
  //	This will round to the nearest 6000fps frame.
  //--------------------------------------------------------------------
  void SetSeconds(float i_Seconds);

  //--------------------------------------------------------------------
  //	Get the time as a floating point value in seconds
  //--------------------------------------------------------------------
  float AsSeconds() const;

  //--------------------------------------------------------------------
  //	Get the time as an integer frame in the given frame rate
  //--------------------------------------------------------------------
  int AsFrame(int i_FrameRate) const;

  //--------------------------------------------------------------------
  // assignment operator
  //--------------------------------------------------------------------
  inline maTime& operator=(const maTime& i_A);

  //--------------------------------------------------------------------
  // comparison operators
  //--------------------------------------------------------------------
  inline bool operator==(const maTime& i_A) const;
  inline bool operator!=(const maTime& i_A) const;
  inline bool operator<(const maTime& i_A) const;
  inline bool operator>(const maTime& i_A) const;
  inline bool operator<=(const maTime& i_A) const;
  inline bool operator>=(const maTime& i_A) const;

  //--------------------------------------------------------------------
  // math operators
  //--------------------------------------------------------------------
  inline maTime operator+(const maTime& i_A) const;
  inline maTime operator-(const maTime& i_A) const;
  inline void operator+=(const maTime& i_A);
  inline void operator-=(const maTime& i_A);

  //--------------------------------------------------------------------
  // division returns a floating point representing percent of time
  //--------------------------------------------------------------------
  float operator/(const maTime& i_A) const;

  //--------------------------------------------------------------------
  // Change sign of time value (i.e. multiply by -1)
  //--------------------------------------------------------------------
  void Negate();

  //--------------------------------------------------------------------
  // Return positive time value
  //--------------------------------------------------------------------
  maTime Abs() const;

private:
  envType::Int64 m_TimeInUnits;
};

//--------------------------------------------------------------------
// assignment operator
//--------------------------------------------------------------------
maTime&
maTime::operator=(const maTime& i_A)
{
  m_TimeInUnits = i_A.m_TimeInUnits;
  return (*this);
}

//--------------------------------------------------------------------
// comparison operators
//--------------------------------------------------------------------
bool
maTime::operator==(const maTime& i_A) const
{
  return (i_A.m_TimeInUnits == m_TimeInUnits);
}
bool
maTime::operator!=(const maTime& i_A) const
{
  return (i_A.m_TimeInUnits != m_TimeInUnits);
}
bool
maTime::operator>(const maTime& i_A) const
{
  return (m_TimeInUnits > i_A.m_TimeInUnits);
}
bool
maTime::operator<(const maTime& i_A) const
{
  return (m_TimeInUnits < i_A.m_TimeInUnits);
}
bool
maTime::operator>=(const maTime& i_A) const
{
  return (m_TimeInUnits >= i_A.m_TimeInUnits);
}
bool
maTime::operator<=(const maTime& i_A) const
{
  return (m_TimeInUnits <= i_A.m_TimeInUnits);
}

//--------------------------------------------------------------------
// math operators
//--------------------------------------------------------------------
maTime
maTime::operator+(const maTime& i_A) const
{
  maTime time;
  time.m_TimeInUnits = (m_TimeInUnits + i_A.m_TimeInUnits);
  return time;
}
maTime
maTime::operator-(const maTime& i_A) const
{
  maTime time;
  time.m_TimeInUnits = (m_TimeInUnits - i_A.m_TimeInUnits);
  return time;
}
void
maTime::operator+=(const maTime& i_A)
{
  m_TimeInUnits += i_A.m_TimeInUnits;
}
void
maTime::operator-=(const maTime& i_A)
{
  m_TimeInUnits -= i_A.m_TimeInUnits;
}
