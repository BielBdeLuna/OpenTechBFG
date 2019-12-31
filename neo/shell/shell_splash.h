/*
 * shell_splash.h
 *
 *  Created on: 26 de març 2019
 *      Author: Biel Bestué de Luna
 */

#ifndef NEO_SHELL_SHELL_SPLASH_H_
#define NEO_SHELL_SHELL_SPLASH_H_

#include <shell/shell_renderable.h>
#include "../framework/Common.h"
#include "../renderer/Material.h"

namespace BFG {

struct 	splashData_t {
	static idMaterial*	material;
	float				aspectRatio;
	idVec4				tintColour;
	idVec4				backgroundColour;
};

class blShellSplash: public blShellRenderable {
public:
					blShellSplash();
	virtual 		~blShellSplash();
	virtual void 	Init( static idMaterial* mat, float ar, idVec4 col, idVec4 back_c  ); 	// inits it with data
	virtual void 	Init(){};
	virtual void 	Update( static idMaterial* mat, float ar, idVec4 col, idVec4 back_c  ); 	// allows to change it's data defining behaviour
	virtual void	Update(){};
	virtual void 	PrepareForRendering();	// prepares for rendering it
	virtual void 	Start(); 			// starts the action within
	virtual void 	TogglePause(); 		// pauses the action within
	virtual void 	ToggleHide(); 		// hide it without actually pausing it
	virtual void    Stop();				// stops the action within
	virtual void 	ClaimFocus() {};
private:
	virtual void 	Clear();
	splashData_t 	data;
	bool			isVideo;
	virtual bool 	isPaused;
	virtual bool	isHidden;
};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_SPLASH_H_ */
