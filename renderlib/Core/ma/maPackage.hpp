/*****************************************************************************
**  maPackage.hpp
**
**      Math contains the initialization functions
**	for the Math package.
**
**  The Ma package's package error index is 8.
**
**
**
\****************************************************************************/

#ifdef MA_PACKAGE_HPP
#error maPackage.hpp multiply included
#endif
#define MA_PACKAGE_HPP

//============================================================================
//============================================================================
class maPackage
{
public:
  //------------------------------------------------------------------------
  //	Init must be called before you use this package.  A good place to
  //	do this is in your main function, with your other package initializers.
  //------------------------------------------------------------------------
  static void Init();

  //------------------------------------------------------------------------
  //	CleanUp should be called after you are done with this package.
  //	A good place to do this is in your main function, after you are done
  //	with other deinitialization and cleanup tasks.
  //------------------------------------------------------------------------
  static void CleanUp();
};
