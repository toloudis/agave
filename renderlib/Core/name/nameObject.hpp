/****************************************************************************\
**	nameObject.hpp
**
**		This class is designed to be a base class for named objects
**
**
**
\****************************************************************************/

#ifdef NAME_OBJECT_HPP
#error nameObject.hpp multiply included
#endif
#define NAME_OBJECT_HPP

#ifndef NAME_STRING_HPP
#include "Core/name/nameString.hpp"
#endif

//============================================================================
//	Forward References
//============================================================================

//============================================================================
//============================================================================
class nameObject
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual ~nameObject();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void SetName(const nameString& i_Name);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  const nameString& GetName() const;

  //--------------------------------------------------------------------
  // UpdateName() is called when the gui sets the name of the object,
  //	derived classes can set dirty bits and do "undo" operations, etc.
  // The default behavior calls SetName()
  //--------------------------------------------------------------------
  // virtual void UpdateName(const std::string& i_Name);

private:
  nameString m_Name;
};
