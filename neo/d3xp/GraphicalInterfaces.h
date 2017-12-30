/*
 * GraphicalInterfaces.h
 *
 *      Author: biel
 *      this file is GPLv3
 */

#ifndef NEO_D3XP_GRAPHICALINTERFACES_H_
#define NEO_D3XP_GRAPHICALINTERFACES_H_

#include "../framework/File.h"
#include "../framework/FileSystem.h"
#include "../idlib/containers/List.h"   // for idList
#include "../idlib/Str.h"               // for idStr
#include "../sound/sound.h"             // for idSoundWorld (ptr only), etc
#include "../ui/UserInterface.h"

//#include "../d3xp/gamesys/SaveGame.h"
#include "../d3xp/gamesys/SysCvar.h"

namespace BFG
{

/*
===============================================================================

	flat guis

===============================================================================
*/

enum flatShellAreasTransitions_t
{
	SHELL_AREA_TRANSITION_NONE = -1,
	SHELL_AREA_TRANSITION_CUT_TO,
	SHELL_AREA_TRANSITION_FADE_TO_BLACK,
	SHELL_AREA_TRANSITION_NUM
};

enum flatShellTransitionStatus_t
{
	SHELL_TRANSITION_STATUS_NONE = -1,
	SHELL_TRANSITION_STATUS_INTRO,
	SHELL_TRNASITION_STATUS_CHANGE,
	SHELL_TRANSITION_STATUS_OUTRO,
	SHELL_TRANSITION_STATUS_NUM
};

enum flatShellState_t
{
	MENU_STATE_INVALID = -1,
	MENU_STATE_PRESS_START,
	MENU_STATE_IDLE,
	MENU_STATE_PARTY_LOBBY,
	MENU_STATE_GAME_LOBBY,
	MENU_STATE_PAUSED,
	MENU_STATE_CONNECTING,
	MENU_STATE_SEARCHING,
	MENU_STATE_LOADING,
	MENU_STATE_BUSY,
	MENU_STATE_IN_GAME
};

enum flatShellAreas_t
{
	FLAT_SHELL_AREA_INVALID = -1,
	FLAT_SHELL_AREA_START, //select dooms ( and maybe the first screens with the developer / publisher stuff )
	FLAT_SHELL_AREA_ROOT, // main menu
	FLAT_SHELL_AREA_DEV, // screen where you can select the starting map
	FLAT_SHELL_AREA_CAMPAIGN, // screen where you can select continue, new game, load game
	FLAT_SHELL_AREA_LOAD,
	FLAT_SHELL_AREA_SAVE,
	FLAT_SHELL_AREA_NEW_GAME, //select what campaing start a new game, doom3, d3xp, lost mission
	FLAT_SHELL_AREA_DIFFICULTY, //select difficulty of the new game
	FLAT_SHELL_AREA_SETTINGS,
	FLAT_SHELL_AREA_CONTROLS,
	FLAT_SHELL_AREA_KEYBOARD,
	//FLAT_SHELL_AREA_GAMEPAD,
	//FLAT_SHELL_AREA_CONTROLLER_LAYOUT,
	FLAT_SHELL_AREA_GAME_OPTIONS,
	FLAT_SHELL_AREA_SYSTEM_OPTIONS,
	FLAT_SHELL_AREA_AUDIO,
	FLAT_SHELL_AREA_VIDEO,
	//FLAT_SHELL_AREA_STEREOSCOPICS,
	FLAT_SHELL_AREA_MULTIPLAYER,
	//FLAT_SHELL_AREA_GAME_LOBBY,			->
	//FLAT_SHELL_AREA_PARTY_LOBBY,
	//FLAT_SHELL_AREA_LEADERBOARDS,
	//FLAT_SHELL_AREA_PLAYSTATION, //?? maybe mp main menu?
	//FLAT_SHELL_AREA_RESOLUTION, //?? maybe the little bar that allows you to quit the game, or to quit the map you're playing?
	//FLAT_SHELL_AREA_MATCH_SETTINGS,
	//FLAT_SHELL_AREA_MODE_SELECT,
	//FLAT_SHELL_AREA_BROWSER,
	FLAT_SHELL_AREA_CREDITS,
	FLAT_SHELL_AREA_PAUSE, //screen of a pause when saving amidst the game in a
	FLAT_SHELL_NUM_AREAS
};

enum flatShellSounds_t
{
	FLAT_SHELL_SOUND_MUSIC,
	FLAT_SHELL_SOUND_SCROLL,
	FLAT_SHELL_SOUND_ADVANCE,
	FLAT_SHELL_SOUND_BACK,
	FLAT_SHELL_SOUND_BUILD_ON,
	FLAT_SHELL_SOUND_BUILD_OFF,
	FLAT_SHELL_SOUND_FOCUS,
	FLAT_SHELL_SOUND_ROLL_OVER,
	FLAT_SHELL_SOUND_ROLL_OUT,
	NUM_FLAT_SHELL_SOUNDS,
};

enum flatSystems_t
{
	FLAT_SYSTEM_IMGUI = -1,
	FLAT_SYSTEM_GUI,
	FLAT_SYSTEM_CEGUI,
	FLAT_SYSTEM_FLASH,
	NUM_FLAT_SYSTEMS // it shouldn't count IMGUI since it's not file-based
};

class blFlatGui;
class blFlatShell;
typedef const char * (*HandleGuiCommand_t)( const char * );

class blFlatShellArea
{
public:
	blFlatShellArea();
	~blFlatShellArea();

	void						Init( idStr fileName_, int areaRef, blFlatShell* shell_, blFlatGui*	flatGuiHandler_ ); //inits the area private-wise
	void						StartArea(); //sets the time in the area to 0, so all it's animations get reset
	void						UpdateFiles();
	void						GetHandleGuiCommand( HandleGuiCommand_t handle ) { guiHandle = handle; };
	int							GetBestOption();
	int							GetAreaRef() { return areaRef; };
	idUserInterface * 			GetFile() { return GUIfile; };


	//class idUserInterface* 		GUIfile;
	idUserInterface* 			GUIfile;
	//idSWF						SWFfile;
	//cegui						ceGUIfile;
	idList<bool>				fileType; // imGUI, GUI, CEGUI, FLASHGUI
	blFlatShell* 				shell; // so we can communicate back
	blFlatGui*					flatGuiHandler;



private:
	void						ClearList();
	idList<bool>  				CompareFilesToFlatSystems( idStr fileName );
	void						LoadAreaFile();
	int							areaRef;
	idStr						fileName;
	HandleGuiCommand_t			guiHandle;
};

class blFlatShell
{
public:
	blFlatShell();
	~blFlatShell();

	void						InitShell( idStr shellName_, blFlatGui*	flatGuiHandler_, idSoundWorld* sw );
	void						GoToArea( int areaNumber, int transition );
	void						Update();
	void						UpdateAreaList();
	void						SetActiveShell();
	void						SetActiveArea( int areaRef, HandleGuiCommand_t handle );
	void						Activate( bool b );
	bool						CheckActive() { return shellActive; };
	void						DoInhibitControl( bool inhibited ) { inhibitControl = inhibited; };
	bool						CheckInhibitedControl() { return inhibitControl; };
	void						SetContinueInsteadOfNewGame( bool valid ) { continueInsteadOfNewGame = valid; };
	void						SetTimeRemaining( int time ) { timeRemaining = time; };
	void						UpdateLeaderboard();//( const idLeaderboardCallback* callback );
	void						ClearWidgetActionRepeater(); //there are no widgets implemented yet so it is a stump now.

	int							nextArea;
	int							currentArea;

	idStr						GetName() { return shellName; };
	idSoundWorld*				GetSW() { return shellSW; };
	void						SetMainMenuGuiHARDCODEDVars();
	void						PlaySound( flatShellSounds_t type, int channel = -1 );
	void						StopSound( int channel = SCHANNEL_ANY );

private:
	void						ClearList();
	void						HARDCODEDsolution();
	void						AddArea( idStr fileName, int intName );
	void						RestartShell();
	void						RestartCurrentArea();
	idStr						shellName;
	idStr						qpath;
	bool						shellActive;
	bool						inhibitControl; // the control in the GUI not the control inGame, does tge GUI use the mouse or the joystick?
	bool						continueInsteadOfNewGame; // should be a variable in the "campaign" screen!
	int							timeRemaining;				// should be a variable in the

	idList<blFlatShellArea*>	areaList;
	idStaticList< idStr, NUM_FLAT_SHELL_SOUNDS >		sounds;

	idSoundWorld* 				shellSW;

	blFlatGui*					flatGuiHandler;
	blFlatShellArea*			activeArea;

	//blFlatShellArea*			nextArea;
	//blFlatShellArea*			lastArea;
	//int							transitionStatus; // -1 none, 0 last area, 1 do the transition, 2 next area
	//int							idealTransitionStatus;

	//flatShellAreasTransitions_t currentTransition;
	//flatShellAreasTransitions_t idealTransition;
};

class blFlatGui
{
public:
	blFlatGui();
	~blFlatGui();

	//void						Save( idSaveGame* savefile ) const;
	//void						Restore( idRestoreGame* savefile );

	void						ReloadShells();
	void						AddShell( idStr name, idSoundWorld* sw );
	void						SetInGame( bool val ) { inGame = val; };
	bool						CheckInGame() { return inGame; };
	void						SetActiveShell( blFlatShell* shell );
	void						ActivateMenu( bool show );
	void						Update();
	void						UpdateSavedGames(); //TODO a stump!
	void						SetGameComplete() { gameComplete = true; };
	bool						GetGameComplete() { return gameComplete; };
	bool						HandleFlatGuiEvent( const sysEvent_t* sev ); //TODO a stump!
	void						SetNextMenuState( flatShellState_t nms ) { nextMenuState = nms; };
	void						SetMenuState( flatShellState_t ms ) { menuState = ms; };
	flatShellState_t			GetNextMenuState() { return nextMenuState; };
	flatShellState_t			GetMenuState() { return menuState; };

	blFlatShell*				activeShell;
	idUserInterface* 			activeGUI;
	HandleGuiCommand_t			guiHandle;
	//idSWF						activeSWF;
	//cegui						activeCeGUI;

private:
	idList<blFlatShell*>			shellList;
	bool						inGame;
	bool						gameComplete;
	flatShellState_t			menuState;
	flatShellState_t			nextMenuState;


	void			ClearList();
	//bool			CheckFile( idStr fileName );
	//flatListItem_t	InitItem( flatListItem_t item, idSoundWorld* sw );
	//bool			CheckInFolders( idStr name, idStr extension, bool binary );
};

/*
===============================================================================

	volumetric guis

===============================================================================
*/

/*

typedef struct
{
	idStr				name;
	bool				hardcoded;
	idList<bool>		fileType;
} volumetricListItem_t;

class blVolumetricGui
{
public:
	blVolumetricGui();
	~blVolumetricGui();

	void			Save( idSaveGame* savefile ) const;
	void			Restore( idRestoreGame* savefile );

	void			Init();

	void			ReloadVolumetricGuis();
private:
	idList<volumetricListItem_t>		volumetricList;
};
*/

} // namespace BFG
#endif /* NEO_D3XP_GRAPHICALINTERFACES_H_ */
