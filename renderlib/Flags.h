#pragma once

class Flags
{
private:
  long m_Bits;

public:
  Flags(void)
    : m_Bits(0)
  {}

  Flags(long b)
    : m_Bits(b)
  {}

  virtual ~Flags(void){};

  Flags& operator=(const Flags& Other)
  {
    m_Bits = Other.m_Bits;

    return *this;
  }

  void SetFlag(const long Flag) { m_Bits |= Flag; };

  long Get(void) { return m_Bits; };

  void ClearFlag(const long Flag) { m_Bits &= ~Flag; };

  void ClearAllFlags(void) { m_Bits = 0; };

  void SetConditional(const long Flag, const int YesNo) { YesNo ? SetFlag(Flag) : ClearFlag(Flag); };

  bool HasFlag(const long Flag) const { return (m_Bits & Flag) != 0; };

  int All(const long Flag) const { return (m_Bits & Flag) == Flag; };

  int Not(const long Flag) const { return (m_Bits & Flag) == 0; };

  void ToggleFlag(const long Flag)
  {
    if (HasFlag(Flag)) {
      ClearFlag(Flag);
    } else {
      SetFlag(Flag);
    }
  };
};
