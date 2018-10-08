/*
 * shell_menu.cpp
 *
 *  Created on: 24 de set. 2018
 *      Author: biel
 */

#include <shell/shell_menu.h>
#include "../d3xp/menus/MenuHandler_SWF.h"

namespace BFG {

idCVar ui_flat_SytemLevelThreshold( "ui_flat_SytemLevelThreshold", "-1", CVAR_INTEGER | CVAR_SYSTEM | CVAR_GUI, "set a forced system level threshold to be used in flat GUIs from a probable set of diverging capabilities, where:\n -1 --> imGUI\n 0 --> GUI\n 1 --> ceGUI\n 2 --> flash based GUI\n" );

blMenu::blMenu( idSoundWorld* sw, idStr filename ) {
	Clear();
	flatSystemLevel = Determine_the_System_level();
	SoundList.Clear();
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			menu_ImGui = new(TAG_OLD_UI) blImGuiMenu;
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				if( filename.Cmp( '\0' ) == 0 ) {
					common->Error( "blMenu: Improper filename!\n" );
				}
				shellHandler->Initialize( filename.c_str(), sw );
			}
			break;
	}
}

blMenu::~blMenu() {
	if( menu_ImGui != NULL ) {
		delete menu_ImGui;
	}
	if( shellHandler != NULL ) {
		delete shellHandler;
	}
	if( MenuSW != NULL ) {
		MenuSW->StopAllSounds();
		delete MenuSW;
	}
	Clear();
}

void blMenu::Clear() {
	shellHandler = NULL;
	menu_ImGui = NULL;
	//menu_Gui = NULL;
	menuActive = false;
	MP_time_remaining = 0;
	MenuSW = NULL;
	SoundList.Clear();
	flatSystemLevel = 0;
}

int blMenu::Determine_the_System_level() {
	bool	Gui_exists = false;
	int decide = ui_flat_SytemLevelThreshold.GetInteger();
	int result = decide;

	if( /*something yet to be determined*/ true ) { //TODO
		Gui_exists = false;
	}

	if( !Gui_exists ) {
		result =  FLAT_SYSTEM_IMGUI;
	} else {
		if( decide >= FLAT_SYSTEM_FLEXIBLE ) {
			result = decide;
		} else {
			result = FLAT_SYSTEM_IMGUI;
		}
	}
	return result;

}

void blMenu::HandleSoundList( idStr element, int item ) {
	int max = SoundList.Num();
	int index = item + 1;
	if( max == item ) {
		SoundList.Append( element );
	} else if( max < index ) {
		for( int i = max; max < index; i++ ) {
			if( i == item ) {
				SoundList.Append( element );
			} else {
				SoundList.Append( "\0" );
			}
		}
	} else {
		if( SoundList[ item ] != element ) {
			SoundList.RemoveIndex( item );
			SoundList.Insert( element, item );
		}
	}
}

bool blMenu::IsActive() {
	bool result = false;
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			result = menuActive;
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				result = shellHandler->IsActive();
			}
			break;
	}
	return result;
}

int blMenu::GetMenuType() {
	int result = MENU_INVALID;
	if( flatSystemLevel == FLAT_SYSTEM_IMGUI ) {
		result = menu_ImGui->getMenuType();
	}
	return result;
}

void blMenu::SetMenuType( int type ) {
	//menu_type = type;
	if( flatSystemLevel == FLAT_SYSTEM_IMGUI ) {
		menu_ImGui->SetMenuType( type );
	}
}
int blMenu::GetMenuScreen() {
	int result = SCREEN_INVALID;
	if( flatSystemLevel == FLAT_SYSTEM_IMGUI ) {
		result = menu_ImGui->getMenuScreen();
	}
	return result;
}

void blMenu::SetMenuScreen( int screen ) {
	//menu_screen = screen;
	if( flatSystemLevel == FLAT_SYSTEM_IMGUI ) {
		menu_ImGui->SetMenuScreen( screen );
	}
}

void blMenu::ToggleMenu( bool preference ) {
	if( menuActive ) {
		if( preference ) {
			return; //we want to be active but WE ARE already!
		}
		menuActive = false;
		common->FocusInputOnMenu( menuActive );
		if( flatSystemLevel == FLAT_SYSTEM_IMGUI ) {
			if( MenuSW != NULL ) {
				MenuSW->StopAllSounds();
			}
			common->Printf( "ToggleMenu: closing the menu!\n" );
			menu_ImGui->closeMenu();
		}
	} else {
		if( !preference ) {
			return;
		}
		idPlayer * mrP = gameLocal.GetLocalPlayer();
		if( common->IsInGame() && ( mrP != NULL ) && mrP->IsDead() && !common->IsMultiplayer() ) {
			return; //so if player is dead and in single player we Open menu //it should be the contrary! isn't it?
		}

		menuActive = true;
		common->FocusInputOnMenu( menuActive );
		if( flatSystemLevel == FLAT_SYSTEM_IMGUI ) {
			static int menuType = GetMenuType();
			if( ( MenuSW != NULL ) && ( ( menuType == MENU_MAIN ) || ( menuType == MENU_PAUSE ) ) ) {
				if( SoundList[UI_SOUND_MUSIC] != "\0" ) {
					MenuSW->PlayShaderDirectly( SoundList[UI_SOUND_MUSIC], -1 ); //<-- does it work?
				}
			}
			common->Printf( "ToggleMenu: opening the menu!\n" );
			menu_ImGui->openMenu();
		}
	}
}

void blMenu::Update() {
	flatSystemLevel = Determine_the_System_level();
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			menu_ImGui->Draw();
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				shellHandler->Update();
			}
			break;
	}

}

//void blMenu::Menu_InitMenu( bool inGame ) {
void blMenu::Menu_InitMenu() {
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			idSoundWorld* sw;
			const idSoundShader* 	s_shader;
			sw = NULL;
			s_shader = NULL;

			// should this call the reset function every time like the shell option do?

			SetMenuScreen( SCREEN_MAIN );
			if( !common->IsInGame() ) {
				SetMenuType( MENU_MAIN );
				sw = common->MenuSW();
				s_shader = declManager->FindSound( "Music/MainMenuMusic", true );
				if( s_shader != NULL ) {
					HandleSoundList( s_shader->GetName(), UI_SOUND_MUSIC );
				}
			} else {
				if( common->IsMultiplayer() ) {
					SetMenuType( MENU_PAUSE_MP );
					sw = common->SW();
				} else {
					SetMenuType( MENU_PAUSE );
					sw = common->MenuSW();
					s_shader = declManager->FindSound( "Music/PauseMenuMusic", true );
					if( s_shader != NULL ) {
						HandleSoundList( s_shader->GetName(), UI_SOUND_MUSIC );
					}
				}
			}
			if( MenuSW != sw) {
				MenuSW = sw;
			}
			menu_ImGui->Init();
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			// why reset the menu when creating it?
			// this was shell_ResetMenu() --> This is done because here, when creating the shell, it only initiates the assets,
			//								  the actual menu is "shown" on or off afterwards, and only then is activated as a menu properly.
			if( shellHandler != NULL ) {
				delete shellHandler;
				shellHandler = new( TAG_SWF ) idMenuHandler_Shell();
			}

			if( shellHandler != NULL ) {
				//if( inGame ) {
				if( common->IsInGame() ) {
					shellHandler->SetInGame( false );
					shellHandler->Initialize( "shell", common->MenuSW() );
				} else {
					shellHandler->SetInGame( true );
					if( common->IsMultiplayer() ) {
						shellHandler->Initialize( "pause", common->SW() );
					} else {
						shellHandler->Initialize( "pause", common->MenuSW() );
					}
				}
			}
			break;
	}
}

void blMenu::Menu_ClosePause() {
	if( !common->IsMultiplayer() && gameLocal.GetLocalPlayer() && gameLocal.GetLocalPlayer()->health <= 0 )	{
		return;
	}

	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			if( gameLocal.GetGameCoompleted() ) {
				return;
			}

			ToggleMenu( false );
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				if( shellHandler->GetGameComplete() ) {
					return;
				}

				shellHandler->SetNextScreen( SHELL_AREA_INVALID, MENU_TRANSITION_SIMPLE );
			}
			break;
	}
}

void blMenu::Menu_ClearRepeater() {
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			common->Printf( "Menu_ClearRepeater::ClearWidgetActionRepeater\n" );
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				shellHandler->ClearWidgetActionRepeater();
			}
			break;
	}
}

void blMenu::Menu_Toggle( bool show ) {
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			ToggleMenu( show );
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				shellHandler->ActivateMenu( show );
			}
			break;
	}
}

bool blMenu::HandleGuiEvent( const sysEvent_t* sev ) {
	bool result = false;
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			result = ImGuiHook::InjectSysEvent( sev );
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				result = shellHandler->HandleGuiEvent( sev );
			}
			break;
	}
	return result;
}

void blMenu::Menu_SyncWithSession() {
	idSession::sessionState_t sessionState = session->GetState();

	if( flatSystemLevel == FLAT_SYSTEM_IMGUI ) {
		//common->Printf( "UI_SyncWithSession: session is: " );
		switch( sessionState )
		{
			case idSession::PRESS_START: // press start menu
				//common->Printf( "PRESS_START. " );
				SetMenuType( MENU_MAIN );
				SetMenuScreen( SCREEN_MAIN );
				break;
			case idSession::INGAME:
				//common->Printf( "INGAME. " );
				if( menuActive ) {
					SetMenuType( MENU_PAUSE );
					SetMenuScreen( SCREEN_MAIN );
				} else {
					Menu_ClosePause();
				}
				break;
			case idSession::IDLE: //main menu
				//common->Printf( "IDLE. " );
				SetMenuType( MENU_MAIN );
				SetMenuScreen( SCREEN_MAIN );
				break;
			case idSession::PARTY_LOBBY:
				//common->Printf( "PARTY_LOBBY. " );
				//shellHandler->SetShellState( SHELL_STATE_PARTY_LOBBY );
				break;
			case idSession::GAME_LOBBY:
				//common->Printf( "GAME_LOBBY. " );
				//shellHandler->SetShellState( SHELL_STATE_GAME_LOBBY );
				break;
			case idSession::SEARCHING:
				//common->Printf( "SEARCHING. " );
				SetMenuType( MENU_LOADING );
				SetMenuScreen( SCREEN_MAIN );
				break;
			case idSession::LOADING:
				//common->Printf( "LOADING. " );
				SetMenuType( MENU_LOADING );
				SetMenuScreen( SCREEN_MAIN );
				break;
			case idSession::CONNECTING:
				//common->Printf( "CONNECTING. " );
				SetMenuType( MENU_LOADING );
				SetMenuScreen( SCREEN_MAIN );
				break;
			case idSession::BUSY:
				//common->Printf( "BUSY. " );
				SetMenuType( MENU_LOADING );
				SetMenuScreen( SCREEN_MAIN );
				break;
		}
		common->Printf( "Sync done!\n" );

	} else {
		if( shellHandler == NULL ) {
			return;
		}
		switch( sessionState ) {
			case idSession::PRESS_START:
				shellHandler->SetShellState( SHELL_STATE_PRESS_START );
				break;
			case idSession::INGAME:
				shellHandler->SetShellState( SHELL_STATE_PAUSED );
				break;
			case idSession::IDLE:
				shellHandler->SetShellState( SHELL_STATE_IDLE );
				break;
			case idSession::PARTY_LOBBY:
				shellHandler->SetShellState( SHELL_STATE_PARTY_LOBBY );
				break;
			case idSession::GAME_LOBBY:
				shellHandler->SetShellState( SHELL_STATE_GAME_LOBBY );
				break;
			case idSession::SEARCHING:
				shellHandler->SetShellState( SHELL_STATE_SEARCHING );
				break;
			case idSession::LOADING:
				shellHandler->SetShellState( SHELL_STATE_LOADING );
				break;
			case idSession::CONNECTING:
				shellHandler->SetShellState( SHELL_STATE_CONNECTING );
				break;
			case idSession::BUSY:
				shellHandler->SetShellState( SHELL_STATE_BUSY );
				break;
		}
	}
}

void blMenu::Menu_UpdateSavedGames() {
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			/*
			 * TODO: somehow devise and implement a way to list the saved games to show on the screen in both load and save situations // it's in old GUI code
			 */
			common->Printf( "Menu_UpdateSavedGames: oops no save games listed yet!\n" );
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				shellHandler->UpdateSavedGames();
			}
			break;
	}
}

void blMenu::Menu_UpdateClientCountdown( int countdown ) {
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			Set_MPtimeRemaining( countdown );
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				shellHandler->SetTimeRemaining( countdown );
			}
			break;
	}
}

void blMenu::Menu_UpdateLeaderboard( const idLeaderboardCallback* callback ) {
	switch( flatSystemLevel ) {
		case FLAT_SYSTEM_IMGUI:
			/*
			 * TODO: somehow devise and implement a way to list the MP leader board // it's in old GUI code
			 */
			common->Printf( "blUI_Handler: oops no leader board listed yet!\n" );
			break;
		case FLAT_SYSTEM_FLASH_SHELL:
			if( shellHandler != NULL ) {
				shellHandler->UpdateLeaderboard( callback );
			}
			break;
	}
}

} /* namespace BFG */
