#pragma once

#include <ps2plus_common.h>
#include <shared/config.h>
#include "ui/pad.h"
#include "ui/gfx.h"
#include "util/firmware-update.h"

typedef enum {
    StatusNone,
    StatusPending,
    StatusRebooting,
    StatusUpdating,
    StatusUpdatePending,
    StatusCompleted,
    StatusFailed
} configurator_ps2plus_controller_update_status;

typedef struct {
    int port;
    bool connected;
    struct {
        uint64_t firmware;
        char microcontroller[33];
        uint16_t configuration;
    } versions;
    ps2plus_configuration configuration;
    struct {
        configurator_ps2plus_controller_update_status status;
        FirmwareUpdate *firmware;
        int last_record_index;
        int total_records;
        float last_check_time;
    } update;
} configurator_ps2plus_controller;

typedef struct {
    configurator_ps2plus_controller controllers[2];
    configurator_ps2plus_controller *current_controller;
    PS2Plus::Gamepad::PadStatus current_pad;
    PS2Plus::Gamepad::PadSummary pad_summary;
    PS2Plus::Graphics::VideoMode video_mode;
} configurator_state;