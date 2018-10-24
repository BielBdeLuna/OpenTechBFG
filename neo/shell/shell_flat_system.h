/*
 * shell_flat_system.h
 *
 *  Created on: 9 d’oct. 2018
 *      Author: biel
 */
 
#ifndef __SHELL_FLAT_SYSTEM_H__
#define __SHELL_FLAT_SYSTEM_H__

namespace BFG
{

enum flatSystems_t
{
	FLAT_SYSTEM_IMGUI = -1,
	FLAT_SYSTEM_FLEXIBLE,
	FLAT_SYSTEM_CEGUI,
	FLAT_SYSTEM_FLASH_SHELL,
	NUM_FLAT_SYSTEMS // it shouldn't count IMGUI since it's not file-based
};

int Determine_flat_System_level();

} // namespace BFG

#endif /* !__SHELL_FLAT_SYSTEM_H__ */
