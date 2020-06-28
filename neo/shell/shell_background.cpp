/*
 * shell_background.cpp
 *
 *  Created on: 10 de set. 2018
 *      Author: biel
 */

#include "../shell/shell_background.h"
#include "../framework/Common.h"
#include "../d3xp/Game.h"

namespace BFG {

blBackground::blBackground( idStr material_name, idVec4 tint ) {
	Clear();
	material = declManager->FindMaterial( material_name.c_str() );
	if( material == NULL ) {
		common->Warning( "blBackground: material with name %s couldn't be found, defaulting it.\n", material_name.c_str() );
		material = declManager->FindMaterial( "_default" );
	}
	colour = tint;
}

blBackground::~blBackground() {
	Clear();
}

void blBackground::Clear() {
	material = NULL;
	colour = idVec4( 1.0f );
}

void blBackground::Update() {
	if( material == NULL ) {
		return;
	} else {
		renderSystem->SetColor4( colour.x, colour.y, colour.z, colour.w );
		renderSystem->DrawStretchPic( 0, 0, renderSystem->GetVirtualWidth(), renderSystem->GetVirtualHeight(), 0, 0, 1, 1, material );
	}
}

} /* namespace BFG */
