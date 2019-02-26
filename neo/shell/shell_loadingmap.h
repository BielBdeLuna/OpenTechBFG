/*
 * shell_loadingmap.h
 *
 *  Created on: 7 d’oct. 2018
 *      Author: biel
 */

#ifndef NEO_SHELL_SHELL_LOADINGMAP_H_
#define NEO_SHELL_SHELL_LOADINGMAP_H_

#include "../shell/shell_interface.h"

#include "../shell/shell_background.h"
#include "../framework/Common.h"
#include "../shell/shell_flat_system.h"

namespace BFG {

class blLoadingMap : public blGeneralInterface {
public:
	blLoadingMap( idDict _data );
	void			Update();
	float			percentage;
	bool			GetWaiting() { return IsWaiting; };
	void			SetWaiting( bool Switch ) { IsWaiting = Switch; };

private:
	void			imGuiDraw();
	void			DataManager();
	bool			IsWaiting; // this case is only active if the shell need the user to confirm after loading the map

};

} /* namespace BFG */

#endif /* NEO_SHELL_SHELL_LOADINGMAP_H_ */
