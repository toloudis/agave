/****************************************************************************\
**  gfPathsPACWin.hpp
**
**      gfPathsPACWin.hpp defines the game paths PAC for windows.
**
**
**
\****************************************************************************/

#ifdef GF_PATHSPACWIN_HPP
#error gfPathsPACWin.hpp multiply included
#endif
#define GF_PATHSPACWIN_HPP

#include <vector>

//============================================================================
//	Forward references
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
namespace gfPathsPAC {
//------------------------------------------------------------------------
//------------------------------------------------------------------------
typedef std::vector<fsLocator> GamePathList;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
InitPaths(std::vector<GamePathList>& o_Vec);
// void InitPaths(std::vector<GamePathList>& o_Vec, const char* i_CDVolumeName);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// bool FindFile( const char* pPath, const char* pFilename );

//------------------------------------------------------------------------
//	returns true if the a CD with the given name is present.  This function
//	can be called without initializing the gf package.
//------------------------------------------------------------------------
// bool CDInDrive(const char* i_CDVolumeName);

//------------------------------------------------------------------------
//	SetCDPath searched drives for i_CDVolumeName and sets the path if found
//------------------------------------------------------------------------
// void SetCDPath(std::vector<GamePathList>& o_Vec, const char* i_CDVolumeName);

//------------------------------------------------------------------------
//	Get the user's "home" directory for storing data.
//	In Windows this is
//    "Documents and Settings/USER_NAME/Local Settings/Application Data/AllenInstitute/agave"
//  In Linux it is the user's root directory
//------------------------------------------------------------------------
void
GetUserDataDirectory(fsLocator& o_Locator);

//------------------------------------------------------------------------
//	Get the user's "My Documents" directory for storing data.
//	In Windows this is the "Documents and Settings/USER_NAME/My Documents" folder
//	In Linux it is the user's root directory
//------------------------------------------------------------------------
void
GetUserDocumentsDirectory(fsLocator& o_Locator);

//------------------------------------------------------------------------
//	Get the public "Documents" directory for storing data.
//	In Windows this is the "Users/Public/Documents" folder
//	In Linux it is the user's root directory
//------------------------------------------------------------------------
void
GetCommonDocumentsDirectory(fsLocator& o_Locator);

//------------------------------------------------------------------------
//	Set the user data path where the user data will be stored.
//	It could be installed under user/My Documents or all users/Documents
//	on Windows.
//	Under one of these types of folders will be appended the sub-folders
//	passed into this function.
//	AllenInstitute/agave (for example).
//------------------------------------------------------------------------
void
SetUserDataSubPath(fsLocator& i_UDSubPath);
}
