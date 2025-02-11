/****************************************************************************\
**	prtyFileChooserUIInfo.hpp
**
**		FileChooser UI info
**
**
**
\****************************************************************************/
#ifdef PRTY_FILECHOOSERUIINFO_HPP
#error prtyFileChooserUIInfo.hpp multiply included
#endif
#define PRTY_FILECHOOSERUIINFO_HPP

#ifndef PRTY_PROPERTYUIINFO_HPP
#include "Core/prty/prtyPropertyUIInfo.hpp"
#endif

#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class prtyProperty;

//============================================================================
//============================================================================
class prtyFileChooserUIInfo : public prtyPropertyUIInfo
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFileChooserUIInfo(prtyProperty* i_pProperty);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyFileChooserUIInfo(prtyProperty* i_pProperty, const std::string& i_Category, const std::string& i_Description);

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

private:
  itString m_FileFilter;
  bool m_bShowFileNameOnly;
  fsLocator m_InitialDirectory;
  std::string m_DirectoryCategory;
};
