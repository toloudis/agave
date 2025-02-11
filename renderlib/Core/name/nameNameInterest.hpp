/****************************************************************************\
**	nameNameInterest.hpp
**
**		A Name Interest is registered by a system that contains named
**	objects that can be searched for by name.
**
**
**
\****************************************************************************/

#ifdef NAME_NAMEINTEREST_HPP
#error nameNameInterest.hpp multiply included
#endif
#define NAME_NAMEINTEREST_HPP

#ifndef NAME_TYPES_HPP
#include "Core/name/nameTypes.hpp"
#endif

#include <vector>

//============================================================================
//	Forward References
//============================================================================
class nameObject;
class nameString;

//============================================================================
//============================================================================
class nameNameInterest
{
public:
  //--------------------------------------------------------------------
  //	Returns object by name.  May return NULL.
  //--------------------------------------------------------------------
  virtual nameObject* GetObjectByName(const nameString& i_String) = 0;

  //--------------------------------------------------------------------
  //	Get a list of all the names
  //--------------------------------------------------------------------
  virtual void GetNameList(nameList& io_NameList) = 0;

  //--------------------------------------------------------------------
  //	Get the name string of the object with the passed in UID.
  //--------------------------------------------------------------------
  virtual void GetNameString(nameUID i_NameUID, std::string& o_NameString) = 0;
};
