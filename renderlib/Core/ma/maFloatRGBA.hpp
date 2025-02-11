/****************************************************************************\
**  maFloatRGBA.hpp
**
**      maFloatRGBA.hpp is a primitive color class.
**
**
**
\****************************************************************************/

#ifdef MA_FLOATRGBA_HPP
#error maFloatRGBA.hpp multiply included
#endif
#define MA_FLOATRGBA_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

//============================================================================
//============================================================================
class maFloatRGBA
{
public:
  typedef float Value;

  //------------------------------------------------------------------------
  //	This constructor does no initialization
  //------------------------------------------------------------------------
  maFloatRGBA();

  //------------------------------------------------------------------------
  //	Copy constructor
  //------------------------------------------------------------------------
  maFloatRGBA(const maFloatRGBA& i_CopyFrom);

  //------------------------------------------------------------------------
  //	This constructor initializes to the given values
  //------------------------------------------------------------------------
  maFloatRGBA(Value i_Red, Value i_Green, Value i_Blue, Value i_Alpha);

  //------------------------------------------------------------------------
  //	Set sets each color value
  //------------------------------------------------------------------------
  void Set(Value i_Red, Value i_Green, Value i_Blue, Value i_Alpha);

  //------------------------------------------------------------------------
  //	These get functions return each color value
  //------------------------------------------------------------------------
  Value GetRed() const;
  Value GetGreen() const;
  Value GetBlue() const;
  Value GetAlpha() const;

  //------------------------------------------------------------------------
  //	These set functions set each color value individually.
  //------------------------------------------------------------------------
  void SetRed(Value i_Red);
  void SetGreen(Value i_Green);
  void SetBlue(Value i_Blue);
  void SetAlpha(Value i_Alpha);

  //------------------------------------------------------------------------
  //	Assignment
  //------------------------------------------------------------------------
  const maFloatRGBA& operator=(const maFloatRGBA& i_CopyFrom);

  //------------------------------------------------------------------------
  //	Equality
  //------------------------------------------------------------------------
  bool operator==(const maFloatRGBA& i_Color) const;
  bool operator!=(const maFloatRGBA& i_Color) const;

  //------------------------------------------------------------------------
  //	Addition
  //------------------------------------------------------------------------
  maFloatRGBA operator+(const maFloatRGBA& i_Color) const;

  //------------------------------------------------------------------------
  //	Subtraction
  //------------------------------------------------------------------------
  maFloatRGBA operator-(const maFloatRGBA& i_Color) const;

  //------------------------------------------------------------------------
  //	Incremental Addition
  //------------------------------------------------------------------------
  void operator+=(const maFloatRGBA& i_Color);

  //------------------------------------------------------------------------
  //	Incremental Subtraction
  //------------------------------------------------------------------------
  void operator-=(const maFloatRGBA& i_Color);

  //------------------------------------------------------------------------
  //	(scalar) Incremental Multiplication
  //------------------------------------------------------------------------
  void operator*=(float i_Factor);

  //------------------------------------------------------------------------
  //	(scalar) Incremental Division
  //------------------------------------------------------------------------
  void operator/=(float i_Factor);

  //------------------------------------------------------------------------
  //	component by component multiplication
  //------------------------------------------------------------------------
  inline void operator*=(const maFloatRGBA& i_ScaleBy);

  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  inline float* Ptr() const;
  inline const float* GetPtr() const;

public:
  Value m_Red, m_Green, m_Blue, m_Alpha;
};

//------------------------------------------------------------------------
//	free operator functions
//------------------------------------------------------------------------
//	(scalar) Multiplication
//------------------------------------------------------------------------
inline maFloatRGBA
operator*(float i_Factor, const maFloatRGBA& i_Color)
{
  return maFloatRGBA(i_Color.GetRed() * i_Factor,
                     i_Color.GetGreen() * i_Factor,
                     i_Color.GetBlue() * i_Factor,
                     i_Color.GetAlpha() * i_Factor);
}

inline maFloatRGBA
operator*(const maFloatRGBA& i_Color, float i_Factor)
{
  return maFloatRGBA(i_Color.GetRed() * i_Factor,
                     i_Color.GetGreen() * i_Factor,
                     i_Color.GetBlue() * i_Factor,
                     i_Color.GetAlpha() * i_Factor);
}

//------------------------------------------------------------------------
//	(scalar) Division
//------------------------------------------------------------------------
inline maFloatRGBA
operator/(const maFloatRGBA& i_Color, float i_Factor)
{
  return maFloatRGBA(i_Color.GetRed() * i_Factor,
                     i_Color.GetGreen() * i_Factor,
                     i_Color.GetBlue() * i_Factor,
                     i_Color.GetAlpha() * i_Factor);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
inline maFloatRGBA::maFloatRGBA()
  : m_Red(0)
  , m_Green(0)
  , m_Blue(0)
  , m_Alpha(0)
{
}

//--------------------------------------------------------------------------------
//	Copy constructor
//--------------------------------------------------------------------------------
inline maFloatRGBA::maFloatRGBA(const maFloatRGBA& i_CopyFrom)
  : m_Red(i_CopyFrom.m_Red)
  , m_Green(i_CopyFrom.m_Green)
  , m_Blue(i_CopyFrom.m_Blue)
  , m_Alpha(i_CopyFrom.m_Alpha)
{
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
inline maFloatRGBA::maFloatRGBA(Value i_Red, Value i_Green, Value i_Blue, Value i_Alpha)
  : m_Red(i_Red)
  , m_Green(i_Green)
  , m_Blue(i_Blue)
  , m_Alpha(i_Alpha)
{
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline float*
maFloatRGBA::Ptr() const
{
  // look out! this amounts to a const_cast.
  // oh well, we know what we're doing, right?
  return (float*)&m_Red;
}

inline const float*
maFloatRGBA::GetPtr() const
{
  return &m_Red;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
maFloatRGBA::Set(Value i_Red, Value i_Green, Value i_Blue, Value i_Alpha)
{
  m_Red = i_Red;
  m_Green = i_Green;
  m_Blue = i_Blue;
  m_Alpha = i_Alpha;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline maFloatRGBA::Value
maFloatRGBA::GetRed() const
{
  return m_Red;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline maFloatRGBA::Value
maFloatRGBA::GetGreen() const
{
  return m_Green;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline maFloatRGBA::Value
maFloatRGBA::GetBlue() const
{
  return m_Blue;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline maFloatRGBA::Value
maFloatRGBA::GetAlpha() const
{
  return m_Alpha;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
inline void
maFloatRGBA::SetRed(Value i_Red)
{
  m_Red = i_Red;
}

inline void
maFloatRGBA::SetGreen(Value i_Green)
{
  m_Green = i_Green;
}

inline void
maFloatRGBA::SetBlue(Value i_Blue)
{
  m_Blue = i_Blue;
}

inline void
maFloatRGBA::SetAlpha(Value i_Alpha)
{
  m_Alpha = i_Alpha;
}

//------------------------------------------------------------------------
//	Equality
//------------------------------------------------------------------------
inline bool
maFloatRGBA::operator==(const maFloatRGBA& i_Color) const
{
  return (m_Red == i_Color.m_Red) && (m_Green == i_Color.m_Green) && (m_Blue == i_Color.m_Blue) &&
         (m_Alpha == i_Color.m_Alpha);
}
inline bool
maFloatRGBA::operator!=(const maFloatRGBA& i_Color) const
{
  return (!(*this == i_Color));
}

//------------------------------------------------------------------------
//	Assignment
//------------------------------------------------------------------------
inline const maFloatRGBA&
maFloatRGBA::operator=(const maFloatRGBA& i_CopyFrom)
{
  // Check for self-assignment
  if (&i_CopyFrom != this) {
    m_Red = i_CopyFrom.m_Red;
    m_Green = i_CopyFrom.m_Green;
    m_Blue = i_CopyFrom.m_Blue;
    m_Alpha = i_CopyFrom.m_Alpha;
  }

  return *this;
}

//------------------------------------------------------------------------
//	Addition
//------------------------------------------------------------------------
inline maFloatRGBA
maFloatRGBA::operator+(const maFloatRGBA& i_Color) const
{
  return maFloatRGBA(
    m_Red + i_Color.m_Red, m_Green + i_Color.m_Green, m_Blue + i_Color.m_Blue, m_Alpha + i_Color.m_Alpha);
}

//------------------------------------------------------------------------
//	Subtraction
//------------------------------------------------------------------------
inline maFloatRGBA
maFloatRGBA::operator-(const maFloatRGBA& i_Color) const
{
  return maFloatRGBA(
    m_Red - i_Color.m_Red, m_Green - i_Color.m_Green, m_Blue - i_Color.m_Blue, m_Alpha - i_Color.m_Alpha);
}

//------------------------------------------------------------------------
//	Incremental Addition
//------------------------------------------------------------------------
inline void
maFloatRGBA::operator+=(const maFloatRGBA& i_Color)
{
  m_Red += i_Color.m_Red;
  m_Green += i_Color.m_Green;
  m_Blue += i_Color.m_Blue;
  m_Alpha += i_Color.m_Alpha;
}

//------------------------------------------------------------------------
//	Incremental Subtraction
//------------------------------------------------------------------------
inline void
maFloatRGBA::operator-=(const maFloatRGBA& i_Color)
{
  m_Red -= i_Color.m_Red;
  m_Green -= i_Color.m_Green;
  m_Blue -= i_Color.m_Blue;
  m_Alpha -= i_Color.m_Alpha;
}

//------------------------------------------------------------------------
//	(scalar) Incremental Multiplication
//------------------------------------------------------------------------
inline void
maFloatRGBA::operator*=(float i_Factor)
{
  m_Red *= i_Factor;
  m_Green *= i_Factor;
  m_Blue *= i_Factor;
  m_Alpha *= i_Factor;
}

//------------------------------------------------------------------------
//	component by component multiplication
//------------------------------------------------------------------------
inline void
maFloatRGBA::operator*=(const maFloatRGBA& i_ScaleBy)
{
  m_Red *= i_ScaleBy.m_Red;
  m_Green *= i_ScaleBy.m_Green;
  m_Blue *= i_ScaleBy.m_Blue;
  m_Alpha *= i_ScaleBy.m_Alpha;
}

//------------------------------------------------------------------------
//	(scalar) Incremental Division
//------------------------------------------------------------------------
inline void
maFloatRGBA::operator/=(float i_Factor)
{
  m_Red /= i_Factor;
  m_Green /= i_Factor;
  m_Blue /= i_Factor;
  m_Alpha /= i_Factor;
}
