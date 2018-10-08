/*
 * shell.cpp
 *
 *  Created on: 10 d’ag. 2018
 *      Author: biel
 */

#include <shell/shell.h>

namespace BFG {

blShell::blShell() {
	Clear();
	//the menu system will be initiated much latter, for now we can init the background system.
	background = new( TAG_OLD_UI ) blBackground();
	shellActive = true;
}

blShell::~blShell() {
	Clear();
}

void blShell::Clear() {
	InTranstion = false;
	transition_ideal = TRANSITION_NONE;
	transition_current = TRANSITION_NONE;
	menuInFocus = false;
	background = NULL;
	shellActive = false;
	nonInterface = NULL;

}

void blShell::Update() {
	//draw the background if necessary
	if( ( background != NULL ) && ( background->IsActive() ) ) {
		background->Update();
	}
	//update the menu
	if( ( menu != NULL ) && ( menu->IsActive() ) ) {
		menu->Update();
	}
	//update the nonInterfaces
	if( ( nonInterface != NULL ) && ( nonInterface->IsActive() ) ) {
		nonInterface->Update();
	}
}

void blShell::Menu_Init( idSoundWorld* sw, idStr filename = NULL ) {
	menu = new(TAG_OLD_UI) blMenu( sw, filename );
}

void blShell::Menu_InitMenu() {
	menu->Menu_InitMenu();
}

void blShell::Menu_ClosePause() {
	menu->Menu_ClosePause();
}

void blShell::Menu_ClearRepeater() {
	menu->Menu_ClearRepeater();
}

void blShell::Menu_Toggle( bool show ) {
	menu->Menu_Toggle( show );
}

bool blShell::Menu_IsActive() {
	bool result = false;
	if( menu != NULL ) {
		result = menu->IsActive();
	}
	return result;
}

bool blShell::Menu_HandleGuiEvent( const sysEvent_t* sev ) {
	return menu->HandleGuiEvent( sev );
}

void blShell::Menu_SyncWithSession() {
	menu->Menu_SyncWithSession();
}

void blShell::Menu_UpdateSavedGames() {
	menu->Menu_UpdateSavedGames();
}

void blShell::Menu_UpdateClientCountdown( int countdown ) {
	menu->Menu_UpdateClientCountdown( countdown );
}

void blShell::Menu_UpdateLeaderboard( const idLeaderboardCallback* callback ) {
	menu->Menu_UpdateLeaderboard( callback );
}

bool blShell::Background_IsActive() {
	bool result = false;
	if( background != NULL ) {
		result = background->IsActive();
	}
	return result;
}

void blShell::Background_InitNone() {
	if( background != NULL ) {
		background->InitNone();
	}
}

void blShell::Background_InitColour( idVec4 Colour ) {
	if( background != NULL ) {
		background->InitColour( Colour );
	}
}

void blShell::Background_InitMaterial( idStr material_name, idVec4 Tint ) {
	if( background != NULL ) {
		background->InitMaterial( material_name, Tint );
	}
}

//NON-INTERFACE

bool blShell::NonInterface_IsActive() {
	bool result = false;
	if( nonInterface != NULL ) {
		result = nonInterface->IsActive();
	}
	return result;
}

void blShell::NonInterface_Init() {
	nonInterface = new( TAG_OLD_UI ) blNonInterface();
}

void blShell::NonInterface_Close() {
	delete nonInterface;
	nonInterface = NULL;
}

} /* namespace BFG */
