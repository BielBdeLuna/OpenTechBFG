/*
    this file is GPLv3
*/


#include <framework/Common.h>
//#include "../../../renderer/RenderSystem.h"
//#include "d3xp/gamesys/SysCvar.h"
//#include <imgui/BFGimgui.h>
#include "../../../imgui/BFGimgui.h"

#include "renderStats.h"

namespace BFG {
namespace Tools {

renderStatsPannel::renderStatsPannel() {
    Clear();
}

void renderStatsPannel::Clear() {
    PannelPresent = false;
    timeStats.previous = 0;
    timeStats.timeMs = 0.0f;
    timeStats.maxTimeMs = 0.0f;
}

void renderStatsPannel::ClosePannel() {
    common->Printf( "\n  -- Render stats pannel ended --  \n\n" );
    Clear();
}

void renderStatsPannel::OpenPannel() {
    common->Printf( "\n  -- Render stats pannel initiated --  \n\n" );
    PannelPresent = true;

}

void renderStatsPannel::Toggle() {
    if( PannelPresent && !com_showFPS.GetBool() ) {
        //only close when the pannel is present and com_showFPS goes off
        ClosePannel();
    } else if( !PannelPresent && com_showFPS.GetBool() ){
        //discard any old info
        Clear();
        //only Open when the pannel is not present and there is some kind of com_showFPS
        OpenPannel();
    }
}

void renderStatsPannel::Draw() {
    bool PannelRemainOpen = PannelPresent;
    if( com_showFPS.GetInteger() == 1 ) {
        if( ImGui::Begin( "Render stats pannel", &PannelRemainOpen, ImGuiWindowFlags_ShowBorders ) ) {
            ImGui::Spacing();
            ImGui::Text( "fps: 0fps" );
            ImGui::Spacing();
            ImGui::Text( "frametime: %.0f ms", timeStats.timeMs );
            ImGui::Text( "max frametime: %.0f ms", timeStats.maxTimeMs );
            ImGui::Spacing();
        }
	    ImGui::End();

	} else {
	    if( ImGui::Begin( "Render stats pannel", &PannelRemainOpen, ImGuiWindowFlags_ShowBorders ) ) {
            ImGui::Spacing();
            ImGui::Text( "parameters:" );
            ImGui::Spacing();
            ImGui::Text( "              YES" );
            ImGui::Spacing();
        }
        ImGui::End();
	}
}

void renderStatsPannel::Update() {
    //only calc the frametimes if we are active
    if( PannelPresent ) {
        int t = Sys_Milliseconds();
	    timeStats.timeMs = t - timeStats.previous;
	    timeStats.previous = t;
	    if( timeStats.maxTimeMs < timeStats.timeMs ) {
	        timeStats.maxTimeMs = timeStats.timeMs;
	    }

	    timeStats.fps = 1000000 * timeStats.timeMs;
	}

    Draw();
}


} //namespace Tools
} //namespace BFG
