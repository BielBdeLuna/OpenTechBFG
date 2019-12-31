/*
 * shell_sync.cpp
 *
 *  Created on: 19 de mar. 2019
 *      Author: Biel
 */


/*
This should be the connection between a threaded shell system for the UI and this in-sync part related to the
common system for more developer-centric UI features in the future
*/

#include <shell/shell.h>
#include "../renderer/RenderSystem.h"
#include "../idlib/CmdArgs.h"
#include "../framework/CmdSystem.h"
#include "../framework/DeclManager.h"

namespace BFG {

blShellSync::blShellSync() {
	Clean();
};

void blShellSync::Clean() {
	AspectRatioSegment = AR_CUSTOM;
}

void blShellSync::Init() {
	idLib::Printf( "Synchronized Shell Initiated\n");

	//ParseGameRT( "exec game.rt\n" );
	//get the game.rt file
		//parse game.rt file
		//list the elements to display
		//list the demos
}
extern typedef struct commandDef_s;

void blShellSync::Update() {
	idLib::Printf( "update sync Shell\n");
}

}
