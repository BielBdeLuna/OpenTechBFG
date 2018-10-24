/*
 * shell_flat_System.cpp
 *
 *  Created on: 9 d’oct. 2018
 *      Author: biel
 */

#include <shell/shell_flat_system.h>
#include "../framework/Common.h"

namespace BFG
{

idCVar ui_flat_SytemLevelThreshold( "ui_flat_SytemLevelThreshold", "-1", CVAR_INTEGER | CVAR_SYSTEM | CVAR_GUI, "set a forced system level threshold to be used in flat GUIs from a probable set of diverging capabilities, where:\n -1 --> imGUI\n 0 --> GUI\n 1 --> ceGUI\n 2 --> flash based GUI\n" );

int Determine_flat_System_level() {
	bool	Gui_exists = false;
	int decide = ui_flat_SytemLevelThreshold.GetInteger();
	int result = decide;

	if( /*something yet to be determined*/ true ) { //TODO
		Gui_exists = false;
	}

	if( !Gui_exists ) {
		result =  FLAT_SYSTEM_IMGUI;
	} else {
		if( decide >= FLAT_SYSTEM_FLEXIBLE ) {
			result = decide;
		} else {
			result = FLAT_SYSTEM_IMGUI;
		}
	}

	return result;
}

} // namespace BFG
