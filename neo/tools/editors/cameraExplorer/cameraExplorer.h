/*
 * CameraExplorer.h
 * this file is GPLv3
 *
 *  Created on: 31 de jul. 2018
 *      Author: Biel Bestué de Luna
 */

#ifndef __CAMERA_EXPLORER_H_
#define __CAMERA_EXPLORER_H_

#include "../../../idlib/Str.h"

namespace BFG
{
namespace Tools
{
//void CameraExplorer_f();

struct cameraInfo_lens_t {
  bool  lensAberration;
	float lens_k;
	float lens_kcube;
	float lens_chromatic;
	int   fov;
};

struct cameraInfo_t {
	//obturation
	bool 	MotionBlur;
	float	MotionBlur_quality;
	float	MotionBlur_angle; //from the whole circe what angle do we expose ( hence how much motion blur per sample )

	//dof
  float focus_radius;
  int focus_ngon; //0 is round, 1-2 is impossible, number of facets the ngon of the defocused image has
  float focus_gradation_exponent; // by what exponent the resultin image to half resolution to the given distance and beyond
  float distance_to_half_far; //distance between focus radius point and the poin at which the image becomes half resolution due the out of focus effect.
  float distance_to_half_near;

  //exposure
  float exposure;
  float absolute_max_exposable;   //incident light - further than this we can't overexpose, so pure-pure-white
  float absolute_min_exposable;   //incident light - further than this we can't underexposureexpose, so pure-pure-black

  //imager
  bool	HDR;       //should always be on
  float latitude;
  float	gamma;        //should be a vec3 as every primary color can have it's own.
  float knee;         //should be a vec3
  float black_point;  //should be a vec3 - this is the resulting black point, nothign to do with the exposure
  float white_point;  //should be a vec3 - this is the resulting white point, nothign to do with the exposure

  //auto exposure
  bool  auto_exposure;
  float auto_exposure_high_threshold; // how much more exposure + latitude + threshold will make the autoexposure change;
  float auto_exposure_low_threshold;  // how much more exposure - latitude - threshold will make the autoexposure change;
  float auto_exposure_rate_of_detection; // how much time will have to pass for the over-exposure to force a change (so no blinking lights turn the system mad)
  float auto_adaptation_rate_up;
  float auto_adaptation_rate_down;

};

class OTE_CameraExplorer {
public:
  OTE_CameraExplorer();
  void HandleKeyInput();

	void Update();
	bool ShowWindow;
private:

  void OpenWindow();
	void Clear();
	void Draw();
	void ApplyChanges();
	void CloseWindow();
  void GatherCurrent();
  void BackToSaved();
  void DiscardChanges( idStr message );

  bool changes;
	bool pause_changes;
	cameraInfo_lens_t cur_lens;
	cameraInfo_lens_t saved_lens;

};

} //namespace Tools
} //namespace BFG

#endif /* __CAMERA_EXPLORER_H_ */
