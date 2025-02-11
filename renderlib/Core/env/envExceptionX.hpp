/****************************************************************************\
**  envExceptionX.hpp
**
**      envExceptionX.hpp defines envExceptionX, the base class for all
**	Terawatt exceptions.
**
**
**
\****************************************************************************/

// uncomment this to have an exception record it's stack at the point of throw (before unwind)
//  Note: Requires symbols libraries (PDB's)
// #define ENV_EXCEPTION_STACK_TRACE

#ifdef ENV_EXCEPTIONX_HPP
#error envExceptionX.hpp multiply included
#endif
#define ENV_EXCEPTIONX_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

#include <string>

//============================================================================
// Base class for all exceptions thrown by our code
//============================================================================
class envExceptionX
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  envExceptionX();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual ~envExceptionX();

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const = 0;

  //--------------------------------------------------------------------
  // For debugging exception stacks
  //--------------------------------------------------------------------
  const char* GetCallStack() const;

private:
#ifdef ENV_EXCEPTION_STACK_TRACE
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  void RecordStackTrace();

  static const size_t cnMaxStackFrames = 32;
  unsigned __int64 nPC[cnMaxStackFrames];
#endif
};
