/*
 * shell_local_inputEvents.cpp
 *
 *  Created on: 9 de Gener 2020
 *      Author: Biel Bestué de Luna
 */

#include "../shell/shell_local.h"
#include "../framework/CmdSystem.h"

namespace BFG {

/*******************************************************************************

                                  INPUT EVENTS

*******************************************************************************/

/*
=================
blShellLocal::ShellInputEvents_SearchInputEvent
compares the givenIEreference with every blInputEvent reference in the ShellInputEvents_l list
if it finds it it returns it's index else -1
=================
*/
int blShellLocal::ShellInputEvents_SearchInputEvent( InputEventRef_s givenIEreference ) {
    for( int i = 0; i < ShellInputEvents_l.Num(); i++ ) {
        if( ShellInputEvents_l[i].GetReference() == givenIEreference ) {
            return i;
        }
    }
    return -1;
}

/*
=================
blShellLocal::ShellCommand_AddRelevantIndex
given a indicatedInputEvent, add a newIndex to the RelevantInputEvents_l list
=================
*/
void blShellLocal::ShellCommand_AddRelevantIndex( InputEvent indicatedInputEvent, int newIndex ) {
    int i = ShellCommand_SearchRelevantInputEvent( InputEvent indicatedInputEvent );

    if( i == -1 ) {
        common->Error( "blShellLocal::ShellCommand_AddRelevantIndex : RelevantInputEvents_l list doesn't have the indicatedInputEvent!\n" );
        return; // irrelevant if we error it out
    }

    // add it if we don't have it already listed
    RelevantInputEvents_l[i].indexes.addUnique( newIndex );

}

/*
=================
blShellLocal::ShellCommand_AddAllRelevantIndexes
given a indicatedInputEvent, search all the relevant indexes in the ShellCommands_l list
=================
*/
void blShellLocal::ShellCommand_AddAllRelevantIndexes( InputEvent indicatedInputEvent ) {
    for( index = 0; index < ShellCommands_l.Num(); index++ ) {
        int n = ShellCommands_l[index].GetAssociatedInputEventsNum();

        //in case there are associated InputEvents to that specific ShellEvent
        if( n > 0 ) {
            if( n == 1 ) {
                // if it is just one associated InputEvent
                if( ShellCommands_l[index].GetAssociatedInputEventAt( 0 ) == indicatedInputEvent ) {
                    ShellCommand_AddRelevantIndex( indicatedInputEvent, index );
                }
            } else {
                // if it is more than just one
                for( int i = 0; i < n; i++ ) {
                    if( ShellCommands_l[index].GetAssociatedInputEventAt( i ) == indicatedInputEvent ) {
                        ShellCommand_AddRelevantIndex( indicatedInputEvent, index );
                    }
                }
            }
        }
        //in another case the ShellCommand could have no associated InputEvents in that case we don't do a thing
    }
}


/*
=================
blShellLocal::ShellInputEvents_AddInputEvent
adds the newInputEvent to the RelevantInputEvents_l list if it isn't there already and returns it's index
=================
*/
void blShellLocal::ShellInputEvents_AddInputEvent( blInputEvent newInputEvent ) {
    int index = ShellInputEvents_SearchInputEvent( newInputEvent.GetReference() );

    if( index  == -1 ) {
        index = ShellInputEvents_l.AddUnique( newInputEvent );
    }

    return index;
}

/*
=================
blShellLocal::ShellCommand_TestInputEventIrelevancy
test if any of the ShellCommands in ShellCommands_l has this indicatedInputEvent associated,
we can skip one of the indexes in the list so we don't make an ineficient check
=================
*/
bool blShellLocal::ShellCommand_TestInputEventIrelevancy( InputEvent indicatedInputEvent, int indexSkip == -1 ) {
    for( int index = 0; index < ShellCommands_l.Num(); index++ ) {
        if( index == indexSkip ) {
            continue;
        }

        if( ShellCommands_l[index].SearchAssociatedInputEvents( indicatedInputEvent ) != -1 ) {
            return true;
        }
    }

    return false
}

/*
=================
blShellLocal::ShellInputEvents_PurgeInputEvent
if a GivenInputEvent from the ShellInputEvents_l has become unecessary purges it
with proper care for the pointers in every ShellCommands_l other than the one skipped by the indexSkip
returns whether or not it erased it or not
=================
*/
void blShellLocal::ShellInputEvents_PurgeInputEvent( blInputEvent* GivenInputEvent, int indexSkip ) {
    bool erase = true;

    //test if those are in use for other shellCommands
    for( int i = 0; i < ShellCommands_l.Num(); i++ ) {
        if( i == indexSkip ) {
            continue;
        }

        for( int j = 0; j < ShellCommands_l[i].GetAssociatedInputEventsNum(); j++ ) {
            if( ShellCommands_l[i].GetAssociatedInputEventAt( j ) == GivenInputEvent ) {
                erase = false;
            }
        }
    }

    if( erase ) {
        // not in use therefore ilegal, erase them with care
        ShellInputEvents_SubInputEvent( GivenInputEvent->GetReference() );
    }
}

/*
=================
blShellLocal::ShellInputEvents_SubInputEvent
substracts the reference to the unwantedInputEvent from the list, all with
proper care to the given pointers in the shellCommands
=================
*/
void blShellLocal::ShellInputEvents_SubInputEvent( InputEventRef_s unwantedIEreference ) {
    int index = ShellInputEvents_SearchInputEvent( unwantedIEreference );

    if( index == -1 ) {
        //in that unlikely case...
        common->Error("blShellLocal::ShellInputEvents_SubInputEvent : The index '%i' is out of bounds from the ShellInputEvents_l list!\n", index );
    }

    // let's first find what elements in the ShellCommands_l list point to an
    // element in the list either being in the index or above, as this list is
    // going to change all it's indices from that point above

    //let's establish a plain old data struct and prepare a list for it
    struct ElementsOfchange {
        int ShCommIndex; //index of the ShellCommand in the ShellCommands_l list
        int OldIndex; //the old index of the InputEvent in the ShellInputEvents_l before we delete the element in Index
        int NewIndex; // either the new index or a '-1' mark to delete that pointer in the associatedInputEventsPtr_l list
    }

    idList<ElementsOfchange> changes;

    for( int i = 0; i < ShellCommands_l.Num(); i++ ) {
        for( int j = 0; j < ShellCommands_l[i].GetAssociatedInputEventsNum(); j++ ) {
            int k = ShellInputEvents_SearchInputEvent( ShellCommands_l[i].GetAssociatedInputEventAt(j)->GetReference() );
            if( k >= index ) {
                ElementsOfchange change;

                if( k == index ) {
                    change.ShCommIndex = i;
                    change.OldIndex = k;
                    change.NewIndex = -1;
                } else {
                    // k > index
                    change.ShCommIndex = i;
                    change.OldIndex = k;
                    change.NewIndex = k-1;
                }

                changes.AddUnique( change );
            }
        }
    }

    //let's actually delete the indexed element in the list, this shortens the list from the index above
    delete ShellInputEvents_l[index];
    ShellInputEvents_l[index] = NULL;
    ShellInputEvents_l.RemoveIndex( index );

    //so now let's repoint all other pointers that referenced to the other elements of the list above index
    //let's first work on the elements that are not to be removed so this list doesn't ive bad indices
    for( int j = 0; j > changes.Num(); j++ ) {
        if( changes[j].NewIndex == -1 ){
            //let's skip those now so this list doesn't turn out giving bad indices!
            continue;
        } else {
            //repoints every legal pointer element in associatedInputEventsPtr_l list in the correct ShellCommand pointing it to the correct element in the ShellInputEvents_l list
            ShellCommands_l[changes[j].ShCommIndex].GetAssociatedInputEventAt( changes[j].OldIndex ) = &( ShellInputEvents_l[changes[j].NewIndex] )
        }
    }
    //now let's delete all the references that needed to be deleted
    for( int j = 0; j > changes.Num(); j++ ) {
        if( changes[j].NewIndex == -1 ){
            //delete the correct pointer element in the associatedInputEventsPtr_l list in the correct ShellCommand
            ShellCommands_l[changes[j].ShCommIndex].DelAssociatedInputEventAt( changes[j].OldIndex );
        }
    }
}

/*
=================
blShellLocal::ShellCommand_RebuildRelevantInputEventList
rebuilds the RelevantInputEvents_l list from scratch
=================
*/
void blShellLocal::ShellCommand_RebuildRelevantInputEventList() {
    for( index = 0; index < ShellCommands_l.Num(); index++ ) {
        int n = ShellCommands_l[index].GetAssociatedInputEventsNum();

        if( n > 1 ) {
            for( int i = 0; i < n; i++ ) {
                ShellCommand_AddRelevantInputEvent( ShellCommands_l[index].GetAssociatedInputEventAt( i ) );
            }
        } else if( n == 1 ) {
            ShellCommand_AddRelevantInputEvent( ShellCommands_l[index].GetAssociatedInputEventAt( 0 ) );
        }
        //in another case the ShellCommand could have no associated InputEvents in that case we don't do a thing
    }
}

/*
=================
blShellLocal::ShellCommand_SetState_AllRelevantInputEventIndexes
switches on all ShellCommands in the ShellCommands_l list referenced
in the ivenindex of the RelevantInputEvents_l list
=================
*/
void blShellLocal::ShellCommand_SetState_AllRelevantInputEventIndexes( int index, int value ) {

    if( ( index > 0 ) && ( index < RelevantInputEvents_l.Num() ) ) {
        InputEventsAndIndexes element = RelevantInputEvents_l[index];
        int n = element.indexes.Num();
        if( n > 0 ) {
            for( int i = 0; i < n; i++ ) {
                ShellCommand_SetState_GivenIndex( element.indexes[i], value );
            }
        } else {
            common->Warning( "blShellLocal::ShellCommand_SetState_AllRelevantInputEventIndexes : there are no ShellCommands associated to that InputEvent yet!\n" );
        }
    } else {
        common->Error( "blShellLocal::ShellCommand_SetState_AllRelevantInputEventIndexes : RelevantInputEvents_l list doesn't have a '%i' index!\n", index );
    }
}

/*
=================
blShellLocal::ShellInputEvents_Synch
find the current value for every InputEvent in the ShellInputEvents_l list
=================
*/
void blShellLocal::ShellInputEvents_Synch() {
    for( int i = 0; i < ShellInputEvents_l.Num(); i++ ) {
        ShellInputEvents_l[i].FindNewValue();
    }
}

} /* namespace BFG */
