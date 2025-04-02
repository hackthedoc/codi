#pragma once

#include "macros.h"

typedef struct {
    i16 startPosX;      // window starting position x axis, if applicable
    i16 startPosY;      // window starting position y axis, if applicable
    i16 startWidth;     // window starting width, if applicable
    i16 startHeight;    // window starting height, if applicable
    char* name;         // the application name used in windowing, if applicable
} CODI_ApplicationConfig;

struct CODI_Game;

CAPI b8 CODI_ApplicationCreate(struct CODI_Game* gInstance);

CAPI b8 CODI_ApplicationRun();
