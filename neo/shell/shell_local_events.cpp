/*
 * shell_local_events.cpp
 *
 *  Created on: 9 de Gener 2020
 *      Author: Biel Bestué de Luna
 */

#include "../shell/shell_local.h"
#include "../framework/CmdSystem.h"

namespace BFG {

/*******************************************************************************
							      SHELL EVENTS
*******************************************************************************/

/*
==============
blShellLocal::Event_LoadGame
==============
*/
void Event_LoadGame( idStr saveFileName ) {
	cmdSystem->AppendCommandText( va( "loadgame %s\n", saveFileName.c_str() ) );
}

/*
==============
blShellLocal::Event_SaveGame
==============
*/
void blShellLocal::Event_SaveGame( idStr saveGameName ) {
	//add a threaded menu icon in order to inform there is saving going on
	cmdSystem->AppendCommandText( va( "savegame %s\n", saveGameName.c_str() ) );
}

/*
==============
blShellLocal::Event_QuitGame
==============
*/
void blShellLocal::Event_QuitGame() {
	cmdSystem->AppendCommandText( "quit" );
}

/*
==============
blShellLocal::Event_QuitToMainMenu
==============
*/
void blShellLocal::Event_QuitToMainMenu() {
	;
}

/*
==============
blShellLocal::Event_ResumeGame
==============
*/
void blShellLocal::Event_ResumeGame() {
	;
}

//session stuff

/*
==============
blShellLocal::Event_Get_SessionState
Gather the state of the session in order to operate in menus and other shell elements given it's state
==============
*/
int blShellLocal::Event_Get_SessionState() {


	idSession::sessionState_t sessionState = session->GetState(); //enum


	return sessionState; //enum to int
}

/*
==============
blShellLocal::Event_Set_SessionState
Set a state in the session, set externally off the engine
==============
*/
void blShellLocal::Event_Set_SessionState( int newState ) {
	//check if newState is within bounds of the sessionState_t enum
	if( ( newState > idSession::MAX_STATES ) || ( newState < idSession::PRESS_START ) ) {
		common->Error( "blShellLocal::Event_Set_SessionState : Session doesn't contain state number '%i' in the list.\n", newState );
	} else {
		; //TODO make it so that the session becomes that state
	}
}

// InputsEvents and ShellCommands

/*
=================
blShellLocal::Event_SetShellCommand
add a new ShellCommand with a name
=================
*/
void blShellLocal::Event_SetShellCommand( idStr name ) {
    ShellCommand_AddShellCommand( name );
}

/*
=================
blShellLocal::Event_SubShellCommand
delete a new ShellCommand given a name
=================
*/
void blShellLocal::Event_SubShellCommand( idStr name ) {
    ShellCommand_RemoveShellCommand( name );
}

/*
=================
blShellLocal::Event_SetInputEventToShellCommand
add a new InputEvent to a ShellCommand with a given name
=================
*/
void blShellLocal::Event_SetInputEventToShellCommand( idStr givenName, int eventType, int event ) {
    InputEventRef_s referenceEvent;
    referenceEvent.eventType = eventType;
    referenceEvent.event = event;

    ShellCommand_AssociateInputEvent( givenName, referenceEvent );
}

/*
=================
blShellLocal::Event_SubInputEventFromShellCommand
delete a InputEvent from a ShellCommand with a given name
=================
*/
void blShellLocal::Event_SubInputEventFromShellCommand( idStr givenName, int eventType, int event ) {
    InputEventRef_s unwantedReferenceEvent;
    unwantedReferenceEvent.eventType = eventType;
    unwantedReferenceEvent.event = event;

    ShellCommand_DisassociateInputEvent( givenName, unwantedReferenceEvent )
}

/*

} /* namespace BFG */
