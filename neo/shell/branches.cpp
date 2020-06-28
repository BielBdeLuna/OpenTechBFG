/*
 * branches.cpp
 *
 *  Created on: 29 12 2019
 *      Author: Biel Bestué de Luna
 */

#include "../shell/branches.h"

namespace BFG {

/*******************************************************************************
								SHELL BRANCHES
*******************************************************************************/

/*
=================
blShellBranchLineal::Init
=================
*/
void blShellBranchLineal::Init() {
	hidden = true;
	Clear();
	PreProcess();
}

/*
=================
blShellBranchLineal::Shutdown
=================
*/
void blShellBranchLineal::Shutdown() {
	Clear();
}

/*
=================
blShellBranchLineal::Clear
=================
*/
void blShellBranchLineal::Clear() {
	splashContent_l.Clear();
}

/*
=================
blShellBranchLineal::PreProcess()
=================
*/
void blShellBranchLineal::PreProcess() {
	LastFrameTime = 0;
	CurrentIndex = 0;
}

/*
=================
blShellBranchLineal::PostProcess()
=================
*/
void blShellBranchLineal::PostProcess() {
	//TODO HARDCODED
	if( GetBranchId() == "First Branch" ) {
		//start main menu
		shell->Event_MainMenu_Toggle();
	}
	shell->BranchControl_Kill( GetBranchId() ); //self kill!
}

/*
=================
blShellBranchLineal::RunFrame
=================
*/
void blShellBranchLineal::RunFrame() {
	if( CurrentIndex < splashContent_l.Num ) {
		ShellSplashStageContent_t SplashContent = splashContent_l[CurrentIndex];

		int CurrentTime = Sys_Milliseconds();
		if( CurrentTime <= ( SplashContent.elapsedTime + LastFrameTime ) ) {
			shell->Render_SingleMaterial( SplashContent );
			if( ( idStr::Icmp( SplashContent.material->GetName(), "splashVideo" ) == 0 ) && !SplashContent.material->CinematicIsPlaying() ) {
				CurrentIndex++;
			}

			if( ( SplashContent.Interrupt_events_l.num() > 0 ) && shell->Check_SystemEvents( SplashContent.Interrupt_events_l ) ) {
				CurrentIndex++;
			}
		} else {
			LastFrameTime = CurrentTime;
			CurrentIndex++;
			if( idStr::Icmp( splashContent_l[CurrentIndex].material->GetName(), "splashVideo" ) == 0 ) {
				splashContent_l[CurrentIndex].material->ResetCinematicTime( CurrentTime );
			}
		}
	} else {
		PostProcess();
	}
}

/****************
	GUI branch
*****************/

/*
=================
blShellBranchGui::Init
=================
*/
void blShellBranchGui::Init() {
	hidden = true;
	Clear();
	PreProcess();
}

/*
=================
blShellBranchGui::Shutdown
=================
*/
void blShellBranchGui::Shutdown() {
	Clear();
}

/*
=================
blShellBranchGui::Clear
=================
*/
void blShellBranchGui::Clear() {
	guiMainMenu = NULL;
}

/*
=================
blShellBranchGui::PreProcess
=================
*/
void blShellBranchGui::PreProcess() {

	//TODO HARDCODED

	// we have a single instance of the main menu
	guiMainMenu = uiManager->FindGui( "guis/mainmenu.gui", true, false, true );
	//guiMainMenu_MapList = uiManager->AllocListGUI();
	//guiMainMenu_MapList->Config( guiMainMenu, "mapList" );
	//idAsyncNetwork::client.serverList.GUIConfig( guiMainMenu, "serverList" );
	//commonLocal.Game(); //TODO find where I can find the server list
	//guiRestartMenu = uiManager->FindGui( "guis/restart.gui", true, false, true );
	//guiGameOver = uiManager->FindGui( "guis/gameover.gui", true, false, true );
	//guiMsg = uiManager->FindGui( "guis/msg.gui", true, false, true );
	//guiTakeNotes = uiManager->FindGui( "guis/takeNotes.gui", true, false, true );
	//guiIntro = uiManager->FindGui( "guis/intro.gui", true, false, true );

	//guiInGame = NULL;
	//guiTest = NULL;

	//what is the function of the handler? we have it here so we can set the GUI in the shell
	HandleGuiCommand_t handler; //so it works
	shell->SetGUI( guiMainMenu, handler );
}

/*
=================
blShellBranchGui::RunFrame
=================
*/
void blShellBranchGui::RunFrame() {
	// send frame and mouse events to active guis
	shell->GuiFrameEvents(); //TODO shouldn't this be handled by the shell to all the guis?
	//then render
	shell->RedrawActiveGUIs();
}

/****************
	full screen material branch
*****************/

/*
=================
blShellBranchFullScreenMaterial::SetContent
=================
*/
void blShellBranchFullScreenMaterial::SetContent( 	static idMaterial* mat,
													idVec4 		tint,
													idVec4 		backCol,
													idVec2 		pos,
													float 		aspectR,
													idVec2      size,
													idVec4 		percent
												)
{
	//it's just a simple setter, just a little wordy
	Material = mat;
	TintColour = tint;
	BackroundColour = backCol;
	Position = pos;
	IntendedAspectRatio = aspectR;
	Size = size;
	TexturePercentages = percent;
}

/*
=================
blShellBranchFullScreenMaterial::RunFrame
=================
*/
void blShellBranchFullScreenMaterial::RunFrame() {

	idMaterial* tintMat = declManager->FindMaterial( "_white" );

	const float	virtWidth = ( float )renderSystem->GetVirtualWidth();
	const float virtHeight = ( float )renderSystem->GetVirtualHeight();
	const float sysWidth = ( float )renderSystem->GetWidth() * renderSystem->GetPixelAspect();
	const float sysHeight = ( float )renderSystem->GetHeight();
	const float sysAspect = sysWidth / sysHeight;
	const float adjustment = sysAspect / IntendedAspectRatio; //bigger than 1 if content is wider than sysAspect else if taller, 1 if square

	const float imageSize;
	const float chop;
	if( adjustment >= 1.0f ) {
		imageSize = virtWidth * IntendedAspectRatio / sysAspect;
		chop = 0.5f * ( virtWidth - imageSize );
	} else {
		imageSize = virtHeight * IntendedAspectRatio / sysAspect;
		chop = 0.5f * ( virtHeight - imageSize );
	}
	const float barHeight = ( adjustment >= 1.0f ) ? 0.0f : ( 1.0f - adjustment ) * virtHeight * 0.25f;
	const float barWidth = ( adjustment <= 1.0f ) ? 0.0f : ( adjustment - 1.0f ) * virtWidth * 0.25f;


		/*
		idStr materialText;
		materialText.Format( "{ translucent { videoMap %s } }", path );

		idMaterial* material = const_cast<idMaterial*>( declManager->FindMaterial( "splashbink" ) );
		material->Parse( materialText.c_str(), materialText.Length(), false );
		material->ResetCinematicTime( Sys_Milliseconds() );


		// RB: FFmpeg might return the wrong play length so I changed the intro video to play max 30 seconds until finished
		int cinematicLength = material->CinematicLength();
		cinematicLength = 30000; //so we don't care  if FFmpeg gave correct time or not, we will kill it if it stops playing anyway
		int	mouseEvents[MAX_MOUSE_EVENTS][2];
		*/

		//then the actual image with the correct aspect ration it should have

	if( idStr::Icmp( Material->GetName(), "splashVideo" ) == 0 ) {
		// is a video

		renderSystem->SetColor( TintColour );
		if( adjustment >= 1.0f ) {
			//wider
			renderSystem->DrawStretchPic( chop, 0, imageSize, virtHeight, 0, 0, 1, 1, Material );
		} else {
			//taller
			renderSystem->DrawStretchPic( 0, chop, virtHeight, imageSize, 0, 0, 1, 1, Material );
		}
	} else {
		// is not a video

		//first the black sides of the image for those images whose aspect ration doesn't correspond to the renderSystem's
		if( barHeight > 0.0f ) {
			renderSystem->SetColor( BackroundColour );
			renderSystem->DrawStretchPic( 0, 0, virtWidth, barHeight, 0, 0, 1, 1, tintMat );
			renderSystem->DrawStretchPic( 0, virtHeight - barHeight, virtWidth, barHeight, 0, 0, 1, 1, tintMat );
		} if( barWidth > 0.0f ) {
			renderSystem->SetColor( BackroundColour );
			renderSystem->DrawStretchPic( 0, 0, barWidth, virtHeight, 0, 0, 1, 1, tintMat );
			renderSystem->DrawStretchPic( virtWidth - barWidth, 0, barWidth, virtHeight, 0, 0, 1, 1, tintMat );
		}

		renderSystem->SetColor( TintColour );
		renderSystem->DrawStretchPic( barWidth, barHeight, virtWidth - barWidth * 2.0f, virtHeight - barHeight * 2.0f, 0, 0, 1, 1, Material );
	}

	const emptyCommand_t* cmd = renderSystem->SwapCommandBuffers( common->Pt_TimeFrontEnd, common->Pt_TimeBackEnd, common->Pt_TimeShadows, common->Pt_TimeGpu );
	renderSystem->RenderCommandBuffers( cmd );

}

/*****************
 processed branch
******************/

/*
=================
blShellBranchProcessed::StartProcess
=================
*/
void blShellBranchProcessed::StartProcess( idList<blShellBranch>* branchesList ) {
    Branches_l_ptr = branchesList;
    shell->ProcessBranch_script( this );

}

} /* namespace BFG */
