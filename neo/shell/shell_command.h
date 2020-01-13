
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

enum {
  UNPRESSED
  STARTED_PRESSING
  PRESSED_VALUE_UNALTERED
  PRESSED_VALUE_INCREASING
  PRESSED_VALUE_DECREASING
  STOPPED_PRESSING
}
class blShellCommand {
                blShellCommand( idStr _name ) { Clear(); Name = _name; };
                ~blShellCommand() { Clear(); };
public:
  idStr         GetName() { return Name; };

  void          GatherStateAndValue();

  int           GetState() { return State; };
  int           GetValue() { return value; };

  int           AssociateInputEvent( blInputEvent& newInputEvent );
  void          DisassociateInputEvent( InputEventRef_s unwantedInputEventReference );
  int           GetAssociatedInputEventsNum() { return associatedInputEventsPtr_l.Num(); };
  blInputEvent* GetAssociatedInputEventAt( int index ) { return associatedInputEventsPtr_l[index]; };
  void          DelAssociatedInputEventAt( int index );
  int           SearchAssociatedInputEvents( InputEventRef_s indicatedInputEventRef );

private:
  void          Clear();

  idStr         Name;
  int           value;
  int           state;

  idList<blInputEvent*> associatedInputEventsPtr_l;
}

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_COMMAND_H_ */
