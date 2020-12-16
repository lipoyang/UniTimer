#include <Arduino.h>
#include "UniTimer.h"

UniTimerClass UniTimer;

// (1) AVR Arduino (e.g. Uno, Mega, Leonardo, Micro, Nano)
//     GR-SAKURA, GR-CITRUS
#if defined(__AVR__) || defined(GRSAKURA) || defined(GRCITRUS)
#include <MsTimer2.h>

// (2) Due
#elif defined(__SAM3X8E__)
#include <DueTimer.h>

// (3) ESP8266
#elif defined(ESP8266)
#define MSEC2CLOCK(ms)    (ms * (ESP8266_CLOCK/1000))

static void timer_callback()
{
  timer0_write(ESP.getCycleCount() + MSEC2CLOCK(UniTimer.m_interval) );
  UniTimer.m_handler();
}

// (4) ESP32
//     GR-ROSE
#elif defined(ESP32) || defined(GRROSE)
extern "C" {
#include "FreeRTOS.h"
#if defined(GRROSE)
#include "timers.h"
#endif
}

static TimerHandle_t s_timer;

static void timer_callback(TimerHandle_t pxTimer)
{
  UniTimer.m_handler();
}

// (5) GR-KURUMI
#elif defined(__RL78__)
static void timer_callback(unsigned long u32Milles)
{
  static int cnt = 0;
  cnt++;
  if(cnt >= UniTimer.m_interval){
    cnt = 0;
    UniTimer.m_handler();
  }
}
#endif


/****************************************
  set interval[msec] and handler
 ****************************************/
void UniTimerClass::set(int interval, void(*handler)(void))
{
    if(handler == NULL) return;
    m_handler  = handler;
    m_interval = interval;

// (1) AVR Arduino (e.g. Uno, Mega, Leonardo, Micro, Nano)
//     GR-SAKURA, GR-CITRUS
#if defined(__AVR__) || defined(GRSAKURA) || defined(GRCITRUS)
    MsTimer2::set(interval, handler);
    MsTimer2::start();

// (2) Due
#elif defined(__SAM3X8E__)
    Timer3.attachInterrupt(handler);
    Timer3.start(interval * 1000);

// (3) ESP8266
#elif defined(ESP8266)
    noInterrupts();
    timer0_isr_init();
    timer0_attachInterrupt(timer_callback);
    timer0_write(ESP.getCycleCount() + MSEC2CLOCK(interval) );
    interrupts();

// (4) ESP32
//     GR-ROSE
#elif defined(ESP32) || defined(GRROSE)
    s_timer = xTimerCreate(
        "interval_timer",   // name
        interval / portTICK_PERIOD_MS, // interval
        pdTRUE,             // repeat
        (void *)0,          // timer ID
        timer_callback);    // callback
    if(s_timer != NULL)
    {
        xTimerStart(s_timer, 0);
    }

// (5) GR-KURUMI
#elif defined(__RL78__)
    attachIntervalTimerHandler(timer_callback);
#endif
}

