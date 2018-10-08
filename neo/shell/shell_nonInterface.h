/*
 * shell_nonInterface.h
 *
 *  Created on: 7 d’oct. 2018
 *      Author: biel
 */

#ifndef NEO_SHELL_SHELL_NONINTERFACE_H_
#define NEO_SHELL_SHELL_NONINTERFACE_H_

#include "../shell/shell_background.h"
#include "../framework/Common.h"

namespace BFG {

class blNonInterface {
public:
	blNonInterface();
	virtual ~blNonInterface();
	void			Update();
	bool			IsActive() { return isActive; };

private:
	void			Clear();
	blBackground*	background;
	bool			isActive;
	idStr			operatingName;
};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_NONINTERFACE_H_ */
