/****************************************************************************\
**	nameObject.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/

#include "Core/name/nameObject.hpp"
#include "Core/name/nameMgr.hpp"

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
nameObject::~nameObject() {}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
nameObject::SetName(const nameString& i_Name)
{
  std::string temp;
  m_Name = i_Name;

  for (int i = 0; i < i_Name.GetUID(); i++) {
    nameMgr::GetNameString(i, temp);
    // DBG_LOG2(" Names %s %s ", temp, m_Name.GetString().c_str());
    if (m_Name.GetString() == temp && temp.size() > 0) {
      std::string msg = "Name \" " + temp + "\" exists. Please use a different name.";
      DBG_ERROR(msg.c_str());
      // guiMessageBox::Show(msg.c_str(), "Error", guiMessageBox::e_OKOnly);
      m_Name.SetString(i_Name.GetString() + ".copy");
    }
  }
  nameMgr::RegisterName(m_Name);
  // DBG_LOG2( "registered Name -------> %d %s", m_Name.GetUID(), m_Name.GetString().c_str() );
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
const nameString&
nameObject::GetName() const
{
  return m_Name;
}

//--------------------------------------------------------------------
// UpdateName() is called when the gui sets the name of the object,
//	derived classes can set dirty bits and do "undo" operations, etc.
// The default behavior calls SetName()
//--------------------------------------------------------------------
// virtual
// void nameObject::UpdateName(const std::string& i_Name)
//{
//	m_Name.SetString(i_Name);
//	this->SetName(i_Name);
//}
