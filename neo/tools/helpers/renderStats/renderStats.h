/*
    this file is GPLv3
*/

#ifndef __SHOW_STATS_H__
#define __SHOW_STATS_H__

//#include <idlib/Str.h>
#include "../../../idlib/Str.h"

namespace BFG {
namespace Tools {

struct time_stats_t {
  int previous;
  float timeMs;
  float maxTimeMs;

  float fps;
};

class renderStatsPannel {
public:
  renderStatsPannel();

  void Toggle();
  void Update();

  bool PannelPresent;
private:
  void Clear();
  void OpenPannel();
  void ClosePannel();
  void Draw();
  time_stats_t timeStats;

};

} //namespace Tools
} //namespace BFG
#endif /* !__SHOW_STATS_H__ */
