/****************************************************************************\
**	prtyTrigger.hpp
**
**		Property with no value, to be used just for triggering callbacks.
**
**
**
\****************************************************************************/
#ifdef PRTY_TRIGGER_HPP
#error prtyTrigger.hpp multiply included
#endif
#define PRTY_TRIGGER_HPP

#ifndef PRTY_PROPERTY_HPP
#include "Core/prty/prtyProperty.hpp"
#endif

//============================================================================
//============================================================================
class prtyTrigger : public prtyProperty
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTrigger();

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  prtyTrigger(const std::string& i_Name);

  //--------------------------------------------------------------------
  //	The type of property it is
  //--------------------------------------------------------------------
  virtual const char* GetType();

  //--------------------------------------------------------------------
  // Call property callbacks assigned to this property.
  //--------------------------------------------------------------------
  void TriggerCallbacks();

private:
  //--------------------------------------------------------------------
  // Create an undo operation of the correct type for this
  // property. A reference to this property should be passed in.
  //--------------------------------------------------------------------
  virtual undoUndoOperation* CreateUndoOperation(shared_ptr<prtyPropertyReference> i_pPropertyRef);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Read(chReader& io_Reader);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  virtual void Write(chWriter& io_Writer) const;
};
