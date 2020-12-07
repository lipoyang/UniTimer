#ifndef _UNI_TIMER_H_
#define _UNI_TIMER_H_

class UniTimerClass
{
public:
    void set(int interval, void(*handler)());
    
    int m_interval;
    void(*m_handler)();
};

extern UniTimerClass UniTimer;

#endif
