/****************************************************************************\
**  maGradient.hpp
**
**      maGradient.hpp defines a 1d gradient color class.
**
**
**
\****************************************************************************/

#ifdef MA_GRADIENT_HPP
#error maGradient.hpp multiply included
#endif
#define MA_GRADIENT_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

#ifndef MA_FLOATRGBA_HPP
#include "Core/Ma/maFloatRGBA.hpp"
#endif

#ifndef DBG_MSG_HPP
#include "Core/dbg/dbgMsg.hpp"
#endif

#include <vector>

//============================================================================
//============================================================================
class maGradient
{
public:
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  typedef std::pair<float, maFloatRGBA> GradientNodePair;
  typedef std::vector<GradientNodePair> Value;

  //------------------------------------------------------------------------
  //	This constructor does no initialization
  //------------------------------------------------------------------------
  maGradient();

  //------------------------------------------------------------------------
  //	Copy constructor
  //------------------------------------------------------------------------
  maGradient(const maGradient& i_CopyFrom);

  //------------------------------------------------------------------------
  //	Get the number of node in maGradient
  //------------------------------------------------------------------------
  int GetSize() const;

  //------------------------------------------------------------------------
  //	Clear list
  //------------------------------------------------------------------------
  void Clear();

  //------------------------------------------------------------------------
  //	Add one node at the end of the list
  //------------------------------------------------------------------------
  void AddNode(float i_pos, const maFloatRGBA& i_color);

  //------------------------------------------------------------------------
  //	operator [] returns the CharType at i_Index.
  //------------------------------------------------------------------------
  GradientNodePair& operator[](int i_index);
  GradientNodePair operator[](int i_index) const;

  //------------------------------------------------------------------------
  //	Assignment
  //------------------------------------------------------------------------
  const maGradient& operator=(const maGradient& i_CopyFrom);

  //------------------------------------------------------------------------
  //	Equality
  //------------------------------------------------------------------------
  bool operator==(const maGradient& i_Gradient) const;
  bool operator!=(const maGradient& i_Gradient) const;

public:
  Value m_GradientList;
};

//------------------------------------------------------------------------
//	operator [] returns the GradientNodePair at i_index.
//------------------------------------------------------------------------
inline maGradient::GradientNodePair&
maGradient::operator[](int i_index)
{
  DBG_ASSERT(0 <= i_index && i_index < (int)m_GradientList.size(), "Invalid index #" << i_index);

  return m_GradientList[i_index];
}

inline maGradient::GradientNodePair
maGradient::operator[](int i_index) const
{
  DBG_ASSERT(0 <= i_index && i_index < (int)m_GradientList.size(), "Invalid index #" << i_index);

  return m_GradientList[i_index];
}