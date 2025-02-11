/*****************************************************************************
**  itPackage.hpp
**
**      itPackage contains the initialization functions for
**	the it package.  The it package's error code package index is 2.
**
**
**
\****************************************************************************/

#ifdef IT_PACKAGE_HPP
#error itPackage.hpp multiply included
#endif
#define IT_PACKAGE_HPP

//============================================================================
//============================================================================
class itPackage
{
public:
  //------------------------------------------------------------------------
  //	Init must be called before you use the it package.  A good place to
  //	do this is in your main function, before you do anything else.
  //------------------------------------------------------------------------
  static void Init();

  //------------------------------------------------------------------------
  //	CleanUp should be called after you are done with the it package.
  //	A good place to do this is in your main function, after you are done
  //	with other deinitialization and cleanup tasks.
  //------------------------------------------------------------------------
  static void CleanUp() throw();
};
