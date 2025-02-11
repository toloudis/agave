/****************************************************************************\
**	prtyTextureFileChooserUIInfo.hpp
**
**		FileChooser UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_TEXTUREFILECHOOSERUIINFO_HPP
#error prtyTextureFileChooserUIInfo.hpp multiply included
#endif
#define PRTY_TEXTUREFILECHOOSERUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

#include <string>
#include <vector>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyTextureFileChooserUIInfo : public prtyPropertyUIInfo
{
public:
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  enum TextureType
  {
    e_Texture,
    e_Ramp,
    e_Paint,
    e_Revert,
    e_Reset,
    e_NumTypes
  };

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTextureFileChooserUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTextureFileChooserUIInfo(prtyProperty* i_pProperty,
                               const std::string& i_Category,
                               const std::string& i_Description);

  //--------------------------------------------------------------------
  // Return pointer to new equivalent prtyPropertyUIInfo
  //--------------------------------------------------------------------
  virtual prtyPropertyUIInfo* Clone();

  //--------------------------------------------------------------------
  //	FileFilter
  //
  //	Examples:	"Sound files (*.wav)|*.wav|All files (*.*)|*.*"
  //				"Model files (*.mx)|*.mx"
  //--------------------------------------------------------------------
  const itString& GetFileFilter() const;
  void SetFileFilter(const std::string& i_FileFilter);
  void SetFileFilter(const itString& i_FileFilter);

  //--------------------------------------------------------------------
  //	ShowFileNameOnly
  //--------------------------------------------------------------------
  const bool GetShowFileNameOnly() const;
  void SetShowFileNameOnly(const bool i_ShowFileNameOnly);

  //--------------------------------------------------------------------
  //	InitialDirectory
  //--------------------------------------------------------------------
  const fsLocator& GetInitialDirectory() const;
  void SetInitialDirectory(const fsLocator& i_InitialDirectory);

  //--------------------------------------------------------------------
  //	DirectoryCategory - string name in order to group
  //	current directory into different categories.
  //--------------------------------------------------------------------
  const std::string& GetDirectoryCategory() const;
  void SetDirectoryCategory(const std::string& i_DirectoryCategory);

  //--------------------------------------------------------------------
  //	Add an item to the choicebox to display.  If no index is given
  //	it will be added to the end.
  //--------------------------------------------------------------------
  void AddItem(const TextureType& i_Type);

  //--------------------------------------------------------------------
  // Clear out the items in the list.
  //--------------------------------------------------------------------
  void ClearItems();

  //--------------------------------------------------------------------
  // Return the texture type string
  //--------------------------------------------------------------------
  std::string GetTextureType(const TextureType& i_Type) const;

public:
  std::vector<std::string> m_List;

private:
  itString m_FileFilter;
  bool m_bShowFileNameOnly;
  fsLocator m_InitialDirectory;
  std::string m_DirectoryCategory;
  static bool sm_bPaintDisabled;
  static bool sm_bRampDisabled;

public:
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  static std::string GetType(const TextureType& i_Type)
  {
    std::string type_string("");
    if (IsTypeDisabled(i_Type))
      return type_string;

    switch (i_Type) {
      case e_Texture:
        type_string = "Texture";
        break;
      case e_Ramp:
        type_string = "Ramp";
        break;
      case e_Paint:
        type_string = "Paint";
        break;
      case e_Revert:
        type_string = "Revert";
        break;
      case e_Reset:
        type_string = "Reset";
        break;
      default:
        break;
    }

    return type_string;
  }

  //----------------------------------------------------------------------------
  // Given the texture type string, return the ID if it exists as a texture type
  //----------------------------------------------------------------------------
  static int GetTextureTypeID(const std::string& i_Type)
  {
    for (int i = 0; i < e_NumTypes; ++i) {
      if (i_Type == GetType(TextureType(i)))
        return i;
    }
    return -1;
  }

  //----------------------------------------------------------------------------
  // Given the texture type string, return the ID if it exists as a texture type
  //----------------------------------------------------------------------------
  static void DisableType(const TextureType& i_Type)
  {
    if (i_Type == e_Paint)
      sm_bPaintDisabled = true;
    if (i_Type == e_Ramp)
      sm_bRampDisabled = true;
  }

  //----------------------------------------------------------------------------
  // Given the texture type string, return the ID if it exists as a texture type
  //----------------------------------------------------------------------------
  static bool IsTypeDisabled(const TextureType& i_Type)
  {
    if (i_Type == e_Paint)
      return sm_bPaintDisabled;
    if (i_Type == e_Ramp)
      return sm_bRampDisabled;
    return false;
  }
};
