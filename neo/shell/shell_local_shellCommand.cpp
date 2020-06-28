/*
 * shell_local_shellCommand.cpp
 *
 *  Created on: 9 de Gener 2020
 *      Author: Biel Bestué de Luna
 */

#include "../shell/shell_local.h"
#include "../framework/CmdSystem.h"

namespace BFG {

/*******************************************************************************

                                 SHELL COMMANDS

*******************************************************************************/

/*
=================
blShellLocal::ShellCommand_SearchShellCommand
search the ShellCommand by that name in the ShellCommands_l list or return -1
=================
*/
int blShellLocal::ShellCommand_SearchShellCommand( idStr name ) {
    for( int i = 0; i < ShellCommands_l.Num(); i++ ) {
        if( ShellCommands_l[i].GetName().Icmp( name ) ) {
            return i;
        }
    }
    //common->Warning( "blShellLocal::ShellCommand_SearchShellCommand : couldn't find ShellCommand '%s' in the list!\n", name.c_str() );
    return -1;
}

/*
=================
blShellLocal::ShellCommand_AddShellCommand
adds a new ShellCommand to the list and returns it's new index.
=================
*/
int blShellLocal::ShellCommand_AddShellCommand( idStr name ) {
    blShellCommand sh = new blShellCommand( name );
    return ShellCommands_l.Append( sh );

}

/*
=================
blShellLocal::ShellCommand_RemoveShellCommand
removes a ShellCommand from the ShellCommands_l list if it exists.
=================
*/
void blShellLocal::ShellCommand_RemoveShellCommand( idStr name ) {
    int index ShellCommand_SearchShellCommand( name );

    if( index == -1 ) {
        common->Warning( "blShellLocal::ShellCommand_SearchShellCommand : couldn't find ShellCommand '%s' in the list!\n", name.c_str() );
        return;
    }

    // purge every associcated InputEvent in the ShellCommand
    for( ShellCommands_l[index].GetAssociatedInputEventsNum() > 0 ) {
        //purge the pointed associated InputEvent if it's present on other ShellCommands
        ShellInputEvents_PurgeInputEvent( ShellCommands_l[index].GetAssociatedInputEventAt( i ), i )
        //delete the InputEvent pointer in this ShellCommand so it doesn't bother us any longer
        ShellCommands_l[index].DelAssociatedInputEventAt( i );
    }

    //eventually delete the whole ShellCommand once it's deletion can't break other pointers
    delete ShellCommands_l[index];
    ShellCommands_l[index] = NULL;
    ShellCommands_l.RemoveIndex( index );
}

/*
=================
blShellLocal::ShellCommand_GetState
gets the state of that CommandState if it exists in the list at all
=================
*/
int blShellLocal::ShellCommand_GetState( idStr name ) {

    int index = ShellCommand_SearchShellCommand( name );

    if( index == -1 ) {
        return -1; // let the "event" function handle the error announement
        //common->Error( "blShellLocal::ShellCommand_GetState : ShellCommand '%s' doesn't exist in the list!\n", name.c_str() );
    }

    return ShellCommands_l[index].GetState();
}

/*
=================
blShellLocal::ShellCommand_GetValue
gets the value of that CommandState if it exists in the list at all
=================
*/
int blShellLocal::ShellCommand_GetValue( idStr name ) {

    int index = ShellCommand_SearchShellCommand( name );

    if( index == -1 ) {
        // unfortunately there is no "worng" value to pass to the "event" function
        // so when the shellCommand isn't found the error will not be well beheaved
        common->Error( "blShellLocal::ShellCommand_GetValue : ShellCommand '%s' doesn't exist in the list!\n", name.c_str() );
    }

    return ShellCommands_l[index].GetState();
}

/*
=================
blShellLocal::ShellCommand_AssociateInputEvent
=================
*/
void blShellLocal::ShellCommand_AssociateInputEvent( idStr name, InputEventRef_s newIEreference ) {
    int index = ShellCommand_SearchShellCommand( name );

    if( index == -1 ) {
        // at this stage there isn't any ShellCommand with that name add a new one
        index = ShellCommand_AddShellCommand( name );
    }

    blInputEvent ie = new blInputEvent( newIEreference );
    int i = ShellInputEvents_AddInputEvent( ie );

    ShellCommands_l[index].AssociateInputEvent( ShellInputEvents_l[i] );
}

/*
=================
blShellLocal::ShellCommand_DisassociateInputEvent
=================
*/
void blShellLocal::ShellCommand_DisassociateInputEvent( idStr name, InputEventRef_s unwantedIEreference ) {
    int index = ShellCommand_SearchShellCommand( name );

    if( index == -1 ) {
        common->Warning( "blShellLocal::ShellCommand_DisassociateInputEvent : couldn't find ShellCommand '%s' in the list!\n", name.c_str() );
        return;
    }

    int i = ShellCommands_l[index].SearchAssociatedInputEvents( unwantedIEreference );
    ShellInpuEvents_PurgeSingleInputEvent( ShellCommands_l[index].GetAssociatedInputEventAt( i ), index );

    ShellCommands_l[index].DisassociateInputEvent( unwantedInputEvent );
}

/*
=================
blShellLocal::ShellCommand_Sync
=================
*/
void blShellLocal::ShellCommand_Sync() {
    for( int i = 0; i < ShellCommands_l.Num(); i++ ) {
        ShellCommands_l[i].GatherStateAndValue();
    }

}

} /* namespace BFG */
