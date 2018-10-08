/*
 * ImGui_Menus.h
 *
 *  Created on: 6 d’ag. 2018
 *      Author: biel
 */

#ifndef NEO_D3XP_MENUS_IMGUI_MENUS_H_
#define NEO_D3XP_MENUS_IMGUI_MENUS_H_

#include "../imgui/ImGui_Hooks.h"


namespace BFG {

enum menuTypes_t
{
	MENU_INVALID = -1,
	MENU_START, // press "start" for initiating, in d3BFG this is the game selection, we could turn it into the Mod Selection.
	MENU_MAIN,
	MENU_PAUSE,
	MENU_PAUSE_MP,
	MENU_LOADING,
	MENU_GAME_SPECIAL, //d3's PDA, could be a map menu, the character screen, or an inventory, it's the button besides "start" in the game-pad.
	//MENU_HUD,
	MENU_DEAD,
	MENU_NUM
};

enum screenTypes_t
{
	SCREEN_INVALID = -1,
	SCREEN_MAIN, //this should be the actual first screen of any type of menu
	SCREEN_QUIT,
	SCREEN_OPTIONS,
	SCREEN_INPUT,
	SCREEN_VIDEO,
	SCREEN_AUDIO,
	SCREEN_SAVE,
	SCREEN_LOAD,
	SCREEN_NUM
};


class blImGuiMenu {
public:
	blImGuiMenu();
	virtual ~blImGuiMenu();

	void	Draw();
	void 	Init();
	void	openMenu();
	void	closeMenu();

	int getMenuScreen() const {
		return current_screen;
	}

	int getMenuType() const {
		return current_type;
	}

	void SetMenuScreen( int screen ) {
		ideal_screen = screen;
	}

	void SetMenuType( int type ) {
		ideal_type = type;
	}

private:
	void 	Clear();
	bool	showImGuiMenu;
	int		ideal_type;
	int		current_type;
	int		ideal_screen;
	int		current_screen;
};

} /* namespace BFG */

#endif /* NEO_D3XP_MENUS_IMGUI_MENUS_H_ */
