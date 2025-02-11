/*****************************************************************************
**  gfDirectoryCategories.hpp
**
**		gfDirectoryCategories allows an application to track multiple
**	"current" directories according to named categories.
**
**		Notice that you need to use a fsFileUtil function to actually
**	change the true current directory, this utility is just tracking
**	the values per category.
**
**
**
\****************************************************************************/

#ifdef GF_DIRECTORYCATEGORIES_HPP
#error gfDirectoryCategories.hpp multiply included
#endif
#define GF_DIRECTORYCATEGORIES_HPP

#include <string>

//============================================================================
//	forward references
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
namespace gfDirectoryCategories {
//================================================================
// Store the current directory for a named category
//================================================================
void
SetCurDirectory(const std::string& i_Category, const fsLocator& i_Directory);

//================================================================
// Query the current directory for a named category,
// returns false if no current directory has been set yet.
//================================================================
bool
GetCurDirectory(const std::string& i_Category, fsLocator& o_Directory);

//================================================================
// Clear out all categories of directories in order to start over
//================================================================
void
ClearAllCategories();

}
