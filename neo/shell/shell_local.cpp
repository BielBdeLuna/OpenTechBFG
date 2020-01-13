/*
 * shell_local.cpp
 *
 *  Created on: 10 d’ag. 2018
 *      Author: Biel Bestué de Luna
 */

#include <shell/shell_local.h>
#include "../framework/CmdSystem.h"

namespace BFG {
int blShellAsync::Run() {
	/*
	this->WaitForThread();
	this->SignalWork()
	*/

	while( true ) {
		/*
		shell->SetGuiPrepStart();

		//get events for the shell
		sysEvent_t* sev = Sys_GetEvent();

		//update the shell as well as make use of the events it has been passed with.
		shell->Asynch_update( sev );
		*/



        //SHELL BASE

        // find the current value for every InputEvent in the ShellInputEvents_l list
        shell->ShellInputEvents_Synch();

		// gather all the status and values of every command given the new values of the inputEvents
        shell->ShellCommand_Sync();

		// run branches given the new values of the ShellCommands
		shell->AsyncFrame_RunBranchLists();

	}
	return 0;
}

/*
=================
blShellLocal::Init
=================
*/
void blShellLocal::Init() {
	Clear();

	/*

	guiTimes.prep_start_time = 0;
	guiTimes.gui_render_start_time = 0;
	guiTimes.gui_render_end_time = 0;

	menuSW = soundSystem->AllocSoundWorld( NULL );
	menuSW->PlaceListener( vec3_origin, mat3_identity, 0 );
	*/

	//--------------------------

	//int MaxJobs = 16;

	//ParallelBranchJobs_l = parallelJobManager->AllocJobList( JOBLIST_SHELL_GUI, JOBLIST_PRIORITY_LOW, MaxJobs, 0, NULL );

	/*
		an example of how to submit a job:

		for( viewLight_t* vLight = tr.viewDef->viewLights; vLight != NULL; vLight = vLight->next ) {
			tr.frontEndJobList->AddJob( ( jobRun_t )R_AddSingleLight, vLight );
		}
		tr.frontEndJobList->Submit();
		tr.frontEndJobList->Wait();

	*/
}

//DELETE
/*
void blShellLocal::InitShellManager() {

	StretchRenderFeed_l.Clear(); //it should already be clear but make sure, for sure, so it is sure-sure

	int stackSize = 0x100000; //TODO measure the stack size, this is a meg
	shellThread.StartWorkerThread( "UI logic", CORE_2B, THREAD_BELOW_NORMAL, stackSize );
}
*/

//DELETE
/*
void blShellLocal::InitFrontEnd() {
	//init the managerial frontend of the shell

	StartUiManager();
	int stackSize = 0x100000; //TODO measure the stack size
	shellThread.StartWorkerThread( "UI logic", CORE_2B, THREAD_BELOW_NORMAL, stackSize );
}
*/

//DELETE
/*
void blShellLocal::UpdateFrontEnd() {
	;
}
*/

//DELETE
/*
void blShellLocal::InitBackEnd() {
	StretchRenderFeed_l.Clear(); //it should already be clear but make sure, for sure, so it is sure-sure

}
*/

//DELETE
/*
void blShellLocal::UpdateBackEnd() {
	;
}
*/

void blShellLocal::Shutdown() {
	printf( "Shell Shutting down;\n" );

	/*
	if( splashContent != NULL ) {
		delete splashContent;
	}
	if ( guiMainMenu_MapList != NULL ) {
		guiMainMenu_MapList->Shutdown();
		uiManager->FreeListGUI( guiMainMenu_MapList );
		guiMainMenu_MapList = NULL;
	}
	*/
	/*
	if( shellThread.IsRunning() ) {
		shellThread.StopThread();
	}
	*/
	//delete menuSW;

	branches_l.DeleteContents();
	Clear();

	//-----------------------------------------------------------

	//parallelJobManager->FreeJobList( ParallelBranchJobs_l );
}

/*
=================
blShellLocal::Clear
=================
*/
void blShellLocal::Clear() {

	branches_active_l.Clear();
	branches_paused_l.Clear();
	branches_l.Clear();

	Shell_event_commands_l.Clear();
	gui_l.Clear();
	guiActive_l.Clear();

	MainMenu_isOpen = false;

	//guiActive = NULL;
	//guiHandle = NULL;

	/*
	shellSplashStagesFeed_l.Clear();
	StretchRenderFeed_l.Clear();
	splashContent = NULL;
	uiManager = NULL;
	menuSW = NULL;
	uiSoundArray.Zero();
	menuActive = false;

	currentShellSplashStage.material = "";
	currentShellSplashStage.Interrupt_events_l.Clear();
	currentShellSplashStage.aspectRatio = 0.0f;
	currentShellSplashStage.elapsedTime = 0;
	currentShellSplashStage.startTime = 0;
	currentShellSplashStage.backgroundColour = idVec4( 0.0f );
	currentShellSplashStage.tintColour = idVec4( 0.0f );
	*/

	/*
	guiInGame = guiMainMenu = guiIntro = guiRestartMenu = NULL;
	guiLoading = guiGameOver = guiActive = guiTest = NULL;
	guiMsg = guiMsgRestore = guiTakeNotes = NULL;
	*/

	//uiArray.Zero();
}

/*
=================
blShellLocal::Render_RenderCommandBuffers
=================
*/
void blShellLocal::Render_RenderCommandBuffers() {
    const emptyCommand_t* cmd = renderSystem->SwapCommandBuffers( common->Pt_TimeFrontEnd, common->Pt_TimeBackEnd, common->Pt_TimeShadows, common->Pt_TimeGpu );
	renderSystem->RenderCommandBuffers( cmd );
}

extern idCVar sys_lang;
extern idCVar com_skipIntroVideos;
extern idCVar com_skipDemosForMenu;

/*
void blShellLocal::FeedSplashStage( idMaterial* mat, float ar, idVec4 tCol, idVec4 bCol, int time, idList<ShellInterruptEvent> interrupt ) {
    ;
}
*/

//DELETE
/*
void blShellLocal::FeedCurrentStage() {
	if( shellSplashStagesFeed_l.Num <= 0 ) {
		common->Error("blShellLocal::FeedCurrentStage() No more stages for the menu in the stages list!");
	}
	currentShellSplashStage = shellSplashStagesFeed_l[0];
	currentShellSplashStage.startTime = Sys_Milliseconds(); //at the time we call the next stage we mark at which point in time we started it

	//in case of videos we should reset their time so they start to play now
	if( currentShellSplashStage.material->GetName() == "splashVideo" ){
		currentShellSplashStage.material->ResetCinematicTime( Sys_Milliseconds() );
	}

	if( shellSplashStagesFeed_l.Num  > 1 ) {
		shellSplashStagesFeed_l.RemoveIndex( 0 );
	} else {
		shellSplashStagesFeed_l.Clear();
	}
}
*/

//DELETE
/*
void blShellLocal::InitFirstSplashContent() {
	//TODO this should all be read from a place like the autoexec.cfg file

	float ar169 = ( 16.0f / 9.0f );
	idMaterial* splashMaterial;


	//the initial splash video stage

	if( !com_skipIntroVideos.GetBool() && fileSystem->UsingResourceFiles() )	{
		idStr materialText;
		materialText.Format( "{ translucent { videoMap %s } }", "video\\loadvideo.bik" );

		splashMaterial = const_cast<idMaterial*>( declManager->FindMaterial( "splashVideo" ) );
		splashMaterial->Parse( materialText.c_str(), materialText.Length(), false );

		// RB: FFmpeg might return the wrong play length so I changed the intro video to play max 30 seconds until finished
		int cinematicLength = splashMaterial->CinematicLength();
		cinematicLength = 30000; //so we don't care  if FFmpeg gave correct time or not, we will kill it if it stops playing anyway

		FeedSplashStage( splashMaterial, ar169, colorBlack, colorBlack, cinematicLength, true ); /// last "true" should now be a list!!!!!

	} else {
		idLib::Printf( "Skipping the showing of the initial splash video\n" );
		//so display the legal splash screen
	}


	//the legal splash screen stage

	if( idStr::Icmp( sys_lang.GetString(), ID_LANG_FRENCH ) == 0 ) {
		// If the user specified french, we show french no matter what SKU
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_francophonie" );
	} else if( idStr::Icmp( Sys_DefaultLanguage(), ID_LANG_FRENCH ) == 0 ) {
		// If the lead sku is french (ie: europe), display figs
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_euro_figs" );
	} else {
		// Otherwise show it in english
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_anglosphere" );
	}
	FeedSplashStage( splashMaterial, ar169, colorBlack, colorBlack, 4000, false ); //lasts 4 seconds /// last "true" should now be a list!!!!!


	//then a possible generic lodaing screen for the case the demos out of the main menu haven't finished loading

	splashMaterial = declManager->FindMaterial( "guis/ui/splash/generic_loading" );
	FeedSplashStage( splashMaterial, ar169, colorBlack, colorBlack, 0, false ); //lasts as long as it needs to last /// last "true" should now be a list!!!!!


	//then we select the first of the list of stages and set it as current

	FeedCurrentStage();
}
*/

//DELETE
/*
bool blShellLocal::HasBeenInterrupted( const sysEvent_t* sev ) {
	bool Interrupt = false;

	//first keyboard events
	if( sev->IsKeyEvent() ) {
		if ( sev->IsKeyDown() && ( ( sev->GetKey() == K_ESCAPE ) || ( sev->GetKey() == K_JOY13 ) ) ) {
			return true;
		}
	}

	//then mouse events
	if( sev->IsMouseEvent() ) {
		int	mouseEvents[MAX_MOUSE_EVENTS][2];
		int numMouseEvents = Sys_PollMouseInputEvents( mouseEvents );
		//if( numMouseEvents > 0 ) {
			for( int i = 0; i < numMouseEvents; i++ ) {
				//int action = mouseEvents[i][0];

				if ( mouseEvents[i][0] == M_ACTION8 ) {	//why only mouse action 8 and why when Action8 is off? IDK
					Interrupt = true;
					break;
				}

				//switch( action ) {
				//	case M_ACTION1:
				//	case M_ACTION2:
				//	case M_ACTION3:
				//	case M_ACTION4:
				//	case M_ACTION5:
				//	case M_ACTION6:
				//	case M_ACTION7:
				//	case M_ACTION8:
				//		return true;
				//		break;
				//	default:	// some other undefined button
				//		break;
				//}

			}
		//}
	}

	//and eventually joystick events
	if( sev->IsJoystickEvent() ) {
		int numJoystickEvents = Sys_PollJoystickInputEvents( 0 );
		//if( numJoystickEvents > 0 ) {
			for( int i = 0; i < numJoystickEvents; i++ ) {
				int action;
				int value;
				if( Sys_ReturnJoystickInputEvent( i, action, value ) ) {
					if( ( action >= J_ACTION1 ) && ( action <= J_ACTION_MAX ) ) {
						Interrupt = ( value != 0 );
						break;
					}
				}
			}

			Sys_EndJoystickInputEvents();
		//}
	}

	return Interrupt;
}
*/

//DELETE
/*
bool blShellLocal::HasBeenInterrupted( const sysEvent_t* sev ) {
	int	mouseEvents[MAX_MOUSE_EVENTS][2];

	Sys_GenerateEvents();

	// FIXME: DG: not sure the following code is the right way to do this...

	// queue system events ready for polling
	Sys_GetEvent();

	// RB: allow to escape video by pressing anything
	//poll keyboard events
	int numKeyEvents = Sys_PollKeyboardInputEvents();
	if( numKeyEvents > 0 ) 	{
		for( int i = 0; i < numKeyEvents; i++ ) {
			int key;
			bool state;

			if( Sys_ReturnKeyboardInputEvent( i, key, state ) ) {
				if( key == K_ESCAPE && state == true ) {
					return true;
				}
				break;
			}
		}
		Sys_EndKeyboardInputEvents();
	}

	//poll mouse events
	int numMouseEvents = Sys_PollMouseInputEvents( mouseEvents );
	if( numMouseEvents > 0 ) {
		for( int i = 0; i < numMouseEvents; i++ ) {
			int action = mouseEvents[i][0];
			switch( action ) {
				case M_ACTION1:
				case M_ACTION2:
				case M_ACTION3:
				case M_ACTION4:
				case M_ACTION5:
				case M_ACTION6:
				case M_ACTION7:
				case M_ACTION8:
					return true;
					break;
				default:	// some other undefined button
					break;
			}
		}
	}

	//poll joystick events
	int numJoystickEvents = Sys_PollJoystickInputEvents( 0 );
	if( numJoystickEvents > 0 ) {
		for( int i = 0; i < numJoystickEvents; i++ ) {
			int action;
			int value;

			if( Sys_ReturnJoystickInputEvent( i, action, value ) ) {
				if( action >= J_ACTION1 && action <= J_ACTION_MAX ) {
					if( value != 0 ) {
						return true;
						break;
					}
				}
			}
		}

		Sys_EndJoystickInputEvents();
	}

	return false;
}
*/

//DELETE
/*
void blShellLocal::Synch_update() {
	if( StretchRenderFeed_l.Num() <= 0 ) {
		return;
	} else {
		for( int i = 0; i <= ( StretchRenderFeed_l.Num() - 1 ); i++ ) {
			stretchedRenderTarget_t item = StretchRenderFeed_l[i];
			renderSystem->SetColor( item.tintColour );
			renderSystem->DrawStretchPic( item.area, item.UVarea, item.material );
			common->ForceRenderSystemSwapCommandBuffer();
			item.material->FreeData();
		}

		StretchRenderFeed_l.Clear();
	}
}
*/

//DELETE
/*
void blShellLocal::Asynch_update( const sysEvent_t* sev  ) {

	//this update tick should be called only after the session has been initiated
	int sessionState = Event_Get_SessionState();

	if( sessionState == idSession::PRESS_START ) {
		//before the main menu when first opening the game
		if( shellSplashStagesFeed_l.Num  == 1 ) {
			//the case where we still need time to load the demos under the Main Menu
			if( !com_skipDemosForMenu ) { //TODO check if the demos have been loaded yet
				splashContent->Update( currentShellSplashStage.material,
									   currentShellSplashStage.aspectRatio,
									   currentShellSplashStage.tintColour,
									   currentShellSplashStage.backgroundColour
									 );
			}
			//TODO session->MoveToMainMenu(); // this should change the session to the main menu

		} else if( shellSplashStagesFeed_l.Num  > 1 ) {
			bool time_expired, interrumpted, must_update = false;

			if( splashContent == NULL ) {
				splashContent = new blShellSplash();
				splashContent->Init( currentShellSplashStage.material,
									 currentShellSplashStage.aspectRatio,
									 currentShellSplashStage.tintColour,
									 currentShellSplashStage.backgroundColour
								   );
				must_update = false;
			} else {

				//test if we need to update

				//if time for it has expired
				time_expired = ( guiTimes.prep_start_time > ( currentShellSplashStage.startTime + currentShellSplashStage.elapsedTime ) );

				must_update = time_expired;

				//if it's still playing the cinematic or it's amount of frames have ended
				if( ( currentShellSplashStage.material->GetName() == "splashVideo" ) && !time_expired ) {
					must_update = ( must_update || !currentShellSplashStage.material->CinematicIsPlaying() );
				}

				//test if it has been interrupted
				if( ( currentShellSplashStage.ShellInterruptEvent.num() > 0  ) && !time_expired ) {
					must_update = must_update || HasBeenInterrupted( sev );
				}
			}

			if( must_update ) {
				if( shellSplashStagesFeed_l.Num < 1 ) {
					delete splashContent;
					InitGuis();
					MoveSessionToIdle();
				} else {
					FeedCurrentStage();
					splashContent->Update( currentShellSplashStage.material,
										   currentShellSplashStage.aspectRatio,
										   currentShellSplashStage.tintColour,
										   currentShellSplashStage.backgroundColour
										 );
				}
			}
		}

	} else if( sessionState == idSession::IDLE ) {
		if( sev->IsKeyEvent() ) {
			if ( sev->IsKeyDown() && ( ( sev->GetKey() == K_ESCAPE ) || ( sev->GetKey() == K_JOY13 ) ) ) {
				if( console->Active() ) {
					console->Close();
				}
			}
		}

		if( !Check_MenuActive() ) {
			OpenMenu();
		}

	} else if( sessionState == idSession::INGAME ) {
		//while in game, opening the main menu
		common->Printf( "Session synchronized while at INGAME.\n" );
		if( sev->IsKeyEvent() ) {
			if ( sev->IsKeyDown() && ( ( sev->GetKey() == K_ESCAPE ) || ( sev->GetKey() == K_JOY13 ) ) ) {
				if( console->Active() ) {
					console->Close();
				} else {
					if( Check_MenuActive() ) {
						CloseMenu();
					} else {
						OpenMenu();
					}
				}
			}
		}
	} else if( sessionState == idSession::PARTY_LOBBY ) {
		// when waiting for the game to start in a ctf game?
		common->Printf( "PARTY_LOBBY. " );
		//shellHandler->SetShellState( SHELL_STATE_PARTY_LOBBY );
	} else if( sessionState == idSession::GAME_LOBBY ) {
		// when waiting for the game to start
		common->Printf( "GAME_LOBBY. " );
		//shellHandler->SetShellState( SHELL_STATE_GAME_LOBBY );
	} else if( sessionState == idSession::SEARCHING ) {
		//searching servers?
		common->Printf( "SEARCHING. " );
		//SetMenuType( MENU_LOADING );
		//SetMenuScreen( SCREEN_MAIN );
	} else if( sessionState == idSession::LOADING ) {
		//when loading a map or a demo
		common->Printf( "LOADING. " );
		//SetMenuType( MENU_LOADING );
		//SetMenuScreen( SCREEN_MAIN );
	} else if( sessionState == idSession::CONNECTING ) {
		//when connecting to a server
		common->Printf( "CONNECTING. " );
		//SetMenuType( MENU_LOADING );
		//SetMenuScreen( SCREEN_MAIN );
	} else if( sessionState == idSession::BUSY ) {
		common->Printf( "BUSY. " );
		//SetMenuType( MENU_LOADING );
		//SetMenuScreen( SCREEN_MAIN );
	}

	guiTimes.gui_render_start_time = Sys_Milliseconds();
	if( splashContent ) {
		splashContent->PrepareForRendering();
	}

	//if( !currentShellStage.isMenu ) {
	//	bool time_expired, interrumpted, must_update = false;
	//	if( splashContent == NULL ) {
	//		splashContent = new blShellSplash();
	//		splashContent->Init( currentShellStage.material,
	//							 currentShellStage.aspectRatio,
	//							 currentShellStage.tintColour,
	//							 currentShellStage.backgroundColour
	//						   );
	//		must_update = false;
	//	} else {
	//		time_expired = ElapsedTimeHasExpired( currentShellStage.startTime + currentShellStage.elapsedTime );

	//		if( ( currentShellStage.ShellInterruptEvent.num() > 0 ) && !time_expired ) {
	//			interrumpted = HasBeenInterrupted();
	//		}

	//		must_update = ( time_expired || PlayTimeHasExpired( currentShellStage.material ) || interrumpted );
	//	}

	//	if( must_update ) {
	//		FeedCurrentStage();
	//		if( currentShellStage.isMenu ) {
	//			continue;
	//		} else {
	//			splashContent->Update( currentShellStage.material,
	//								   currentShellStage.aspectRatio,
	//								   currentShellStage.tintColour,
	//								   currentShellStage.backgroundColour
	//								 );
	//		}
	//	}

	//	guiTimes.gui_render_time = Sys_Milliseconds();
	//	splashContent->PrepareForRendering();

	//} else {
		//is a menu

		//in strange case of another splash stage when we are in a menu stage we go back to splash rendering again.
	//	if( shellStagesFeed_l.Num > 1 ) {
	//		FeedCurrentStage();
	//	}

		//is the menu initiated?
	//	if( !Check_MenuActive() && ( guiActive == NULL ) ) {
	//		InitMainMenu();
	//	}

		//if we press ESC or "menu" button ingame

	//	if( game && game->IsInGame() ) {
	//		if( sev->evType == SE_KEY && sev->evValue2 == 1 && ( sev->evValue == K_ESCAPE || sev->evValue == K_JOY9 ) )	{
	//			if( game->CheckInCinematic() == false ) {
	//				if( idCommonLocal::MenuEvent( sev ) ) {
	//					return;
	//				}

	//				if( Check_MenuActive() ){
	//					Printf( "Process Event: start menu should start as menu is not active!\n" );
	//					StartMenu();
	//					return; //true;
	//				} else {
	//					Printf( "Process Event: since menu is active, we shall close it!\n" );
	//					game->shell_menu_ClosePause();
	//				}
	//			}
	//		}

	//	} else {
	//		if( idCommonLocal::MenuEvent( sev ) ) {
	//			return;
	//		}
	//	}
	//}

}
*/

//DELETE
/*
void blShellLocal::MoveSessionToIdle() {

	//if we are already in the main menu or ingame and we wish to go to idle main menu
	if( game || Check_MenuActive() ) {
		CloseMenu();
	}

	uiSoundArray[UI_SOUND_MUSIC_MAINMENU] = "music/mainMenu_theme"; //go back to the idle main menu music

	//anyways we go to idle session
	session->MoveToMainMenu();
}
*/

struct stretchedRenderTarget_t
{
	static idMaterial*	material;
	idVec4				tintColour;
	idVec4 				area;
	idVec4				UVarea;
};

//DELETE
/*
void blShellLocal::FeedTheStretchRenderList( idMaterial* mat, idVec4 tCol, idVec4 _area, idVec4 _UVarea ) {
	stretchedRenderTarget_t newTarget;
	newTarget.material = mat;
	newTarget.tintColour = tCol;
	newTarget.area = _area;
	newTarget.UVarea = _UVarea;

	StretchRenderFeed_l.Append( newTarget );
}
*/

//DELETE
/*
void blShellLocal::RenderSplashImages() {
    ;
}
*/

//DELETE
/*
idUserInterface* blShellLocal::GetActiveMenu( void ) {
	return uiArray[UI_ACTIVE];
}
*/

//DELETE
/*
void blShellLocal::SetGuiReference( int element, idUserInterface* ref ) {
	if( ( element > NUM_UI_ELEMENTS ) || ( element < 0 ) ) {
		common->Printf( "WARNING blShellLocal::SetGuiReference: incorrect element!\n" );
		return;
	}
	uiArray[element] = ref;
}
*/


//DELETE
/*
void blShellLocal::SetGui( idUserInterface *gui, HandleGuiCommand_t handle ) {
	uiArray[UI_ACTIVE] = gui;
	guiHandle = handle;

	if ( uiArray[UI_ACTIVE] == NULL ) {
		return;
	}

	if ( uiArray[UI_ACTIVE] == uiArray[UI_MAINMENU] ) {
		//SetSaveGameGuiVars();
		//SetMainMenuGuiVars();
	} else if ( uiArray[UI_ACTIVE] == uiArray[UI_DEAD] ) {
		//SetSaveGameGuiVars();
	}

	sysEvent_t  ev;
	memset( &ev, 0, sizeof( ev ) );
	ev.evType = SE_NONE;

	//int time = idCommonLocal::frameTiming.startGameTime; //FIXME figure out at which time it will be called
	int time = Sys_Microseconds();

	uiArray[UI_ACTIVE]->HandleEvent( &ev, time );
	uiArray[UI_ACTIVE]->Activate( true, time );
}
*/

//DELETE
/*
bool blShellLocal::Check_MenuActive() {
	return( ( GetActiveMenu() == uiArray[UI_MAINMENU] ) && ( uiArray[UI_MAINMENU] != NULL ) );
};
*/

//DELETE
/*
void blShellLocal::OpenMenu() {
	if( ( common->Game() != NULL ) && Check_MenuActive() ) {
		return;
	}

	if( common->ReadDemo() != NULL ) {
		common->StopPlayingDemo();
	}

	if( ( common->SW() != NULL ) && !common->SW()->IsPaused() ) {
		common->SW()->Pause();
	}

	soundSystem->SetPlayingSoundWorld( menuSW );

	//play music
	if( menuSW != NULL ) {
		if( Event_Get_SessionState() == idSession::IDLE ) {
			if( uiSoundArray[UI_SOUND_MUSIC_MAINMENU] != "\0" ) {
				menuSW->PlayShaderDirectly( uiSoundArray[UI_SOUND_MUSIC_MAINMENU], -1 );
			}
		}
	}


	SetGui( uiArray[UI_MAINMENU], guiHandle );
	uiArray[UI_MAINMENU]->HandleNamedEvent( "restart" );
}
*/

//DELETE
/*
void blShellLocal::CloseMenu() {
	uiArray[UI_ACTIVE] = NULL;
	menuSW->StopAllSounds();

	if( common->SW() != NULL ) {
		if( common->SW()->IsPaused() ) {
			common->SW()->UnPause();
		}
		soundSystem->SetPlayingSoundWorld( common->SW() );
	}

}
*/

//DELETE
/*
void blShellLocal::InitGuis() {

	SetGuiReference( UI_MAINMENU, uiManager->FindGui( "guis/mainmenu.gui", true, false, true ) );
	guiMainMenu_MapList = uiManager->AllocListGUI();
	guiMainMenu_MapList->Config( uiArray[UI_MAINMENU], "mapList" );
	//idAsyncNetwork::client.serverList.GUIConfig( guiMainMenu, "serverList" ); //TODO add the list of servers to the main menu
	//SetGuiReference( UI_DEAD, uiManager->FindGui( "guis/killed.gui", true, false, true ) ); //dead screen
	SetGuiReference( UI_DEAD, NULL ); //dead screen
	SetGuiReference( UI_LOADING, NULL );
	uiArray[UI_INGAME_HUD] = NULL; //ingame HUD
	uiArray[UI_INGAME_TRANSCRIPT] = NULL;
	uiArray[UI_INGAME_INVENTORY] = NULL;
	uiArray[UI_INGAME_MAP] = NULL;
	uiArray[UI_INGAME_TALK] = NULL;
	uiArray[UI_MSG] = NULL; 	//any announcement that gets focus in the menu (when opening a new session, or when getting into a server)
	uiArray[UI_TEST] = NULL; 	//a space for testing GUIs


	uiArray[UI_ACTIVE] = NULL; 	//the UI_ACTIVE is NULL at the moment;
	guiHandle = NULL;

	uiSoundArray[UI_SOUND_MUSIC_MAINMENU] = "music/mainMenu_theme";
}
*/

/*
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
*/

/*
=================
blShellLocal::Render_SingleMaterial
=================
*/
void blShellLocal::Render_SingleMaterial( ShellSplashStageContent_t content ) {
	idMaterial* tintMat = declManager->FindMaterial( "_white" );

	const float sysWidth = renderSystem->GetWidth() * renderSystem->GetPixelAspect();
	const float sysHeight = renderSystem->GetHeight();
	const float sysAspect = sysWidth / sysHeight;
	const float adjustment = sysAspect / content.aspectRatio; //bigger than 1 if content is wider than sysAspect else if taller, one if both aspects are the same

	const float imageSize;
	const float chop;
	if( adjustment >= 1.0f ) {
		imageSize = renderSystem->GetVirtualWidth() * content.aspectRatio / sysAspect;
		chop = 0.5f * ( renderSystem->GetVirtualWidth() - imageSize );
	} else {
		imageSize = renderSystem->GetVirtualHeight() * content.aspectRatio / sysAspect;
		chop = 0.5f * ( renderSystem->GetVirtualHeight() - imageSize );
	}
	const float barHeight = ( adjustment >= 1.0f ) ? 0.0f : ( 1.0f - adjustment ) * ( float )renderSystem->GetVirtualHeight() * 0.25f;
	const float barWidth = ( adjustment <= 1.0f ) ? 0.0f : ( adjustment - 1.0f ) * ( float )renderSystem->GetVirtualWidth() * 0.25f;


	/*
	idStr materialText;
	materialText.Format( "{ translucent { videoMap %s } }", path );

	idMaterial* material = const_cast<idMaterial*>( declManager->FindMaterial( "splashbink" ) );
	material->Parse( materialText.c_str(), materialText.Length(), false );
	material->ResetCinematicTime( Sys_Milliseconds() );


	// RB: FFmpeg might return the wrong play length so I changed the intro video to play max 30 seconds until finished
	int cinematicLength = material->CinematicLength();
	cinematicLength = 30000; //so we don't care  if FFmpeg gave correct time or not, we will kill it if it stops playing anyway
	int	mouseEvents[MAX_MOUSE_EVENTS][2];
	*/

	//then the actual image with the correct aspect ration it should have

	if( idStr::Icmp( content.material->GetName(), "splashVideo" ) == 0 ) {
		// is a video

		renderSystem->SetColor( content.tintColour );
		if( adjustment >= 1.0f ) {
			//wider
			renderSystem->DrawStretchPic( chop, 0, imageSize, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, content.material );
		} else {
			//taller
			renderSystem->DrawStretchPic( 0, chop, renderSystem->GetVirtualHeight(), imageSize, 0, 0, 1, 1, content.material );
		}

	} else {
		// is not a video

		//first the black sides of the image for those images whose aspect ration doesn't correspond to the renderSystem's
		if( barHeight > 0.0f ) {
			renderSystem->SetColor( content.backgroundColour );
			renderSystem->DrawStretchPic( 0, 0, renderSystem->GetVirtualWidth(), barHeight, 0, 0, 1, 1, tintMat );
			renderSystem->DrawStretchPic( 0, renderSystem->GetVirtualHeight() - barHeight, renderSystem->GetVirtualWidth(), barHeight, 0, 0, 1, 1, tintMat );
		} if( barWidth > 0.0f ) {
			renderSystem->SetColor( content.backgroundColour );
			renderSystem->DrawStretchPic( 0, 0, barWidth, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, tintMat );
			renderSystem->DrawStretchPic( renderSystem->GetVirtualWidth() - barWidth, 0, barWidth, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, tintMat );
		}

		renderSystem->SetColor( content.tintColour );
		renderSystem->DrawStretchPic( barWidth, barHeight, renderSystem->GetVirtualWidth() - barWidth * 2.0f, renderSystem->GetVirtualHeight() - barHeight * 2.0f, 0, 0, 1, 1, content.material );
	}
	Render_RenderCommandBuffers();
}

/*
=================
blShellLocal::Check_SystemEvents
=================
*/

bool blShellLocal::Check_SystemEvents( idList<ShellEvent> PossibleSystemEvents_l ) {
	bool escapeEvent = false;

	Sys_GenerateEvents();

	// queue system events ready for polling
	Sys_GetEvent();

	for( int i = 0; i < PossibleSystemEvents_l.Num(); i++ ) {
		if( PossibleSystemEvents_l[i].EventType == SE_KEY ) {
			int numKeyEvents = Sys_PollKeyboardInputEvents();
			if( numKeyEvents > 0 ) {
				for( int i = 0; i < numKeyEvents; i++ )	{
					int key;
					bool state;

					if( Sys_ReturnKeyboardInputEvent( i, key, state ) ) {
						if( ( key == PossibleSystemEvents_l[i].Event ) && state ) {
							escapeEvent = true;
							break;
						}
					}
				}

				Sys_EndKeyboardInputEvents();
			}
		} else if( PossibleSystemEvents_l[i].EventType == SE_MOUSE ) {
			int	mouseEvents[MAX_MOUSE_EVENTS][2];
			int numMouseEvents = Sys_PollMouseInputEvents( mouseEvents );
			if( numMouseEvents > 0 ) {
				for( int i = 0; i < numMouseEvents; i++ ) {
					int action = mouseEvents[i][0];
					if( action == PossibleSystemEvents_l[i].Event ) {
						escapeEvent = true;
						break;
					}
				}
			}
		} else if( PossibleSystemEvents_l[i].EventType == SE_JOYSTICK ) {
			int numJoystickEvents = Sys_PollJoystickInputEvents( 0 );
			if( numJoystickEvents > 0 ) {
				for( int i = 0; i < numJoystickEvents; i++ ) {
					int action;
					int value;

					if( Sys_ReturnJoystickInputEvent( i, action, value ) ) {
						if( ( action == PossibleSystemEvents_l[i].Event ) && ( value != 0 ) ) {
							escapeEvent = true;
							break;
						}
					}
				}

				Sys_EndJoystickInputEvents();
			}
		}
	}
	return escapeEvent;

}

//shell innards

/*
=================
blShellLocal::InhibitUserCommandsGeneration
=================
*/
void blShellLocal::InhibitUserCommandsGeneration( bool force_off ) {
	common->Printf( "blShellLocal::InhibitUserCommandsGeneration : is forcing the User Command Generation " );
	common->Printf( force_off ? "off.\n" : "on.\n" );
	usercmdGen->InhibitUsercmd( INHIBIT_SESSION, force_off );
}

/*
=================
blShellLocal::ToggleMouseCursorFocus
=================
*/
void blShellLocal::ToggleMouseCursorFocus( bool grabMouse ) {
	common->Printf( "blShellLocal::GrabMouseCursorControl : " );
	common->Printf( grabMouse ? "IS " : "IS NOT " );
	common->Printf( "grabbing the mouse cursor control.\n" );
	Sys_GrabMouseCursor( grabMouse );
}

/*
=================
blShellLocal::SetGUI
=================
*/
void blShellLocal::SetGUI( idUserInterface *gui, HandleGuiCommand_t handle ) {

	if ( !gui ) {
		return;
	}

	GUI_element GUIelement;
	GUIelement.gui = gui;
	GUIelement.handle = handle;

	/*
	if ( guiMsgRestore ) {
		common->DPrintf( "idSessionLocal::SetGUI: cleared an active message box\n" );
		guiMsgRestore = NULL;
	}
	*/

	/*   		//all this is hardcoded
	if ( guiActive == guiMainMenu ) {
		SetSaveGameGuiVars();
		SetMainMenuGuiVars();
	} else if ( guiActive == guiRestartMenu ) {
		SetSaveGameGuiVars();
	}
	*/

	int i = gui_l.Append( GUIelement );
	//this should be done in a controlled way
	int index = guiActive_l.Append( gui_l[i] );

	sysEvent_t  ev;
	memset( &ev, 0, sizeof( ev ) );
	ev.evType = SE_NONE;

	int cur_frameTime = commonLocal.frameTiming.startGameTime; // I think that's the frame start time

	guiActive_l[index].gui->HandleEvent( &ev, cur_frameTime );
	guiActive_l[index].gui->Activate( true, cur_frameTime );

}

/*
=================
blShellLocal::DispatchCommand
=================
*/
void blShellLocal::DispatchCommand( idUserInterface *gui, const char *menuCommand, bool doIngame ) {
	;

	//TODO HARDCODED

	/*
	if ( !gui ) {
		gui = guiActive;
	}

	if ( gui == guiMainMenu ) {
		HandleMainMenuCommands( menuCommand );
		return;
	} else if ( gui == guiIntro) {
		HandleIntroMenuCommands( menuCommand );
	} else if ( gui == guiMsg ) {
		HandleMsgCommands( menuCommand );
	} else if ( gui == guiTakeNotes ) {
		HandleNoteCommands( menuCommand );
	} else if ( gui == guiRestartMenu ) {
		HandleRestartMenuCommands( menuCommand );
	} else if ( game && guiActive && guiActive->State().GetBool( "gameDraw" ) ) {
		const char *cmd = game->HandleGuiCommands( menuCommand );
		if ( !cmd ) {
			guiActive = NULL;
		} else if ( idStr::Icmp( cmd, "main" ) == 0 ) {
			StartMenu();
		} else if ( strstr( cmd, "sound " ) == cmd ) {
			// pipe the GUI sound commands not handled by the game to the main menu code
			HandleMainMenuCommands( cmd );
		}
	} else if ( guiHandle ) {
		if ( (*guiHandle)( menuCommand ) ) {
			return;
		}
	} else if ( !doIngame ) {
		common->DPrintf( "idSessionLocal::DispatchCommand: no dispatch found for command '%s'\n", menuCommand );
	}

	if ( doIngame ) {
		HandleInGameCommands( menuCommand );
	}
	*/

}

/*
=================
blShellLocal::GuiFrameEvents
=================
*/
void blShellLocal::GuiFrameEvents() {
	const char	*cmd;
	sysEvent_t  ev;

	//this check from dhewm3 is already handled by idCommonLocal::Frame() but still, it should be handled by the shell somehow!

	// stop generating move and button commands when a local console or menu is active
	// running here so SP, async networking and no game all go through it
	if ( console->Active() || ( guiActive_l.Num() > 0 ) ) { //TODO maybe the console should have it's own shell branch when active
		//usercmdGen->InhibitUsercmd( INHIBIT_SESSION, true );
		InhibitUserCommandsGeneration( true );
	} else {
		//usercmdGen->InhibitUsercmd( INHIBIT_SESSION, false );
		InhibitUserCommandsGeneration( false );
	}


	/*
	if ( guiTest ) {
		gui = guiTest;
	} else if ( guiActive ) {
		gui = guiActive;
	} else {
		return;
	}
	*/

	int frameTime = commonLocal.frameTiming.startGameTime; // I think that's the frame start time

	memset( &ev, 0, sizeof( ev ) );
	ev.evType = SE_NONE;

	for( int i = 0; i < guiActive_l.Num(); i++ ) {

		idUserInterface	*gui = guiActive_l[i].gui;

		cmd = gui->HandleEvent( &ev, frameTime );
		if ( cmd && cmd[0] ) {
			DispatchCommand( gui, cmd );
		}
	}


}

/*
=================
blShellLocal::RedrawActiveGUIs
=================
*/
void blShellLocal::RedrawActiveGUIs() {
	for( int i = 0; i < guiActive_l.Num(); i++ ) {
		guiActive_l[i].gui->Redraw( commonLocal.frameTiming.startGameTime );
	}
}

/*******************************************************************************
							SHELL INPUT EVENTS
*******************************************************************************/

/*
=================
blShellLocal::ShellInputEvent_TestInputEvents
will test any new InputEvents against the RelevantInputEvents_l list and with this,
it will set the value of the respectful ShellCommands from the ShellCommands_l list if needed
=================
*/
void blShellLocal::ShellInputEvent_TestInputEvents() {

    Sys_GenerateEvents();

    // queue system events ready for polling
	Sys_GetEvent();

    for( index = 0; index < RelevantInputEvents_l.Num(); index++ ) {
        InputEventsAndIndexes ie = RelevantInputEvents_l[i];

	    if( *(ie.InputEventPtr)->EventType == SE_KEY ) {
            int numKeyEvents = Sys_PollKeyboardInputEvents();
            if( numKeyEvents > 0 ) {
                for( int i = 0; i < numKeyEvents; i++ )	{
					int key;
					bool state;

					if( Sys_ReturnKeyboardInputEvent( i, key, state ) ) {
						if( ( key == *(ie.InputEventPtr)->Event ) && state ) {
						    ShellCommand_SwitchOn_AllRelevantInputEventIndexes( index, value ); //'value' is bool ported to int
							break; //TODO should it break?
						}
					}
				}

				Sys_EndKeyboardInputEvents();
            }
	    } else if( *(ie.InputEventPtr)->EventType == SE_MOUSE ) {
	        int	mouseEvents[MAX_MOUSE_EVENTS][2];
			int numMouseEvents = Sys_PollMouseInputEvents( mouseEvents );
			if( numMouseEvents > 0 ) {
				for( int i = 0; i < numMouseEvents; i++ ) {
					int action = mouseEvents[i][0];
					int value = mouseEvents[i][1];

					if( action == *(ie.InputEventPtr)->Event ) {
						ShellCommand_SwitchOn_AllRelevantInputEventIndexes( index, value );
						break; //TODO should it break?
					}
				}
			}
	    } else if( *(ie.InputEventPtr)->EventType == SE_JOYSTICK ) {
			int numJoystickEvents = Sys_PollJoystickInputEvents( 0 );
			if( numJoystickEvents > 0 ) {
				for( int i = 0; i < numJoystickEvents; i++ ) {
					int action;
					int value;

					if( Sys_ReturnJoystickInputEvent( i, action, value ) ) {
						if( ( action == *(ie.InputEventPtr)->Event ) && ( value != 0 ) ) {
							ShellCommand_SwitchOn_AllRelevantInputEventIndexes( index, value );
							break; //TODO should it break?
						}
					}
				}

				Sys_EndJoystickInputEvents();
			}
		}
    }
}

/*******************************************************************************
								SHELL MANAGER
*******************************************************************************/

/*
=================
blShellManagerLocal::Manager_SystemEvents
=================
*/

void blShellManagerLocal::Manager_SystemEvents() {

	/*
	bool escapeEvent = false;

	while( true ) {

		Sys_GenerateEvents();

		// queue system events ready for polling
		Sys_GetEvent();

		if( flags & SYSTEM_EVENTS_KEYBOARD ) {

			int numKeyEvents = Sys_PollKeyboardInputEvents();
			if( numKeyEvents > 0 ) {
				for( int i = 0; i < numKeyEvents; i++ )	{
					int key;
					bool state;

					if( Sys_ReturnKeyboardInputEvent( i, key, state ) ) {
						if( key == K_ESCAPE && state == true ) {
							escapeEvent = true;
						}
						break;
					}
				}

				Sys_EndKeyboardInputEvents();
			}
		}

		if( flags & SYSTEM_EVENTS_MOUSE ) {

			int	mouseEvents[MAX_MOUSE_EVENTS][2];
			int numMouseEvents = Sys_PollMouseInputEvents( mouseEvents );
			if( numMouseEvents > 0 ) {
				for( int i = 0; i < numMouseEvents; i++ ) {
					int action = mouseEvents[i][0];
					switch( action ) {
						case M_ACTION1:
						case M_ACTION2:
						case M_ACTION3:
						case M_ACTION4:
						case M_ACTION5:
						case M_ACTION6:
						case M_ACTION7:
						case M_ACTION8:
							escapeEvent = true;
							break;

						default:	// some other undefined button
							break;
					}
				}
			}
		}

		if( flags & SYSTEM_EVENTS_JOYSTICK ) {

			int numJoystickEvents = Sys_PollJoystickInputEvents( 0 );
			if( numJoystickEvents > 0 ) {
				for( int i = 0; i < numJoystickEvents; i++ ) {
					int action;
					int value;

					if( Sys_ReturnJoystickInputEvent( i, action, value ) ) {
						if( action >= J_ACTION1 && action <= J_ACTION_MAX ) {
							if( value != 0 ) {
								escapeEvent = true;
								break;
							}
						}
					}
				}

				Sys_EndJoystickInputEvents();
			}
		}

		break;
	}

	return escapeEvent;
	*/

	//get events for the shell
	sysEvent_t* sev = Sys_GetEvent();


	/*
	if( sev->IsKeyEvent() ) {
				if ( sev->IsKeyDown() && ( ( sev->GetKey() == K_ESCAPE ) || ( sev->GetKey() == K_JOY13 ) ) ) {
					if( console->Active() ) {
						console->Close();
					} else {
						if( Check_MenuActive() ) {
							CloseMenu();
						} else {
							OpenMenu();
						}
					}
				}
			}
	*/

}

/*
=================
blShellLocal::AsyncFrame_BuildBranchLists
every frame constructs the active and passive branches or the thread to work on them
=================
*/
void blShellLocal::AsyncFrame_RunBranchLists() {

	if( branches_l.Num() == 0 ) {
		common->Warning( "AsyncFrame_RunBranchLists : on the current frame, no branches present in the list of available branches!" );
		return;
	}

	int i;

    //make sure the branches_l is complete with all branches for this frame
    for( i = 0; i < branches_l.Num(); i++ ) {
        if( branches_l[i].IsHidden() == false ) {
            if( branches_l[i].Style == BRANCH_PROCESSED ) {
                blShellBranchProcessed * branch = branches_l[i];
			    if( branch->ProcessStatus == BRANCH_PROCESS_NOT_STARTED ) {
				    branch->ProcessStatus = BRANCH_PROCESS_STARTED;
				    branch->StartProcess( branches_l ); //this should put active sub-branches into the branches_l
			    }
            }
        }
    }

    //now we are sure all sub-branches are in branches_l

    branches_active_l.Clear();
	branches_paused_l.Clear();

	for( i = 0; i < branches_l.Num(); i++ ) {
		if( branches_l[i].IsHidden() ) {
			branches_paused_l.Append( branches_l[i] );
		} else {
			branches_active_l.Append( branches_l[i] );
		}
	}

	if( branches_active_l.Num() == 0 ) {
		common->Warning( "AsyncFrame_RunBranchLists : on the current frame, no active branches are present!" );
		return;
	}

    bool hasRenderedSomething = false;

	for( i = 0; i < branches_active_l.Num(); i++ ) {
		if( branches_active_l[i]->Style == BRANCH_FRAMED ) {
			blShellBranchFramed * branch = branches_active_l[i];
			branch->RunFrame();
			hasRenderedSomething = true;
		}
	}

	if( hasRenderedSomething ) {
	    Render_RenderCommandBuffers();
	}
}

/*
=================
blShellLocal::CheckIdIndexInList
returns the index if the element exists in the list or -1 if the element isn't in the list
=================
*/
int blShellLocal::CheckIdIndexInList( idStr BranchId, idList<blShellBranch> List ) {
	int index = -1;
	for( int i = 0; i < List.Num(); i++ ) {
		if( List[i].GetBranchId() == BranchId ) {
			index = i;
			break;
		}
	}

	if( index == -1 ) {
		common->Printf( "blShellLocal::CheckIdIndexInList : Branch '$s' doesn't exist in the checked list!\n", BranchId );
	}
	return index;
}

/*
=================
blShellLocal::BranchControl_Kill
=================
*/
void blShellLocal::BranchControl_Kill( idStr BranchId ) {
	int branchIndex = CheckIdIndexInList( BranchId, branches_l );

	if( branchIndex != -1 ) {
		branches_l.RemoveIndex( branchIndex );
	} else {
		common->Warning( "blShellLocal::BranchControl_Kill : not able to Kill branch '%s' because it is not present in the branches list!\n", BranchId.c_str() );
	}
}



/*
=================
blShellLocal::BranchControl_IsPaused
=================
*/
bool blShellLocal::BranchControl_IsPaused( idStr BranchId ) {
	int branchIndex = CheckIdIndexInList( BranchId, branches_l );

	if( branchIndex != -1 ) {
		return branches_l[branchIndex].IsHidden();
	} else {
		common->Warning( "blShellLocal::BranchControl_IsPaused : Not able to pause branch '%s' because it is not in the branches list!\n", BranchId.c_str() );
		return false;
	}
}

/*
=================
blShellLocal::BranchControl_TogglePause
=================
*/
void blShellLocal::BranchControl_TogglePause( idStr BranchId ) {
	int branchIndex = CheckIdIndexInList( BranchId, branches_l );

	if( branchIndex != -1 ) {
		branches_l[branchIndex].ToggleHidden();
		//next frame will be set at the required list
	} else {
		common->Warning( "blShellLocal::BranchControl_TogglePause : Not able to toggle the pause in branch '%s' because it is not in the branches list!\n", BranchId.c_str() );
	}
}

/*
=================
blShellLocal::Event_MainMenu_Toggle()
=================
*/
void blShellLocal::Event_MainMenu_Toggle() {

	//TODO HARDCODED

	if( Event_MainMenu_isOpen() ) {
		//close menu
		MainMenu_isOpen = false;

	} else {
		//open menu
		MainMenu_isOpen = true;

		//the state of session will determine what main menu will be opened
		int sessionState = Event_Get_SessionState();

		if( ( sessionState == idSession::PRESS_START ) || ( sessionState == idSession::IDLE ) ) {

			//if session is start then open start main menu
			if ( sessionState == idSession::PRESS_START ) {
				if( BranchControl_IsPaused( "Second Branch" ) ) {
					BranchControl_TogglePause( "Second Branch" );
				}
			}

			session->MoveToMainMenu(); //TODO this should be handled by a shell event
			//open start main menu

		} else if( sessionState == idSession::INGAME ) {
			//if game is single player
				//if player is dead
					//open death menu
				//else
					//if session is ingame then open ingame main menu
						// open sp menu
			//else
				//open mp main menu
		} else {
			common->Warning("blShellManagerLocal::ToggleMainMenu : incorrect session menu, opening start main menu!\n");
			//open start main menu
		}
	}
}

void blShellLocal::ProcessBranch_script( blShellBranchProcessed* parentBranch ) {

    //if dispaly videos is on
        //diable console
        //display company video for lenght time
        //enable console
    //enable esc for skipping
    //if dispaly videos is on
        //enable skip1 function
        //display enine video for length time
        //disable skip 1
    //enable skip2
    //dispaly legal text for n seconds
    //disable skip2
    //diable esc or skipping
    //enable esc for menu toggling
    //force open menu

    float ar169 = ( 16.0f / 9.0f );
    idMaterial* splashMaterial;

    if( !com_skipIntroVideos.GetBool() && fileSystem->UsingResourceFiles() )	{
        //diable console

        //display company video for lenght time
        idStr materialText;
		materialText.Format( "{ translucent { videoMap %s } }", "video\\loadvideo.bik" );

		splashMaterial = const_cast<idMaterial*>( declManager->FindMaterial( "splashVideo" ) );
		splashMaterial->Parse( materialText.c_str(), materialText.Length(), false );

		// RB: FFmpeg might return the wrong play length so I changed the intro video to play max 30 seconds until finished
		int cinematicLength = splashMaterial->CinematicLength();
		cinematicLength = 30000; //so we don't care  if FFmpeg gave correct time or not, we will kill it if it stops playing anyway

        blShellBranchFullScreenMaterial firstVideo = new blShellBranchFullScreenMaterial();
        idVec2 wantedSize = idVec2( 1.0f, 1.0f ); //TODO figure out what "size" means in a fullscreen-streteched material, if it's not meaningless to begin with.
        firstVideo.SetContent( &splashMaterial, colorBlack, colorBlack, idVec2( 0.0f, 0.0f), ar169, wantedSize, idVec4idVec2( 0.0f, 0.0f, 1.0f, 1.0f ) );
        firstVideo.SetBranchId( "company video" );
        firstVideo.SetParentBranch( parentBranch );


        /*
        	newStage.material = splashMaterial;
		newStage.aspectRatio = ar169;
		newStage.Interrupt_events_l.Clear();
		ShellEvent event;
		event.EventType = SE_KEY;
		event.Event = K_ESCAPE;
		newStage.Interrupt_events_l.Append( event ); //esc
		event.EventType = SE_KEY; // should be SE_JOYSTICK
		event.Event = K_JOY9; // it was K_JOY13
		newStage.Interrupt_events_l.Append( event ); //hardcoded gamepad - menu
		newStage.tintColour = colorBlack;
		newStage.backgroundColour = colorBlack;
		newStage.startTime = splashTime; // this is not significant
		newStage.elapsedTime = cinematicLength;

		branch.FeedSplashList( newStage );

		splashTime += cinematicLength;
        */

        //enable console


    } else {
		idLib::Printf( "Skipping the showing of the initial splash videos\n" );
		//so display the legal splash screen
	}

    //the legal splash screen stage

    /*
	if( idStr::Icmp( sys_lang.GetString(), ID_LANG_FRENCH ) == 0 ) {
		// If the user specified french, we show french no matter what SKU
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_francophonie" );
	} else if( idStr::Icmp( Sys_DefaultLanguage(), ID_LANG_FRENCH ) == 0 ) {
		// If the lead sku is french (ie: europe), display figs
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_euro_figs" );
	} else {
		// Otherwise show it in english
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_anglosphere" );
	}

	newStage.material = splashMaterial;
	newStage.aspectRatio = ar169;
	newStage.Interrupt_events_l.Clear(); // no interrupt
	newStage.tintColour = colorBlack;
	newStage.backgroundColour = colorBlack;
	newStage.startTime = splashTime; // this is not significant
	newStage.elapsedTime = 4000; //lasts 4 seconds

	branch.FeedSplashList( newStage );

	splashTime += 4000;


	//then a possible generic lodaing screen for the case the demos out of the main menu haven't finished loading

	splashMaterial = declManager->FindMaterial( "guis/ui/splash/generic_loading" );

	newStage.material = splashMaterial;
	newStage.aspectRatio = ar169;
	newStage.Interrupt_events_l.Clear(); // no interrupt
	newStage.tintColour = colorBlack;
	newStage.backgroundColour = colorBlack;
	newStage.startTime = splashTime; // this is not significant
	newStage.elapsedTime = 0; //lasts 0 seconds !? "lasts as long as it needs to last" so zero seconds is a special case for the computer decides?

	branch.FeedSplashList( newStage );
    */
}



/*
=================
blShellLocal::PrepFirstSplashContent

build the initial splash content to show up to the user
=================
*/
void blShellLocal::PrepFirstSplashContent( blShellBranchLineal branch ) {
	//TODO this should all be read from a place like the autoexec.cfg file

	float ar169 = ( 16.0f / 9.0f );
	idMaterial* splashMaterial;
	ShellSplashStageContent_t newStage;

	int splashTime = 0;

	//the initial splash video stage

	if( !com_skipIntroVideos.GetBool() && fileSystem->UsingResourceFiles() )	{
		idStr materialText;
		materialText.Format( "{ translucent { videoMap %s } }", "video\\loadvideo.bik" );

		splashMaterial = const_cast<idMaterial*>( declManager->FindMaterial( "splashVideo" ) );
		splashMaterial->Parse( materialText.c_str(), materialText.Length(), false );

		// RB: FFmpeg might return the wrong play length so I changed the intro video to play max 30 seconds until finished
		int cinematicLength = splashMaterial->CinematicLength();
		cinematicLength = 30000; //so we don't care  if FFmpeg gave correct time or not, we will kill it if it stops playing anyway

		newStage.material = splashMaterial;
		newStage.aspectRatio = ar169;
		newStage.Interrupt_events_l.Clear();
		ShellEvent event;
		event.EventType = SE_KEY;
		event.Event = K_ESCAPE;
		newStage.Interrupt_events_l.Append( event ); //esc
		event.EventType = SE_KEY; // should be SE_JOYSTICK
		event.Event = K_JOY9; // it was K_JOY13
		newStage.Interrupt_events_l.Append( event ); //hardcoded gamepad - menu
		newStage.tintColour = colorBlack;
		newStage.backgroundColour = colorBlack;
		newStage.startTime = splashTime; // this is not significant
		newStage.elapsedTime = cinematicLength;

		branch.FeedSplashList( newStage );

		splashTime += cinematicLength;

	} else {
		idLib::Printf( "Skipping the showing of the initial splash video\n" );
		//so display the legal splash screen
	}


	//the legal splash screen stage

	if( idStr::Icmp( sys_lang.GetString(), ID_LANG_FRENCH ) == 0 ) {
		// If the user specified french, we show french no matter what SKU
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_francophonie" );
	} else if( idStr::Icmp( Sys_DefaultLanguage(), ID_LANG_FRENCH ) == 0 ) {
		// If the lead sku is french (ie: europe), display figs
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_euro_figs" );
	} else {
		// Otherwise show it in english
			splashMaterial = declManager->FindMaterial( "guis/assets/splash/legal_anglosphere" );
	}

	newStage.material = splashMaterial;
	newStage.aspectRatio = ar169;
	newStage.Interrupt_events_l.Clear(); // no interrupt
	newStage.tintColour = colorBlack;
	newStage.backgroundColour = colorBlack;
	newStage.startTime = splashTime; // this is not significant
	newStage.elapsedTime = 4000; //lasts 4 seconds

	branch.FeedSplashList( newStage );

	splashTime += 4000;


	//then a possible generic lodaing screen for the case the demos out of the main menu haven't finished loading

	splashMaterial = declManager->FindMaterial( "guis/ui/splash/generic_loading" );

	newStage.material = splashMaterial;
	newStage.aspectRatio = ar169;
	newStage.Interrupt_events_l.Clear(); // no interrupt
	newStage.tintColour = colorBlack;
	newStage.backgroundColour = colorBlack;
	newStage.startTime = splashTime; // this is not significant
	newStage.elapsedTime = 0; //lasts 0 seconds !? "lasts as long as it needs to last" so zero seconds is a special case for the computer decides?

	branch.FeedSplashList( newStage );

}

/*
=================
blShellLocal::StartContent
=================
*/
void blShellLocal::StartContent() {

	// build a lineal branch for the content before the Menu
	// fire a thread to execute it before starting the Menu

	//TODO HARDCODED

	blShellBranchLineal branch_0 = new blShellBranchLineal();
	PrepFirstSplashContent( branch_0 );


	//MainMenu->InitFromFile();
	blShellBranchGui branch_1 = new blShellBranchGui();

	/*
	blShellBranchEscapeInputs branch_2 = new blShellBranchEscapeInputs();
	idList<ShellInterruptEvent> Possible_Escape_Inputs_l;
	Possible_Escape_Inputs_l.Clear();
	ShellInterruptEvent event;
	event.EventType = SE_KEY;
	event.Event = K_ESCAPE;
	Possible_Escape_Inputs_l.Append( event );
	event.EventType = SE_KEY;
	event.Event = K_GRAVE; // the console key
	Possible_Escape_Inputs_l.Append( event );
	branch_2.AddEscapeInputs( Possible_Escape_Inputs_l );
	*/

	branch_0.SetBranchId( "First Branch" );	//this names should be random generated
	branch_1.SetBranchId( "Second Branch" ); //this names should be random generated
	//branch_2.SetBranchId( "Input Branch" ); //this names should be random generated

	BranchControl_TogglePause( "First Branch" );
	//the menu branch reamins paused


    BranchControl_AppendBranch( branch_0 );
    BranchControl_AppendBranch( branch_1 );
	//branches_l.Append( branch_2 );


	//calculate the stack for the logic of the UI to perform
	int stackSize = 0x100000; //TODO measure the stack size, this is a meg

	//create a thread for the first content
	shellThread.StartWorkerThread( "Graphical user interface content", CORE_2B, THREAD_BELOW_NORMAL, stackSize );

}


} /* namespace BFG */
