#include <UniTimer.h>

#define TEST_PIN        2
#define INTERVAL_MSEC   50

void handler()
{
    static int blink = 0;
    blink = 1 - blink;
    digitalWrite(TEST_PIN, blink);
}

void setup()
{
    Serial.begin(9600);
    pinMode(TEST_PIN, OUTPUT);
    digitalWrite(TEST_PIN, LOW);
    
    UniTimer.set(INTERVAL_MSEC, handler);
}

void loop()
{
    static int cnt = 0;
    cnt++;
    Serial.println(cnt);
    delay(1000);
}
