/*
 * shell.cpp
 *
 *  Created on: 10 d’ag. 2018
 *      Author: biel
 */

#include <shell/shell.h>
#include "../shell/shell_flat_system.h"
#include "../idlib/StrStatic.h"
#include "../idlib/LangDict.h"
#include "../framework/FileSystem.h"
#include "../framework/DeclEntityDef.h"
#include "../framework/Common_local.h"

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
	bool specialCondition = false;
	idStr displayName;
	idStr subtitle;
	idStr desc;
	idStr tip;
	idVec4 tint = idVec4( 0.0, 0.0, 0.0, 1.0 );

	// use a systemic path for the material of the background image, this is not the images themselves but the material
	idStrStatic< MAX_OSPATH > stripped = mapName;
	stripped.StripFileExtension();
	stripped.StripPath();

	idStrStatic< MAX_OSPATH > matName = "guis/assets/loadscreens/";
	matName.Append( stripped );
	const idMaterial* mat = declManager->FindMaterial( matName );

	//renderSystem->LoadLevelImages();

	if( mat->GetImageWidth() < 32 )	{
		//default values
		mat = declManager->FindMaterial( "guis/assets/loadscreens/default" );
		//renderSystem->LoadLevelImages();
	}

	const idDeclEntityDef* mapDef = static_cast<const idDeclEntityDef*>( declManager->FindType( DECL_MAPDEF, mapName, false ) );
	if( mapDef != NULL ) {
		//do we want to tint the loading screen?
		tint = mapDef->dict.GetVec4( "tintLoading", "0 0 0 1" ); // the last digit acts as a full percentage of the whole tint

		//gather if we are in special map condition ( in doom3 "inhell" )
		specialCondition = ( mapDef->dict.GetBool( "hellMap", false ) || mapDef->dict.GetBool( "specCond", false ) );

		//generate the text to display in the load screen, a name for the level,
		//a subtitle for that name, a description of the place, and also a tip for the game.
		displayName = idLocalization::GetString( mapDef->dict.GetString( "name", mapName ) );

		//in single player the subtitle will feature a longer name for the map,
		//or if we are within a story a subtitle of the current chapter we're playing,
		//and the desc will feature the description of the location the map happens in.
		//but in multiplayer, subtitle will feature the game mode we're playing in, and desc will describe it.
		const idMatchParameters& matchParameters = session->GetActingGameStateLobbyBase().GetMatchParms();
		if( matchParameters.gameMode == GAME_MODE_SINGLEPLAYER ) {
			desc = idLocalization::GetString( mapDef->dict.GetString( "desc", "" ) );
			subtitle = idLocalization::GetString( mapDef->dict.GetString( "subTitle", "" ) );
		} else {
			const idStrList& modes = common->GetModeDisplayList();
			subtitle = modes[ idMath::ClampInt( 0, modes.Num() - 1, matchParameters.gameMode ) ];

			const char* modeDescs[] = { "#str_swf_deathmatch_desc", "#str_swf_tourney_desc", "#str_swf_team_deathmatch_desc", "#str_swf_lastman_desc", "#str_swf_ctf_desc" };
			desc = idLocalization::GetString( modeDescs[matchParameters.gameMode] );
		}
	}

	//TODO create a way to gather a tip from a list of tips that is not hard-coded and is multi-lingual
	tip = "press 'shift' to run.\n Running is like walking, only faster!\n";

	generatedData.Set( "background_material", mat->GetName() );
	//generatedData.Set( "background_tint", "0, 0, 0, 1.0" );
	generatedData.SetVec4("background_tint", tint );
	//generatedData.Set( "specialMap", specialCondition ? "True" : "False" ); //Esthetic or sistemic difference for a given map in the load screen
	generatedData.SetBool( "specialMap", specialCondition ); //Esthetic or sistemic difference for a given map in the load screen
	generatedData.Set( "title", displayName.c_str() ); //could be the name of the map or the narrative title associated to that map
	generatedData.Set( "subtitle", subtitle.c_str() ); //an accompanying subtitle
	generatedData.Set( "description", desc.c_str() ); // this could be used to describe the location where this map happen
	generatedData.Set( "tip", tip.c_str() );

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

bool blShell::LoadingMap_GetWaiting() {
	bool result = false;
	if( LoadingMap_IsActive() ) {
		result = loadingMap->GetWaiting();
	}
	return result;
}

void blShell::LoadingMap_SetWaiting( bool  Switch ) {
	if( LoadingMap_IsActive() ) {
		//in the case we clicked to close the Lading screen and play or we don't use this stage after all
		if( Switch == false ) {
			LoadingMap_Close();
		} else {
			loadingMap->SetWaiting( true );
		}
	}
}

} /* namespace BFG */
