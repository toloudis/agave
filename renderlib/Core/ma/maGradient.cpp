/****************************************************************************\
**  maGradient.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/

#include "Core/ma/maGradient.hpp"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
maGradient::maGradient()
{
  AddNode(0.0f, maFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f));
  AddNode(1.0f, maFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f));
}

//--------------------------------------------------------------------------------
//	Copy constructor
//--------------------------------------------------------------------------------
maGradient::maGradient(const maGradient& i_CopyFrom)
  : m_GradientList(i_CopyFrom.m_GradientList)
{
}

//------------------------------------------------------------------------
//	Get the number of node in maGradient
//------------------------------------------------------------------------
int
maGradient::GetSize() const
{
  return m_GradientList.size();
}

//------------------------------------------------------------------------
//	Clear list
//------------------------------------------------------------------------
void
maGradient::Clear()
{
  m_GradientList.clear();
}

//------------------------------------------------------------------------
//	Add one node to list
//------------------------------------------------------------------------
void
maGradient::AddNode(float i_pos, const maFloatRGBA& i_color)
{
  i_pos = (i_pos > 1.0f) ? 1.0f : i_pos;
  i_pos = (i_pos < 0.0f) ? 0.0f : i_pos;

  m_GradientList.push_back(GradientNodePair(i_pos, i_color));
}

//------------------------------------------------------------------------
//	Equality
//------------------------------------------------------------------------
bool
maGradient::operator==(const maGradient& i_Gradient) const
{
  if (m_GradientList.size() != i_Gradient.m_GradientList.size()) {
    return false;
  }
  for (int i = 0; i < m_GradientList.size(); i++) {
    if (!(m_GradientList[i] == i_Gradient.m_GradientList[i])) {
      return false;
    }
  }

  return true;
}
bool
maGradient::operator!=(const maGradient& i_Gradient) const
{
  return (!(*this == i_Gradient));
}

//------------------------------------------------------------------------
//	Assignment
//------------------------------------------------------------------------
const maGradient&
maGradient::operator=(const maGradient& i_CopyFrom)
{
  m_GradientList.clear();
  m_GradientList = i_CopyFrom.m_GradientList;
  return *this;
}
