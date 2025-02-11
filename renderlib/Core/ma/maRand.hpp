/*****************************************************************************
**  maRand.hpp
**
**	How to use:  Instantiate an maRand32 object.  Call its Rand() member, destroy
**				it when you are done.
**
**	Most of the code in this module has been pulled off the web.  Variables
**  members, and methods have been renamed to be appropriate for Terawatt.
**
**    C++ TEMPLATE VERSION OF Robert J. Jenkins Jr.'s
**    ISAAC Random Number Generator.
**    ISAAC (Indirection, Shift, Accumulate, Add, and Count)
**    Designed to be / have:
**			Deducing internal state from the results should be intractable
**			No biases should be detectable for the entire cycle length
**			Optimized for speed
**			Orderly states become disorderly quickly,
**			No short cycles
**
**    Ported from vanilla C to to template C++ class
**    by Quinn Tyler Jackson on 16-23 July 1998.
**
**        quinn@qtj.net
**
**    The function for the expected period of this
**    random number generator, according to Jenkins is:
**
**        f(a,b) = 2**((a+b*(3+2^^a)-1)
**
**        (where a is ALPHA and b is bitwidth)
**
**    So, for a bitwidth of 32 and an ALPHA of 8,
**    the expected period of ISAAC is:
**
**        2^^(8+32*(3+2^^8)-1) = 2^^8295
**
**    Jackson has been able to run implementations
**    with an ALPHA as high as 16, or
**
**        2^^2097263
**
**  For more information refer to http://burtleburtle.net/bob/rand/isaac.html
**
**	Note, there is much use in this file of single character variable names.  This
**	is short-hand carried over from the html page above and the creator's original
**	C-code.  It is unchanged here to remain faithful to the very well explained
**	documentation.
**
**
**
**
\****************************************************************************/

#ifdef MA_RAND_HPP
#error maRand.hpp multiply included
#endif
#define MA_RAND_HPP

#ifndef DBG_MSG_HPP
#include "Core/dbg/dbgMsg.hpp"
#endif
#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

#include <string.h>

//============================================================================
//============================================================================
// __ISAAC64 specifies a 64 bit random number generator is supported
// as in 64 bit platforms.  Usually this will just be 32.
// Note:  GOLDEN_RATIO is defined by Jenkins in his code.
#ifndef __ISAAC64
const envType::UInt32 GOLDEN_RATIO = envType::UInt32(0x9e3779b9);
#else  // __ISAAC64
const envType::UInt64 GOLDEN_RATIO = envType::UInt64(0x9e3779b97f4a7c13);
typedef envType::UInt64 ISAAC_INT;
#endif // __ISAAC64

//////////////////////////
// Predeclaration
template<int ALPHA = 8, class T = envType::UInt32>
class maRand;

///////////////////////////////////
// Typedef that users should use
typedef maRand<8, envType::UInt32> maRand32;

//============================================================================
//============================================================================
// unsigned 32 bit random number generator
template<int ALPHA, class T> // by default unsigned 32 bit numbers!
class maRand
{
public:
  // Members

  //------------------------------------------------------------------------
  // Construction with a seed value that will preset the accumulator.  This
  // must not be something predictable, like zero.  This will create the first
  // result set for the object.
  //------------------------------------------------------------------------
  maRand(T i_Accumulator);

  //------------------------------------------------------------------------
  // Construction with a starting set.  This is a set of numbers generated
  // by the user that will be shuffled by the system (in a rather predictable
  // fashion -- so be smart about your set creation), then used to generate
  // the first set of random results.  Note:  The accumulator will start at
  // zero, but if the starting set is sufficiently interesting and unpredictable
  // this is a non-issue.
  //		i_StartingSet is a (1<<ALPHA) sized array of T values.  It will be used
  //				to calculate the first resulting set.  It is the starting point from
  //				which all the ISAAC begins.  It may be NULL.
  //------------------------------------------------------------------------
  maRand(const T i_pStartingSet[1 << ALPHA]);

  //------------------------------------------------------------------------
  // Destruction -- clean up
  //------------------------------------------------------------------------
  ~maRand();

  //------------------------------------------------------------------------
  // Rand returns the next random number in the pseudorandom sequence
  // When all values in the currently allocated sequence have been exhausted
  // a new sequence is generated at ~19 machine instructions per 32-bit number.
  //------------------------------------------------------------------------
  inline T Rand();

  //------------------------------------------------------------------------
  // The maximum value returned by Rand
  //------------------------------------------------------------------------
  static inline T RandMax();

  //------------------------------------------------------------------------
  // Returns a random number between the given min and given max.
  // Note:  This is not exactly high performing, but will work accurately for
  //		all given min and max values where max > min ALWAYS.
  //------------------------------------------------------------------------
  inline T Rand(T i_Min, T i_Max);

private:
  // Methods

  //------------------------------------------------------------------------
  // Initialize the object based on the Jenkins randinit function from his
  // original code.
  //------------------------------------------------------------------------
  void Init();

  //------------------------------------------------------------------------
  // Isaac performs the Isaac algorithm to generate the pseudorandom number
  // sequence.
  //------------------------------------------------------------------------
  void Isaac();

  //------------------------------------------------------------------------
  // Shuffles the given numbers with a series of XORs, shifts and additions
  // also part of the ISAAC computation.
  //------------------------------------------------------------------------
  void Shuffle(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h);

  // Members

  ///////////////////////////////////////////////////////////////////////
  //  The value of SIZE, the size of our memory and results arrays
  // defined as 2^^ALPHA or 1<<ALPHA
  const T m_SIZE;      // = 1<<ALPHA
  const T m_HALF_SIZE; // = m_SIZE / 2 for Isaac to improve performance

  //////////////////////////////////////////////////////////////////
  // ResultCount is the count of remaining results in the result sequence
  // This is decremented whenever a random number is removed from the sequence
  // by the user.  When it reaches zero, Isaac is called and a new sequence
  // is generated.
  T m_ResultCount; // ;  not explicitly part of Isaac

  /////////////////////////////////////////////////////////////////
  // pResults is the random number sequence generated on the last
  // call to Isaac
  T* m_pResults; // randrsl; or rr

  /////////////////////////////////////////////////////////////////
  // pMemory is the memory block used to create the random number sequence.
  // Memory:  array of SIZE ALPHA-bit terms.
  T* m_pMemory; // randmem; or mm

  ////////////////////////////////////////////////////////////////////
  // Accumulator is a single bvalue used in the calculation and recalculation
  // of the random number sequence.  A member here to avoid over-allocation
  T m_Accumulator; // randa; or aa

  ////////////////////////////////////////////////////////////////////
  // The previous pseudorandom number generated for the sequence during
  // generation in Isaac.  A member her to avoid over-allocation.
  T m_PreviousResult; // randb; or bb

  ////////////////////////////////////////////////////////////////////
  // A counter, counting each call to Isaac.  Used to mix up the random
  // number generation on each call even more.  one ALPHA-bit value
  T m_Counter; // m_Counter;  or cc
};
// INLINES

//------------------------------------------------------------------------
// Construction with a seed value that will preset the accumulator.  This
// must not be something predictable, like zero.  This will create the first
// result set for the object.
//------------------------------------------------------------------------
template<int ALPHA, class T>
maRand<ALPHA, T>::maRand(T i_Accumulator)
  : m_SIZE(1 << ALPHA)
  , m_HALF_SIZE(m_SIZE / 2)
  , m_pResults(new T[m_SIZE])
  , m_pMemory(new T[m_SIZE])
  , m_Accumulator(i_Accumulator)
  , m_PreviousResult(0)
  , m_Counter(0)
{
  memset(m_pResults, 0, sizeof(T) * m_SIZE);
  memset(m_pMemory, 0, sizeof(T) * m_SIZE);

  // Finish initializing and create initial result set
  Init();
}

//------------------------------------------------------------------------
// Construction with a starting set.  This is a set of numbers generated
// by the user that will be shuffled by the system (in a rather predictable
// fashion -- so be smart about your set creation), then used to generate
// the first set of random results.  Note:  The accumulator will start at
// zero, but if the starting set is sufficiently interesting and unpredictable
// this is a non-issue.
//		i_StartingSet is a (1<<ALPHA) sized array of T values.  It will be used
//				to calculate the first resulting set.  It is the starting point from
//				which all the ISAAC begins.  It may be NULL.
//------------------------------------------------------------------------
template<int ALPHA, class T>
maRand<ALPHA, T>::maRand(const T i_pStartingSet[1 << ALPHA])
  : m_SIZE(1 << ALPHA)
  , m_HALF_SIZE(m_SIZE / 2)
  , m_pResults(new T[m_SIZE])
  , m_pMemory(new T[m_SIZE])
  , m_Accumulator()
  , m_PreviousResult(0)
  , m_Counter(0)
{
  // copy from input
  memcpy(m_pResults, i_pStartingSet, m_SIZE);

  memset(m_pMemory, 0, sizeof(T) * m_SIZE);

  // Finish initializing and create initial result set
  Init();
}

//------------------------------------------------------------------------
// Destruction -- clean up
//------------------------------------------------------------------------
template<int ALPHA, class T>
maRand<ALPHA, T>::~maRand()
{
  // clean up from constructor
  delete[] m_pResults;
  delete[] m_pMemory;
}

//------------------------------------------------------------------------
// Rand returns the next random number in the pseudorandom sequence
// When all values in the currently allocated sequence have been exhausted
// a new sequence is generated at ~19 machine instructions per 32-bit number.
//------------------------------------------------------------------------
template<int ALPHA, class T>
inline T
maRand<ALPHA, T>::Rand()
{
  if (0 == m_ResultCount--) {
    // Create a new results list
    Isaac();
    // m_ResultCount is initialized in Isaac based on the creation of the
    // results set, we decrement it here because we return one result below
    --m_ResultCount;
  }

  return m_pResults[m_ResultCount];
}

//------------------------------------------------------------------------
// The maximum value returned by Rand
//------------------------------------------------------------------------
// static
template<int ALPHA, class T>
inline T
maRand<ALPHA, T>::RandMax()
{
  return (T)(-1);
}

//------------------------------------------------------------------------
// Returns a random number between the given min and given max.
// Note:  This is not exactly high performing, but will work accurately for
//		all given min and max values where max > min ALWAYS.
//------------------------------------------------------------------------
template<int ALPHA, class T>
inline T
maRand<ALPHA, T>::Rand(T i_Min, T i_Max)
{
  DBG_ASSERT(i_Max >= i_Min, "maRand: Max must be greater than min!!");

  T ret = 0;

  // if the max-min range is the same as what type T can handle we have to
  // special case because te formula below will fail.
  if (i_Max - i_Min == RandMax()) {
    return (i_Min + Rand());
  }

  // works for every min-max range except RandMax<T>()
  ret = i_Min + (T)((double)Rand() * (i_Max - i_Min + 1) / RandMax());

  return ret;
}

//------------------------------------------------------------------------
// Initialize the object based on the Jenkins randinit function from his
// original code.
//------------------------------------------------------------------------
template<int ALPHA, class T>
void
maRand<ALPHA, T>::Init()
{
  ////////////////////////////////////////////////
  // Initialize the result set for initial Isaac
  T a, b, c, d, e, f, g, h;
  a = b = c = d = e = f = g = h = GOLDEN_RATIO;

  /////////////////////////////////////////////////////////////
  // Note:  This code is taken directly from Jenkins's original
  // code in his randinit function.  The names have been changed
  // to protect the innocent.
  envType::UInt32 i;
  for (i = 0; i < 4; ++i) {
    Shuffle(a, b, c, d, e, f, g, h);
  }

  // Now essentially take eight numbers at a time, a..h from the results set
  // and shuffle those numbers (See Shuffle, or the mix macro in Jenkins.)
  // There is bonus addition to help scramble the numbers more effectively
  // The result is that m_pMemory (used in Isaac later) will be sufficiently
  // scrambled to make a good starting set for the random number generation.

  // first pass, m_pResults to m_pMemory
  for (i = 0; i < m_SIZE; i += 8) {
    a += m_pResults[i];
    b += m_pResults[i + 1];
    c += m_pResults[i + 2];
    d += m_pResults[i + 3];
    e += m_pResults[i + 4];
    f += m_pResults[i + 5];
    g += m_pResults[i + 6];
    h += m_pResults[i + 7];

    Shuffle(a, b, c, d, e, f, g, h);

    m_pMemory[i] = a;
    m_pMemory[i + 1] = b;
    m_pMemory[i + 2] = c;
    m_pMemory[i + 3] = d;
    m_pMemory[i + 4] = e;
    m_pMemory[i + 5] = f;
    m_pMemory[i + 6] = g;
    m_pMemory[i + 7] = h;
  }

  // do a second pass to make all of the seed affect all of m_pMemory
  // second pass, m_pMemory to m_pMemory
  for (i = 0; i < m_SIZE; i += 8) {
    a += m_pMemory[i];
    b += m_pMemory[i + 1];
    c += m_pMemory[i + 2];
    d += m_pMemory[i + 3];
    e += m_pMemory[i + 4];
    f += m_pMemory[i + 5];
    g += m_pMemory[i + 6];
    h += m_pMemory[i + 7];

    Shuffle(a, b, c, d, e, f, g, h);

    m_pMemory[i] = a;
    m_pMemory[i + 1] = b;
    m_pMemory[i + 2] = c;
    m_pMemory[i + 3] = d;
    m_pMemory[i + 4] = e;
    m_pMemory[i + 5] = f;
    m_pMemory[i + 6] = g;
    m_pMemory[i + 7] = h;
  }

  // Perform initial Isaac computation and create
  // first results set
  Isaac();
}

//------------------------------------------------------------------------
// Isaac performs the Isaac algorithm to generate the pseudorandom number
// sequence.
//------------------------------------------------------------------------
template<int ALPHA, class T>
void
maRand<ALPHA, T>::Isaac()
{
  // use register variables, as this call is made fequently during operation

  // x, y -- intermediate variables for indirection calculation
  T x, y;
  envType::UInt32 i;

  // Jenkins:  [Counter] "just gets incremented once per 256 results then combined with" previous result.
  m_PreviousResult += (++m_Counter);

  // Algorithm from Jenkins "readable" code, our variable names

  for (i = 0; i < m_SIZE; ++i) {
    x = m_pMemory[i];

#ifndef __ISAAC64
    switch (i % 4) // 4 different XOR operations
    {
      case 0:
        m_Accumulator = m_Accumulator ^ (m_Accumulator << 13);
        break;
      case 1:
        m_Accumulator = m_Accumulator ^ (m_Accumulator >> 6);
        break;
      case 2:
        m_Accumulator = m_Accumulator ^ (m_Accumulator << 2);
        break;
      case 3:
        m_Accumulator = m_Accumulator ^ (m_Accumulator >> 16);
        break;
    }
    // Note:  2 is a number from Jenkins, ALPHA+2 used to be ten for ALPHA = 8.
    m_Accumulator = m_pMemory[(i + (m_HALF_SIZE)) % m_SIZE] + m_Accumulator;
    m_pMemory[i] = y = m_pMemory[(x >> 2) % m_SIZE] + m_Accumulator + m_PreviousResult;
    m_pResults[i] = m_PreviousResult = m_pMemory[(y >> (ALPHA + 2)) % m_SIZE] + x;

    // Jenkins quote:
    /* Note that bits 2..9 are chosen from x but 10..17 are chosen
    from y.  The only important thing here is that 2..9 and 10..17
    don't overlap.  2..9 and 10..17 were then chosen for speed in
    the optimized version (rand.c) */
    /* See http://burtleburtle.net/bob/rand/isaac.html
    for further explanations and analysis. */

#else // __ISAAC64
    switch (i % 4) // 4 different XOR operations
    {
      case 0:
        m_Accumulator = ~(m_Accumulator ^ (m_Accumulator << 21));
        break;
      case 1:
        m_Accumulator = m_Accumulator ^ (m_Accumulator >> 5);
        break;
      case 2:
        m_Accumulator = m_Accumulator ^ (m_Accumulator << 12);
        break;
      case 3:
        m_Accumulator = m_Accumulator ^ (m_Accumulator >> 33);
        break;
    }
    m_Accumulator = m_pMemory[(i + (m_HALF_SIZE)) % m_SIZE] + m_Accumulator;
#error
    /*
    #error Don't know yet how to convert old indirection to new concise 64 bit version
    #error used to be ind(x) { return (*(T*)((envType::UInt8*)(m_pMemory) + ((x) & ((m_SIZE-1)<<3)))); }
    #error These y>>n & m_SIZE parts need to be replaced with someting else?
    */
    m_pMemory[i] = y = m_pMemory[(x >> 2) % m_SIZE] + m_Accumulator + m_PreviousResult;
    m_pResults[i] = m_PreviousResult = m_pMemory[(y >> 10) % m_SIZE] + x;
#endif // __ISAAC64
  }

  m_ResultCount = m_SIZE;
}

//------------------------------------------------------------------------
// Shuffles the given numbers with a series of XORs, shifts and additions
// also part of the ISAAC computation.
//------------------------------------------------------------------------
template<int ALPHA, class T>
void
maRand<ALPHA, T>::Shuffle(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h)
{
#ifndef __ISAAC64
  // Note:  This is identical to Jenkins's mix macro
  a ^= b << 11;
  d += a;
  b += c;
  b ^= c >> 2;
  e += b;
  c += d;
  c ^= d << 8;
  f += c;
  d += e;
  d ^= e >> 16;
  g += d;
  e += f;
  e ^= f << 10;
  h += e;
  f += g;
  f ^= g >> 4;
  a += f;
  g += h;
  g ^= h << 8;
  b += g;
  h += a;
  h ^= a >> 9;
  c += h;
  a += b;
#else  // __ISAAC64
       // TODO:  Where does this come from?
  a -= e;
  f ^= h >> 9;
  h += a;
  b -= f;
  g ^= a << 9;
  a += b;
  c -= g;
  h ^= b >> 23;
  b += c;
  d -= h;
  a ^= c << 15;
  c += d;
  e -= a;
  b ^= d >> 14;
  d += e;
  f -= b;
  c ^= e << 20;
  e += f;
  g -= c;
  d ^= f >> 17;
  f += g;
  h -= d;
  e ^= g << 14;
  g += h;
#endif // __ISAAC64
}
