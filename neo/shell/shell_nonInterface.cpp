/*
 * shell_nonInterface.cpp
 *
 *  Created on: 7 d’oct. 2018
 *      Author: biel
 */

#include <shell/shell_nonInterface.h>

namespace BFG {

blNonInterface::blNonInterface() {
	Clear();
	background = new( TAG_OLD_UI ) blBackground();
	isActive = true;
}

blNonInterface::~blNonInterface() {
	delete background;
	Clear();
}

void blNonInterface::Clear() {
	background = NULL;
	isActive = false;
}

} /* namespace BFG */
