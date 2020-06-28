/*
 * Shell_renderable.h
 *
 *  Created on: 26 de març 2019
 *      Author: Biel Bestué de Luna
 */

#ifndef NEO_SHELL_SHELL_RENDERABLE_H_
#define NEO_SHELL_SHELL_RENDERABLE_H_

#include "../shell/shell.h"

namespace BFG {

class blShellRenderable {
public:
	//virtual 		blShellRenderable() = 0;
	virtual			~blShellRenderable() = 0;
	virtual void 	Init() = 0; 	// inits it with data
	virtual void 	Update() = 0; 	// allows to change it's data defining behaviour
	virtual void 	PrepareForRendering() = 0; 	// prepares it for rendering
	virtual void 	Start() = 0; 	// starts tha action within
	virtual void    Stop() = 0;		// stops the action within, needs to start again
	virtual void 	TogglePause() = 0;	// pauses / unpauses the action within
	virtual void 	ToggleHide() = 0; 	// hide it without actually pausing it
	// menu stuff
	virtual void 	ClaimFocus() = 0;	// recovers the focus of the importance

private:
	virtual void 	Clear() = 0;
	virtual bool 	isPaused = 0;
	virtual bool	isHidden = 0;
	// menu stuff
	virtual bool	hasFocus = 0;
};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_RENDERABLE_H_ */
