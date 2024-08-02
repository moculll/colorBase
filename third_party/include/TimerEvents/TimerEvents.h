#pragma once
#include <functional>
#include <windows.h>
#include <mmsystem.h>

namespace TimerEvents {

class TimerEvents {
public:
    using timer_events_handler_t = std::function<void(void*)>;

    TimerEvents(bool loop, uint32_t _executeTime, timer_events_handler_t _handler, void* objptr)
        : executeTime(_executeTime), handler(_handler), objectPtr(objptr), runningStatus(false), timerID(0) {
        if (loop) {
            timerType = TIME_PERIODIC;
        } else {
            timerType = TIME_ONESHOT;
        }
    }
    TimerEvents() {}
    ~TimerEvents() {
        stopExecute();
    }
    void setObj(void *arg)
    {
        objectPtr = arg;
    }
    void setType(uint16_t type){
        timerType = type;
    }
    bool setCallback(timer_events_handler_t _handler) {
        handler = _handler;
        return true;
    }

    bool setExecuteTime(uint32_t _executeTime) {
        executeTime = _executeTime;
        return true;
    }

    bool execute() {
        runningStatus = true;
        setTimer(executeTime, timerType);
        return true;
    }

    bool getRunningStatus()
    {
        return runningStatus;
    }

    bool stopExecute() {
        runningStatus = false;
        if (timerID) {
            timeKillEvent(timerID);
            timerID = 0;
        }
        return true;
    }

    uint32_t getRemainingTime() {

        return 0;
    }

    void setTimer(int intervalMs, UINT type) {
        timeKillEvent(timerID);

        timerID = timeSetEvent(intervalMs, 0, reinterpret_cast<LPTIMECALLBACK>(&TimerEvents::StaticTimerCallback), reinterpret_cast<DWORD_PTR>(this), type);
        if (!timerID) {
            return;
        }
    }
private:
    uint32_t executeTime;
    timer_events_handler_t handler;
    uint16_t timerType;
    void* objectPtr;
    bool runningStatus;
    UINT timerID;

    

    static void CALLBACK StaticTimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
        TimerEvents* timerInstance = reinterpret_cast<TimerEvents*>(dwUser);
        if (timerInstance != nullptr) {
            timerInstance->timerCallback();
        }
    }

    void timerCallback() {
        handler(objectPtr);
        
    }
};

} /* TimerEvents */

