/*
 * inputEvent.cpp
 *
 *  Created on: 5 de gener 2020
 *      Author: Biel Bestué de Luna
 */

#include "../shell/inputEvent.h"

namespace BFG {

/*
=================
blInputEvent::FindNewValue
=================
*/
void blInputEvent::FindNewValue() {

    bool foundValue = false;

    Sys_GenerateEvents();

    // queue system events ready for polling
	Sys_GetEvent();

	if( reference.eventType == SE_KEY ) {
        int numKeyEvents = Sys_PollKeyboardInputEvents();
        if( numKeyEvents > 0 ) {
            for( int i = 0; i < numKeyEvents; i++ )	{
			    int key;
				bool state;

				if( Sys_ReturnKeyboardInputEvent( i, key, state ) ) {
					if( key == reference.event ) {
						    value = state; //'value' is bool ported to int
						    foundValue = true;
						    break;
					}
				}
			}

			Sys_EndKeyboardInputEvents();
        }
	} else if( reference.eventType == SE_MOUSE ) {
	    int	mouseEvents[MAX_MOUSE_EVENTS][2];
		int numMouseEvents = Sys_PollMouseInputEvents( mouseEvents );
		if( numMouseEvents > 0 ) {
		    for( int i = 0; i < numMouseEvents; i++ ) {
				int action = mouseEvents[i][0];
				int mouValue = mouseEvents[i][1];

				if( action == reference.event ) {
				    value = mouValue;
				    foundValue = true;
					break;
			    }
			}
		}
	} else if( reference.eventType == SE_JOYSTICK ) {
        int numJoystickEvents = Sys_PollJoystickInputEvents( 0 );
		if( numJoystickEvents > 0 ) {
		    for( int i = 0; i < numJoystickEvents; i++ ) {
			    int action;
			    int joyValue;

				if( Sys_ReturnJoystickInputEvent( i, action, joyValue ) ) {
					if( action == reference.event ) {
						value = joyValue;
						foundValue = true;
						break;
					}
				}
			}

			Sys_EndJoystickInputEvents();
		}
	} else {
	    common->Error("the hell?!\n"); //TODO figure out a better message error! XD
	}

    if( !foundValue ) {
        value = 0;
    }
}

} /* namespace BFG */
