/****************************************************************************\
**  fsResourceFinderJoin.hpp
**
**      fsResourceFinderJoin.hpp joints together multiple fsResourceFinders
**	in order to look in multiple lcoations for a resource.
**
**
**
\****************************************************************************/

#ifdef FS_RESOURCEFINDERJOIN_HPP
#error fsResourceFinderJoin.hpp multiply included
#endif
#define FS_RESOURCEFINDERJOIN_HPP

#ifndef FS_RESOURCEFINDER_HPP
#include "Core/fs/fsResourceFinder.hpp"
#endif
#ifndef FS_LOCATOR_HPP
#include "Core/fs/fsLocator.hpp"
#endif

//============================================================================
//============================================================================
class fsResourceFinderJoin : public fsResourceFinder
{
public:
  //----------------------------------------------------------------
  // A constructor is provided for the 2-finder join which
  //	is the most likely use of this class.
  //----------------------------------------------------------------
  fsResourceFinderJoin();
  fsResourceFinderJoin(const fsResourceFinder* i_pFinder1, const fsResourceFinder* i_pFinder2);

  //----------------------------------------------------------------
  // Add another finder to the list to search with. The pointer
  // is used but not owned.
  //----------------------------------------------------------------
  void AddResourceFinder(const fsResourceFinder* i_pFinder);

  //----------------------------------------------------------------
  //	Trying to find resource i_FileName.  If this finder
  //	knows where it is, it should return true and
  //	put the full path to the resource (including filename)
  //	into o_FoundLocator.
  //----------------------------------------------------------------
  virtual bool FindResource(const itString& i_FileName, fsLocator& o_FoundLocator) const;

  //----------------------------------------------------------------
  // This function works like the other FindResource, but this
  //	one provides a locator of the resource that is asking
  //	for the file to be found, which may be important for
  //	some implementations.
  //----------------------------------------------------------------
  virtual bool FindResource(const itString& i_FileName,
                            const fsLocator& i_AskingLocator,
                            fsLocator& o_FoundLocator) const;

private:
  std::vector<const fsResourceFinder*> m_Finders;
};
