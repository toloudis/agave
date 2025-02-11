/*****************************************************************************\
**	nameMgr.hpp
**
**		Provides method for looking up a named object throughout all systems
**
**
**
\****************************************************************************/

#ifdef NAME_MGR_HPP
#error nameMgr.hpp multiply included
#endif
#define NAME_MGR_HPP

#ifndef NAME_TYPES_HPP
#include "Core/name/nameTypes.hpp"
#endif

#include <functional>
#include <vector>

//============================================================================
//	Forward References
//============================================================================
class nameString;
class nameObject;
class nameNameInterest;

//============================================================================
//============================================================================
namespace nameMgr {
//--------------------------------------------------------------------
//	Returns object that matches by Name UID.  If the Name UID is
//	invalid then a string search is performed.	 May return NULL.
//--------------------------------------------------------------------
nameObject*
GetObjectByName(const nameString& i_String);

//--------------------------------------------------------------------
//	Find first matching name and return its UID
//	Note: this only returns the FIRST occurrence.  beware.
//	this should only be used under controlled situations...if at all.
//--------------------------------------------------------------------
nameUID
GetNameUIDFromString(const nameString& i_String);

//--------------------------------------------------------------------
//	Get a list of all the names
//--------------------------------------------------------------------
void
GetNameList(nameList& io_NameList);

//--------------------------------------------------------------------
//	RegisterNameInterest() - add a Name interest to the system
//--------------------------------------------------------------------
void
RegisterNameInterest(nameNameInterest* i_pInterest);

//--------------------------------------------------------------------
//	UnRegisterNameInterest() - remove a Name interest from the system.
//
//	Note: this will NOT delete the Name interest.  It is up to the
//	registerer.
//--------------------------------------------------------------------
void
UnRegisterNameInterest(nameNameInterest* i_pInterest);

//--------------------------------------------------------------------
//	Get the name string of the object with the passed in UID.
//--------------------------------------------------------------------
void
GetNameString(nameUID i_NameUID, std::string& o_NameString);

//--------------------------------------------------------------------
//	RegisterName() - register the string with the NameMgr.
//
//		the input is the UID and the string.
//		if the UID is invalid (-1) then it will be assigned a UID.
//--------------------------------------------------------------------
void
RegisterName(nameString& io_NameString);

//--------------------------------------------------------------------
//	ResetUID() - whenever you want to reset the UID counter.
//--------------------------------------------------------------------
void
ResetUID();

//--------------------------------------------------------------------
//	Create Default Name For an Object
//--------------------------------------------------------------------
typedef std::function<bool(char*)> VerifyNoDupNameFunc;
void
CreateDefaultName(const nameString& i_Filename,
                  const VerifyNoDupNameFunc& i_VerifyFunc,
                  nameString& o_NameString,
                  int& io_ObjectCounter);

//--------------------------------------------------------------------
//	Create Duplicated Name For an Object
//--------------------------------------------------------------------
void
CreateDuplicateName(const nameString& i_Filename, const VerifyNoDupNameFunc& i_VerifyFunc, nameString& o_NameString);
};
