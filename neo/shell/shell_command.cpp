/*
 * shell_command.cpp
 *
 *  Created on: 23 dec 2019
 *      Author: Biel Bestué de Luna
 */

#include "../shell/shell_command.h"
#include "../framework/Common.h"

namespace BFG {

/*
=================
blShellCommand::Clear
=================
*/
void blShellCommand::Clear() {
    Name.Clear();
    state = UNPRESSED;

    if( associatedInputEventsPtr_l.Num() > 0 ) {
        associatedInputEventsPtr_l.DeleteContents( true );
    }

    associatedInputEventsPtr_l.Clear();
}

/*
=================
blShellCommand::GatherStateAndValue
TODO work out a slution to gather the relevant input
=================
*/

void blShellCommand::GatherStateAndValue() {
    int newValue = 0;

    // gather the highest of the values for all the given inputs
    //TODO this needs to be able to accomodate a Focus feature
    for( int i = 0; i < associatedInputEventsPtr_l.Num(); i++ ) {
        int val = associatedInputEventsPtr_l[i]->GetValue();
        if( newValue < val ) {
            newValue = val;
        }
    }

    // using the gathered newValue and the old one, now gather the correct state
    if( newValue == 0 ) {
        if( state == STOPPED_PRESSING ) {
          state = UNPRESSED;
        } else {
            if( state != UNPRESSED ) {
                state = STOPPED_PRESSING;
            }
        }
    } else {
        if( ( state == UNPRESSED ) || ( state == STOPPED_PRESSING ) ) {
            state = STARTED_PRESSING;
        } else {
            if( newValue == value ) {
                state = PRESSED_VALUE_UNALTERED;
            } else if( newValue > value ) {
                state = PRESSED_VALUE_INCREASING;
            } else {
                state = PRESSED_VALUE_DECREASING;
            }
        }
    }

    // actually set the reported value as the newValue
    value = newValue;

}

/*
=================
blShellCommand::DelAssociatedInputEventAt
deletes an element for the associatedInputEventsPtr_l list given an index
=================
*/
void blShellCommand::DelAssociatedInputEventAt( int index ) {
    if( ( index >= associatedInputEventsPtr_l.Num() ) || ( index < 0 ) ) {
        common->Error( "blShellCommand::DelAssociatedInputEventAt : Index '%i' is out of bounds from the list of pointers!\n", index );
    }

    //delete associatedInputEventsPtr_l[index]; //I don't wanna erase whatever this element is pointing to
    associatedInputEventsPtr_l[index] = NULL;
    associatedInputEventsPtr_l.RemoveIndex( index );
}

/*
=================
blShellCommand::AssociateInputEvent
associates this new inputEvent as a reference into the associatedInputEvents_l
list if and only if doesn't exist there already and returns it's new index.
=================
*/
int blShellCommand::AssociateInputEvent( blInputEvent& newInputEvent ) {
    return associatedInputEventsPtr_l.AddUnique( newInputEvent );
}

/*
=================
blShellCommand::DisassociateInputEvent
disassociates the inputEvent from the list given the correct reference if and only if it exists
=================
*/
void blShellCommand::DisassociateInputEvent( InputEventRef_s unwantedInputEventReference ) {
    int index = SearchAssociatedInputEvents( unwantedInputEventReference );

    if( index == -1 ) {
        common->Warning( "blShellCommand::AssociateInputEvent : Couldn't find this unwantedInputEvent in the list of the '%s' ShellCommand! \n", Name.c_str() );
        return;
    }

    DelAssociatedInputEventAt( index );
}

/*
=================
blShellCommand::ConfrontInputEventAndState
test the indicatedInputEvent against the ones in the list,
and then if it exists, and if the actUpon flag is set,
then it will change the State of the ShellCommand to active.
=================
*/
int blShellCommand::ConfrontInputEventAndState( blInputEvent indicatedInputEvent, bool actUpon ) {
    int index = SearchAssociatedInputEvents( indicatedInputEvent.GetReference() );
    if( ( index != -1 ) && ( State != 0 ) && ( actUpon ) ) {
        State = indicatedInputEvent.value;
    }
    return index;
}

/*
=================
blShellCommand::SearchAssociatedInputEvents
confronts the given inputEventReference with all the InputEvents references stored
in the associatedInputEvents_l list, returns the index if one is found or -1 otherways
=================
*/
int blShellCommand::SearchAssociatedInputEvents( InputEventRef_s inputEventReference ) {
    for( int i = 0; i < associatedInputEvents_l.Num(); i++ ) {
        if( associatedInputEventsPtr_l[i].confront( inputEventReference ) ) {
            return i;
        }
    }
    return -1;
}

} /* namespace BFG */
