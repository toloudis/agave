/****************************************************************************\
**	prtyTrigger.cpp
**
**		see .hpp
**
**
**
\****************************************************************************/
#include "Core/prty/prtyTrigger.hpp"

#include "Core/Dbg/dbgMsg.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTrigger::prtyTrigger()
  : prtyProperty("Trigger")
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
prtyTrigger::prtyTrigger(const std::string& i_Name)
  : prtyProperty(i_Name)
{
}

//--------------------------------------------------------------------
//	The type of property it is
//--------------------------------------------------------------------
const char*
prtyTrigger::GetType()
{
  return "Trigger";
}

//--------------------------------------------------------------------
// Call property callbacks assigned to this property.
//--------------------------------------------------------------------
void
prtyTrigger::TriggerCallbacks()
{
  const bool bWithUndo = false;
  NotifyCallbacksPropertyChanged(bWithUndo);
}

//--------------------------------------------------------------------
// Create an undo operation of the correct type for this
// property. A reference to this property should be passed in.
//--------------------------------------------------------------------
// virtual
undoUndoOperation*
prtyTrigger::CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef)
{
  DBG_ASSERT(false, "Cannot create undo operation for trigger property.");
  return NULL;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyTrigger::Read(chReader& io_Reader)
{
  DBG_ASSERT(false, "Cannot read trigger property.");
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
// virtual
void
prtyTrigger::Write(chWriter& io_Writer) const
{
  DBG_ASSERT(false, "Cannot write trigger property.");
}
