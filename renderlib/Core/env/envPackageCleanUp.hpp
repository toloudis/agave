/*****************************************************************************
**  envPackageCleanUp.hpp
**
**      envPackageCleanUp is a resource acquisition class which cleans up
**	a package when it is destroyed.
**
**
**
\****************************************************************************/

#ifdef ENV_PACKAGECLEANUP_HPP
#error envPackageCleanUp.hpp multiple included
#endif
#define ENV_PACKAGECLEANUP_HPP

//============================================================================
//============================================================================
template<class T>
class envPackageCleanUp
{
public:
  //------------------------------------------------------------------------
  //	If i_bDoInitialize is true, the envPackageCleanUp class initializes
  //	the package in the constructor; otherwise the constructor does
  //	nothing
  //------------------------------------------------------------------------
  envPackageCleanUp(bool i_bDoInitialize = true)
  {
    if (i_bDoInitialize)
      T::Init();
  }

  //------------------------------------------------------------------------
  //	The destructor calls the package's CleanUp function
  //------------------------------------------------------------------------
  ~envPackageCleanUp() throw() { T::CleanUp(); }
};
