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
blShellLocal::Event_MainMenu_Toggle
==============
*/
int blShellLocal::Event_MainMenu_Toggle() {
    /*
    idSession::sessionState_t sessionState = session->GetState()
    if( sessionState != idSession::IDLE ) {
        session->MoveToMainMenu();
    } else {

    }
    //then let's start the content
    */
    ;
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
		session->SetState( newState );
	}
}

// InputsEvents and ShellCommands

/*
=================
blShellLocal::Event_Set_ShellCommand
add a new ShellCommand with a name
=================
*/
void blShellLocal::Event_Set_ShellCommand( idStr name ) {
    ShellCommand_AddShellCommand( name );
}

/*
=================
blShellLocal::Event_Sub_ShellCommand
delete a new ShellCommand given a name
=================
*/
void blShellLocal::Event_Sub_ShellCommand( idStr name ) {
    ShellCommand_RemoveShellCommand( name );
}

/*
=================
blShellLocal::Event_Get_ShellCommand_State
returns the state of a given ShellCommand or errors out
=================
*/
int blShellLocal::Event_Get_ShellCommand_State( idStr name ) {
    int state = ShellCommand_GetState( name );
    if( state == -1 ) {
        common->Error( "blShellLocal::Event_Get_ShellCommand_State : ShellCommand '%s' doesn't exist in the list!\n", name.c_str() );
    }
    return state;
}

/*
=================
blShellLocal::Event_Get_ShellCommand_value
returns the state of a given ShellCommand
CAUTION: the error is not well beheaved, it will error out in the ShellCommand_GetValue function
=================
*/
int blShellLocal::Event_Get_ShellCommand_value( idStr name ) {
    return ShellCommand_GetValue( name );
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
