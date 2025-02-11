/****************************************************************************\
**	nameMgr.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/name/nameMgr.hpp"

#include "Core/name/nameNameInterest.hpp"
#include "Core/name/nameObject.hpp"

//	library
#include "Core/env/envSTLHelpers.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

//============================================================================
//============================================================================
namespace {
std::vector<nameNameInterest*> l_NameInterestList;

// Max ID is the largest registered ID.
//
nameUID l_MaxID = 0;

//	This is done this way in leiu of having a map of names and
//	instead using the name interest list.
//
nameUID
generate_UID()
{
  l_MaxID++;
  return l_MaxID;
}
}

//--------------------------------------------------------------------
//	Returns object by name.  May return NULL.
//--------------------------------------------------------------------
nameObject*
nameMgr::GetObjectByName(const nameString& i_String)
{
  int num_interests = l_NameInterestList.size();
  for (int i = 0; i < num_interests; i++) {
    nameObject* obj = l_NameInterestList[i]->GetObjectByName(i_String);
    if (obj != NULL)
      return obj;
  }
  return NULL;
}

//--------------------------------------------------------------------
//	Find first matching name and return its UID
//	Note: this only returns the FIRST occurrence.  beware.
//	this should only be used under controlled situations...if at all.
//--------------------------------------------------------------------
nameUID
nameMgr::GetNameUIDFromString(const nameString& i_String)
{
  nameString name = i_String;
  name.SetUID(nameString::e_InvalidUID);

  int num_interests = l_NameInterestList.size();
  for (int i = 0; i < num_interests; i++) {
    nameObject* pObj = l_NameInterestList[i]->GetObjectByName(name);
    if (pObj != NULL) {
      return pObj->GetName().GetUID();
    }
  }

  return nameString::e_InvalidUID;
}

//--------------------------------------------------------------------
//	Get a list of all the names
//--------------------------------------------------------------------
void
nameMgr::GetNameList(nameList& io_NameList)
{
  int num_interests = l_NameInterestList.size();
  for (int i = 0; i < num_interests; i++) {
    l_NameInterestList[i]->GetNameList(io_NameList);
  }
}

//--------------------------------------------------------------------
//	RegisterNameInterest() - add a Name interest to the system
//--------------------------------------------------------------------
void
nameMgr::RegisterNameInterest(nameNameInterest* i_pInterest)
{
  DBG_ASSERT(i_pInterest != 0, "Cannot register a NULL Name Interest");
  l_NameInterestList.push_back(i_pInterest);
}

//--------------------------------------------------------------------
//	UnRegisterNameInterest() - remove a Name interest from the system.
//
//	Note: this will NOT delete the Name interest.  It is up to the
//	registerer.
//--------------------------------------------------------------------
void
nameMgr::UnRegisterNameInterest(nameNameInterest* i_pInterest)
{
  envSTLHelpers::RemoveOneValue(l_NameInterestList, i_pInterest);
}

//--------------------------------------------------------------------
//	Get the name string of the object with the passed in UID.
//--------------------------------------------------------------------
void
nameMgr::GetNameString(nameUID i_NameUID, std::string& o_NameString)
{
  int num_interests = l_NameInterestList.size();
  for (int i = 0; i < num_interests; i++) {
    l_NameInterestList[i]->GetNameString(i_NameUID, o_NameString);
  }
}

//--------------------------------------------------------------------
//	RegisterName() - register the string with the NameMgr.
//
//		the input is the UID and the string.
//		if the UID is invalid (-1) then it will be assigned a UID.
//--------------------------------------------------------------------
void
nameMgr::RegisterName(nameString& io_NameString)
{
  std::string temp_str;
  temp_str = io_NameString.GetString();
  // DBG_LOG3( "RegName %d of %d - (%s)", io_NameString.GetUID(), l_MaxID, temp_str.c_str() );

  if (io_NameString.GetUID() == nameString::e_InvalidUID) {
    io_NameString.SetUID(generate_UID());
  } else {
    if (io_NameString.GetUID() > l_MaxID) {
      l_MaxID = io_NameString.GetUID();
    }
  }

  // DBG_LOG2( "        %d of %d", io_NameString.GetUID(), l_MaxID );
}

//--------------------------------------------------------------------
//	ResetUID() - whenever you want to reset the UID counter.
//--------------------------------------------------------------------
void
nameMgr::ResetUID()
{
  l_MaxID = 0;
}

//--------------------------------------------------------------------
//	Create Default Name For an Object
//--------------------------------------------------------------------

void
nameMgr::CreateDefaultName(const nameString& i_Filename,
                           const VerifyNoDupNameFunc& i_VerifyFunc,
                           nameString& o_NameString,
                           int& io_ObjectCounter)
{
  std::ostringstream name_ss(std::ostringstream::out);
  std::string name;
  char filename_base[64];
  char* filename;
  // strcpy( filename_base, itStringUtil::GetStdString( i_Filename ).c_str() );
  strcpy(filename_base, i_Filename.GetString().c_str());
  filename = strtok(filename_base, ".");

  do {
    name_ss.str("");
    name_ss << filename << "-" << std::setw(3) << std::setfill('0') << io_ObjectCounter;
    name = name_ss.str();
    o_NameString.SetString(name.c_str());

    io_ObjectCounter++;
  } while (!i_VerifyFunc(const_cast<char*>(name.c_str())));
}

//--------------------------------------------------------------------
//	Create Duplicated Name For an Object
//--------------------------------------------------------------------
void
nameMgr::CreateDuplicateName(const nameString& i_Filename,
                             const VerifyNoDupNameFunc& i_VerifyFunc,
                             nameString& o_NameString)
{
  const int NAME_BUFF = 64;
  char strName[NAME_BUFF]; // Output string
  char numbers[] = "1234567890";
  char filename_base[NAME_BUFF];
  char filename_digit[NAME_BUFF];
  int digit_num = 0;
  int digit_counter;
  int current_digit = 0;
  char* filename;

  strcpy(filename_base, i_Filename.GetString().c_str());
  filename = strtok(filename_base, ".");

  // extract the digit appending after filename
  digit_counter = strlen(filename) - 1;
  while (digit_counter >= 0) {
    if (strchr(numbers, filename[digit_counter]) != NULL) {
      digit_num++;
      memmove(filename_digit + 1, filename_digit, digit_num - 1);
      filename_digit[0] = filename[digit_counter];
      digit_counter--;
    } else {
      break;
    }
  };

  if (digit_num == 0) {
    current_digit = 0;
  } else {
    current_digit = atoi(filename_digit);
  }

  std::ostringstream name_ss(std::ostringstream::out);
  std::string name;
  do {
    name_ss.str("");
    current_digit++;

    memset(strName, 0, NAME_BUFF);
    strncpy(strName, filename, strlen(filename) - digit_num);

    name_ss << strName << std::setw(digit_num) << std::setfill('0') << current_digit;
    name = name_ss.str();
    o_NameString.SetString(name.c_str());
  } while (!i_VerifyFunc(const_cast<char*>(name.c_str())));
}
