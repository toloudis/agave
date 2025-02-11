/*****************************************************************************
**  chExceptionX.hpp
**
**      chExceptionX defines the exceptions which can be thrown by the
**	ch package.
**
**
**
\****************************************************************************/

#ifdef CH_EXCEPTIONX_HPP
#error chExceptionX.hpp multiply included
#endif
#define CH_EXCEPTIONX_HPP

#ifndef ENV_EXCEPTIONX_HPP
#include "Core/env/envExceptionX.hpp"
#endif

//============================================================================
//============================================================================
class chInvalidChunkX : public envExceptionX
{
public:
  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;
};
