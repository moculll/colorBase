#include <TimerEvents/TimerEvents.h>
#include <map>
namespace colorBase {

class colorBaseMgr;

struct colorBasePortMgr {
    TimerEvents::TimerEvents timer;
    void initImpl(colorBaseMgr &mgr);

    
};



} /* colorBase */