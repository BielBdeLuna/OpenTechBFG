/*
 * GraphicalInterfaces_flat.cpp
 *
 *      Author: biel
 *      this file is GPLv3
 */

#define MAIN_MENU_SHELL_NAME "shell";
#define PAUSE_MENU_SHELL_NAME "pause";

#include "../d3xp/GraphicalInterfaces.h"
#include "../idlib/Str.h"
#include "../framework/Common_local.h"
#include "../framework/CVarSystem.h"
#include "../d3xp/Game_local.h"

namespace BFG
{

idCVar fgui_SytemLevelThreshold( "fgui_SytemLevelThreshold", "3", CVAR_INTEGER | CVAR_SYSTEM | CVAR_GUI, "set a forced system level threshold to be used in flat GUIs from a probable set of diverging capabilities, where:\n 0 --> imGUI\n 1 --> GUI\n 2 --> ceGUI\n 3 --> flash based GUI\n" );

/*
===============================================================================
			blFlatShellArea
===============================================================================
*/

/*
========================
class constructor
========================
*/

blFlatShellArea::blFlatShellArea() {
	ClearList();
	areaRef = -1;
	fileName = NULL;
	GUIfile = NULL;
	shell = NULL;
	flatGuiHandler = NULL;
	guiHandle = NULL;
}

/*
========================
class destructor
========================
*/

blFlatShellArea::~blFlatShellArea() {
	ClearList();
	GUIfile = NULL;
	shell = NULL;
	flatGuiHandler = NULL;
	guiHandle = NULL;
}

/*
========================
blFlatShellArea::ClearList
========================
*/
void blFlatShellArea::ClearList() {
	fileType.Clear();
}

/*
========================
blFlatShellArea::Init
========================
*/
void blFlatShellArea::Init( idStr fileName_, int intAreaRef, blFlatShell* shell_, blFlatGui* flatGuiHandler_ ) {
	fileName = fileName_;
	areaRef = intAreaRef; //TODO this should be extracted from the shell file associated to the fileName!
	shell = shell_;
	flatGuiHandler = flatGuiHandler_;

	// the name is correct?
	const char* name_ = fileName.c_str();

	if( name_ == NULL || name_[ 0 ] == '\0' )
	{
		gameLocal.Error( "the filename in the area is null!" );
	}

	//the name is correct let's fill the fileType list with permissions
	fileType = CompareFilesToFlatSystems( fileName );

	//load the GUI file
	LoadAreaFile();
	//the area is initiated now, it's prepared to start functioning, in order to make it work call the method RestartArea();
}

/*
========================
blFlatShellArea::LoadArea
========================
*/
void blFlatShellArea::LoadAreaFile() {
	int option = GetBestOption();
	if( option == FLAT_SYSTEM_GUI ) {
		idStr qpath = gameLocal.BuildAqpath( fileName, "gui", "guis" ); //the path to the actual file from the fs_game folder
		GUIfile = uiManager->FindGui( qpath.c_str(), true, false, true );
		if( GUIfile == NULL ) {
			gameLocal.Error( "the file %s has not been found!", fileName );
		}
	} else if( option == FLAT_SYSTEM_CEGUI ) {
		/*
		 * TODO nothing yet
		 */
	} else {
		/*
		 * TODO nothing yet
		 */
	}
}

/*
========================
blFlatShellArea::RestartArea
========================
*/
void blFlatShellArea::StartArea() {
	int option = GetBestOption();
	if( option == FLAT_SYSTEM_IMGUI ) {
		gameLocal.Warning( "the area %s hasn't a hard-coded method coded yet", fileName );
	} else if( option == FLAT_SYSTEM_GUI ) {
		if( GUIfile == NULL ) {
			LoadAreaFile();
		}

		sysEvent_t  ev;
		memset( &ev, 0, sizeof( ev ) );
		ev.evType = SE_NONE;

		GUIfile->HandleEvent( &ev, gameLocal.GetTime() );
		GUIfile->Activate( true, gameLocal.GetTime() );
	} else {
		//die a paceful dead as none of this is implemented yet
		gameLocal.Warning( "the area %s has nothing coded yet", fileName );
	}

}

/*
========================
blFlatShellArea::UpdateFiles
========================
*/
void blFlatShellArea::UpdateFiles() {
	ClearList();
	fileType = CompareFilesToFlatSystems( fileName );
}

/*
========================
blFlatShellArea::GetBestOption
========================
*/
int blFlatShellArea::GetBestOption() {
	//with the cvar we can limit the scope of systems we try to use,
	//so if we have even better systems we might discard them in order to work with the worse systems

	int threshold, maxAvailable = fileType.Num() - 1;
	int cvarValue = fgui_SytemLevelThreshold.GetInteger();
	if( cvarValue < maxAvailable ) {
		threshold = cvarValue;
	}

	for( int i = threshold; i >= 0; i-- ) {
		//we always check from the best selected to the worse
		if( fileType[i] == true ) {
			return i;
		}
	}
	return FLAT_SYSTEM_IMGUI; //this means that we don't have any file Available so we should use the hard-coded method
}
/*
================
blFlatShellArea::CompareFilesToFlatSystems
================
*/
idList<bool>  blFlatShellArea::CompareFilesToFlatSystems( idStr fileName ) {
	idList<bool> theList;

	for( int i = 0; i < NUM_FLAT_SYSTEMS; i++ ) {
		///if( i == FLAT_SYSTEM_IMGUI ) { theList.Append( false ); } //no need to test it since it's lower than 0 and in the end should be hardcoded
		if( i == FLAT_SYSTEM_GUI ) { theList.Append( gameLocal.CheckInFolders( fileName, "gui", "guis", true ) ); }
		if( i == FLAT_SYSTEM_CEGUI ) { theList.Append( false ); } //TODO at the moment I haven't studied it yet!
		//if( i == FLAT_SYSTEM_FLASH ) { theList.Append( gameLocal.CheckInFolders( fileName, "swf", "swf", true ) ); } //FIXME man FLASH has become a pain in the ass!
		if( i == FLAT_SYSTEM_FLASH ) { theList.Append( false ); }
	}

	return theList;
}

/*
===============================================================================
			blFlatShell
===============================================================================
*/

/*
========================
blFlatShell::blFlatShell
========================
*/

blFlatShell::blFlatShell() {
	shellName = NULL;
	qpath = NULL;
	flatGuiHandler = NULL;
	shellSW = NULL;
	nextArea = FLAT_SHELL_AREA_INVALID;
	currentArea = FLAT_SHELL_AREA_INVALID;
	continueInsteadOfNewGame = false;
	timeRemaining = 0;
	sounds.Clear();
	ClearList();
}

/*
========================
blFlatShell::~blFlatShell
========================
*/

blFlatShell::~blFlatShell() {
	ClearList();
}

/*
========================
blFlatShell::GoToArea
========================
*/

void blFlatShell::GoToArea( int areaNumber, int transition ) {

	if ( currentArea != areaNumber ) {
		nextArea = areaNumber;
	} else {
		gameLocal.Warning( "we're already on this area!" );
	}

	//TODO select the next transition (even if we are amidst another.
	//idealTransition = transition;
}

/*
========================
blFlatShell::RestartShell
========================
*/
void blFlatShell::RestartShell() {
	GoToArea( FLAT_SHELL_AREA_START, SHELL_AREA_TRANSITION_CUT_TO );

}

/*
========================
blFlatShell::RestartCurrentArea
========================
*/
void blFlatShell::RestartCurrentArea() {
	areaList[currentArea]->StartArea();
}

/*
========================
blFlatShell::Update
========================
*/

void blFlatShell::Update() {
	////// status

	if( flatGuiHandler->GetNextMenuState() != flatGuiHandler->GetMenuState() ) {
		if( flatGuiHandler->GetNextMenuState() == MENU_STATE_PRESS_START ) {
			//HidePacifier();
			RestartShell();
			flatGuiHandler->SetMenuState( flatGuiHandler->GetNextMenuState() );
			PlaySound( FLAT_SHELL_SOUND_MUSIC );
		} else if( flatGuiHandler->GetNextMenuState() == MENU_STATE_IDLE ) {
			//HidePacifier();
			if( nextArea == FLAT_SHELL_AREA_START || /*nextArea == FLAT_SHELL_AREA_PARTY_LOBBY || nextArea == FLAT_SHELL_AREA_GAME_LOBBY ||*/ nextArea == FLAT_SHELL_AREA_INVALID ) {
				GoToArea( FLAT_SHELL_AREA_ROOT, SHELL_AREA_TRANSITION_CUT_TO );
			}
			flatGuiHandler->SetMenuState( flatGuiHandler->GetNextMenuState() );
		} else if( flatGuiHandler->GetNextMenuState() == MENU_STATE_PARTY_LOBBY ) {
			//HidePacifier();
			//nextScreen = SHELL_AREA_PARTY_LOBBY;
			//transition = MENU_TRANSITION_SIMPLE;
			flatGuiHandler->SetMenuState( flatGuiHandler->GetNextMenuState() );
		} else if( flatGuiHandler->GetNextMenuState() == MENU_STATE_GAME_LOBBY ) {
			//HidePacifier();
			if( flatGuiHandler->GetMenuState() != MENU_STATE_IN_GAME )
			{
				//timeRemaining = WAIT_START_TIME_LONG;
				//idMatchParameters matchParameters = session->GetActivePlatformLobbyBase().GetMatchParms();
				//if ( MatchTypeIsPrivate( matchParameters.matchFlags ) && ActiveScreen() == SHELL_AREA_PARTY_LOBBY ) {
				//	timeRemaining = 0;
				//	session->StartMatch();
				//	state = SHELL_STATE_IN_GAME;
				//} else {
				//nextScreen = SHELL_AREA_GAME_LOBBY;
				//transition = MENU_TRANSITION_SIMPLE;
				//}

				flatGuiHandler->SetMenuState( flatGuiHandler->GetNextMenuState() );
			}
		} else if( flatGuiHandler->GetNextMenuState() == MENU_STATE_PAUSED ) {
			//HidePacifier();
			if( flatGuiHandler->GetGameComplete() ) {
				GoToArea( FLAT_SHELL_AREA_CREDITS, SHELL_AREA_TRANSITION_CUT_TO );
			} else {
				GoToArea( FLAT_SHELL_AREA_ROOT, SHELL_AREA_TRANSITION_CUT_TO );
			}
			flatGuiHandler->SetMenuState( flatGuiHandler->GetNextMenuState() );
		}
		else if( flatGuiHandler->GetNextMenuState() == MENU_STATE_CONNECTING )
		{
			//ShowPacifier( "#str_dlg_connecting" );
			flatGuiHandler->SetMenuState( flatGuiHandler->GetNextMenuState() );
		}
		else if( flatGuiHandler->GetNextMenuState() == MENU_STATE_SEARCHING )
		{
			//ShowPacifier( "#str_online_mpstatus_searching" );
			flatGuiHandler->SetMenuState( flatGuiHandler->GetNextMenuState() );
		}
	}
	/*
	if( nextState != state )
	{
		if( introGui != NULL && introGui->IsActive() )
		{
			gui->StopSound();
			showingIntro = false;
			introGui->Activate( false );
			PlaySound( GUI_SOUND_MUSIC );
		}

		if( nextState == SHELL_STATE_PRESS_START )
		{
			HidePacifier();
			nextScreen = SHELL_AREA_START;
			transition = MENU_TRANSITION_SIMPLE;
			state = nextState;
			if( menuBar != NULL && gui != NULL )
			{
				menuBar->ClearSprite();
			}
		}
		else if( nextState == SHELL_STATE_IDLE )
		{
			HidePacifier();
			if( nextScreen == SHELL_AREA_START || nextScreen == SHELL_AREA_PARTY_LOBBY || nextScreen == SHELL_AREA_GAME_LOBBY || nextScreen == SHELL_AREA_INVALID )
			{
				nextScreen = SHELL_AREA_ROOT;
			}

			if( menuBar != NULL && gui != NULL )
			{
				idSWFScriptObject& root = gui->GetRootObject();
				menuBar->BindSprite( root );
				SetupPCOptions();
			}
			transition = MENU_TRANSITION_SIMPLE;
			state = nextState;
		}
		else if( nextState == SHELL_STATE_PARTY_LOBBY )
		{
			HidePacifier();
			nextScreen = SHELL_AREA_PARTY_LOBBY;
			transition = MENU_TRANSITION_SIMPLE;
			state = nextState;
		}
		else if( nextState == SHELL_STATE_GAME_LOBBY )
		{
			HidePacifier();
			if( state != SHELL_STATE_IN_GAME )
			{
				timeRemaining = WAIT_START_TIME_LONG;
				idMatchParameters matchParameters = session->GetActivePlatformLobbyBase().GetMatchParms();
				//if ( MatchTypeIsPrivate( matchParameters.matchFlags ) && ActiveScreen() == SHELL_AREA_PARTY_LOBBY ) {
				//	timeRemaining = 0;
				//	session->StartMatch();
				//	state = SHELL_STATE_IN_GAME;
				//} else {
				nextScreen = SHELL_AREA_GAME_LOBBY;
				transition = MENU_TRANSITION_SIMPLE;
				//}

				state = nextState;
			}
		}
		else if( nextState == SHELL_STATE_PAUSED )
		{
			HidePacifier();
			transition = MENU_TRANSITION_SIMPLE;

			if( gameComplete )
			{
				nextScreen = SHELL_AREA_CREDITS;
			}
			else
			{
				nextScreen = SHELL_AREA_ROOT;
			}

			state = nextState;
		}
		else if( nextState == SHELL_STATE_CONNECTING )
		{
			ShowPacifier( "#str_dlg_connecting" );
			state = nextState;
		}
		else if( nextState == SHELL_STATE_SEARCHING )
		{
			ShowPacifier( "#str_online_mpstatus_searching" );
			state = nextState;
		}
	}
	*/

	//////transitions

	/*
	//always want to change to a neutral transition status
	if( transitionStatus != SHELL_TRANSITION_STATUS_NONE ) {
		//finish the transition
	}
	if( idealTransition != currentTransition ) {

	}

	if( idealArea != currentArea ) {
		if( idealTransition != SHELL_AREA_TRANSITION_NONE ) {
			if( transitionStatus != SHELL_TRANSITION_STATUS_NONE) {
				if
			}

		} else {
		if( currentTransition != idealTransition ) {
					if( transitionStatus == SHELL_TRANSITION_STATUS_NONE)
				}

		if( currentTransition == SHELL_AREA_TRANSITION_NONE ) {
			currentTransition = idealTransition;
		}
		if( currentTransition == idealTransition ) {
			if( transitionStatus == SHELL_TRANSITION_STATUS_NONE)
		}
	}
	*/
}

/*
========================
blFlatShell::SetActiveArea
========================
*/
void blFlatShell::SetActiveArea( int areaRef, HandleGuiCommand_t handle = NULL ) {// ( blFlatShellArea *area, HandleGuiCommand_t handle ) {
	blFlatShellArea * area = NULL;
	for( int i = 0; areaList.Num() > i; i++ ) {
		if ( areaList[i]->GetAreaRef() == areaRef ) {
			area = areaList[i];
		}
	}

	if( !area ) {
		return;
	}
	int bo = area->GetBestOption();
	if( bo == FLAT_SYSTEM_GUI ) {
		activeArea = area;
		activeArea->GetHandleGuiCommand( handle );

		//TODO this sets up the hardcoded variables for the main menu only in the case of the correct shell,
		//this should be done taking that info from a text file, shells should be defined in *.menu files that called the different areas.
		if( ( idStr::Icmp( shellName, "shell" ) != 0 ) || ( idStr::Icmp( shellName, "pause" ) != 0 ) ) {
			SetMainMenuGuiHARDCODEDVars();
			/*
			SetSaveGameGuiVars();
			*/
			if( ( idStr::Icmp( shellName, "shell" ) != 0 ) && ( activeArea->GetAreaRef() == FLAT_SHELL_AREA_ROOT ) ) {
				/*
				SetMainMenuGuiVars();
				*/
			}
		}
		activeArea->StartArea();
	} else if( bo == FLAT_SYSTEM_CEGUI ) {
		//TODO nothing yet
	} else if( bo == FLAT_SYSTEM_FLASH ) {
		//TODO nothing yet
	} else {
		//IMGUI
		//TODO nothing yet
	}


}

/*
========================
blFlatShell::Activate
========================
*/

void blFlatShell::Activate( bool b ) {
	if ( b && !shellActive ) {
		RestartShell();
		DoInhibitControl( false ); // this should depend on the shell file!
	}
	shellActive = true;
}

/*
========================
blFlatShell::ClearList
========================
*/

void blFlatShell::ClearList() {
	if( areaList.Num() != 0 ) {
		blFlatShellArea * area;
		for( int i = 0; i < areaList.Num(); i ++ ) {
			area = areaList[i];
			delete area;
		}
	}
	areaList.Clear();
}

/*
========================
blFlatShell::InitShell
========================
*/

void blFlatShell::InitShell( idStr shellName_, blFlatGui* flatGuiHandler_, idSoundWorld* sw ) {
	shellName = shellName_;
	flatGuiHandler = flatGuiHandler_;
	shellSW = sw;
	sounds.SetNum( NUM_FLAT_SHELL_SOUNDS );
	HARDCODEDsolution(); //TODO make a way to get this info from a file
	flatGuiHandler->SetActiveShell( this );
	//FIXME it should get activated!
	flatGuiHandler->activeShell->GoToArea( FLAT_SHELL_AREA_START, SHELL_AREA_TRANSITION_CUT_TO );
}

/*
========================
blFlatShell::AddArea
========================
*/

void blFlatShell::AddArea( idStr fileName, int intName ) {

	blFlatShellArea* area = new( TAG_FLATGUIS ) blFlatShellArea;
	area->Init( fileName, intName, this, flatGuiHandler );

	areaList.Append( area );
	//delete area;

}

/*
========================
blFlatShell::HARDCODEDsolution
========================GetAreaRef()
*/

void blFlatShell::HARDCODEDsolution() {
	//TODO in time we should implement a way to read this info from a fileH

	if( ( idStr::Icmp( shellName, "shell" ) != 0 ) || ( idStr::Icmp( shellName, "pause" ) != 0 ) ) {
		for( int i = 0; i < FLAT_SHELL_NUM_AREAS; i++ ) {
			if( i == FLAT_SHELL_AREA_START ) {
				AddArea( "select_doom", FLAT_SHELL_AREA_START );
			}
			if( i == FLAT_SHELL_AREA_ROOT ) {
				AddArea( "main_menu", FLAT_SHELL_AREA_ROOT );
			}
			if( i == FLAT_SHELL_AREA_DEV ) {
				AddArea( "dev_screen", FLAT_SHELL_AREA_DEV );
			}
			if( i == FLAT_SHELL_AREA_CAMPAIGN ) {
				AddArea( "campaing_screen", FLAT_SHELL_AREA_CAMPAIGN );
			}
			if( i == FLAT_SHELL_AREA_LOAD ) {
				AddArea( "load_screen", FLAT_SHELL_AREA_LOAD );
			}
			if( i == FLAT_SHELL_AREA_SAVE ) {
				AddArea( "save_screen", FLAT_SHELL_AREA_SAVE );
			}
			if( i == FLAT_SHELL_AREA_NEW_GAME ) {
				AddArea( "new_game_screen", FLAT_SHELL_AREA_NEW_GAME );
			}
			if( i == FLAT_SHELL_AREA_DIFFICULTY ) {
				AddArea( "difficutly_screen", FLAT_SHELL_AREA_DIFFICULTY );
			}
			if( i == FLAT_SHELL_AREA_SETTINGS ) {
				AddArea( "settings_screen", FLAT_SHELL_AREA_SETTINGS );
			}
			if( i == FLAT_SHELL_AREA_CONTROLS ) {
				AddArea( "controls_screen", FLAT_SHELL_AREA_CONTROLS );
			}
			if( i == FLAT_SHELL_AREA_KEYBOARD ) {
				AddArea( "keyboard_screen", FLAT_SHELL_AREA_KEYBOARD );
			}
			if( i == FLAT_SHELL_AREA_GAME_OPTIONS ) {
				AddArea( "game_options_screen", FLAT_SHELL_AREA_GAME_OPTIONS );
			}
			if( i == FLAT_SHELL_AREA_SYSTEM_OPTIONS ) {
				AddArea( "system_options_screen", FLAT_SHELL_AREA_SYSTEM_OPTIONS );
			}
			if( i == FLAT_SHELL_AREA_AUDIO ) { //<--- currently not used in doom3BFG
				AddArea( "audio_screen", FLAT_SHELL_AREA_AUDIO );
			}
			if( i == FLAT_SHELL_AREA_VIDEO ) { //<--- currently not used in doom3BFG
				AddArea( "video_screen", FLAT_SHELL_AREA_VIDEO );
			}
			if( i == FLAT_SHELL_AREA_MULTIPLAYER ) {
				AddArea( "multiplayer_screen", FLAT_SHELL_AREA_MULTIPLAYER );
			}
			if( i == FLAT_SHELL_AREA_CREDITS ) {
				AddArea( "credits_screen", FLAT_SHELL_AREA_CREDITS );
			}
			if( i == FLAT_SHELL_AREA_PAUSE ) {
				AddArea( "paused_screen", FLAT_SHELL_AREA_PAUSE ); // i think this is when the game saves that the game is paused
			}
		}
	}

	// precache sounds
	const idSoundShader* soundShader = NULL;
	// --> don't load gui music for the pause menu to save some memory <-- no longer done :)
	soundShader = declManager->FindSound( "gui/music", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_MUSIC ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/list_scroll", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_SCROLL ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/btn_PDA_advance", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_ADVANCE ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/btn_PDA_back", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_BACK ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/pda_next_tab", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_BUILD_ON ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/pda_prev_tab", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_BUILD_OFF ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/btn_set_focus", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_FOCUS ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/btn_roll_over", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_ROLL_OVER ] = soundShader->GetName();
	}
	soundShader = declManager->FindSound( "gui/btn_roll_out", true );
	if( soundShader != NULL ) {
		sounds[ FLAT_SHELL_SOUND_ROLL_OUT ] = soundShader->GetName();
	}
}

/*
===============
blFlatShell::SetMainMenuGuiHARDCODEDVars
===============
*/
void blFlatShell::SetMainMenuGuiHARDCODEDVars() {

	activeArea->GetFile()->SetStateString( "serverlist_sel_0", "-1" );
	activeArea->GetFile()->SetStateString( "serverlist_selid_0", "-1" );

	//activeArea->GetFile()->SetStateInt( "com_machineSpec", com_machineSpec.GetInteger() );

	// "inetGame" will hold a hand-typed inet address, which is not archived to a cvar
	activeArea->GetFile()->SetStateString( "inetGame", "" );

	// key bind names
	activeArea->GetFile()->SetKeyBindingNames();

	// flag for in-game menu
	if ( gameLocal.IsInGame() ) {
		activeArea->GetFile()->SetStateString( "inGame", ( gameLocal.gameType != GAME_SP )  ? "2" : "1" );
	} else {
		activeArea->GetFile()->SetStateString( "inGame", "0" );
	}

	activeArea->GetFile()->SetStateString( "nightmare", cvarSystem->GetCVarBool( "g_nightmare" ) ? "1" : "0" );
	activeArea->GetFile()->SetStateString( "browser_levelshot", "guis/assets/splash/pdtempa" );

	/*
	SetMainMenuSkin();
	// Mods Menu
	SetModsMenuGuiVars();

	guiMsg->SetStateString( "visible_hasxp", fileSystem->HasD3XP() ? "1" : "0" );
	*/
	activeArea->GetFile()->SetStateString( "driver_prompt", "0" );
}

/*
========================
blFlatShell::UpdateLeaderboard
========================
*/
void blFlatShell::UpdateLeaderboard() //( const idLeaderboardCallback* callback ) // the original variables of the method
{
	//TODO work on the leaderboards in the future
	/*
	lbCache->Update( callback );

	if( callback->GetErrorCode() != LEADERBOARD_ERROR_NONE )
	{
		if( !session->GetSignInManager().IsMasterLocalUserOnline() )
		{
			refreshWhenMasterIsOnline = true;
		}
	}

	refreshLeaderboard = true;
	*/
}

/*
========================
blFlatShell::ClearWidgetActionRepeater
========================
*/

void blFlatShell::ClearWidgetActionRepeater() {
	//TODO nothing implemented yet
	/*GetAreaRef()
	actionRepeater.isActive = false;
	*/
}

/*
================================================
blFlatShell::PlaySound
================================================
*/
void blFlatShell::PlaySound( flatShellSounds_t type, int channel )
{
	if( !CheckActive() ) {
		return;
	}

	if( type >= sounds.Num() ) {
		return;
	}

	if( sounds[ type ].IsEmpty() ) {
		return;
	}

	int c = SCHANNEL_ANY;
	if( channel != -1 ) {
		c = channel;
	}

	if( shellSW != NULL ) {
		shellSW->PlayShaderDirectly( sounds[ type ], c );
	} else  {
		gameLocal.Warning( "No playing sound world on soundSystem in shell '%s' play sound!", shellName.c_str() );
	}

}

/*
================================================
blFlatShell::StopSound
================================================
*/
void blFlatShell::StopSound( int channel )
{
	if( shellSW != NULL ) {
		shellSW->PlayShaderDirectly( NULL, channel );
	} else {
		gameLocal.Warning( "No playing sound world on soundSystem in shell '%s' play sound!", shellName.c_str() );
	}
}

/*
===============================================================================
			blFlatGui
===============================================================================
*/

/*
========================
blFlatGui::blFlatGui
========================
*/
blFlatGui::blFlatGui() {
	ClearList();
	activeShell = NULL;
	activeGUI = NULL;
	guiHandle = NULL;
	SetInGame( false );
	gameComplete = false;
}

/*
================
blFlatGui::~blFlatGui
================
*/
blFlatGui::~blFlatGui() {
	ClearList();
}

/*
================
blFlatGui::ClearList
================
*/
void blFlatGui::ClearList() {
	//flatList.DeleteContents( true );
	if( shellList.Num() != 0 ) {
		blFlatShell * shell;
		for( int i = 0; i < shellList.Num(); i ++ ) {
			shell = shellList[i];
			delete shell; //deletes the object but does it delete the pointer?
		}
	}
	shellList.Clear();
}

/*
================
blFlatGui::Update
================
*/
void blFlatGui::Update() {
	activeShell->Update();
	/*
	for ( int i= 0; i < ShellList.Num(); i++ ) {
		ShellList[i]->Update();
	}
	*/
}

/*
================
blFlatGui::SetActiveShell
================
*/
void blFlatGui::SetActiveShell( blFlatShell* shell ) {
	activeShell = shell;
}

/*
========================
blFlatGui::ActivateMenu
========================
*/
void blFlatGui::ActivateMenu( bool show )
{
	if( show && activeShell != NULL && activeShell->CheckActive() ) {
		return;
	} else if ( !show && activeShell != NULL && !activeShell->CheckActive() ) {
		return;
	}

	if( inGame ) {
		idPlayer* player = gameLocal.GetLocalPlayer();
		if( player != NULL ) {
			if( !show )	{
				if( ( player->health <= 0 ) && !common->IsMultiplayer() ) {
					return;
				}
			}
		}
	}

	if( activeShell == NULL ) {
		return;
	}
	if( !show )	{
		activeShell->Activate( show );
		return;
	}

	activeShell->Activate( show );

	if( show ) {
		if( !inGame ) {
			activeShell->PlaySound( FLAT_SHELL_SOUND_MUSIC );
		} else {
			return;
		}
	} else {
		activeShell->GoToArea( FLAT_SHELL_AREA_INVALID, SHELL_AREA_TRANSITION_CUT_TO );
		common->Dialog().ClearDialog( GDM_LEAVE_LOBBY_RET_NEW_PARTY ); //FIXME i don't know what dialog this closes!
	}
}

/*
================
blFlatGui::AddShell
================
*/
void blFlatGui::AddShell( idStr name, idSoundWorld* sw ) {

	//first let's check if the name is right
	const char* name_ = name.c_str();
	if( name_ == NULL || name_[ 0 ] == '\0' )
	{
		gameLocal.Warning( "the name of the shell to be added to the flat GUIs shell list is null" );
	}

	//let's then check if we already have it in the shell list
	if( shellList.Num() > 0 ) {
		for( int i = 0; i < shellList.Num(); i++ ) {
			if( shellList[i]->GetName().c_str() == name_ ) {
				gameLocal.Warning( "The flat GUIs shell list already contains a '%s' item.", name_ );
				return;
			}
		}
	}

	//let's add it and initiate it
	blFlatShell* shellItem = new( TAG_FLATGUIS ) blFlatShell();
	shellItem->InitShell( name, this, sw );

	shellList.Append( shellItem );
	//delete shellItem;
}

/*
================
blFlatGui::ReloadShells
================
*/
void blFlatGui::ReloadShells() {

	if( shellList.Num() == 0 ) {
		gameLocal.Warning( "The flat GUIs shell list is empty!" );
		return;
	}

	//let's first get the names in the flat list so we can recover them
	idList<idStr> storedNames;
	idList<idSoundWorld*> storedSW;
	storedNames.Clear();
	storedSW.Clear();

	for( int i = 0; i < shellList.Num(); i++ ) {
		storedNames.Append( shellList[i]->GetName() );
		storedSW.Append( shellList[i]->GetSW() );
	}

	//remake the shellList
	ClearList();
	for( int i = 0; i < storedNames.Num(); i++ ) {
		AddShell( storedNames[i], storedSW[i] );
	}

	//storedNames.DeleteContents( true ); // it doesn't seem to work
	storedNames.Clear();
	storedSW.Clear();
}

/*
========================
blFlatGui::UpdateSavedGames
========================
*/
void blFlatGui::UpdateSavedGames()
{
	/*
	if( activeScreen == SHELL_AREA_LOAD )
	{
		idMenuScreen_Shell_Load* screen = dynamic_cast< idMenuScreen_Shell_Load* >( menuScreens[ SHELL_AREA_LOAD ] );
		if( screen != NULL )
		{
			screen->UpdateSaveEnumerations();
		}
	}
	else if( activeScreen == SHELL_AREA_SAVE )
	{
		idMenuScreen_Shell_Save* screen = dynamic_cast< idMenuScreen_Shell_Save* >( menuScreens[ SHELL_AREA_SAVE ] );
		if( screen != NULL )
		{
			screen->UpdateSaveEnumerations();
		}
	}
	*/
}

/*
================
blFlatGui::HandleFlatGuiEvent
================
*/
bool blFlatGui::HandleFlatGuiEvent( const sysEvent_t* sev ) {
	return false; //TODO this code handles keybinding in the swf code

	/*
	if( IsPacifierVisible() )
	{
		return true;
	}

	if( showingIntro )
	{
		return true;
	}

	if( waitForBinding )
	{

		if( sev->evType == SE_KEY && sev->evValue2 == 1 )
		{

			if( sev->evValue >= K_JOY_STICK1_UP && sev->evValue <= K_JOY_STICK2_RIGHT )
			{
				return true;
			}

			if( sev->evValue == K_ESCAPE )
			{
				waitForBinding = false;

				idMenuScreen_Shell_Bindings* bindScreen = dynamic_cast< idMenuScreen_Shell_Bindings* >( menuScreens[ SHELL_AREA_KEYBOARD ] );
				if( bindScreen != NULL )
				{
					bindScreen->ToggleWait( false );
					bindScreen->Update();
				}

			}
			else
			{

				if( idStr::Icmp( idKeyInput::GetBinding( sev->evValue ), "" ) == 0 )  	// no existing binding found
				{

					idKeyInput::SetBinding( sev->evValue, waitBind );

					idMenuScreen_Shell_Bindings* bindScreen = dynamic_cast< idMenuScreen_Shell_Bindings* >( menuScreens[ SHELL_AREA_KEYBOARD ] );
					if( bindScreen != NULL )
					{
						bindScreen->SetBindingChanged( true );
						bindScreen->UpdateBindingDisplay();
						bindScreen->ToggleWait( false );
						bindScreen->Update();
					}

					waitForBinding = false;

				}
				else  	// binding found prompt to change
				{

					const char* curBind = idKeyInput::GetBinding( sev->evValue );

					if( idStr::Icmp( waitBind, curBind ) == 0 )
					{
						idKeyInput::SetBinding( sev->evValue, "" );
						idMenuScreen_Shell_Bindings* bindScreen = dynamic_cast< idMenuScreen_Shell_Bindings* >( menuScreens[ SHELL_AREA_KEYBOARD ] );
						if( bindScreen != NULL )
						{
							bindScreen->SetBindingChanged( true );
							bindScreen->UpdateBindingDisplay();
							bindScreen->ToggleWait( false );
							bindScreen->Update();
							waitForBinding = false;
						}
					}
					else
					{

						idMenuScreen_Shell_Bindings* bindScreen = dynamic_cast< idMenuScreen_Shell_Bindings* >( menuScreens[ SHELL_AREA_KEYBOARD ] );
						if( bindScreen != NULL )
						{
							class idSWFScriptFunction_RebindKey : public idSWFScriptFunction_RefCounted
							{
							public:
								idSWFScriptFunction_RebindKey( idMenuScreen_Shell_Bindings* _menu, gameDialogMessages_t _msg, bool _accept, idMenuHandler_Shell* _mgr, int _key, const char* _bind )
								{
									menu = _menu;
									msg = _msg;
									accept = _accept;
									mgr = _mgr;
									key = _key;
									bind = _bind;
								}
								idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
								{
									common->Dialog().ClearDialog( msg );
									mgr->ClearWaitForBinding();
									menu->ToggleWait( false );
									if( accept )
									{
										idKeyInput::SetBinding( key, bind );
										menu->SetBindingChanged( true );
										menu->UpdateBindingDisplay();
										menu->Update();
									}
									return idSWFScriptVar();
								}
							private:
								idMenuScreen_Shell_Bindings* menu;
								gameDialogMessages_t msg;
								bool accept;
								idMenuHandler_Shell* mgr;
								int key;
								const char* bind;
							};

							common->Dialog().AddDialog( GDM_BINDING_ALREDY_SET, DIALOG_ACCEPT_CANCEL, new idSWFScriptFunction_RebindKey( bindScreen, GDM_BINDING_ALREDY_SET, true, this, sev->evValue, waitBind ), new idSWFScriptFunction_RebindKey( bindScreen, GDM_BINDING_ALREDY_SET, false, this, sev->evValue, waitBind ), false );
						}

					}
				}
			}
		}

		return true;
	}

	return {
		if( gui != NULL && activeScreen != -1 )
		{
			return gui->HandleEvent( sev );
		}

		return false;
	}
	*/
}

} // namespace BFG
//EOF
