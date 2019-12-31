/*
 * shell.h
 *
 *  Created on: 10 d’ag. 2018
 *      Author: Biel Bestué de Luna
 */

#ifndef NEO_SHELL_SHELL_H_
#define NEO_SHELL_SHELL_H_

//#include "../sys/sys_public.h"
//#include "../sys/sys_session.h"
#include "../idlib/Thread.h"
#include <shell/shell_loadingmap.h> //FIXME old
#include "../framework/Common.h"
#include "../imgui/ImGui_Menus.h"
#include "../d3xp/menus/MenuState.h"
#include "../shell/shell_splash.h" //FIXME old
#include "../ui/UserInterface.h"
/*
#include "../shell/shell_background.h"
#include "../shell/shell_menu.h"
*/

namespace BFG {

/*
enum popUps_e
{
	POP_UP_NONE,
	POP_UP_INFO,
	POP_UP_ACCEPT,
	POP_UP_ACCEPT_CANCEL,
	NUM_POP_UPS
};

enum transitions_e
{
	TRANSITION_CUT, //same as no transition
	TRANSITION_FADE_TO_COLOUR,
	TRANSITION_CROSSFADE,
	TRANSITION_WIPE_LEFT,
	TRANSITION_WIPE_RIGHT,
	TRANSITION_WIPE_UP,
	TRANSITION_WIPE_DOWN,
	TRANSITION_BURNOUT,
	TRANSITION_DOOM_SNAKES,
	NUM_TRANSITIONS
};
*/
enum CommonAspectRatios_e
{
	AR_NONE = -1, //stretched to virtual resolution so effectually no clear AR
	AR_SQUARE,	//purely square
	AR_SQUARE_4_3,
	AR_WIDE,	//custom horizontal
	AR_WIDE_16_9,
	AR_WIDE_16_10,
	AR_VERTICAL, //custom vertical
	AR_VERTICAL_9_16,
	NUM_COMMONASPECTRATIOS
};

/*
enum
{
	SYSTEM_EVENTS_KEYBOARD	= 1 << 0, 	// same as 1, binary 0001
	SYSTEM_EVENTS_MOUSE		= 1 << 1, 	// same as 2, binary 0010
	SYSTEM_EVENTS_GAMEPAD	= 1 << 2, 	// same as 3, binary 0100
	SYSTEM_EVENTS_ALL 		= SYSTEM_EVENTS_KEYBOARD | SYSTEM_EVENTS_MOUSE | SYSTEM_EVENTS_GAMEPAD // binary 0111
};
*/

enum
{
	MACRO_SYSTEM,			// "ps" button / "xbox" button / "steam" button functionality
	MACRO_MENU, 			// "play" button / "esc" button functionality
	MACRO_OPTIONS,			// "option" button functionality
	MACRO_ACCEPT,			// "A" button
	MACRO_CANCEL,			// "B" button
	MACRO_UP,				// up in the menu
	MACRO_DOWN,				// down in the menu
	MACRO_LEFT,				// left in the menu
	MACRO_RIGHT,			// right in the menu
	MACRO_SCROLL_UP,		// scroll up in the menu texts
	MACRO_SCROLL_DOWN,		// scroll down in the menu texts
	MACRO_SCROLL_LEFT,		// scroll left in the menu texts
	MACRO_SCROLL_RIGHT,		// scroll right in the menu texts
	MACRO_NEXT,				// select next section in the menu
	MACRO_PREVIOUS,			// select previous section in the menu

};

typedef const char * ( *HandleGuiCommand_t )( const char* );

//this is the simple event
struct ShellEvent {
	int					EventType;
	int					Event;
};

//this is what an specific event means for the shell.
struct ShellEventCommand {
	ShellEvent ev;
	idStr				command;
};

struct ShellSplashStageContent_t {
	static idMaterial*		material;
	float					aspectRatio;	//the aspect ratio of the video, picture, or menu
	idVec4					backgroundColour; //the colour of the backround black bars, which will mostly be black
	idVec4					tintColour;
	idList<ShellEvent>		Interrupt_events_l;
	int						startTime;	//time, in milliseconds
	int						elapsedTime;
};

struct stretchedRenderTarget_t
{
	static idMaterial*	material;
	idVec4				tintColour;
	float				x;
	float				y;
	float				length;
	float				height;
	float				topmost_tex_pct;
	float				leftmost_tex_pct;
	float				downmost_tex_pct;
	float 				rightmost_text_pct;
};

class blShellAsync : public idSysThread {
private:
	int	Run();
};

struct guiTimes_t {
	uint64 prep_start_time;
	uint64 gui_render_start_time;
	uint64 gui_render_end_time;
};

struct GUI_element {
	idUserInterface* gui;
	HandleGuiCommand_t handle;
}

class blShell {
public:
	//virtual blShell() = 0;
	virtual 		~blShell() = 0;
	virtual void	Init() = 0;
	virtual void	Shutdown() = 0;

	virtual void	StartContent() = 0;
	virtual void	AsyncFrame_RunBranchLists() = 0;

  virtual void  BranchControl_AppendBranch( blShellBranch newBranch ) = 0;
	virtual void	BranchControl_ToggleHidden( idStr BranchId ) = 0;
	virtual bool	BranchControl_IsHidden( idStr BranchId ) = 0;
	virtual void	BranchControl_Kill( idStr BranchId ) = 0;

	//inputs and system events
	virtual bool			Check_SystemEvents( idList<ShellEvent> PossibleSystemEvents_l ) = 0;

	//GUIs
	virtual void			SetGUI( idUserInterface* gui, HandleGuiCommand_t handle ) = 0;
	virtual void			GuiFrameEvents() = 0;
	virtual void			DispatchCommand( idUserInterface* gui, const char* menuCommand, bool doIngame ) = 0;
	virtual void			RedrawActiveGUIs() = 0;

	//rendering
	virtual void			Render_SingleMaterial( ShellSplashStageContent_t content ) = 0;

	//events
	virtual void			Event_LoadGame( idStr saveGameName ) = 0;
	virtual void			Event_SaveGame( idStr saveGameName ) = 0;
	virtual void			Event_QuitGame() = 0;
	virtual void			Event_QuitToMainMenu() = 0;
	virtual void			Event_ResumeGame() = 0;

	virtual int				Event_Get_SessionState() = 0;
	virtual void			Event_Set_SessionState( int newState ) = 0;

	virtual bool			Event_MainMenu_isOpen() = 0;
	virtual void			Event_MainMenu_Toggle() = 0;

  virtual void      Event_SetCommandEvent( idStr commandEvent ) = 0;
  virtual bool      Event_CheckCommandEvent( idStr commandEvent ) = 0;
  virtual void      Event_DeleteCommandEvent( idStr commandEvent ) = 0;

  virtual void      Event_SetInputCommandEvent( idStr key, idStr commandEvent ) = 0;
  virtual void      Event_DeleteInputCommandEvent( idStr key ) = 0;

	/*
	virtual void	InitFrontEnd() = 0;
	virtual void	UpdateFrontEnd() = 0;
	virtual void	InitBackEnd() = 0;
	virtual void 	UpdateBackEnd() = 0;
	virtual void	InitSynch() = 0;
	virtual void	InitAsynch() = 0;
	virtual void 	InitFirstSplashContent() = 0;
	virtual void 	InitGuis() = 0;
	virtual void	MoveSessionToIdle() = 0;
	virtual bool 	Check_MenuActive() = 0;
	virtual void 	OpenMenu() = 0;
	virtual void 	CloseMenu() = 0;
	virtual void	Synch_update() = 0;
	virtual void 	Asynch_update( const sysEvent_t* sev ) = 0;
	virtual void	FeedSplashStage( idMaterial* mat, float ar, idVec4 tCol, idVec4 bCol, int time, bool interrupt ) = 0;
	virtual void	FeedMenuStage( float ar, idVec4 bCol, int sTime ) = 0;
	virtual void	FeedCurrentStage() = 0;
	virtual void 	FeedTheStretchRenderList( idMaterial* mat, idVec4 tCol, idVec4 area, idVec4 UVarea ) = 0;
	virtual void	SetGuiPrepStart( uint64 _time = Sys_Milliseconds() ) = 0;
	virtual uint64	GetGuiPrepStart() = 0;
	virtual void 	SetGuiRenderStart( uint64 _time = Sys_Milliseconds() ) = 0;
	virtual uint64	GetGuiRenderStart() = 0;
	virtual void 	SetGuiRenderEnd( uint64 _time = Sys_Milliseconds() ) = 0;
	virtual uint64	GetGuiRenderEnd() = 0;
	virtual void	SetGuiReference( int element, idUserInterface* ref ) = 0;

	virtual guiTimes_t guiTimes = 0;
	 */
protected:
	virtual void 	  Clear() = 0;
  virtual void    Render_RenderCommandBuffers() = 0;

private:
	virtual void 			InhibitUserCommandsGeneration( bool force_off ) = 0;
	virtual void			ToggleMouseCursorFocus( bool grabMouse ) = 0;
/*
public:

	//bool 	IsActive() { return shellActive; };
	void	Update();

	void 	SetNextTranstition( int _next ) { transition_ideal = _next; };
	int		GetCurrentTransition() { return transition_current; };

	bool	GetInputFocus() { return menuInFocus; };
	void 	FocusInputOnMenu( bool set ) { menuInFocus = set; };


	void			Menu_Init( idSoundWorld* sw, idStr filename );
	void			Menu_InitMenu();
	void			Menu_ClosePause();
	void			Menu_ClearRepeater();
	void			Menu_Toggle( bool show );
	bool			Menu_IsActive();
	bool			Menu_HandleGuiEvent( const sysEvent_t* sev ); //this is called mainly when triying to bind a key to an action in the key/joy binding menu
	//void			Menu_Update();
	void			Menu_SyncWithSession();
	void			Menu_UpdateSavedGames();
	void			Menu_UpdateClientCountdown( int countdown );
	void			Menu_UpdateLeaderboard( const idLeaderboardCallback* callback );
	//void			Menu_SetCanContinue( bool valid );
	//void			Menu_SetGameComplete();

	bool			Background_IsActive() { return ( background != NULL ); };
	void			Background_Init( idStr material_name, idVec4 tint );
	void 			Background_Close();

	//non interface are the loading screens, they are not a menu, nor they are as simple as a background
	void			LoadingMap_Init( idStr mapName );
	idDict			LoadingMap_GatherData( idStr mapName );
	void			LoadingMap_SetPercentage( float percentage );
	bool			LoadingMap_IsActive() {  return ( loadingMap != NULL ); };
	bool			LoadingMap_GetWaiting();
	void			LoadingMap_SetWaiting( bool Switch );
	void			LoadingMap_Close();


	//heads Up Dispaly  or HUD are a sub-family of a non inreface
	//void			HUD_Init();
	//void			HUD_Update();
	//void			HUD_Pause();
	//void			HUD_UnPause();

protected:
	void 	Clear();

	blMenu*			menu;
	blBackground*	background;
	blLoadingMap *	loadingMap;
	//blHud*		headsUpDisplay;

	int				transition_ideal;
	int				transition_current;
	bool			InTranstion;

	//blImGuiPopUp	menu_popUp_ImGui;
	//blGuiPopUp	menu_popUp_Gui;
	//blBackground	menu_background;

	//int				menu_type;
	//int				menu_screen;


	bool			menuInFocus;
	bool			shellActive;

//protected:
	//struct MenuVars;
	//MenuVars* ourVars;

*/
};

extern blShell* 	shell;

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_H_ */
