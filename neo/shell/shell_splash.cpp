/*
 * shell_splash.cpp
 *
 *  Created on: 26 de març 2019
 *      Author: biel
 */

#include <shell/shell_splash.h>
#include "../renderer/RenderSystem.h"

namespace BFG {

blShellSplash::blShellSplash() {
	Clear();
}

blShellSplash::~blShellSplash() {
	Stop();
	Clear();
}

void blShellSplash::Clear() {
	isVideo = false;
	isPaused = false;
	isHidden = false;
}

void blShellSplash::Update( static idMaterial* mat, float ar, idVec4 col, idVec4 back_c ) {
	data.material = mat;
	data.aspectRatio = ar;
	data.tintColour = col;
	data.backgroundColour = back_c;
}

void blShellSplash::Start() {
	isPaused = false;
	isHidden = false;
	if( isVideo ) {
		data.material->ResetCinematicTime( Sys_Milliseconds() );
	}
}

void blShellSplash::Stop() {
	data.material->FreeData();
}

void blShellSplash::Init( static idMaterial* mat, float ar, idVec4 col, idVec4 back_c ) {
	Update( mat, ar, col, back_c );
	isVideo = ( mat->GetName() == "splashVideo" );
	Start();
}

void blShellSplash::TogglePause() {
	isPaused = ( isPaused != true );
}

void blShellSplash::ToggleHide() {
	isHidden = ( isHidden != true );
}

void blShellSplash::PrepareForRendering() {
	stretchedRenderTarget_t stretchRenderContent;
	idMaterial* whiteMaterial = declManager->FindMaterial( "_white" );

	float sysWidth = renderSystem->GetWidth() * renderSystem->GetPixelAspect();
	float sysHeight = renderSystem->GetHeight();
	float sysAspect = sysWidth / sysHeight;
	float adjustment = sysAspect / data.aspectRatio;
	float barHeight = ( adjustment >= 1.0f ) ? 0.0f : ( 1.0f - adjustment ) * ( float )renderSystem->GetVirtualHeight() * 0.25f;
	float barWidth = ( adjustment <= 1.0f ) ? 0.0f : ( adjustment - 1.0f ) * ( float )renderSystem->GetVirtualWidth() * 0.25f;

	idVec4 UVarea = (0.0f, 0.0f, 1.0f, 1.0f );

	// first let's prepare to render the bars if any is needed
	if( barHeight > 0.0f ) {
		// topmost black bar
		idVec4 area = ( 0.0f, 0.0f, renderSystem->GetVirtualWidth(), barHeight );
		shell->FeedTheStretchRenderList( whiteMaterial, data.backgroundColour, area, UVarea );

		// lowest black bar
		idVec4 area = ( 0.0f, ( renderSystem->GetVirtualHeight() - barHeight ), renderSystem->GetVirtualWidth(), barHeight );
		shell->FeedTheStretchRenderList( whiteMaterial, data.backgroundColour, area, UVarea );

	}

	if( barWidth > 0.0f ) {
		// leftmost black bar
		idVec4 area = ( 0.0f, 0.0f, barWidth, renderSystem->GetVirtualHeight() );
		shell->FeedTheStretchRenderList( whiteMaterial, data.backgroundColour, area, UVarea );

		// rightmost black bar
		idVec4 area = ( ( renderSystem->GetVirtualWidth() - barWidth ), 0.0f, barWidth, renderSystem->GetVirtualHeight() );
		shell->FeedTheStretchRenderList( whiteMaterial, data.backgroundColour, area, UVarea );

	}

	// now let's prepare to render the actual splash content
	idVec4 area = ( barWidth, barHeight, ( renderSystem->GetVirtualWidth() - ( barWidth * 2.0f ) ), ( renderSystem->GetVirtualHeight() - ( barHeight * 2.0f ) ) );
	shell->FeedTheStretchRenderList( data.material, data.tintColour, area, UVarea );


}

} /* namespace BFG */
