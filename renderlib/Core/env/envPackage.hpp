/**********************************************************
**  envPackage.hpp
**
**      envPackage contains the initialization functions
**	for the env package.
**
**
**
\*********************************************************/
#ifdef ENV_PACKAGE_HPP
#error envPackage.hpp multiple included
#endif
#define ENV_PACKAGE_HPP

//============================================================================
//============================================================================
class envPackage
{
public:
  //------------------------------------------------------------------------
  //	Init must be called before you use the env package.  A good place to
  //	do this is in your main function, before you do anything else.
  //------------------------------------------------------------------------
  static void Init();

  //------------------------------------------------------------------------
  //	CleanUp should be called after you are done with the env package.
  //	A good place to do this is in your main function, after you are done
  //	with other deinitialization and cleanup tasks.
  //	The CleanUp function is written with the throw() exception
  //	to suggest that it should not throw any exceptions, since typically
  //	the caller is in the process of de-initializing and won't be able
  //	to do much with them.
  //------------------------------------------------------------------------
  static void CleanUp() throw();
};
