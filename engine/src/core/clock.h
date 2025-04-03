#pragma once

#include "macros.h"

typedef struct {
    f64 startTime;
    f64 elapsed; 
} CODI_Clock;

// Updates the provided clock. Should be called just before checking elapsed time.
// Has no effect on non-started clocks
void clockUpdate(CODI_Clock* clock);

// Starts the provided clock. Resets elapsed time.
void clockStart(CODI_Clock* clock);

// Stops the provided clock. Does not reset elapsed time.
void clockStop(CODI_Clock* clock);
