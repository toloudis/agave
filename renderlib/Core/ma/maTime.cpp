/*****************************************************************************
**  maTime.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/ma/maTime.hpp"

#include "Core/dbg/dbgMsg.hpp"

#include <limits>

//============================================================================
//============================================================================
namespace {

}

//--------------------------------------------------------------------
// In order to avoid unintended implicit conversions, there is only
// the default constructor and instead construction functions are
// provided as static functions with distinct names.
//--------------------------------------------------------------------
maTime
maTime::FromSeconds(float i_Seconds)
{
  maTime time;
  time.SetSeconds(i_Seconds);
  return time;
}
maTime
maTime::FromFrame(int i_Frame, int i_FrameRate)
{
  maTime time;
  time.SetFrame(i_Frame, i_FrameRate);
  return time;
}
maTime
maTime::FromUnits(envType::Int64 i_TimeValue)
{
  maTime time;
  time.SetValue(i_TimeValue);
  return time;
}

//--------------------------------------------------------------------
// To make comparisons with zero easier, define a static constant
//--------------------------------------------------------------------
const maTime maTime::c_ZeroTime = maTime::FromUnits(0);

//----------------------------------------------------------------------------
// default constructor, time == 0
//----------------------------------------------------------------------------
maTime::maTime()
  : m_TimeInUnits(0)
{
}

//--------------------------------------------------------------------
// Get and Set value in its internal units (6000fps frame rate)
//--------------------------------------------------------------------
envType::Int64
maTime::GetValue() const
{
  return m_TimeInUnits;
}
void
maTime::SetValue(envType::Int64 i_TimeValue)
{
  m_TimeInUnits = i_TimeValue;
}

//--------------------------------------------------------------------
// Set time value as a frame within a given frame rate.
// The frame rate should be some factor of 6000 or else
// precision will be lost.
//--------------------------------------------------------------------
void
maTime::SetFrame(int i_Frame, int i_FrameRate)
{
  if (i_FrameRate > 0)
    m_TimeInUnits = i_Frame * (6000 / i_FrameRate);
}

//--------------------------------------------------------------------
//	Set time value from a floating point value in seconds.
//	This will round to the nearest 6000fps frame.
//--------------------------------------------------------------------
void
maTime::SetSeconds(float i_Seconds)
{
  // the (+-)0.5f is to round to nearest frame since casting from float to int
  // drops down to integer below without rounding when positive and drops up
  // when negative.
  float fUnits = i_Seconds * 6000.0f;
  if (fUnits < 0)
    fUnits -= 0.5f;
  else
    fUnits += 0.5f;
  if (fUnits > std::numeric_limits<envType::Int64>::max())
    m_TimeInUnits = std::numeric_limits<envType::Int64>::max();
  else if (fUnits < std::numeric_limits<envType::Int64>::min())
    m_TimeInUnits = std::numeric_limits<envType::Int64>::min();
  else
    m_TimeInUnits = envType::Int64(fUnits);
}

//--------------------------------------------------------------------
//	Get the time as a floating point value in seconds
//--------------------------------------------------------------------
float
maTime::AsSeconds() const
{
  return (m_TimeInUnits / 6000.0f);
}

//--------------------------------------------------------------------
//	Get the time as an integer frame in the given frame rate
//--------------------------------------------------------------------
int
maTime::AsFrame(int i_FrameRate) const
{
  if (i_FrameRate <= 0)
    return 0;

  // should this be rounded off to nearest frame or clamped down or return float?
  return (m_TimeInUnits / (6000 / i_FrameRate));
}

//--------------------------------------------------------------------
// division returns a floating point representing percent of time
//--------------------------------------------------------------------
float
maTime::operator/(const maTime& i_A) const
{
  if (i_A.m_TimeInUnits == 0)
    return 0;
  return (m_TimeInUnits / (float)i_A.m_TimeInUnits);
}

//--------------------------------------------------------------------
// Change sign of time value (i.e. multiply by -1)
//--------------------------------------------------------------------
void
maTime::Negate()
{
  m_TimeInUnits *= -1;
}

//--------------------------------------------------------------------
// Return positive time value
//--------------------------------------------------------------------
maTime
maTime::Abs() const
{
  maTime time(*this);
  if (m_TimeInUnits < 0)
    time.Negate();
  return time;
}
