/*
 * shell_loadingmap.cpp
 *
 *  Created on: 7 d’oct. 2018
 *      Author: biel
 */

#include <shell/shell_loadingmap.h>
#include "../imgui/BFGimgui.h"

namespace BFG {

blLoadingMap::blLoadingMap( idDict _data ) {
	percentage = 0.0f;
	IsWaiting = false;
	if( Determine_flat_System_level() != FLAT_SYSTEM_IMGUI ) {
		DataManager();
	} else {
		common->Printf( "starting the imGui loading map display!\n");
	}

}

void blLoadingMap::DataManager() {
	/*
	idStr materialName;
	idVec4 tintColour;

	materialName = data.GetString( "backgroundMaterial", '\0' );
	tintColour = data.GetVec4( "backgroundTint", "0 0 0 1.0" );

	if( materialName.IsEmpty() ) {
		background->InitNone();
	} else {
		background->InitMaterial( materialName, tintColour );
	}
	*/
	common->Printf( "loading screen: oops non-imgui system not yet implemented| \n" );
}

void blLoadingMap::Update() {
	if( Determine_flat_System_level() == FLAT_SYSTEM_IMGUI ) {
		imGuiDraw();
	} else {
		common->Printf( "loading map with guis not yet implemented!\n");
	}
}

void blLoadingMap::imGuiDraw() {
	bool showingGUI = true;

	ImGui::Begin( "Main Menu", &showingGUI, ImGuiWindowFlags_NoInputs );
	ImGui::Spacing();
	ImGui::Indent();
	ImGui::Indent();
	ImGui::Text( data.GetString( "name", "non-level-name" ) );
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text( "loading ...\n" );
	ImGui::Spacing();
	const ImVec2* size_arg;
	size_arg->ImVec2();
	//ImGui::ProgressBar( percentage, size_arg, "loaded" );
	ImGui::End();
}

} /* namespace BFG */
