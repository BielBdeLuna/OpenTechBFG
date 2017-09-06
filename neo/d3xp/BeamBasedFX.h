/*
 * BeamBasedFX.h
 *
 *  Created on: 24 ag. 2017
 *      Author: biel
 */

#ifndef NEO_D3XP_BEAMBASEDFX_H_
#define NEO_D3XP_BEAMBASEDFX_H_

#include "../d3xp/Entity.h"

namespace BFG {

class blBeam: public idEntity {
	CLASS_PROTOTYPE( blBeam );
public:
	blBeam();

	void				Spawn();

	void				ChangeShaderParm( const int shader_parm, float data_in);
	void				UpdateChanges();

	void				ChangeWidth( float in );
	void				ChangeOrigin( idVec3 in );
	void				ChangeTarget( idVec3 in );
	void				ChangeColour( idVec4 in );
	//void				ChangeSkin( idDeclSkin in );
private:
	bool				infinite;
};

} /* namespace BFG */

#endif /* NEO_D3XP_BEAMBASEDFX_H_ */
