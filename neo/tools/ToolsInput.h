/*
  this file is GPLv3
*/

#ifndef __TOOLS_INPUT__
#define __TOOLS_INPUT__

namespace BFG {

static bool releaseMouse = false;

namespace Tools {

// things in impl need to be used in at least one other file, but should generally not be touched
namespace impl {

void ReleaseToolMouse( bool doRelease );


} //namespace impl

bool ReleaseMouseForTools();


} //namespace Tools
} //namespace BFG
#endif /* !__TOOLS_INPUT__ */
