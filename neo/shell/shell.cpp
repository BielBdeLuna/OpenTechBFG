/*
 * shell.cpp
 *
 *  Created on: 10 d’ag. 2018
 *      Author: biel
 */

#include <shell/shell.h>
#include "../shell/shell_flat_system.h"

namespace BFG {

blShell::blShell() {
	Clear();
	shellActive = true;
}

blShell::~blShell() {
	if( Background_IsActive() ) {
		Background_Close();
	}
	if( LoadingMap_IsActive() ) {
		LoadingMap_Close();
	}
	Clear();
}

void blShell::Clear() {
	InTranstion = false;
	transition_ideal = TRANSITION_NONE;
	transition_current = TRANSITION_NONE;
	menuInFocus = false;
	background = NULL;
	shellActive = false;
	loadingMap = NULL;

}

void blShell::Update() {
	//draw the background if necessary
	if( Background_IsActive() ) {
		background->Update();
	}
	//update the menu
	if( ( menu != NULL ) && ( menu->IsActive() ) ) {
		menu->Update();
	}
	//update the nonInterfaces
	if( LoadingMap_IsActive() ) {
		loadingMap->Update();
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

void blShell::Background_Init( idStr material_name, idVec4 tint ) {
	Background_Close();

	if( material_name.IsEmpty() ) {
		if( tint.Compare( idVec4( 0.0f ) ) == false ) { // the tint colour is not translucent therefore meaningless
			material_name = "_white";
		} else {
			return; //no background due the tint colour being meaningless
		}
	}

	blBackground background = new( TAG_OLD_UI ) blBackground( material_name, tint );
}


void blShell::Background_Close() {
	if( Background_IsActive() ) {
		delete background;
		background = NULL;
	}
}

//NON-INTERFACE

void blShell::LoadingMap_Init( idStr mapName ) {
	idDict data = LoadingMap_GatherData( mapName );

	//set a new background
	if( Background_IsActive() ) {
		Background_Close();
	}

	Background_Init( data.GetString( "background_material", "default_loading_background" ), data.GetVec4("background_tint", "0, 0, 0, 1.0" ) );

	//add the loading screen on top
	if( LoadingMap_IsActive() ) {
		loadingMap->SetWholeData( data );
	} else {
		loadingMap = new( TAG_OLD_UI ) blLoadingMap( data );
	}
}

idDict blShell::LoadingMap_GatherData( idStr mapName ) {
	idDict generatedData;

	//default values
	//generatedData.Set( "background_material", "default_loading_background" );
	//generatedData.Set( "background_tint", "0, 0, 0, 1.0" );
	generatedData.Set( "name", mapName );

	//TODO create a way to gather a tip from a list of tips that is not hard-coded and is multi-lingual
	generatedData.Set( "tip", "press 'shift' to run.\n Running is like walking, only faster!\n" );

	/*
	if( Determine_flat_System_level() != FLAT_SYSTEM_IMGUI ) {
		//TODO gather the name somewhat using the mapName
	} else {
		//TODO the same above

	}
	*/

	return generatedData;
}

void blShell::LoadingMap_SetPercentage( float _percentage ) {
	if( _percentage > 100.0f ) {
		_percentage = 100.0f;
	} else if( _percentage < 0.0f ) {
		_percentage = 0.0f;
	}

	loadingMap->percentage = _percentage;
}

void blShell::LoadingMap_Close() {
	if( LoadingMap_IsActive() ) {
		delete loadingMap;
		loadingMap = NULL;
	}

	Background_Close();

	//TODO return the focus of the controls from the shell to the game
}

} /* namespace BFG */
