/*
 * CameraExplorer.cpp
 * this file is GPLv3
 *
 *  Created on: 31 de jul. 2018
 *      Author: Biel Bestué de Luna
 */

#include "../cameraExplorer/cameraExplorer.h"
#include "../../../imgui/BFGimgui.h"

#include "../../../renderer/RenderSystem.h"
#include "../../../d3xp/gamesys/SysCvar.h"

#include <stdio.h>

namespace BFG
{
namespace Tools
{

blCameraExplorer blCameraExplorer::TheCameraExplorer;
bool blCameraExplorer::ShowWindow = false;

void blCameraExplorer::Init() {
	Clear();
	GatherCurrent();
	org_lens = cur_lens;
}

void blCameraExplorer::Clear() {
    changes = false;
	pause_changes = false;
	ShowWindow = false;
}

void blCameraExplorer::GatherCurrent() {
    cur_lens.lensAberration = r_useLens.GetBool();
    cur_lens.lens_k = r_lens_k.GetFloat();
	cur_lens.lens_kcube = r_lens_kcube.GetFloat();
	cur_lens.lens_chromatic = r_lens_chromatic.GetFloat();
	cur_lens.fov = g_fov.GetInteger();
}

void blCameraExplorer::OpenWindow() {
	if( !ShowWindow ) {
		common->Printf( "\n  -- Camera Explorer pannel initiated --  \n\n" );
		TheCameraExplorer.Init();
		ShowWindow = true;
	} else {
		ShowWindow = false;
	}
}

void blCameraExplorer::CloseWindow() {
    Clear();
	common->Printf( "  -- Camera Explorer pannel terminated -- \n\n" );
	impl::SetReleaseToolMouse( false );
}
void blCameraExplorer::Update() {
	TheCameraExplorer.Draw();
}

void blCameraExplorer::BackToOriginal() {
    cur_lens = org_lens;
    ApplyChanges();
}

void blCameraExplorer::ApplyChanges() {

	g_fov.SetInteger( cur_lens.fov );
	r_useLens.SetBool( cur_lens.lensAberration );
	r_lens_k.SetFloat( cur_lens.lens_k );
	r_lens_kcube.SetFloat( cur_lens.lens_kcube );
	r_lens_chromatic.SetFloat( cur_lens.lens_chromatic );
}
void blCameraExplorer::Draw() {
	bool ShowingWindow = ShowWindow;
	if( ImGui::Begin( "Camera Explorer pannel", &ShowingWindow, ImGuiWindowFlags_ShowBorders ) ) {
	ImGui::Spacing();
	    char buffer [5];
	    sprintf ( buffer, "%.0d", org_lens.fov );
	    idStr number = buffer;
	    idStr SliderTitle = " Focal Of View ( " + number + ")";
		ImGui::SliderInt( SliderTitle, &cur_lens.fov, 1, 180, "%.0f degrees" );
	ImGui::Spacing();
	ImGui::Spacing();
		ImGui::Checkbox( "use lens aberration", &cur_lens.lensAberration);
	ImGui::Spacing();
		ImGui::Text( "parameters:" );
	ImGui::Spacing();
	    sprintf ( buffer, "%.2f", org_lens.lens_k );
	    number = buffer;
	    SliderTitle =" k ( " + number + ")";
		ImGui::SliderFloat( SliderTitle, &cur_lens.lens_k, -15.0f, 5.0f, "%.2f", 2.0f );
	ImGui::Spacing();
	    sprintf ( buffer, "%.2f", org_lens.lens_kcube );
	    number = buffer;
	    SliderTitle = " kcube ( " + number + ")";
		ImGui::SliderFloat( SliderTitle, &cur_lens.lens_kcube, -15.0f, 10.0f, "%.2f", 2.0f );
	ImGui::Spacing();
	    sprintf ( buffer, "%.2f", org_lens.lens_chromatic );
	    number = buffer;
	    SliderTitle = " Chromatic aberration ( " + number + ")";
		ImGui::SliderFloat( SliderTitle, &cur_lens.lens_chromatic, -5.0f, 5.0f, "%.2f", 2.0f );
	ImGui::Spacing();
	    //decide if we are changing anything
	    changes = false;
	    changes |= ( cur_lens.fov != org_lens.fov );
	    changes |= ( cur_lens.lensAberration != org_lens.lensAberration );
	    changes |= ( cur_lens.lens_k != org_lens.lens_k );
	    changes |= ( cur_lens.lens_kcube != org_lens.lens_kcube );
	    changes |= ( cur_lens.lens_chromatic != org_lens.lens_chromatic );
		ImGui::Text( "you can test several effects at once by pausing the changes,\nalso, you can discard the changes and go back to the initial values." );
	ImGui::Spacing();
		if( ImGui::Button( " Apply " ) ) {
			//only apply changes when there are changes
			if( changes ) {
			    common->Printf( "  Camera Explorer pannel : Applying changes.\n" );
			    ApplyChanges();
			    org_lens = cur_lens;
			    changes = false;
			} else {
			    common->Printf( "  Camera Explorer pannel : No changes to apply.\n" );
			}
		}
	ImGui::SameLine();
	    idStr discardButton;
	    if( !changes  ) {
			discardButton = " Cancel ";
		} else {
			discardButton = " Discard ";
		}

		if( ImGui::Button( discardButton ) ) {
			if( changes ) {
			    common->Printf( "  Camera Explorer pannel : Discarding changes.\n" );
			    BackToOriginal();
			    ApplyChanges();
			}

			CloseWindow();
		}
	ImGui::SameLine();
		idStr changeButton;
		if( !pause_changes  ) {
			changeButton = " pause changes ";
		} else {
			changeButton = " un-pause changes ";
		}

		if( ImGui::Button( changeButton ) ) {
			if( !pause_changes  ) {
				pause_changes = true;
			} else {
				pause_changes = false;
			}
		}


		/*if( changes  && !pause_changes ) {
			ApplyChanges();
		}*/
		if( !pause_changes ) {
		    ApplyChanges();
		}
	}
	ImGui::End();

    //thise is only called when closing with the closing window gadget
	if( ShowWindow && !ShowingWindow ) {
	    if( !changes ) {
	        common->Printf( "  Camera Explorer pannel : windows closed.\n" );
	    } else {
	        common->Printf( "  Camera Explorer pannel : windows closed discarding changes.\n" );
		    BackToOriginal();
		    ApplyChanges();
		}
		//ShowWindow = ShowingWindow;
	    CloseWindow();

	}
}

} //namespace Tools
} //namespace BFG
