/*
 * shell.h
 *
 *  Created on: 10 d’ag. 2018
 *      Author: biel
 */

#ifndef NEO_SHELL_SHELL_H_
#define NEO_SHELL_SHELL_H_

//#include "../sys/sys_public.h"
//#include "../sys/sys_session.h"
#include <shell/shell_loadingmap.h>
#include "../framework/Common.h"
#include "../imgui/ImGui_Menus.h"
#include "../d3xp/menus/MenuState.h"
#include "../shell/shell_background.h"
#include "../shell/shell_menu.h"

namespace BFG {

enum popUps_t
{
	POP_UP_NONE = -1,
	POP_UP_INFO,
	POP_UP_ACCEPT,
	POP_UP_ACCEPT_CANCEL,
	NUM_POP_UPS
};

enum transitions_t
{
	TRANSITION_NONE = -1,
	TRANSITION_CUT,
	TRANSITION_FADE,
	TRANSITION_WIPE,
	TRANSITION_BURNOUT,
	TRANSITION_DOOM_SNAKES,
	NUM_TRANSITIONS
};

class blShell {
public:
	blShell();
	~blShell();

	bool 	IsActive() { return shellActive; };
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
	bool			Menu_HandleGuiEvent( const sysEvent_t* sev );
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
};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_H_ */
