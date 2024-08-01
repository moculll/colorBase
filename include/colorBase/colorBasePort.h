#include <TimerEvents/TimerEvents.h>

namespace colorBase {

class colorBaseMgr;

struct colorBasePortMgr {
    TimerEvents::TimerEvents timer;
    void initImpl(colorBaseMgr &mgr);
};



} /* colorBase */