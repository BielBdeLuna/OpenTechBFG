/*
 * this file is GPLv3
 */


#include "../imgui/BFGimgui.h"

#include "Tools.h"
#include "ToolsInput.h"

#include "./editors/lightEditor/LightEditor.h"
#include "./editors/cameraExplorer/cameraExplorer.h"

namespace BFG
{

//extern idCVar g_editEntityMode;

#if 0 // currently this doesn't make too much sense
void ShowEditors_f( const idCmdArgs& args )
{
	showToolWindows = true;
}
#endif // 0


namespace Tools
{

// things in impl need to be used in at least one other file, but should generally not be touched
namespace impl
{

void ReleaseToolMouse( bool doRelease )
{
	releaseMouse = doRelease;
}

} //namespace impl


static OTE_CameraExplorer TheCameraExplorer;
//static LightEditor TheLightEditor; // FIXME: maybe at some point we could allow more than one..

bool AreEditorsActive()
{
	// FIXME: this is not exactly clean and must be changed if we ever support game dlls
	//return g_editEntityMode.GetInteger() > 0;
	return true;

}

bool ReleaseMouseForTools()
{
	return AreEditorsActive() && releaseMouse;
}

void DrawToolWindows()
{
#if 0
	ImGui::Begin( "Show Ingame Editors", &showToolWindows, 0 );

	ImGui::Checkbox( "Light", &LightEdicameraExp.tor::showIt );
	ImGui::SameLine();
	ImGui::Checkbox( "Particle", &showParticlesEditor );
#endif // 0
    /*
	if( TheLightEditor.showIt )
	{
		TheLightEditor.Draw();
	}
	*/
	if( TheCameraExplorer.ShowWindow )
	{
		TheCameraExplorer.Update();
	}

	// TODO: other editor windows..
	//ImGui::End();
}
/*
void LightEditorInit( const idDict* dict, idEntity* ent )
{
	if( dict == NULL || ent == NULL ) return;

	// NOTE: we can't access idEntity (it's just a declaration), because it should
	// be game/mod specific. but we can at least check the spawnclass from the dict.
	idassert( idStr::Icmp( dict->GetString( "spawnclass" ), "idLight" ) == 0
			  && "LightEditorInit() must only be called with light entities or NULL!" );

    impl::ReleaseToolMouse( true );
	TheLightEditor.showIt = true;
	TheLightEditor.ReInit( dict, ent );
}
*/
void CameraExplorerInit() {
    ;
}

} //namespace Tools

void CameraExplorer_f( const idCmdArgs& args ) {
	//Tools::impl::ReleaseToolMouse( true );
	Tools::TheCameraExplorer.HandleKeyInput(); // TheCameraExplorer defined after the class definition
}

} //namespace BFG
