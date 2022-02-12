#pragma once

#include <ps2plus_common.h>
#include <shared/config.h>
#include "ui/pad.h"

typedef struct {
    int port;
    bool connected;
    struct {
        uint16_t firmware;
        char microcontroller[33];
        uint16_t configuration;
    } versions;
    ps2plus_configuration configuration;
} configurator_ps2plus_controller;

typedef struct {
    configurator_ps2plus_controller controllers[2];
    configurator_ps2plus_controller *current_controller;
    PadStatus pad_status;
} configurator_state;