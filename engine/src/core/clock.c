#include "clock.h"

#include "platform/platform.h"

void clockUpdate(CODI_Clock* clock) {
    if (clock->startTime == 0) return;
    else clock->elapsed = platformGetAbsoluteTime() - clock->startTime;
}

void clockStart(CODI_Clock* clock) {
    clock->startTime = platformGetAbsoluteTime();
    clock->elapsed = 0;
}

void clockStop(CODI_Clock* clock) {
    clock->startTime = 0;
}
