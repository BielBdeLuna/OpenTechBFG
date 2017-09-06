/*
 * EntityEffects.h
 *
 *  Created on: 22 ag. 2017
 *      Author: biel
 */

#ifndef NEO_D3XP_ENTITYEFFECTS_H_
#define NEO_D3XP_ENTITYEFFECTS_H_

#include "../d3xp/Entity.h"

namespace BFG {

class idRestoreGame;
class idSaveGame;
class idDict;

/*
// TypeInfo flags
enum {
	EE_TYPE_BEAM_SIMPLE				= 1, 	// simple beam with a start, an end point and width, with think controlling all
	EE_TYPE_BEAM_CORD_SIMPLE		= 2,	// a cord made of beams with physics movement
	EE_TYPE_PARTICLE_SIMPLE 		= 32,	// a particle playing from a specified origin, think controls it all
	EE_TYPE_PARTICLE_BONE			= 64,	// the same above but instead of a vector a bone
	EE_TYPE_PARTICLE_BONES			= 128,	// particles from all bones from a animated entity
	EE_TYPE_PARTICLE_TRACE			= 256,	// particles in a line traced from a specified origin to a specified end
	EE_TYPE_JET_SIMPLE				= 2048, // beam that starts in a direction but then gravity or flow affects it's direction
	EE_TYPE_JET_ELECTRICITY			= 4096,	// multiple jets with other beams connected forming a electrical arc
	EE_TYPE_JET_SMOKE				= 8192 	// a beam that follows the barrel of a gun simulating smoke
};
*/

static const int MAX_ENT_EFFECTS = 16;

class blFxManagerBase {
public:
	CLASS_PROTOTYPE( blFxManagerBase );

	blFxManagerBase();
	virtual ~blFxManagerBase();

	void				init( int uniqueId );

	void				SetFxId( int numId ) { fxManagerId = numId; }
	int					GetFxId() { return fxManagerId; }

	void				Save( idSaveGame *savefile ) const;
	void				Restore( idRestoreGame *savefile );

	void 				EffectStart( void );
	void				EffectEnd( void );

	void				Think();

protected:
	int				fxManagerId;
	int				StartTime;
	bool			effectActive;

	//idList<EntityEffect, MAX_ENT_EFFECTS>	list_effects;
};

class blBeamManager : public blFxManagerBase {
public:
	CLASS_PROTOTYPE( blBeamManager );

	blBeamManager();
	virtual ~blBeamManager();

	void 				init( int uniqueId, idEntity* owner, idEntity* second_entity );

	void				Save( idSaveGame *savefile ) const;
	void				Restore( idRestoreGame *savefile );

	bool				BoneVec( idEntity* ent, const char* bone_name, idVec3& out );
	void				TracedVec( idVec3 org, idVec3 dir, idVec3& out );

	void				ReverseEnds( void );
	void				BeamCreate( void );
	void				BeamUpdate( void );

	void 				EffectStart( void );
	void				EffectEnd( void );

	void				Think();



protected:
	idEntity*		dictator;
	idEntity* 		ent_ref;
	blBeam* 		beam;
	float			beam_width;
	float			beam_widthOriginal;
	float			beam_widthFinal;
	float			beam_length;
	float			beam_lengthOriginal;
	float			beam_lengthFinal;
	float			beam_offset;
	const char*		beam_BoneNameOrigin;
	const char*		beam_BoneNameEnd;
	const char*		beam_skinName;
	idDeclSkin		beam_skin;
	idVec4			beam_colour;
	idVec4			beam_colourOriginal;
	idVec4			beam_colourFinal;
	idVec3			beam_origin;
	idVec3			beam_target;
	idVec3			beam_dir;
	bool			beam_reversed;
	bool			beam_infinite;
	bool			beam_colourInfinite;
	bool			beam_singleEntity;
	bool			beam_referencedDir;
	bool			beam_staticDir
	float			beam_time;


};

} /* namespace BFG */

#endif /* NEO_D3XP_ENTITYEFFECTS_H_ */
