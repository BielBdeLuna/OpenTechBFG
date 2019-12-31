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
    ClearState();

    if( associatedInputEvents_l.Num() > 0 ) {
        associatedInputEvents_l.DeleteContents( true );
    }

    associatedInputEvents_l.Clear();
}


/*
=================
blShellCommand::ToggleState
toggles the State from null to 1 and the inverse, just as if State were a boolean,
but if the State is more than 1 then it toggles it back to 0, but if toggled back again,
it won't remember the previous higher value and it will set it to 1 as in the first statement.
=================
*/
void blShellCommand::ToggleState() {
    if( State == 0 ) {
        State = 1;
    } else {
        //State is 1 or higher
        State = 0;
    }
}

/*
=================
blShellCommand::AssociateInputEvent
assosiates this new inputEvent to the list if and only if doesn't exist there already
=================
*/
void blShellCommand::AssociateInputEvent( blInputEvent newInputEvent ) {
    associatedInputEvents_l.AddUnique( newInputEvent );
}

/*
=================
blShellCommand::DisassociateInputEvent
assosiates this new inputEvent to the list if and only if doesn't exist there already
=================
*/
void blShellCommand::DisassociateInputEvent( blInputEvent unwantedInputEvent ) {
    int index = associatedInputEvents_l.FindIndex( unwantedInputEvent );

    if( index == -1 ) {
        common->Error( "blShellCommand::AssociateInputEvent : couldn't find this unwantedInputEvent in the list of the '%s' ShellCommand! \n", Name.c_str() );
        return;
    }

    delete associatedInputEvents_l[index];
    associatedInputEvents_l[index] = NULL;
    associatedInputEvents_l.RemoveIndex( index );
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
    int index = associatedInputEvents_l.FindIndex( indicatedInputEvent );
    if( ( index != -1 ) && ( State != 0 ) && ( actUpon ) ) {
        ToggleState();
    }
    return index;
}

} /* namespace BFG */
