//****************************************************************************
//	\file dbgPackage.hpp
//
// \brief Handles the initializing and clean-up of the dbg package.
//
// dbgPackage contains the initialization functions for the dbg package.
//
//
//
//****************************************************************************
#ifdef DBG_PACKAGE_HPP
#error dbgPackage.hpp multiply included
#endif
#define DBG_PACKAGE_HPP

//============================================================================
//============================================================================
class fsLocator;

//============================================================================
// Debug Package interface
//============================================================================
class dbgPackage
{
public:
  //--------------------------------------------------------------------
  //	Init must be called before you use the dbg package.  A good place
  //	to do this is in your main function, before you do anything else.
  //
  // \return void
  //--------------------------------------------------------------------
  static void Init();

  //--------------------------------------------------------------------
  //	CleanUp should be called after you are done with the dbg package.
  //	A good place to do this is in your main function, after you are done
  //	with other deinitialization and cleanup tasks.
  //	The CleanUp function is written with the throw() exception
  //	to suggest that it should not throw any exceptions, since typically
  //	the caller is in the process of de-initializing and won't be able
  //	to do much with them.
  //
  // \return void
  //--------------------------------------------------------------------
  static void CleanUp() throw();

private:
  static int m_RefCount; //< reference counter so Inits and CleanUps match.
  static int m_OtherVar; //< something else.
};

//****************************************************************************
// \dir Dbg
//
//	the Debug Package
//
//****************************************************************************
