/****************************************************************************\
**  fsResourceFinderJoin.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/

#include "Core/fs/fsResourceFinderJoin.hpp"

//----------------------------------------------------------------
// A constructor is provided for the 2-finder join which
//	is the most likely use of this class.
//----------------------------------------------------------------
fsResourceFinderJoin::fsResourceFinderJoin() {}
fsResourceFinderJoin::fsResourceFinderJoin(const fsResourceFinder* i_pFinder1, const fsResourceFinder* i_pFinder2)
{
  this->AddResourceFinder(i_pFinder1);
  this->AddResourceFinder(i_pFinder2);
}

//----------------------------------------------------------------
// Add another finder to the list to search with. The pointer
// is used but not owned.
//----------------------------------------------------------------
void
fsResourceFinderJoin::AddResourceFinder(const fsResourceFinder* i_pFinder)
{
  m_Finders.push_back(i_pFinder);
}

//----------------------------------------------------------------
//	Trying to find resource i_FileName.  If this finder
//	knows where it is, it should return true and
//	put the full path to the resource (including filename)
//	into o_FoundLocator.
//----------------------------------------------------------------
bool
fsResourceFinderJoin::FindResource(const itString& i_FileName, fsLocator& o_FoundLocator) const
{
  for (int i = 0; i < m_Finders.size(); ++i) {
    if (m_Finders[i]->FindResource(i_FileName, o_FoundLocator))
      return true;
  }
  return false;
}

//----------------------------------------------------------------
// This function works like the other FindResource, but this
//	one provides a locator of the resource that is asking
//	for the file to be found, which may be important for
//	some implementations.
//----------------------------------------------------------------
bool
fsResourceFinderJoin::FindResource(const itString& i_FileName,
                                   const fsLocator& i_AskingLocator,
                                   fsLocator& o_FoundLocator) const
{
  for (int i = 0; i < m_Finders.size(); ++i) {
    if (m_Finders[i]->FindResource(i_FileName, i_AskingLocator, o_FoundLocator))
      return true;
  }
  return false;
}
