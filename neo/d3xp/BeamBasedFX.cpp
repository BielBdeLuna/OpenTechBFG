/*
 * BeamBasedFX.cpp
 *
 *  Created on: 24 ag. 2017
 *      Author: biel
 */

#include "BeamBasedFX.h"

namespace BFG {

blBeam::blBeam() {
	infinite = false;
}

blBeam::~blBeam() {
	// TODO Auto-generated destructor stub
}

void blBeam::Spawn() {
	float width;
	idVec3 target_origin;


	if( spawnArgs.GetFloat( "width", "0", width ) )
	{
		renderEntity.shaderParms[ SHADERPARM_BEAM_WIDTH ] = width;
	}

	target_origin = spawnArgs.GetVector( "target_origin", "0.0 0.0 0.0" );
	if( target_origin == vec3_zero ) {
        const char* targ_name = spawnArgs.GetString( "target" );
		if(  targ_name[0] != '\0' ) {
			idEntity targ_ent = gameLocal.FindEntity( targ_name );

			if ( targ_ent ) {
				target_origin = targ_ent.GetPhysics()->GetOrigin();
			}
		}
	}
	renderEntity.shaderParms[ SHADERPARM_BEAM_END_X ] = target_origin[0];
	renderEntity.shaderParms[ SHADERPARM_BEAM_END_Y ] = target_origin[1];
	renderEntity.shaderParms[ SHADERPARM_BEAM_END_Z ] = target_origin[2];

	spawnArgs.GetBool( "infinite", false, infinite );

	SetModel( "_BEAM" );
	Hide();
	PostEventMS( &EV_PostSpawn, 0 );
}

void blBeam::ChangeShaderParm( const int shader_parm, float data_in) {
	renderEntity.shaderParms[ shader_parm ] = data_in;
}

void blBeam::UpdateChanges() {
	UpdateVisuals();
}

void blBeam::ChangeWidth( float in ) {
	ChangeShaderParm( SHADERPARM_BEAM_WIDTH, in);
}

void blBeam::ChangeOrigin( idVec3 in ) {

	//in = GetLocalCoordinates( in ); //we're never binded to another entity
	SetOrigin( in );
}

void blBeam::ChangeTarget( idVec3 in ) {
	ChangeShaderParm( SHADERPARM_BEAM_END_X, in[0]);
	ChangeShaderParm( SHADERPARM_BEAM_END_Y, in[1]);
	ChangeShaderParm( SHADERPARM_BEAM_END_Z, in[2]);
}

void blBeam::ChangeColour( idVec4 in ) {
	ChangeShaderParm( SHADERPARM_RED, in[0]);
	ChangeShaderParm( SHADERPARM_GREEN, in[1]);
	ChangeShaderParm( SHADERPARM_BLUE, in[2]);
	ChangeShaderParm( SHADERPARM_ALPHA, in[3]);
}


} /* namespace BFG */
