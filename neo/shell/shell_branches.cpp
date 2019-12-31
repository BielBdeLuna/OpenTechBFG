/*
 * shell_branches.cpp
 *
 *  Created on: 29 12 2019
 *      Author: Biel Bestué de Luna
 */

#include "../shell/shell_branches.h"

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

/****************
	input branch
*****************/

/*
=================
blShellBranchEscapeInputs::Init
=================
*/
/*
void blShellBranchEscapeInputs::Init() {
	paused = true;
	Clear();
}
*/
/*
=================
blShellBranchEscapeInputs::Shutdown
=================
*/
/*
void blShellBranchEscapeInputs::Shutdown() {
	Clear();
}
*/
/*
=================
blShellBranchEscapeInputs::Clear
=================
*/
/*
void blShellBranchEscapeInputs::Clear() {
	Escape_Inputs_l.Clear();
}
*/
/*
=================
blShellBranchEscapeInputs::RunFrame
=================
*/

//void blShellBranchEscapeInputs::RunFrame() {
	/*
	if( shell->Check_SystemEvents( Escape_Inputs_l ) ) {

	}
	*/
//	;
//}

/*
=================
blShellBranchEscapeInputs::AddEscapeInputs
=================
*/
/*
void blShellBranchEscapeInputs::AddEscapeInputs( idList<ShellInterruptEvent> Possible_Escape_Inputs_l ) {
	Escape_Inputs_l.Append( Possible_Escape_Inputs_l );
}
*/

/*
=================
blShellBranchInput::Init
=================
*/
void blShellBranchInput::Init() {
    Clear();
}

/*
=================
blShellBranchInput::Shutdown
=================
*/
void blShellBranchInput::Shutdown() {
    Clear();
}

/*
=================
blShellBranchInput::Clear
=================
*/
void blShellBranchInput::Clear() {
    UiCommands_l.Clear();
    pairs_l.Clear();
    UiCommandsFlagged_l.Clear();
}

/*
=================
blShellBranchInput::CheckUiCommand

is the UiCommand in the UiCommands list?
=================
*/
bool blShellBranchInput::CheckUiCommand( idStr DistinctUiCommand ) {
    bool result = false;

    while( int i = 0; i < UiCommands_l.Num(); i++ ) {
        if( UiCommands_l[i].Icmp( DistinctUiCommand ) ) {
            result = true;
            break;
        }
    }

    return result;
}

/*
=================
blShellBranchInput::Set_UiCommand

add the UiCommand to the list
=================
*/
void blShellBranchInput::Set_UiCommand( idStr NewUiCommand ) {
    UiCommands_l.AddUnique( NewUiCommand );
}

/*
=================
blShellBranchInput::Del_UiCommand

extract the UiCommand from the list
and clean off the paired input events to that UiCommand rom the pairs list
=================
*/
void blShellBranchInput::Del_UiCommand( idStr UnwantedUiCommand ) {
    int index;
    index = UiCommands_l.find( UnwantedUiCommand );
    if( index != -1 ) {
        UiCommands_l.RemoveIndex( index );
    }

    //clean off the command pair lists too or that specific command
    for( int i = pairs_l.Num() - 1; i >= 0; i-- ) {
        if( pairs_l[i].UiCommand.Icmp( UnwantedUiCommand ) ) {
            pairs_l.RemoveIndex( i );
        }
    }
}

/*
=================
blShellBranchInput::Set_InputEventPairedWithUiCommand

pairs a new InputEvent to a UiCommandPair
and adds the new UiCommand to the UiCommands list if it didn't exist there already
=================
*/
void blShellBranchInput::Set_InputEventPairedWithUiCommand( InputUiCommandPair NewPair, bool exclusive = false ) {
    //TODO add the exclusivity feature:
    // so if the event or the UiCommandsEvent are related both to
    // other events or UiCommandsEvent then delete those other relationships
    // before we add the new one

    //it sets a new UiCommand in the UiComand list if it doesn't exists already
    Set_UICommandEvent( NewPair.UiCommand );

    bool hasUiCommand = false;
    bool hasInputEvent = false;

    for( int i = 0; i < pairs_l.Num(); i++ ) {
        //let's check if we have those same events paired to that UiCommandsEvent
        if( pairs_l[i].UiCommand.Icmp( NewPair.UiCommand ) ) {
            hasUiCommand = true;
            bool isTheSame = ( pairs_l[i].InputEvent.EventType == NewPair.InputEvent.EventType );

            if( isTheSame ) {
                isTheSame = ( pairs_l[i].InputEvent.Event == NewPair.InputEvent.Event );
            }

            //if the events for that UiCommandsEvent are different then we should append them
            if( !isTheSame ) {
                hasInputEvent = false;
                break;
            } else {
                hasInputEvent = true;
            }
        } else {
            hasUiCommand = false;
            //it is then irelevant if the pairs in the list
            //have the same evnts given that are for other UiCommandsEvents
        }
    }

    //we only append if we don't have the same event registered to the same UiCommandsEvents
    if( !( hasUiCommand && hasInputEvent ) ) {
        pairs_l.Append( NewPair );
    }
}

/*
=================
blShellBranchInput::Del_InputEvent
removes the UnwantedInputEvent rom the pairs list i it exists
=================
*/
void blShellBranchInput::Del_InputEvent( ShellEvent UnwantedInputEvent ) {
    for( int i = pairs_l.Num() - 1; i >= 0; i-- ) {
        if( pairs_l[i].InputEvent.Event == UnwantedInputEvent.Event ) {
            if( pairs_l[i].InputEvent.EventType == UnwantedInputEvent.EventType ) {
                // the UnwantedInputEvent is the same from the one in the pair so
                // remove the pair associated
                pairs_l.RemoveIndex( i );
            }
        }
    }
}

/*
=================
blShellBranchInput::flag_UiCommand
add the UiCommand to the flagged list that gets checked by external branches
=================
*/
void blShellBranchInput::flag_UiCommand( idStr DistinctUiCommand ) {

    if( UiCommands_l.Num() == 0 ) {
        return;
    }

    if( !CheckUiCommand( DistinctUiCommand ) ) {
        common->Error( "blShellBranchInput::flag_UiCommand : attempting to flag active the ui command '%s' while not in the ui commands list!\n", DistinctUiCommand.c_str() );
    } else {
        UiCommandsFlagged_l.AddUnique( DistinctUiCommand );
    }

}

/*
=================
blShellBranchInput::flag_UiCommand
flag the UiCommands paired to the DistinctInputEvent ShellEvent
=================
*/
void blShellBranchInput::flag_PairedUiCommands( ShellEvent DistinctInputEvent ) {
    for( int i = 0; i < pairs_l.Num(); i++ ) {
        if( ( pairs_l[i].InputEvent.Event == DistinctInputEvent.Event ) && ( pairs_l[i].InputEvent.EventType == DistinctInputEvent.EventType ) ){
            flag_UiCommand( pairs_l.UiCommand );
        }
    }
}

/*
=================
blShellBranchInput::RunFrame
=================
*/

void blShellBranchInput::RunFrame() {
    idList<ShellEvent> PossibleInputEvents_l;

    for( int i = 0; i < pairs_l.Num(); i++ ) {
        PossibleInputEvents_l.AddUnique( pairs_l[i].InputEvent );
    }

    UiCommandsFlagged_l.Clear();

    //TODO
    //TODO bool escapeEvent = false;
    //TODO

    Sys_GenerateEvents();

	// queue system events ready for polling
	Sys_GetEvent();

	for( int i = 0; i < PossibleInputEvents_l.Num(); i++ ) {
	    ShellEvent ie = PossibleInputEvents_l[i];

	    if( ie.EventType == SE_KEY ) {
            int numKeyEvents = Sys_PollKeyboardInputEvents();
            if( numKeyEvents > 0 ) {
                for( int i = 0; i < numKeyEvents; i++ )	{
					int key;
					bool state;

					if( Sys_ReturnKeyboardInputEvent( i, key, state ) ) {
						if( ( key == ie.Event ) && state ) {
							//TODO
							//TODO escapeEvent = true;
							//TODO
							break;
						}
					}
				}

				Sys_EndKeyboardInputEvents();
            }
	    } else if( ie.EventType == SE_MOUSE ) {
	        int	mouseEvents[MAX_MOUSE_EVENTS][2];
			int numMouseEvents = Sys_PollMouseInputEvents( mouseEvents );
			if( numMouseEvents > 0 ) {
				for( int i = 0; i < numMouseEvents; i++ ) {
					int action = mouseEvents[i][0];
					if( action == ie.Event ) {
						//TODO
						//TODO escapeEvent = true;
						//TODO
						break;
					}
				}
			}
	    } else if( ie.EventType == SE_JOYSTICK ) {
			int numJoystickEvents = Sys_PollJoystickInputEvents( 0 );
			if( numJoystickEvents > 0 ) {
				for( int i = 0; i < numJoystickEvents; i++ ) {
					int action;
					int value;

					if( Sys_ReturnJoystickInputEvent( i, action, value ) ) {
						if( ( action == ie.Event ) && ( value != 0 ) ) {
							//TODO
							//TODO escapeEvent = true;
							//TODO
							break;
						}
					}
				}

				Sys_EndJoystickInputEvents();
			}
		}
	}

	//TODO
	//TODO return escapeEvent;
	//TODO
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
