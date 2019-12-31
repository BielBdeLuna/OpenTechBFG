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

class blInputEvent {
public:
    blInputEvent( int newEventType, int newEvent, int newValue );
    const int   eventType;
    const int   event;
    const int   value;

    //only returns true if both eventtype and event match the proposed input event
    bool Confront( int indicatedEventType, int indicatedEvent );

    bool operator==( const blInputEvent& other );
    bool operator!=( const blInputEvent& other );
}

ID_INLINE blInputEvent( int newEventType, int newEvent, int newValue ) {
  eventType = newEventType;
  event = newEvent;
  value = newValue;
}

ID_INLINE bool blInputEvent::Confront( int indicatedEventType, int indicatedEvent ) {
  return( ( eventType == indicatedEventType ) && ( event == indicatedEvent ) );
}

ID_INLINE bool blInputEvent::operator==( const blInputEvent& other ) {
  return Confront( other.eventType, other.event );
}

ID_INLINE bool blInputEvent::operator!=( const blInputEvent& other ) {
  return !Confront( other.eventType, other.event );
}

} /* namespace BFG */
#endif /* NEO_SHELL_INPUTEVENT_H_ */
