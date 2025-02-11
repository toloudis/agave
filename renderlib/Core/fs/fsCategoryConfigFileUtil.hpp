/****************************************************************************\
**  fsCategoryConfigFileUtil.hpp
**
**      Routines for handling a "category" based XML configuration file.
**
**	The code will parse a file like the example below into a 2 dimensional
**	vector.  The first column will be the category and the second column
**	will be the element.  The data will be read is as:
**
**	resolution_list_type the_array;
**	the_array[0][0] = category
**	the_array[1][0] = element
**	the_array[0][1] = category
**	the_array[1][1] = element
**	the_array[0][2] = category
**	the_array[1][2] = element
**		.				.
**		.				.
**		.				.
**
**	the data file looks like:
**
**	<?xml version="1.0" encoding="utf-8"?>
**	<Resolutions>
**	  <Category>1:2.4</Category>
**	    <Resolution>2048x1168</Resolution>
**	    <Resolution>1024x584</Resolution>
**	  <Category>4:3</Category>
**	  	<Resolution>320x240</Resolution>
**	  	<Resolution>640x480</Resolution>
**	  	<Resolution>720x540</Resolution>
**	  	<Resolution>960x720</Resolution>
**	  	<Resolution>1920x1440</Resolution>
**	  	<Resolution>3840x2880</Resolution>
**	  <Category>16:9</Category>
**	 		<Resolution>640x360</Resolution>
**	 		<Resolution>852x480</Resolution>
**	 		<Resolution>1280x720</Resolution>
**	 		<Resolution>2560x1440</Resolution>
**	 		<Resolution>5120x2880</Resolution>
**	  <Category>digi-beta</Category>
**	    <Resolution>720x486</Resolution>
**	  	<Resolution>1440x972</Resolution>
**	  <Category>mpeg-2</Category>
**	    <Resolution>720x480 NTSC</Resolution>
**	  	<Resolution>720x576 PAL</Resolution>
**	  <Category>High-Def</Category>
**	    <Resolution>1920x1080 NTSC</Resolution>
**	</Resolutions>
**
**
**
**
\****************************************************************************/
#ifdef FS_CATEGORYCONFIGFILEUTIL_HPP
#error fsCategoryConfigFileUtil.hpp multiply included
#endif
#define FS_CATEGORYCONFIGFILEUTIL_HPP

#include <string>
#include <vector>

//============================================================================
//	forward references
//============================================================================
class fsLocator;

//============================================================================
//	typedefs
//============================================================================
typedef std::vector<std::vector<std::string>> category_list_type; // "category", "resolution"

//============================================================================
//============================================================================
namespace fsCategoryConfigFileUtil {
//------------------------------------------------------------------------
//	the category tag for the whole file (e.g. "Category")
//
//	NOTE: if NOT set, it will default to "category"
//------------------------------------------------------------------------
void
SetCategoryTag(std::string& i_Tag);

//------------------------------------------------------------------------
//	the tag for each element (e.g. "Resolution")
//
//	NOTE: if NOT set, it will default to "element"
//------------------------------------------------------------------------
void
SetElementTag(std::string& i_Tag);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
ReadConfigFile(fsLocator& i_ConfigFile, category_list_type& o_List);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// void WriteConfigFile( fsLocator& i_ConfigFile, category_list_type& i_List );
}
