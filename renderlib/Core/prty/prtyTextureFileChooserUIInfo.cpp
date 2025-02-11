/****************************************************************************\
**	prtyTextureFileChooserUIInfo.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyTextureFileChooserUIInfo.hpp"

#include "Core/prty/prtyProperty.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTextureFileChooserUIInfo::prtyTextureFileChooserUIInfo(prtyProperty* i_pProperty)
  : prtyPropertyUIInfo(i_pProperty)
  , m_bShowFileNameOnly(false)
{
  SetControlName("TextureFileChooser");
  m_List.push_back(GetTextureType(e_Texture));
  m_List.push_back(GetTextureType(e_Revert));
  m_List.push_back(GetTextureType(e_Reset));
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTextureFileChooserUIInfo::prtyTextureFileChooserUIInfo(prtyProperty* i_pProperty,
                                                           const std::string& i_Category,
                                                           const std::string& i_Description)
  : prtyPropertyUIInfo(i_pProperty, i_Category, i_Description)
  , m_bShowFileNameOnly(false)
{
  SetControlName("TextureFileChooser");
  m_List.push_back(GetTextureType(e_Texture));
  m_List.push_back(GetTextureType(e_Revert));
  m_List.push_back(GetTextureType(e_Reset));
}

//--------------------------------------------------------------------
// Return pointer to new equivalent prtyPropertyUIInfo
//--------------------------------------------------------------------
// virtual
prtyPropertyUIInfo*
prtyTextureFileChooserUIInfo::Clone()
{
  return new prtyTextureFileChooserUIInfo(this->GetProperty(0));
}

//--------------------------------------------------------------------
//	FileFilter
//--------------------------------------------------------------------
const itString&
prtyTextureFileChooserUIInfo::GetFileFilter() const
{
  return m_FileFilter;
}
void
prtyTextureFileChooserUIInfo::SetFileFilter(const std::string& i_FileFilter)
{
  m_FileFilter = itString(i_FileFilter.c_str());
}
void
prtyTextureFileChooserUIInfo::SetFileFilter(const itString& i_FileFilter)
{
  m_FileFilter = i_FileFilter;
}

//--------------------------------------------------------------------
//	ShowFileNameOnly
//--------------------------------------------------------------------
const bool
prtyTextureFileChooserUIInfo::GetShowFileNameOnly() const
{
  return m_bShowFileNameOnly;
}
void
prtyTextureFileChooserUIInfo::SetShowFileNameOnly(const bool i_ShowFileNameOnly)
{
  m_bShowFileNameOnly = i_ShowFileNameOnly;
}

//--------------------------------------------------------------------
//	InitialDirectory
//--------------------------------------------------------------------
const fsLocator&
prtyTextureFileChooserUIInfo::GetInitialDirectory() const
{
  return m_InitialDirectory;
}
void
prtyTextureFileChooserUIInfo::SetInitialDirectory(const fsLocator& i_InitialDirectory)
{
  m_InitialDirectory = i_InitialDirectory;
}

//--------------------------------------------------------------------
//	DirectoryCategory - string name in order to group
//	current directory into different categories.
//--------------------------------------------------------------------
const std::string&
prtyTextureFileChooserUIInfo::GetDirectoryCategory() const
{
  return m_DirectoryCategory;
}
void
prtyTextureFileChooserUIInfo::SetDirectoryCategory(const std::string& i_DirectoryCategory)
{
  m_DirectoryCategory = i_DirectoryCategory;
}

//--------------------------------------------------------------------
//	Add an item to the choicebox to display. Always add it to the end
//--------------------------------------------------------------------
void
prtyTextureFileChooserUIInfo::AddItem(const TextureType& i_Type)
{
  if (IsTypeDisabled(i_Type))
    return;

  std::string label;
  std::string revert;
  std::string reset;
  label = GetTextureType(i_Type);
  revert = GetTextureType(e_Revert);
  reset = GetTextureType(e_Reset);

  // always add the item to the end of the choice box
  int add_index = m_List.size();
  if (add_index == 0) {
    add_index += 2;
  } else if (add_index == 1) {
    add_index += 1;
  }
  m_List.resize(add_index + 1);

  // make sure the revert and reset tags are always at the end of the list
  m_List[add_index - 2] = label;
  m_List[add_index - 1] = revert;
  m_List[add_index] = reset;
}

//--------------------------------------------------------------------
// Clear out the items in the list.
//--------------------------------------------------------------------
void
prtyTextureFileChooserUIInfo::ClearItems()
{
  m_List.clear();
}

//--------------------------------------------------------------------
// Return the texture type string
//--------------------------------------------------------------------
std::string
prtyTextureFileChooserUIInfo::GetTextureType(const TextureType& i_Type) const
{
  return GetType(i_Type);
}

// static disable member inits
bool prtyTextureFileChooserUIInfo::sm_bPaintDisabled = false;
bool prtyTextureFileChooserUIInfo::sm_bRampDisabled = false;
