/*****************************************************************************
**  fsPackage.hpp
**
**      fsPackage contains the initialization functions
**	for the fs package.
**
**
**
\****************************************************************************/

#ifdef FS_PACKAGE_HPP
#error fsPackage.hpp multiply included
#endif
#define FS_PACKAGE_HPP

//============================================================================
//============================================================================
class fsPackage
{
public:
  //------------------------------------------------------------------------
  //	Init must be called before you use the fs package.  A good place to
  //	do this is in your main function, with your other package initializers.
  //------------------------------------------------------------------------
  static void Init();

  //------------------------------------------------------------------------
  //	CleanUp should be called after you are done with the fs package.
  //	A good place to do this is in your main function, after you are done
  //	with other deinitialization and cleanup tasks.
  //------------------------------------------------------------------------
  static void CleanUp() throw();
};
