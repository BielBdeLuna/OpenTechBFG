/*
 * ImGui_Menus.cpp
 *
 *  Created on: 6 d’ag. 2018
 *      Author: biel
 */

#include "../imgui/ImGui_Menus.h"
#include "../imgui/BFGimgui.h"
//#include "../d3xp/Game_local.h"
#include "../framework/Common.h"

namespace BFG {

blImGuiMenu::blImGuiMenu() {
	Clear();
}

void blImGuiMenu::Clear() {
	showImGuiMenu = false;
	ideal_type = MENU_INVALID;
	ideal_screen = MENU_INVALID;
	current_type = MENU_INVALID;
	current_screen = SCREEN_INVALID;
}

blImGuiMenu::~blImGuiMenu() {
	Clear();
}

void blImGuiMenu::Init(){
	if( !showImGuiMenu ) {
		showImGuiMenu = true;
	}
}

void blImGuiMenu::openMenu() {
	if( !showImGuiMenu ) {
		showImGuiMenu = true;
	}

	if( current_type != ideal_type ) {
		if( ideal_type == MENU_INVALID ) {
			common->Error( "blImGuiMenu::openMenu error: ImGUI menu set-up for MENU_INVALID\n" );
		}
		current_type = ideal_type;
	}
	if( current_screen != ideal_screen ) {
		if( ideal_screen == SCREEN_INVALID ) {
			ideal_screen = SCREEN_MAIN;
		}
		current_screen = ideal_screen;
	}
}

void blImGuiMenu::closeMenu() {
	ideal_screen = SCREEN_MAIN; // make sure we always open up the main screen the next time we open the pause menu
	if( showImGuiMenu ) {
		showImGuiMenu = false;
	}
}

void blImGuiMenu::Draw() {
	bool showingMenu = showImGuiMenu;

	switch( current_type ) {
		case MENU_MAIN:
			if( ImGui::Begin( "Main Menu", &showingMenu, ImGuiWindowFlags_ShowBorders ) ) {
				ImGui::Text( "the will be the main menu." );

			}
			ImGui::End();
			break;
		case MENU_PAUSE:
			if( ImGui::Begin( "Pause Menu", &showingMenu, ImGuiWindowFlags_ShowBorders ) ) {
				ImGui::Text( "the will be the pause menu." );

			}
			ImGui::End();
			break;
		case MENU_LOADING:
			if( ImGui::Begin( "Loading...", &showingMenu, ImGuiWindowFlags_ShowBorders ) ) {
				ImGui::Text( "Loading..." );

			}
			ImGui::End();
			break;
		/*case MENU_HUD:
			if( ImGui::Begin( "hud", &showingMenu, ImGuiWindowFlags_ShowBorders ) ) {
				ImGui::Text( "health:" );

			}
			ImGui::End();
			break;*/
		default: //MENU_START
			/*
			if( ImGui::Begin( "Open Tech Engine", &showingMenu, ImGuiWindowFlags_ShowBorders ) ) {
				ImGui::Text( "press 'Start' to initiate." );
			}
			ImGui::End();
			*/
			break;
	}

	/*
		ImGui::Begin( "Open Tech Engine", &showingMenu, ImGuiWindowFlags_ShowBorders );
		ImGui::Text( "hello world!" );
		ImGui::End();
	*/

}

} /* namespace BFG */
