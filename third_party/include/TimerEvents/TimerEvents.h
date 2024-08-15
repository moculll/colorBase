#pragma once
#include <functional>

#ifdef __linux__

#include <signal.h>
#include <time.h>
#include <unistd.h>

#else

#include <windows.h>
#include <mmsystem.h>

#endif

#ifdef __linux__

#else

#endif
namespace TimerEvents {

class TimerEvents {
public:
    static constexpr uint8_t TIMER_TYPE_PERIODIC = 0;
    static constexpr uint8_t TIMER_TYPE_ONESHOT = 1;

    using timer_events_handler_t = std::function<void(void*)>;
    TimerEvents(bool loop, uint32_t _executeTime, timer_events_handler_t _handler, void* objptr)
        : executeTime(_executeTime), handler(_handler), objectPtr(objptr), runningStatus(false), timerID(nullptr) {
        if (loop) {
            timerType = TIMER_TYPE_PERIODIC;
        } else {
            timerType = TIMER_TYPE_ONESHOT;
        }
    }
    TimerEvents() {
        executeTime = 0;
        handler = nullptr;
        objectPtr = nullptr;
        timerID = nullptr;
        runningStatus = false;
        timerType = TIMER_TYPE_ONESHOT;
    }
    ~TimerEvents() {
        stopExecute();
    }
    void setObj(void *arg)
    {
        objectPtr = arg;
    }
    void setType(uint8_t type){
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

    uint32_t getRemainingTime() {
        return 0;
    }
    bool getRunningStatus(){
        return runningStatus;
    }
#ifdef __linux__
    bool stopExecute() {
        runningStatus = false;
        if (timerID) {
            struct itimerspec value = {0};
            timer_settime (timerID, 0, &value, NULL);
        }
        return true;
    }

    void setTimer(int intervalMs, uint8_t type) {
        struct sigevent te;
        struct itimerspec value = {0};
        struct sigaction sigAction;
        if(timerID)
            timer_settime (timerID, 0, &value, NULL);

        te.sigev_notify = SIGEV_THREAD;
        te.sigev_notify_function = &TimerEvents::StaticTimerCallback;
        te.sigev_value.sival_ptr = static_cast<void *>(this);
        te.sigev_notify_attributes = nullptr;
        timer_create (CLOCK_REALTIME, &te, &timerID);

        if(type == TIMER_TYPE_PERIODIC){
            value.it_value.tv_sec = intervalMs / 1000;
            value.it_value.tv_nsec = intervalMs % 1000 * 1000000;
            value.it_interval.tv_sec = intervalMs / 1000;
            value.it_interval.tv_nsec = intervalMs % 1000 * 1000000;
        }
        else if(type == TIMER_TYPE_ONESHOT){
            value.it_value.tv_sec = intervalMs / 1000;
            value.it_value.tv_nsec = intervalMs % 1000 * 1000000;
            value.it_interval.tv_sec = 0;
            value.it_interval.tv_nsec = 0;
        }
        this->timerType = type;

        timer_settime (timerID, 0, &value, NULL);

    }
#else
    bool stopExecute() {
        runningStatus = false;
        if (timerID) {
            timeKillEvent(timerID);
            timerID = 0;
        }
        return true;
    }
    void setTimer(int intervalMs, uint8_t type) {
        timeKillEvent(timerID);

        timerID = timeSetEvent(intervalMs, 0, reinterpret_cast<LPTIMECALLBACK>(&TimerEvents::StaticTimerCallback), reinterpret_cast<DWORD_PTR>(this), type);
        if (!timerID) {
            return;
        }
    }
#endif

private:
    uint32_t executeTime;
    timer_events_handler_t handler;
    uint8_t timerType;
    void* objectPtr;
    bool runningStatus;
    
#ifdef __linux__
    timer_t timerID;
    static void StaticTimerCallback(union sigval val) {
        TimerEvents* timerInstance = static_cast<TimerEvents*>(val.sival_ptr);
        if (timerInstance != nullptr) {
            timerInstance->timerCallback();
        }
    }


    void timerCallback() {
        handler(objectPtr);
    }
#else
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

#endif

};

} /* TimerEvents */

