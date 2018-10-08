/*
 * shell_menu.h
 *
 *  Created on: 24 de set. 2018
 *      Author: biel
 */

#ifndef NEO_SHELL_SHELL_MENU_H_
#define NEO_SHELL_SHELL_MENU_H_

//#include "../sys/sys_public.h"
//#include "../sys/sys_session.h"
#include "../framework/Common.h"
#include "../imgui/ImGui_Menus.h"
#include "../d3xp/menus/MenuState.h"

namespace BFG {

class idSoundWorld;
class idMenuHandler_Shell;
class idLeaderboardCallback;

enum flatSystems_t
{
	FLAT_SYSTEM_IMGUI = -1,
	FLAT_SYSTEM_FLEXIBLE,
	FLAT_SYSTEM_CEGUI,
	FLAT_SYSTEM_FLASH_SHELL,
	NUM_FLAT_SYSTEMS // it shouldn't count IMGUI since it's not file-based
};

enum uiSounds_t
{
	UI_SOUND_MUSIC,
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
	NUM_UI_SOUNDS,
};

class blMenu {
public:
	blMenu( idSoundWorld* sw, idStr filename );
	virtual ~blMenu();
	bool	IsActive();
	void	Update();

	void 	ToggleMenu( bool preference );
	//void UpdateSavedGames();
	//void SetShellState( shellState_t s );
	//void SetNextScreen( int screen, int trans );
	int		GetMenuType();
	void 	SetMenuType( int type );
	int		GetMenuScreen();
	void 	SetMenuScreen( int screen );

	//void 	SetInGame( bool val );
	//void UpdateLeaderboard( const idLeaderboardCallback* callback );
	//void 	SetCanContinue( bool valid );
	//void SetGameComplete();
	//bool GetGameComplete();

	void 	Set_MPtimeRemaining( int time ) { MP_time_remaining = time; };
	int		Get_MPtimeRemaining() { return MP_time_remaining; }

	bool 	HandleGuiEvent( const sysEvent_t* sev );

	//void			Menu_InitMenu( bool inGame );
	void			Menu_InitMenu();
	void			Menu_ClosePause();
	void			Menu_ClearRepeater();
	void			Menu_Toggle( bool show );
	void			Menu_SyncWithSession();
	void			Menu_UpdateSavedGames();
	void			Menu_UpdateClientCountdown( int countdown );
	void			Menu_UpdateLeaderboard( const idLeaderboardCallback* callback );
	//void			Menu_SetCanContinue( bool valid );
	//void			Menu_SetGameComplete();

private:
	void	Clear();
	int		Determine_the_System_level();
	void	HandleSoundList( idStr element, int item );

	idMenuHandler_Shell* 	shellHandler; // the shell system has it's own handler already coded in
	blImGuiMenu* 	menu_ImGui;
	//blGuiMenu*	menu_Gui;
	int				flatSystemLevel;

	bool			menuActive;
	int				MP_time_remaining;
	idSoundWorld*	MenuSW;
	idList<idStr> 	SoundList;
};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_MENU_H_ */
