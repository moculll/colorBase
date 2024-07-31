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

    ~TimerEvents() {
        StopExecute();
    }

    bool SetCallback(timer_events_handler_t _handler) {
        handler = _handler;
        return true;
    }

    bool SetExecuteTime(uint32_t _executeTime) {
        executeTime = _executeTime;
        setTimer(executeTime, timerType);
        return true;
    }

    bool Execute() {
        runningStatus = true;
        setTimer(executeTime, timerType);
        return true;
    }

    bool StopExecute() {
        runningStatus = false;
        if (timerID != NULL) {
            timeKillEvent(timerID);
            timerID = NULL;
        }
        return true;
    }

    uint32_t getRemainingTime() {

        return 0;
    }

private:
    uint32_t executeTime;
    timer_events_handler_t handler;
    uint16_t timerType;
    void* objectPtr;
    bool runningStatus;
    UINT timerID;

    void setTimer(int intervalMs, UINT type) {
        timeKillEvent(timerID);

        timerID = timeSetEvent(intervalMs, 0, reinterpret_cast<LPTIMECALLBACK>(&TimerEvents::StaticTimerCallback), reinterpret_cast<DWORD_PTR>(this), type);
        if (timerID == NULL) {

        }
    }

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

