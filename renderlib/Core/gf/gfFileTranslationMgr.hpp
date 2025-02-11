/*****************************************************************************
**  gfFileTranslationMgr.hpp
**
**      gfFileTranslationMgr is a utility which will automatically track down
**		the fully pathed location of the given locator based on its index
**		(for example, app level code can be written by overriding gfFileTranslator
**		that will seek out the given filename from among a number of directories
**		which have been associated to a given path)
**
**
**
\****************************************************************************/

#ifdef GF_FILETRANSLATIONMGR_HPP
#error gfFileTranslationMgr.hpp multiply included
#endif
#define GF_FILETRANSLATIONMGR_HPP

//============================================================================
//============================================================================
class fsLocator;

//============================================================================
//============================================================================
namespace gfFileTranslationMgr {
//========================================================================
//========================================================================
class gfFileTranslator
{
public:
  //----------------------------------------------------------------
  //----------------------------------------------------------------
  gfFileTranslator();
  virtual ~gfFileTranslator();

  //----------------------------------------------------------------
  //	TranslateFile should be overloaded to return in o_TranslatedFile
  //	a locator to the file specified in i_File which is fully pathed
  //	to the actual location of the given file. Will throw a
  //	fsFileDoesntExistX exception if it cannot locate the file at all
  //	This will return i_File in o_TranslatedFile if i_File has no index
  //----------------------------------------------------------------
  virtual void TranslateFile(const fsLocator& i_File, fsLocator& o_TranslatedFile) = 0;
};

//------------------------------------------------------------------------
//	*** NOTE: this also checks gfPakSearcher::FileExists as a convenience ***
//	FileExists returns true if the file can be found in the correct
//	location (either pak files or on disk, depending on the decision
//	of the PakLocator).
//------------------------------------------------------------------------
bool
FileExists(const fsLocator& i_Locator);

//------------------------------------------------------------------------
//	DirectoryExists returns true if the directory can be found among the
//	possible paths based on the index (if any)
//------------------------------------------------------------------------
bool
DirectoryExists(const fsLocator& i_Locator);

//------------------------------------------------------------------------
//	ExpandLocator causes the given locator to be transformed to it's
//	full path representation (removes the index).
//------------------------------------------------------------------------
void
ExpandLocator(fsLocator& io_Locator);

//------------------------------------------------------------------------
//	SetFileTranslator sets the FileTranslator object which is used by the
//	gfFileTranslationMgr to translate locators.  The gfFileTranslationMgr
//	does not own the FileTranslator; you must ensure that it is destroyed yourself.
//------------------------------------------------------------------------
void
SetFileTranslator(gfFileTranslator* i_FileTranslator);
}
