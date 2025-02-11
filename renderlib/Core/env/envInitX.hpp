/****************************************************************************\
**  envInitX.hpp
**
**      envInitX.hpp defines envInitX, the exception thrown when a package
**	cannot initialize and must signal failure.
**
**
**
\****************************************************************************/
#ifdef ENV_INITX_HPP
#error envInitX.hpp multiply included
#endif
#define ENV_INITX_HPP

#include <string>

#ifndef ENV_EXCEPTIONX_HPP
#include "Core/env/envExceptionX.hpp"
#endif

//============================================================================
//============================================================================
class envInitX : public envExceptionX
{
public:
  //------------------------------------------------------------------------
  //	The constructor argument is the name of the package that is reporting
  //	a failure to initialize.
  //------------------------------------------------------------------------
  envInitX(const char* i_Package);

  //--------------------------------------------------------------------
  // Returns error message string
  //--------------------------------------------------------------------
  virtual std::string GetErrorMessage() const;

private:
  std::string m_Package;
};
