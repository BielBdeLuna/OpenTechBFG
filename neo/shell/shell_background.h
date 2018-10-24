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

class blBackground {
public:
	blBackground( idStr material_name, idVec4 tint );
	virtual ~blBackground();
	void		Update();
private:
	void 		Clear();
	const idMaterial*	material;
	idVec4		colour;
};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_BACKGROUND_H_ */
