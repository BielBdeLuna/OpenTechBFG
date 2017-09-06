/*
 * EntityEffects.cpp
 *
 *  Created on: 22 ag. 2017
 *      Author: biel
 */

#include "../d3xp/EntityEffects.h"
#include "../d3xp/Game_local.h"

namespace BFG {

blFxManagerBase::blFxManagerBase() {
	fxId = 0;
	StartTime = 0;
	effectActive = false;
}

blFxManagerBase::~blFxManagerBase() {
	effectActive = false;
}

void blFxManagerBase::init( int uniqueId ) {
	SetFxId( uniqueId );
}

void blFxManagerBase::EffectStart( void ) {
	StartTime = gameLocal.time;
	effectActive = true;
}

void blFxManagerBase::Think( void ) {
	if ( effectActive ) {
		//do something
	}
}

void blFxManagerBase::EffectEnd( void ) {
	effectActive = false;
}

/*
 *
 * 		blBeamManager
 * 		One manager to manage them all
 *
 */

blBeamManager::blBeamManager() {
	dictator = NULL;
	ent_ref = NULL;
	beam = NULL;
	beam_width = 0.0f;
	beam_widthOriginal = 0.0f;
	beam_widthFinal = 0.0f;
	beam_length = 0.0f;
	beam_lengthOriginal = 0.0f;
	beam_lengthFinal = 0.0f;
	beam_offset = 0.0f;
	beam_BoneNameOrigin = "";
	beam_BoneNameEnd = "";
	beam_skinName = "";
	beam_skin = NULL;
	beam_colour = vec4_zero;
	beam_colourOriginal = vec4_zero;
	beam_colourFinal = vec4_zero;
	beam_origin = vec3_zero;
	beam_target = vec3_zero;
	beam_dir = vec3_zero;
	beam_reversed = false;
	beam_infinite = false;
	beam_singleEntity = false;
	beam_referencedDir = false;
	beam_staticDir = false;
	beam_colourInfinite = false;
	beam_time = 0.0f;
}

blBeamManager::~blBeamManager() {
	if ( effectActive ) {
		EffectEnd();
	} else {
		if( beam )
		{
			delete beam;
		}
	}
}

bool blBeamManager::BoneVec( idEntity* ent, const char* bone_name, idVec3& out ) {
	const char* boneName = ent->spawnArgs.GetString( bone_name );
	if ( boneName[0] != '\0' ) {
		jointHandle_t bone_handle = ent->GetAnimator()->GetJointHandle( boneName );
		if ( bone_handle == INVALID_JOINT ) {
			gameLocal.Error("blBeamManager: no bone '%s'in entity '%s'!", boneName, ent->GetName() );
		}
		ent->GetAnimator()->CreateFrame( gameLocal.time, false );
		idVec3 bone_offset = ent->GetAnimator()->joints[ bone_handle ].ToVec3();
		out = ent->GetPhysics()->GetOrigin() + bone_offset;
		return true;
	} else {
		return false;
	}
}
void blBeamManager::TracedVec( idVec3 org, idVec3 dir, idVec3& out ) {
	dir.Normalize();
	out = org + ( dir * beam_length );
}

void blBeamManager::init( int uniqueId, idEntity* owner, idEntity* second_entity ) {

	blFxManagerBase::init( uniqueId );

	dictator = owner;
	ent_ref = second_entity;

	beam_singleEntity = ( dictator->GetName() == ent_ref->GetName() );

	if( !dictator->spawnArgs.GetFloat( "beam_width", "0.0", beam_width ) ) {
		gameLocal.Warning( "blBeamManager: No 'beam_width' specified in entity '%s' spawnargs, defaulting to zero.", dictator->GetName() );
	}
	beam_widthOriginal = beam_width;
	const char* beam_skinName = dictator->spawnArgs.GetString( "beam_skin" );
	if ( beam_skinName[0] == '\0' ) {
		gameLocal.Error( "blBeamManager: No 'beam_skin' specified in entity '%s' spawnargs!", dictator->GetName() );

	} else {

		beam_skin = declManager->FindSkin( beam_skinName );
		if( beam_skin == NULL ) {
			gameLocal.Error( "blBeamManager: 'beam_skin' specified in entity '%s' doesn't exist!", dictator->GetName() );
		}
	}
	dictator->spawnArgs.GetVec4( "beam_color", "0.0 0.0 0.0 0.0", beam_colour );

	/*
	const char* boneName = dictator->spawnArgs.GetString( "beam_joint" );
	if ( boneName[0] != '\0' ) {
		jointHandle_t bone_handle = dictator->GetAnimator()->GetJointHandle( "boneName" );
		GetGlobalJointTransform( true, barrelJointView, origin, axis );
	}
	beam_origin = dictator->animator.GetJointHandle( "barrel" );
	*/

	dictator->spawnArgs.GetBool( "beam_referencedDir", false, beam_referencedDir );
	dictator->spawnArgs.GetBool( "beam_staticDir", false, beam_staticDir );



	if ( beam_referencedDir && beam_staticDir ) {
		gameLocal.Error("blBeamManager: impossible beam direction inputs in entity '%s'!", dictator->GetName() );
	}

	if ( beam_staticDir ) {
		if ( !dictator->spawnArgs.GetVector( "beam_dir", "0.0 0.0 0.0", beam_dir ) ) {
			gameLocal.Error("blBeamManager: no beam direction inputs in entity '%s'!", dictator->GetName() );
		}
	}

	dictator->spawnArgs.GetFloat( "beam_length", "0.0", beam_length );

	if ( beam_length != 0.0f ) {

		//certam longitudinem limitatur sed libero
		if ( beam_singleEntity ) {
			if ( beam_referencedDir ) {
				gameLocal.Error("blBeamManager: entity '%s' itself can't be used as a origin and reference for the beam direction!", dictator->GetName() );
			}
			if( !BoneVec( dictator, "beam_joint", beam_origin ) ) {
				beam_origin = dictator->GetPhysics()->GetOrigin();
			}
		} else {
			if( !BoneVec( dictator, "beam_joint", beam_origin ) ) {
				beam_origin = dictator->GetPhysics()->GetOrigin();
			}
			if ( beam_staticDir ) {
				TracedVec( beam_origin, beam_dir, beam_target );
			} else {
				idVec3 target_pos, delta_dir;
				if( !BoneVec( ent_ref, "beam_joint", target_pos ) ) {
					target_pos = ent_ref->GetPhysics()->GetOrigin();
				}

					//certam longitudinem limitatur sed libero
					delta_dir = target_pos - beam_origin;
			}
		}
	} else {
		//ad finitam infinitas
		if ( beam_referencedDir || beam_staticDir ) {
			gameLocal.Error("blBeamManager: impossible beam length and directionality input in entity '%s'!", dictator->GetName() );
		}

		//proportioni sese adstrinxit sui limit
		if ( beam_singleEntity ) {
			beam_origin = dictator->GetPhysics()->GetOrigin();
			if( !BoneVec( ent_ref, "beam_joint", beam_target ) ) {
				gameLocal.Error("blBeamManager: impossible beam origin/destine inputs in the single entity '%s'!", dictator->GetName() );
			}
		} else {
			if( !BoneVec( dictator, "beam_joint", beam_origin ) ) {
				beam_origin = dictator->GetPhysics()->GetOrigin();
			}
			if( !BoneVec( ent_ref, "beam_joint", beam_target ) ) {
				beam_target = ent_ref->GetPhysics()->GetOrigin();
			}
		}
	}

	dictator->spawnArgs.GetBool( "beam_reversed", false, beam_reversed ); //whether we start from end to start or viceversa
	dictator->spawnArgs.GetFloat( "beam_time", "0.0", beam_time );
	if ( beam_time == 0.0 ) {
		beam_infinite = true;
	}

	//SetOrigin( const idVec3& org );
}

void blBeamManager::BeamUpdate() {
	if ( !beam ) {
		beam->ChangeWidth( beam_width );
		beam->ChangeOrigin( beam_origin );
		beam->ChangeTarget( beam_target );
		beam->ChangeColour( beam_colour );
		//beam->ChangeSkin( beam_skin );
		beam->UpdateChanges();
	} else {
		BeamCreate();
	}
}

void blBeamManager::ReverseEnds() {
	idVec3 beam_original = beam_origin;
	beam_origin = beam_target;
	beam_target = beam_original;
}


void blBeamManager::BeamCreate() {
	if ( !beam ) {
		if ( beam_reversed ) {
			ReverseEnds();
		}

		idDict args;
		args.SetVector( "origin", beam_origin );
		args.SetVector( "target_origin", beam_target );
		args.SetBool( "start_off", true );
		args.SetFloat( "width", beam_width );
		args.Set( "skin", beam_skinName );
		args.SetVec4( "_color", beam_colour );
		args.SetBool( "infinite", beam_infinite );
		beam = ( blBeam* )gameLocal.SpawnEntityType( blBeam::Type, &args );
		//beam->SetShaderParm( 6, 1.0f );
	} else {
		BeamUpdate();
	}
}
void blBeamManager::EffectStart() {
	StartTime = gameLocal.time;
	if ( effectActive ) {
		BeamUpdate();
	} else {
		effectActive = true;
		BeamCreate();
	}
}

void blBeamManager::EffectEnd() {
	//try to end gracefully
	effectActive = false;
	if( beam )
	{
		delete beam; // yeah! very gracefully indeed! :P
	}
}

void blBeamManager::Think() {
	if ( !beam_infinite && effectActive ) {
		if ( beam_width > 0.0f ) {
			float customTime, orgWidth;
			customTime = gameLocal.time;

			float normalized_elapsed_time = ( 1 / ( StartTime + beam_time ) ) * customTime;
			beam_width = ( 1 / beam_widthOriginal ) * normalized_elapsed_time;

			beam_origin = dictator->GetPhysics()->GetOrigin();

			beam_target = beam_end;

			if ( beam_reversed ) {
				ReverseEnds();
			}

			//check colour

			BeamUpdate();
		} else {
			EffectEnd();
		}
	}
}

} /* namespace BFG */
