#ifndef __beep__
#define __beep__
#include "include.cpp"
#include "led.cpp"

void *beep(void *arg) {
    led(0, 4);
    usleep(500000);
    led(1, 4);
    return arg;
}


#endif