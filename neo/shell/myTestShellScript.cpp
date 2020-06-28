/*
 * myTestShellScript.cpp
 *
 *  Created on: 18 de gen. 2020
 *      Author: Biel Bestué de Luna
 */

namespace myTestShellScript {

bool demoLoaded;
int  demoLoop_t;

idSoundWorld    GameSpaceSoundWorld;
idSoundWorld    MainMenuSoundWorld;

void scriptedFunctionLoadNextDemo() {
    demoLoaded = false;
    #Event_LoadDemo( #NextDemoData );
    demoLoaded = true;
}

void scriptedFunctionDemoLoop() {
    int LoadingNext_t;
    while( Event_Get_SessionState() == #idSession::IDLE ) {
        #demoData = #NextDemoData;
        #Event_ClearDemoData( #NextDemoData );
        #LoadingNext_t = Event_StartThread( scriptedFunctionLoadNextDemo() );
        #Event_SetUpSoundWorld( GameSpaceSoundWorld );
        #Event_DisplayDemo( #demoData, GameSpaceSoundWorld );
        #Event_ClearDemoData( #demoData );
        while( !demoLoaded ) {
            //#Event_SetUpStretchedBackgroundContent( "", #tintColour, #backColour, #stretchAR );
            #Event_SetUpStretchedImageContent( #"defaultMenuLoading", ar169 );
            #Event_Wait( #frame );
        }
    }
    #Event_KillThreadNum( LoadingNext_t );
}

void scriptedFunctionStart() {
    demoLoaded = false;
    float time = 0;
    float ar169 = ( 16.0f / 9.0f );
    if( #Event_TestCvarBool( "com_skipIntroVideos" ) ) {
        #Event_DisableConsole();
        //#Event_SetUpStretchedBackgroundContent( "", #tintColour, #backColour, #stretchAR );
        time = #Event_SetUpStretchedVideoContent( "video\\loadvideo.bik" );
        #Event_Wait( time );
        #Event_EnableConsole();
    }
    Event_SetShellCommand( "SkipContent" );
    Event_SetInputEventToShellCommand( "SkipContent", SE_KEY, K_ESCAPE );
    Event_SetInputEventToShellCommand( "SkipContent", SE_JOYSTICK, #K_JOY13 );
    if( #Event_TestCvarBool( "com_skipIntroVideos" ) ) {
        //#Event_SetUpStretchedBackgroundContent( "", #tintColour, #backColour, #stretchAR );
        time = #Event_SetUpStretchedVideoContent( "video\\engine.bik" );
        #Event_Wait( time, "SkipContent" );
    }
    //#Event_SetUpStretchedBackgroundContent( "", #tintColour, #backColour, #stretchAR );
    #Event_SetUpStretchedImageContent( #"legalImage", ar169 );
    #Event_Wait( time "SkipContent" );
    Event_SubShellCommand( "SkipContent" );
    while( !demoLoaded ) {
        //#Event_SetUpStretchedBackgroundContent( "", #tintColour, #backColour, #stretchAR );
        #Event_SetUpStretchedImageContent( #"defaultMenuLoading", ar169 );
        #Event_Wait( #frame );
    }
    demoLoop_t = #Event_StartThread( scriptedFunctionDemoLoop() );
    #Event_SetUpSoundWorld( MainMenuSoundWorld );
    Event_SetShellCommand( "StartPressed" );
    #Event_SetUpStretchedGuiContent( "guis\\PressStart.gui", ar169 );
    #Event_SetGuiCallToShellCommand( "StartPressed", "Gui_StartPressed" );
    #Event_SetMouseFocusOnGui();
    while( true ) {
        if( #Event_OnStart_Pressing( "StartPressed" ) ) {
            break;
        }
        #Event_Wait( #frame );
    }
    Event_SubShellCommand( "StartPressed" );
    #Event_StartFunction( scriptedFunctionOpenMainMenu() );
}

void scriptedFunctionToggleMainMenu() {
    if( #Event_Console_isOpen() ) {
        #Event_CloseConsole();
        return;
    } else {
        if( Event_Get_SessionState() == #idSession::PRESS_START ) {
            return;
        }
    }

    if( Event_MainMenu_isOpen() ) {
        #Event_MuteSoundWorld( GameSpaceSoundWorld );
        #Event_StartFunction( scriptedFunctionCloseMainMenu() );
    } else {
        #Event_HearSoundWorld( GameSpaceSoundWorld );
        #Event_StartFunction( scriptedFunctionOpenMainMenu() );
    }
}

void scriptedFunctionOpenMenu() {
    if( Event_Get_SessionState() == #idSession::PRESS_START ) {
        Event_SetShellCommand( "ToggleMenu" );
        Event_SetInputEventToShellCommand( "ToggleMenu", SE_KEY, K_ESCAPE );
        Event_SetInputEventToShellCommand( "ToggleMenu", SE_JOYSTICK, #K_JOY13 );
        #Event_AssociateFunctionToShellCommand( "ToggleMenu", scriptedFunctionToggleMainMenu() );
        #Event_RegisterUserName( #"thisNewUser" ); // register new user
        Event_Set_SessionState( #idSession::IDLE ); // we're no longer in "press start" session state
        #Event_SetUpStretchedGuiContent( #mainMenuGui, "guis\\MainMenu.gui", ar169 );
    }
    #Event_RestartGui( #mainMenuGui );
    #Event_HearSoundWorld( MainMenuSoundWorld );
}

void scriptedFunctionCloseMainMenu() {
    #Event_StopGui( #mainMenuGui );
    #Event_MuteSoundWorld( MainMenuSoundWorld );
}

void ScriptedFunctionFinishMenuDemos() {
    #Event_StopSoundWorld( GameSpaceSoundWorld );
    #Event_KillThreadNum( demoLoop_t );
}

} /* namespace myTestShellScript */
