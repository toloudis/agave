/****************************************************************************\
**	prtyFileChooserUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyFileChooserUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFileChooserUIInfo::prtyFileChooserUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_bShowFileNameOnly(false)
{
  SetControlName("FileChooser");
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyFileChooserUIInfo::prtyFileChooserUIInfo(prtyProperty* i_pProperty,
                                             const std::string& i_Category,
                                             const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_bShowFileNameOnly(false)
{
  SetControlName("FileChooser");
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyFileChooserUIInfo::Clone()
{
  return new prtyFileChooserUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	FileFilter
//--------------------------------------------------------------------
const itString&
prtyFileChooserUIInfo::GetFileFilter() const
{
  return m_FileFilter;
}
void
prtyFileChooserUIInfo::SetFileFilter(const std::string& i_FileFilter)
{
  m_FileFilter = itString(i_FileFilter.c_str());
}
void
prtyFileChooserUIInfo::SetFileFilter(const itString& i_FileFilter)
{
  m_FileFilter = i_FileFilter;
}

//--------------------------------------------------------------------
//	ShowFileNameOnly
//--------------------------------------------------------------------
const bool
prtyFileChooserUIInfo::GetShowFileNameOnly() const
{
  return m_bShowFileNameOnly;
}
void
prtyFileChooserUIInfo::SetShowFileNameOnly(const bool i_ShowFileNameOnly)
{
  m_bShowFileNameOnly = i_ShowFileNameOnly;
}

//--------------------------------------------------------------------
//	InitialDirectory
//--------------------------------------------------------------------
const fsLocator&
prtyFileChooserUIInfo::GetInitialDirectory() const
{
  return m_InitialDirectory;
}
void
prtyFileChooserUIInfo::SetInitialDirectory(const fsLocator& i_InitialDirectory)
{
  m_InitialDirectory = i_InitialDirectory;
}

//--------------------------------------------------------------------
//	DirectoryCategory - string name in order to group
//	current directory into different categories.
//--------------------------------------------------------------------
const std::string&
prtyFileChooserUIInfo::GetDirectoryCategory() const
{
  return m_DirectoryCategory;
}
void
prtyFileChooserUIInfo::SetDirectoryCategory(const std::string& i_DirectoryCategory)
{
  m_DirectoryCategory = i_DirectoryCategory;
}
