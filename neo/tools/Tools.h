/*
 * this file is GPLv3
 */

#ifndef NEO_TOOLS_TOOLS_H_
#define NEO_TOOLS_TOOLS_H_
//#include "../idlib/CmdArgs.h"
//#include "../framework/CVarSystem.h"
#include <idlib/Dict.h>
//#include "../idlib/Dict.h"

// in moddable doom3 Game.h was in framework/ and not to be modified by mods
// we need it for GameEdit definition and idEntity declaration
#include <d3xp/Game.h>
//#include "../d3xp/Game.h"

#include "./compilers/compiler_public.h"
#include "./editors/editors_public.h"

namespace BFG
{

void ShowEditors_f( const idCmdArgs& args );

namespace Tools
{

bool AreEditorsActive();

void DrawToolWindows();

//void LightEditorInit( const idDict* dict, idEntity* entity );

void CameraExplorerInit();

}//namespace tools
}//namespace BFG

#endif /* NEO_TOOLS_TOOLS_H_ */
