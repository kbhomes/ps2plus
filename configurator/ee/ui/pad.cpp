#include "pad.h"

#include "cstring"
#include "stdio.h"

#define PAD_NUM_PORTS 2

static char _padBuffers[PAD_NUM_PORTS][256] __attribute__((aligned(64)));
static PS2Plus::Gamepad::PadStatus _statuses[PAD_NUM_PORTS]; 

namespace PS2Plus::Gamepad {
    void Start(int port) {
        int ret;

        // Reset the pad state
        PadStatus &status = _statuses[port];
        status.Reset();
        status.status = PadPortOpening;
        status.port = port;
        status.slot = 0;

        // Initialize the pad port
        padInit(0);
        ret = padPortOpen(status.port, status.slot, _padBuffers[port]);
        if (!ret) {
            printf("[PS2Plus::Gamepad::Start] padPortOpen(%d, %d, ...) = %d - failed\n", status.port, status.slot, ret);
            status.status = PadPortError;
        } else {
            printf("[PS2Plus::Gamepad::Start] padPortOpen(%d, %d, ...) succeeded!\n", status.port, status.slot);
        }
    }

    void Stop(int port) {
        // Reset the pad state
        PadStatus &status = _statuses[port];
        status.Reset();
    }

    const PadStatus& Read(int port) {
        PadStatus& status = _statuses[port];
        status.Update();
        return status;
    }

    void PadStatus::Reset() {
        status = PadPortStopped;
        rawButtons = 0;
        rawButtonsPrevious = 0;
        rawButtonsNew = 0;
        memset(&pad, 0, sizeof(pad));
    }

    bool PadStatus::Update() {
        if (status == PadPortStopped || status == PadPortError || status == PadPortDisconnected) {
            return false;
        }

        int rawState = padGetState(port, slot);

        if (status == PadPortOpening) {
            if (rawState == PAD_STATE_STABLE || rawState == PAD_STATE_FINDCTP1) {
                // Determine if there are any non-digital controller modes
                int modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
                bool hasDualShockMode = false;
                for (int i = 0; i < modes; i++) {
                    if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK) {
                        hasDualShockMode = true;
                        break;
                    }
                }

                // If DualShock mode is available, enable it and lock it
                if (hasDualShockMode) {
                    printf("[PS2Plus::Gamepad::Update][status=%d] Setting DualShock mode\n", status);
                    padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);
                } else {
                    printf("[PS2Plus::Gamepad::Update][status=%d] Controller remaining in digital mode\n", status);
                }

                // Update the status
                status = PadPortSettingMode;
            }
        } else if (status == PadPortSettingMode) {
            if (rawState == PAD_STATE_STABLE || rawState == PAD_STATE_FINDCTP1) {
                printf("[PS2Plus::Gamepad::Update][status=%d] Controller is ready\n", status);

                // Update the status
                status = PadPortReady;
            }
        } else if (status == PadPortReady) {
            if (rawState == PAD_STATE_STABLE || rawState == PAD_STATE_FINDCTP1) {
                int ret = padRead(port, slot, &pad);

                if (ret != 0) {
                    rawButtonsPrevious = rawButtons;
                    rawButtons = pad.btns ^ 0xFFFF;
                    rawButtonsNew = rawButtons & ~rawButtonsPrevious;
                    return true;
                } else {
                    rawButtons = 0;
                    rawButtonsNew = 0;
                    rawButtonsPrevious = 0;
                    return false;
                }
            } else if (rawState == PAD_STATE_DISCONN) {
                printf("[PS2Plus::Gamepad::Update][status=%d] Controller disconnected!\n", status);
                status = PadPortDisconnected;
            } else if (rawState == PAD_STATE_ERROR) {
                printf("[PS2Plus::Gamepad::Update][status=%d] Controller error!\n", status);
                status = PadPortError;
            }
        }

        return false;
    }

    bool PadStatus::IsButtonDown(int button) {
        return (rawButtons & button) != 0;
    }

    bool PadStatus::IsButtonPressed(int button) {
        return (rawButtonsNew & button) != 0;
    }

    uint8_t PadStatus::GetJoystickAxisRaw(JoystickAxis axis) {
        switch (axis) {
            case JSRightX:  return pad.rjoy_h;
            case JSRightY:  return pad.rjoy_v;
            case JSLeftX:   return pad.ljoy_h;
            case JSLeftY:   return pad.ljoy_v;
            default:        return 0x7F;
        }
    }

    float PadStatus::GetJoystickAxis(JoystickAxis axis) {
        uint8_t raw = GetJoystickAxisRaw(axis);
        return (raw / 255.f) * 2 - 1;
    }

    PadPortStatus PadStatus::GetStatus() {
        return status;
    }
}