/****************************************************************************\
**  fsCategoryConfigFileUtil.cpp
**
**      see .hpp
**
**
**
\****************************************************************************/
#include "Core/fs/fsCategoryConfigFileUtil.hpp"

#include "Core/dbg/dbgMsg.hpp"
#include "Core/fs/fsLocator.hpp"
#include "Core/fs/fsFileUtil.hpp"
// #include "Tool/gui/guiXMLTextReader.hpp"

#include <string>

//============================================================================
//============================================================================
namespace fsCategoryConfigFileUtil {
namespace {
std::string l_Category_Tag("category");
std::string l_Element_Tag("element");

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
read_elements_from_configfile(fsLocator& i_ConfigFile, category_list_type& o_List)
{
  std::string category = "None";
  std::string cfgpath;
  fsFileUtil::LocatorToANSIFilename(i_ConfigFile, cfgpath);

  if (guiXMLTextReader::Open(cfgpath.c_str())) {
    //	read in the preferences
    //
    guiXMLTextReader::gui_Node_Type node_type;
    std::string keyname, strvalue;
    while ((node_type = guiXMLTextReader::ReadNode(keyname, strvalue)) != guiXMLTextReader::e_EOF) {
      if ((node_type == guiXMLTextReader::e_Text) && (keyname.length() > 0)) {
        if (keyname == l_Element_Tag) {
          std::string res;
          guiXMLTextReader::Convert(strvalue, res);
          o_List[0].push_back(category);
          o_List[1].push_back(res);
        } else if (keyname == l_Category_Tag) {
          guiXMLTextReader::Convert(strvalue, category);
        }
      }
    }

    guiXMLTextReader::Close();
  } else {
    DBG_ERROR("Cannot find file (" << cfgpath.c_str() << ")");
  }
}
}

//------------------------------------------------------------------------
//	the category tag for the whole file (e.g. "Category")
//
//	NOTE: if NOT set, it will default to "category"
//------------------------------------------------------------------------
void
SetCategoryTag(std::string& i_Tag)
{
  l_Category_Tag = i_Tag;
}

//------------------------------------------------------------------------
//	the tag for each element (e.g. "Resolution")
//
//	NOTE: if NOT set, it will default to "element"
//------------------------------------------------------------------------
void
SetElementTag(std::string& i_Tag)
{
  l_Element_Tag = i_Tag;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
ReadConfigFile(fsLocator& i_ConfigFile, category_list_type& o_List)
{
  read_elements_from_configfile(i_ConfigFile, o_List);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// void WriteConfigFile( fsLocator& i_ConfigFile, category_list_type& i_List )
//{
//	write_elements_from_configfile( i_ConfigFile, i_List );
//}
}
