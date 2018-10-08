/*
 * shell_background.h
 *
 *  Created on: 10 de set. 2018
 *      Author: biel
 */

#ifndef NEO_SHELL_SHELL_BACKGROUND_H_
#define NEO_SHELL_SHELL_BACKGROUND_H_

#include "../idlib/math/Vector.h"
#include "../renderer/Material.h"
#include "../idlib/Str.h"

namespace BFG {

enum backgrounds_t
{
	BACKGROUND_COLOUR = 0,
	BACKGROUND_IMAGE,
	BACKGROUND_MATERIAL,
	NUM_BACKGROUNDS
};

class blBackground {
public:
	blBackground();
	virtual ~blBackground();
	void		InitNone();
	void 		InitColour( idVec4 Colour );
	void		InitMaterial( idStr material_name, idVec4 Tint );
	void		Update();
	bool		IsActive() { return active; };
private:
	void 		Clear();
	void		Clean_up();
	const idMaterial*	material;
	idVec4		colour;
	bool		active;
};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_BACKGROUND_H_ */
