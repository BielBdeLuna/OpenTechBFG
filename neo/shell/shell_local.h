/*
 * shell_local.h
 *
 *  Created on: 15 de maig 2019
 *      Author: Biel Bestué de Luna
 */

#include <shell/shell.h>
#include "../shell/shell_flat_system.h" //no longer used
#include "../idlib/ParallelJobList.h"
#include "../idlib/LangDict.h"
#include "../idlib/StrStatic.h"
#include "../idlib/LangDict.h"
#include "../sys/sys_local.h"
#include "../sys/sys_session.h"
#include "../framework/FileSystem.h"
#include "../framework/DeclEntityDef.h"
#include "../framework/Common_local.h"
#include "../framework/Console.h"

#include "../shell/inputEvent.h"
#include "../shell/shell_command.h"
#include "../shell/shell_branches.h"

#ifndef NEO_SHELL_SHELL_LOCAL_H_
#define NEO_SHELL_SHELL_LOCAL_H_

namespace BFG {

enum uiSounds_t
{
	UI_SOUND_MUSIC_MAINMENU,
	UI_SOUND_SCROLL,
	UI_SOUND_ADVANCE,
	UI_SOUND_BACK,
	UI_SOUND_BUILD_ON,
	UI_SOUND_BUILD_OFF,
	UI_SOUND_FOCUS,
	UI_SOUND_ROLL_OVER,
	UI_SOUND_ROLL_OUT,
	UI_SOUND_INPUT_CONNECTED,
	UI_SOUND_INPUT_DISCONNECTED,
	NUM_UI_SOUNDS
};

enum guiMenus_t
{
	/*
	GUI_ACTIVE,
	GUI_INGAME,
	GUI_MAINMENU,
	GUI_RESTARTMENU,
	GUI_LOADING,
	GUI_INTRO,
	GUI_GAMEOVER,
	GUI_TEST,
	GUI_TAKENOTES,
	GUI_MSG,
	GUI_MSGRESTORE,
	*/

	UI_ACTIVE,
	UI_MAINMENU,
	UI_LOADING,
	UI_INGAME_HUD,
	UI_INGAME_TRANSCRIPT,
	UI_INGAME_INVENTORY,
	UI_INGAME_MAP,
	UI_INGAME_TALK,
	UI_DEAD,
	UI_MSG,
	UI_TEST,
	NUM_UI_ELEMENTS
};

struct InputEventsAndIndexes {
	blInputEvent InputEventPtr;
  idList<int> indexes;
};

class blShellLocal : public blShell
{
public:
		//virtual			blShellLocal();
		//virtual			~blShellLocal();
		virtual void		Init();
		virtual void		Shutdown();

		virtual void		StartContent();
		virtual void		AsyncFrame_RunBranchLists();


    virtual void    BranchControl_AppendBranch( blShellBranch newBranch ) { branches_l.append( newBranch ); };
		virtual void		BranchControl_TogglePause( idStr BranchId );
		virtual bool		BranchControl_IsPaused( idStr BranchId );
		virtual void		BranchControl_Kill( idStr BranchId );

		//inputs and system events
		virtual bool		Check_SystemEvents( idList<ShellEvent> PossibleSystemEvents_l );

		//GUIs
		virtual void		SetGUI( idUserInterface* gui, HandleGuiCommand_t handle );
		virtual void		GuiFrameEvents();
		virtual void		DispatchCommand( idUserInterface* gui, const char * menuCommand, bool doIngame ); //this should be in every gui shell branch so every menu scripts it's actions
		virtual void		RedrawActiveGUIs();

		//rendering
		virtual void		Render_SingleMaterial( ShellSplashStageContent_t content );

		//menu events
		virtual void		Event_LoadGame( idStr saveGameName );
		virtual void		Event_SaveGame( idStr saveGameName );
		virtual void		Event_QuitGame(); //FIXME
		virtual void		Event_QuitToMainMenu(); //FIXME
		virtual void		Event_ResumeGame(); //FIXME

		//session state
		virtual int			Event_Get_SessionState(); //enum : idSession::sessionState_t
		virtual void		Event_Set_SessionState( int newState ); //FIXME

		//main menu stuff
		virtual bool		Event_MainMenu_isOpen() { return MainMenu_isOpen; }
		virtual void		Event_MainMenu_Toggle();

    //input stuff
    virtual void    Event_SetCommandEvent( idStr commandEvent );
    virtual bool    Event_CheckCommandEvent( idStr commandEvent );
    virtual void    Event_DeleteCommandEvent( idStr commandEvent );

    virtual void    Event_SetInputCommandEvent( idStr key, idStr commandEvent );
    virtual void    Event_DeleteInputCommandEvent( idStr key );

    virtual void    Event_SetShellCommand( idStr name );
    virtual void    Event_SubShellCommand( idStr name );

    virtual void    Event_SetInputEventToShellCommand( idStr givenName, int eventType, int event );
    virtual void    Event_SubInputEventFromShellCommand( idStr givenName, int eventType, int event );

		//virtual void			InitShellManager();
		/*
		virtual void			InitFrontEnd();
		virtual void			UpdateFrontEnd();
		virtual void			InitBackEnd();
		virtual void 			UpdateBackEnd();
		virtual void			InitSynch();
		virtual void			InitAsynch();
		virtual void			InitFirstSplashContent();
		virtual void 			HardcodedInitialContent();
		*/
		/*
		virtual void			MoveSessionToIdle();
		virtual void 			InitGuis();
		virtual bool 			Check_MenuActive();
		virtual void 			OpenMenu();
		virtual void 			CloseMenu();
		virtual void			Synch_update();
		virtual void			Asynch_update( const sysEvent_t* sev );
		virtual void			FeedSplashStage( idMaterial* mat, float ar, idVec4 tCol, idVec4 bCol, int time, bool interrupt );
		virtual void			FeedMenuStage( float ar, idVec4 bCol, int sTime );
		virtual void			FeedCurrentStage();
		virtual void 			FeedTheStretchRenderList( idMaterial* mat, idVec4 tCol, idVec4 area, idVec4 UVarea );

		idUserInterface*		GetActiveMenu( void );
		void					SetGui( idUserInterface *gui, HandleGuiCommand_t handle );

		virtual void			SetGuiPrepStart( uint64 _time = Sys_Milliseconds() ) { guiTimes.prep_start_time = _time; };
		virtual uint64			GetGuiPrepStart() { return guiTimes.prep_start_time; };
		virtual void 			SetGuiRenderStart( uint64 _time = Sys_Milliseconds() ) { guiTimes.gui_render_start_time = _time; };
		virtual uint64			GetGuiRenderStart() { return guiTimes.gui_render_start_time; };
		virtual void 			SetGuiRenderEnd( uint64 _time = Sys_Milliseconds() ){ guiTimes.gui_render_end_time = _time; };
		virtual uint64			GetGuiRenderEnd() { return guiTimes.gui_render_end_time; };

		virtual void			SetGuiReference( int element, idUserInterface* ref );


		virtual guiTimes_t 		guiTimes;
		*/
protected:
		virtual void		Clear();
    virtual void    Render_RenderCommandBuffers();
private:

    //HARDCODED SCRIPTS

    void      ProcessBranch_script( blShellBranchProcessed* parentBranch );

		//branch system

		void			PrepFirstSplashContent( blShellBranchLineal branch );

		idList<blShellBranch>	branches_l;
		idList<blShellBranch*>	branches_active_l;
		idList<blShellBranch*>	branches_paused_l;

		int 			CheckIdIndexInList( idStr BranchId, idList<blShellBranch> List );

		blShellAsync	shellThread; // the logic of the UI is worked in it's independent thread from the rendering thread


		bool			MainMenu_isOpen;

		//idUserInterface *		guiActive;
		//HandleGuiCommand_t		guiHandle;
		idList<ShellEventCommand>		Shell_event_commands_l; //FIXME OLD
		idList<GUI_element>				gui_l;
		idList<GUI_element>				guiActive_l;

		//FIXME		does this two need to be private and virtual? I don't think so
		virtual void 			InhibitUserCommandsGeneration( bool force_off );
		virtual void			ToggleMouseCursorFocus( bool grabMouse );

    idList<idStr>             CommandEvents_l; //FIXME OLD
    idList<idStr>             InputCommandEvents_l //FIXME OLD

    //ShellCommands functions
    //the ShellCommands_l list is a ShellCommand centric list of all ShellCommands important to shell

    int                       ShellCommand_SearchShellCommand( idStr name );

    int                       ShellCommand_AddShellCommand( idStr name );
    void                      ShellCommand_RemoveShellCommand( idStr name );
    void                      ShellCommand_GetState( idStr name );
    void                      ShellCommand_AssociateInputEvent( idStr name, InputEventRef_s newIEreference );
    void                      ShellCommand_DissociateInputEvent( idStr name, InputEventRef_s unwantedIEreerence );
    void                      ShellCommand_Sync();

    idList<blShellCommand>    ShellCommands_l;

    //input events stuf
    int                       ShellInputEvents_SearchInputEvent( InputEventRef_s indicatedIEreference );
    void                      ShellInputEvents_PurgeInputEvent( blInputEvent* GivenInputEvent, int indexSkip );

    int                       ShellInputEvents_AddInputEvent( blInputEvent newInputEvent );
    void                      ShellInputEvents_SubInputEvent( InputEventRef_s unwantedIEreference );
    void                      ShellInputEvents_Synch();

    idList<blInputEvent>      ShellInputEvents_l;

    //idList<InputEventsAndIndexes>      RelevantInputEvents_l;
    //idList<InputEvent*>       RelevantInputEvents_l;

    //InputEvent Stuff

    void                      ShellInputEvent_TestInputEvents();

		/*
		bool					HasBeenInterrupted( const sysEvent_t* sev );
		ShellSplashStageContent_t			currentShellSplashStage;
		idList<ShellSplashStageContent_t>	shellSplashStagesFeed_l;
		idList<stretchedRenderTarget_t> 	StretchRenderFeed_l;

		bool					menuActive;

		blShellSplash* 				splashContent;
		idUserInterfaceManager*		uiManager;

		HandleGuiCommand_t	guiHandle;
		idListGUI *			guiMainMenu_MapList;		// easy map list handling

		idSoundWorld* 		menuSW;
		idArray<idStr, NUM_UI_SOUNDS> 				uiSoundArray;
		idArray<idUserInterface*, NUM_UI_ELEMENTS>	uiArray;

		blShellAsync		shellThread; // the logic of the UI is asynchronous with the rendering
		*/
};

blShellLocal	shellLocal;
blShell* 		shell = &shellLocal;

/*
class blShellManagerLocal : blShellManager {

//friend class blShellLocal;

public:




	virtual void			Manager_SystemEvents();

private:


	blShellAsync	shellThread; // the logic of the UI is worked in it's independent thread from the rendering thread

	idList<int> EscapeKs;


	//idParallelJobList*	ParallelBranchJobs_l;
};

blShellManagerLocal	shellManagerLocal;
blShellManager* 	shellManager = &shellManagerLocal;
*/
} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_LOCAL_H_ */
