/*
 * CameraExplorer.cpp
 * this file is GPLv3
 *
 *  Created on: 31 de jul. 2018
 *      Author: Biel Bestué de Luna
 */

#include "../cameraExplorer/cameraExplorer.h"
#include "../../../imgui/BFGimgui.h"

#include "renderer/RenderSystem.h"
#include "d3xp/gamesys/SysCvar.h"

#include "../../ToolsInput.h"

namespace BFG
{
namespace Tools
{

//bool OTE_CameraExplorer::ShowWindow = false;

OTE_CameraExplorer::OTE_CameraExplorer() {
    Clear();
}

void OTE_CameraExplorer::HandleKeyInput() {
    if( ShowWindow ) {
        if( changes ) {
            DiscardChanges( "  Camera Explorer pannel : pannel key pressed, discarding changes.\n" );
        }
        CloseWindow();
    } else {
        ShowWindow = true;
        Clear();
        OpenWindow();
    }
}

void OTE_CameraExplorer::DiscardChanges( idStr message ) {
    common->Printf( message );
	BackToSaved();
	ApplyChanges();
}

void OTE_CameraExplorer::Clear() {
    changes = false;
	pause_changes = false;
	ShowWindow = false;
}

void OTE_CameraExplorer::GatherCurrent() {
    cur_lens.lensAberration = r_useLens.GetBool();
    cur_lens.lens_k = r_lens_k.GetFloat();
	cur_lens.lens_kcube = r_lens_kcube.GetFloat();
	cur_lens.lens_chromatic = r_lens_chromatic.GetFloat();
	cur_lens.fov = g_fov.GetInteger();
}

void OTE_CameraExplorer::OpenWindow() {
    impl::ReleaseToolMouse( true );
    ShowWindow = true;
	common->Printf( "\n  -- Camera Explorer pannel initiated --  \n\n" );
    GatherCurrent();
	saved_lens = cur_lens;
}

void OTE_CameraExplorer::CloseWindow() {
    impl::ReleaseToolMouse( false );
    Clear();
	common->Printf( "  -- Camera Explorer pannel terminated -- \n\n" );
}

void OTE_CameraExplorer::BackToSaved() {
    cur_lens = saved_lens;
    ApplyChanges();
}

void OTE_CameraExplorer::ApplyChanges() {

	g_fov.SetInteger( cur_lens.fov );
	r_useLens.SetBool( cur_lens.lensAberration );
	r_lens_k.SetFloat( cur_lens.lens_k );
	r_lens_kcube.SetFloat( cur_lens.lens_kcube );
	r_lens_chromatic.SetFloat( cur_lens.lens_chromatic );
}
void OTE_CameraExplorer::Draw() {
	bool windowReaminOpen = ShowWindow;
	if( ImGui::Begin( "Camera Explorer pannel", &windowReaminOpen, ImGuiWindowFlags_ShowBorders ) ) {
	ImGui::Spacing();
	    ImGui::SliderInt( "Focal Of View", &cur_lens.fov, 1, 180, "%.0f degrees" );
	ImGui::Spacing();
	ImGui::Spacing();
		ImGui::Checkbox( "use lens aberration", &cur_lens.lensAberration);
	ImGui::Spacing();
		ImGui::Text( "parameters:" );
	ImGui::Spacing();
	    ImGui::SliderFloat( "k", &cur_lens.lens_k, -15.0f, 5.0f, "%.2f", 2.0f );
	ImGui::Spacing();
		ImGui::SliderFloat( "kcube", &cur_lens.lens_kcube, -15.0f, 10.0f, "%.2f", 2.0f );
	ImGui::Spacing();
		ImGui::SliderFloat( "Chromatic aberration", &cur_lens.lens_chromatic, -5.0f, 5.0f, "%.2f", 2.0f );
	ImGui::Spacing();
	    //decide if we are changing anything
	    changes = false;
	    changes |= ( cur_lens.fov != saved_lens.fov );
	    changes |= ( cur_lens.lensAberration != saved_lens.lensAberration );
	    changes |= ( cur_lens.lens_k != saved_lens.lens_k );
	    changes |= ( cur_lens.lens_kcube != saved_lens.lens_kcube );
	    changes |= ( cur_lens.lens_chromatic != saved_lens.lens_chromatic );
		ImGui::Text( "you can test several effects at once by pausing the changes,\nalso, you can discard the changes and go back to the initial values." );
	ImGui::Spacing();
		if( ImGui::Button( " Apply & Save " ) ) {
			//only apply changes when there are changes
			if( changes ) {
			    common->Printf( "  Camera Explorer pannel : Applying changes, and saving them.\n" );
			    ApplyChanges();
			    saved_lens = cur_lens;
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
			    DiscardChanges( "  Camera Explorer pannel : Discarding changes back to the last saved.\n" );
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
	}
	ImGui::End();

    //thise is only called when closing with the closing window gadget
	if( ShowWindow && !windowReaminOpen ) {
	    if( !changes ) {
	        common->Printf( "  Camera Explorer pannel : windows closed.\n" );
	    } else {
            DiscardChanges( "  Camera Explorer pannel : windows closed, discarding changes.\n" );
		}
	    CloseWindow();
	}
}

void OTE_CameraExplorer::Update() {

    //draw the window
	Draw();

	//apply changes if we have to
	if( !pause_changes ) {
	    ApplyChanges();
	}
}

} //namespace Tools
} //namespace BFG
