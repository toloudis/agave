/****************************************************************************\
**	prtyIntCallbackMgr.hpp
**
**		Property callback manager.  It handles to routing of callbacks from
**	an unmanaged property with a managed control.
**
**
**
\****************************************************************************/
#ifdef PRTY_INTCALLBACKMGR_HPP
#error prtyIntCallbackMgr.hpp multiply included
#endif
#define PRTY_INTCALLBACKMGR_HPP

#ifndef ENV_ABSTRACTION_HPP
#include "Core/env/envAbstraction.hpp"
#endif

//============================================================================
// forward declaration
//============================================================================
class prtyProperty;
class prtyPropertyUIInfo;
class prtyIntCallbackMgrImpl;

//============================================================================
// static functions define API
//============================================================================
class prtyIntCallbackMgr : public envAbstraction<prtyIntCallbackMgrImpl>
{
public:
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  static void PropertyChanged(prtyProperty* i_pProperty, int i_Index);

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  static void UpdateControl(prtyPropertyUIInfo* i_pUII);
};

//============================================================================
// Implementation class, needs to be derived in implementation library
//============================================================================
class prtyIntCallbackMgrImpl
{
public:
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  virtual void PropertyChanged(prtyProperty* i_pProperty, int i_Index) = 0;

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  virtual void UpdateControl(prtyPropertyUIInfo* i_pUII) = 0;
};
