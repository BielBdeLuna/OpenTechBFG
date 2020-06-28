/*
 * inputEvent.h
 *
 *  Created on: 30 12 2019
 *      Author: Biel Bestué de Luna
 */

#ifndef NEO_SHELL_INPUTEVENT_H_
#define NEO_SHELL_INPUTEVENT_H_

#include "../framework/Common.h"

namespace BFG {

//this is the simple event reerence
struct InputEventRef_s {
	const int   eventType;
	const int   event;
};

//this is the actual class
class blInputEvent{
public:
  blInputEvent( InputEventRef_s newRef );

  InputEventRef_s GetReference() { return reference; };
  bool            Confront( InputEventRef_s indicatedRef );

  int             GetValue() { return value; };
  void            FindNewValue();

  bool            operator==( const blInputEvent& other );
  bool            operator!=( const blInputEvent& other );

private:
    const InputEventRef_s reference;
    int                   value;
}

ID_INLINE blInputEvent( InputEventRef_s newRef ) {
  reference.eventType = newRef.eventType;
  reference.event = newRef.event;
}

ID_INLINE bool blInputEvent::Confront( InputEventRef_s indicatedRef ) {
  return( ( reference.eventType == indicatedRef.eventType ) && ( reference.event == indicatedRef.event ) );
}

ID_INLINE bool blInputEvent::operator==( const blInputEvent& other ) {
  return Confront( other.GetReference().eventType, other.GetReference().event );
}

ID_INLINE bool blInputEvent::operator!=( const blInputEvent& other ) {
  return !(Confront( other.GetReference().eventType, other.GetReference().event ));
}

} /* namespace BFG */
#endif /* NEO_SHELL_INPUTEVENT_H_ */
