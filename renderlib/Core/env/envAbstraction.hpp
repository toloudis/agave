/****************************************************************************\
**  envAbstraction.hpp
**
**      envAbstraction.hpp defines template for the pattern where
**	generic functions are defined in a static class, but implemented
**	in derived classes that are defined in separate libraries.
**
**
**
\****************************************************************************/
#ifdef ENV_ABSTRACTION_HPP
#error envAbstraction.hpp multiply included
#endif
#define ENV_ABSTRACTION_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//================================================================================
// Derive a class from this class in order to automatically get the
//	implementation pointer and function to set it.
//================================================================================
template<class ImplClass>
class envAbstraction
{
public:
  //--------------------------------------------------------------------
  // Set new implementation method, returns pointer to last one
  // that was being used.  Both can be NULL.
  // Ownership for the pointer remains with the caller.
  //--------------------------------------------------------------------
  static ImplClass* SetImplementation(ImplClass* i_pImpl)
  {
    ImplClass* old_impl = sm_pImplementation;
    sm_pImplementation = i_pImpl;
    return old_impl;
  }

protected:
  static ImplClass* sm_pImplementation;
};

//============================================================================
// Initialiazing static members
//============================================================================
template<class ImplClass>
ImplClass* envAbstraction<ImplClass>::sm_pImplementation = NULL;
