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

blBackground::blBackground() {
	Clear();
}

blBackground::~blBackground() {
	Clear();
}

void blBackground::Clear() {
	Clean_up();
	active = false;
}

void blBackground::Clean_up() {
	material = NULL;
	colour = idVec4( 0.00f, 0.00f, 0.00f, 0.00f );
}

void blBackground::InitNone() {
	if( active ) {
		active = false;
	}
	Clean_up();
}

void blBackground::InitColour( idVec4 Colour ) {
	if( !active ) {
		active = true;
	}

	material = declManager->FindMaterial( "_white" );
	colour = Colour;
}

void blBackground::InitMaterial( idStr material_name, idVec4 Tint ) {
	if( !active ) {
		active = true;
	}

	material = declManager->FindMaterial( material_name.c_str() );
	if( material == NULL ) {
		common->Error( "blBackground::InitMaterial: material with name %s couldn't be found!", material_name.c_str() );
	}
	colour = Tint;
}

void blBackground::Update() {
	if( active && ( material != NULL ) ) {
		renderSystem->SetColor4( colour.x, colour.y, colour.z, colour.w );
		renderSystem->DrawStretchPic( 0, 0, renderSystem->GetVirtualWidth(), renderSystem->GetVirtualHeight(), 0, 0, 1, 1, material );
	} else {
		return;
	}
}

} /* namespace BFG */
