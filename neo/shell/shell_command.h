
/*
 * shell_command.h
 *
 *  Created on: 22 December 2019
 *      Author: Biel Bestué de Luna
 *      This File is under the GPLv3 Lic.
 */
#ifndef NEO_SHELL_SHELL_COMMAND_H_
#define NEO_SHELL_SHELL_COMMAND_H_

#include "../idlib/containers/List.h"
#include "../idlib/Str.h"

namespace BFG {

enum
{
	MACRO_SYSTEM,			// "ps" button / "xbox" button / "steam" button functionality
	MACRO_MENU, 			// "play" button / "esc" button functionality
	MACRO_OPTIONS,			// "option" button functionality
	MACRO_ACCEPT,			// "A" button
	MACRO_CANCEL,			// "B" button
	MACRO_UP,				// up in the menu
	MACRO_DOWN,				// down in the menu
	MACRO_LEFT,				// left in the menu
	MACRO_RIGHT,			// right in the menu
	MACRO_SCROLL_UP,		// scroll up in the menu texts
	MACRO_SCROLL_DOWN,		// scroll down in the menu texts
	MACRO_SCROLL_LEFT,		// scroll left in the menu texts
	MACRO_SCROLL_RIGHT,		// scroll right in the menu texts
	MACRO_NEXT,				// select next section in the menu
	MACRO_PREVIOUS,			// select previous section in the menu

};

//this is the simple event
struct InputEvent {
	int     EventType;
	int	    Event;
};

class blShellCommand {
                blShellCommand( idStr _name ) { Clear(); Name = _name; };
                ~blShellCommand() { Clear(); };
public:
  idStr         GetName() { return Name; };

  void          SetState( int newState ) { State = newState; };
  void          ToggleState();               // Treats State as a boolean
  int           GetState() { return State; };

  void          AssociateInputEvent( blInputEvent newInputEvent );
  void          DisassociateInputEvent( blInputEvent unwantedInputEvent );
  int           GetAssociatedInputEventsNum() { return associatedInputEvents_l.Num(); };
  blInputEvent  GetAssociatedInputEventAt( index i ) { return associatedInputEvents_l[i]; };

  int           ConfrontInputEventAndState( blInputEvent indicatedInputEvent, bool actUpon );
  void          ClearState() { State = 0; };

private:
  void          Clear();

  idStr         Name;
  int           State;

  idList<blInputEvent> associatedInputEvents_l;
}

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_COMMAND_H_ */
