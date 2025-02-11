/****************************************************************************\
**  envAuditor.hpp
**
**      envAuditor.hpp defines classes for callbacks from a subject to
**	an observer when the subject is deleted. This is to allow the
**	observer to clean up its pointer reference.
**
**
**
\****************************************************************************/
#ifdef ENV_AUDITOR_HPP
#error envAuditor.hpp multiply included
#endif
#define ENV_AUDITOR_HPP

#include <vector>

//================================================================================
//================================================================================
class envAuditable;

//================================================================================
// envAuditor is the observer object. It defines a callback to be called when
//	the the subject is deleted.
//================================================================================
class envAuditor
{
public:
  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  virtual ~envAuditor();

  //------------------------------------------------------------------------
  // This function is called when the subject is being deleted.
  // RemoveAuditor() does not need to be called in this function,
  // the object is being deleted anyway.
  //------------------------------------------------------------------------
  virtual void AuditorNotify(envAuditable* i_pSubject) = 0;
};

//================================================================================
// envAuditable is the subject object. It maintains a list of observers
// and notifies them when it is deleted.
//================================================================================
class envAuditable
{
public:
  //------------------------------------------------------------------------
  //------------------------------------------------------------------------
  virtual ~envAuditable();

  //------------------------------------------------------------------------
  // Add observer to list. This pointer is just used, not owned.
  //------------------------------------------------------------------------
  void AddAuditor(envAuditor* i_pObserver);

  //------------------------------------------------------------------------
  // Remove observer from list.
  //------------------------------------------------------------------------
  void RemoveAuditor(envAuditor* i_pObserver);

private:
  std::vector<envAuditor*> m_Auditors;
};
