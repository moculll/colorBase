#include <TimerEvents/TimerEvents.h>
#include <map>
namespace colorBase {

class colorBaseMgr;

struct colorBasePortMgr {
    TimerEvents::TimerEvents timer;
    void initImpl(colorBaseMgr &mgr);
    void initImpl(colorBaseMgr &mgr, uint8_t bitDepth);
    static int getRandom(int min, int max);
    
};



} /* colorBase */