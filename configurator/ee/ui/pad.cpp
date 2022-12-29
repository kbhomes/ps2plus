#include "pad.h"

#include "cstring"
#include "stdio.h"
#include <algorithm>

#define PAD_NUM_PORTS 2

static char _padBuffers[PAD_NUM_PORTS][256] __attribute__((aligned(64)));
static PS2Plus::Gamepad::PadStatus _statuses[PAD_NUM_PORTS]; 
static PS2Plus::Gamepad::PadSummary _summary;
static char _actuators[PAD_NUM_PORTS][6];

namespace PS2Plus::Gamepad {
    void Initialize() {
        // Initialize the pad module
        int ret = padInit(0);
        if (!ret) {
            printf("[PS2Plus::Gamepad::Initialize] padInit(0) = %d - failed\n", ret);
        } else {
            printf("[PS2Plus::Gamepad::Initialize] padInit(0) succeeded!\n");
        }

        // Initialize the pad summary
        _summary.status = PadSummaryInactive;
        _summary.rawButtons = 0;
        _summary.rawButtonsPrevious = 0;
        _summary.rawButtonsNew = 0;
    }

    void StartAll() {
        for (int i = 0; i < PAD_NUM_PORTS; i++) {
            Start(i);
        }
    }

    void StopAll() {
        for (int i = 0; i < PAD_NUM_PORTS; i++) {
            Stop(i);
        }
    }

    const PadSummary& ReadSummary() {
        memset(&_summary.pad, 0, sizeof(_summary.pad));
        _summary.pad.btns = 0xffff;
        _summary.status = PadSummaryInactive;

        // Set accumulators to average out the values across all active pads
        int num_active_pads = 0;
        int rjoy_h = 0, rjoy_v = 0, ljoy_h = 0, ljoy_v = 0;
        u32 right_p = 0, left_p = 0, up_p = 0, down_p = 0, 
            triangle_p = 0, circle_p = 0, cross_p = 0, square_p = 0, 
            l1_p = 0, r1_p = 0, l2_p = 0, r2_p = 0;

        for (int i = 0; i < PAD_NUM_PORTS; i++) {
            const PadStatus& status = Read(i);
            if (status.status == PadPortReady) {
                num_active_pads++;
                _summary.pad.ok = std::max(status.pad.ok, _summary.pad.ok); // Highest `ok` value across pads
                _summary.pad.mode = std::max(status.pad.mode, _summary.pad.mode); // Highest `mode` across pads
                _summary.pad.btns &= status.pad.btns; // Merge all active-low button values across pads
                
                // Accumulate joystick differences from center
                rjoy_h += status.pad.rjoy_h - 0x7f;
                rjoy_v += status.pad.rjoy_v - 0x7f;
                ljoy_h += status.pad.ljoy_h - 0x7f;
                ljoy_v += status.pad.ljoy_v - 0x7f;

                // Accumulate pressure values
                right_p += status.pad.right_p;
                left_p += status.pad.left_p;
                up_p += status.pad.up_p;
                down_p += status.pad.down_p;
                triangle_p += status.pad.triangle_p;
                circle_p += status.pad.circle_p;
                cross_p += status.pad.cross_p;
                square_p += status.pad.square_p;
                l1_p += status.pad.l1_p;
                r1_p += status.pad.r1_p;
                l2_p += status.pad.l2_p;
                r2_p += status.pad.r2_p;
            }
        }

        // Calculate button changes
        _summary.rawButtonsPrevious = _summary.rawButtons;
        _summary.rawButtons = _summary.pad.btns ^ 0xFFFF;
        _summary.rawButtonsNew = _summary.rawButtons & ~_summary.rawButtonsPrevious;

        if (num_active_pads > 0) {
            // Apply accumulated values to the pad status
            _summary.pad.rjoy_h = std::clamp(0x7f + rjoy_h, 0, 0xff);
            _summary.pad.rjoy_v = std::clamp(0x7f + rjoy_v, 0, 0xff);
            _summary.pad.ljoy_h = std::clamp(0x7f + ljoy_h, 0, 0xff);
            _summary.pad.ljoy_v = std::clamp(0x7f + ljoy_v, 0, 0xff);
            _summary.pad.right_p = right_p / num_active_pads;
            _summary.pad.left_p = left_p / num_active_pads;
            _summary.pad.up_p = up_p / num_active_pads;
            _summary.pad.down_p = down_p / num_active_pads;
            _summary.pad.triangle_p = triangle_p / num_active_pads;
            _summary.pad.circle_p = circle_p / num_active_pads;
            _summary.pad.cross_p = cross_p / num_active_pads;
            _summary.pad.square_p = square_p / num_active_pads;
            _summary.pad.l1_p = l1_p / num_active_pads;
            _summary.pad.r1_p = r1_p / num_active_pads;
            _summary.pad.l2_p = l2_p / num_active_pads;
            _summary.pad.r2_p = r2_p / num_active_pads;

            // Update the pad summary status
            _summary.status = PadSummaryActive;
        }

        return _summary;
    }

    void Start(int port) {
        int ret;

        // Reset the pad state
        PadStatus &status = _statuses[port];
        status.Reset();
        status.status = PadPortOpening;
        status.port = port;
        status.slot = 0;

        // Open the pad port
        ret = padPortOpen(status.port, status.slot, _padBuffers[port]);
        if (!ret) {
            printf("[PS2Plus::Gamepad::Start] padPortOpen(%d, %d, ...) = %d - failed\n", status.port, status.slot, ret);
            status.status = PadPortError;
        } else {
            printf("[PS2Plus::Gamepad::Start] padPortOpen(%d, %d, ...) succeeded!\n", status.port, status.slot);
        }
    }

    void Stop(int port) {
        int ret;
        PadStatus &status = _statuses[port];

        // Close the pad port
        printf("[PS2Plus::Gamepad::Stop] padPortClose(%d, %d) ...\n", status.port, status.slot);
        ret = padPortClose(status.port, status.slot);
        if (!ret) {
            printf("[PS2Plus::Gamepad::Stop] padPortClose(%d, %d) = %d - failed\n", status.port, status.slot, ret);
            status.status = PadPortError;
        } else {
            printf("[PS2Plus::Gamepad::Stop] padPortClose(%d, %d) succeeded!\n", status.port, status.slot);
        }

        // // End pad communication
        // printf("[PS2Plus::Gamepad::Stop] padEnd() ...\n", status.port, status.slot);
        // ret = padEnd();
        // if (!ret) {
        //     printf("[PS2Plus::Gamepad::Stop] padEnd() = %d - failed\n", ret);
        //     status.status = PadPortError;
        // } else {
        //     printf("[PS2Plus::Gamepad::Stop] padEnd() succeeded!\n");
        // }
        
        // Reset the pad state
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

                    if (padInfoAct(port, slot, -1, 0)) {
                        printf("[PS2Plus::Gamepad::Update][status=%d] Controller has actuators\n", status);
                        
                        // Update the status
                        status = PadPortSettingActuators;
                    } else {
                        printf("[PS2Plus::Gamepad::Update][status=%d] Controller has no actuators\n", status);

                        // Update the status
                        status = PadPortSettingMode;
                    }
                } else {
                    printf("[PS2Plus::Gamepad::Update][status=%d] Controller remaining in digital mode\n", status);

                        // Update the status
                        status = PadPortSettingMode;
                }
            }
        } else if (status == PadPortSettingActuators) {
            if (rawState == PAD_STATE_STABLE || rawState == PAD_STATE_FINDCTP1) {
                printf("[PS2Plus::Gamepad::Update][status=%d] Controller is in DualShock mode\n", status);

                // Initialize both motors
                _actuators[port][0] = 0;   // Enable small engine
                _actuators[port][1] = 1;   // Enable big engine
                _actuators[port][2] = 0xff;
                _actuators[port][3] = 0xff;
                _actuators[port][4] = 0xff;
                _actuators[port][5] = 0xff;

                int ret = padSetActAlign(port, slot, _actuators[port]);
                printf("[PS2Plus::Gamepad::Update][status=%d] Controller setting actuators: ret=%d\n", status, ret);


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

    void PadStatus::SetRumbleActuatorSmall(bool active) {
        _actuators[port][0] = active ? 1 : 0;
        padSetActDirect(port, slot, _actuators[port]);
    }

    void PadStatus::SetRumbleActuatorLargePower(uint8_t power) {
        _actuators[port][1] = power;
        padSetActDirect(port, slot, _actuators[port]);
    }

    bool PadStatus::IsRumbleActuatorSmallActive() {
        return _actuators[port][0] != 0;
    }

    uint8_t PadStatus::GetRumbleActuatorLargePower() {
        return _actuators[port][1];
    }

    bool PadSummary::IsButtonDown(int button) {
        return (rawButtons & button) != 0;
    }

    bool PadSummary::IsButtonPressed(int button) {
        return (rawButtonsNew & button) != 0;
    }

    uint8_t PadSummary::GetJoystickAxisRaw(JoystickAxis axis) {
        switch (axis) {
            case JSRightX:  return pad.rjoy_h;
            case JSRightY:  return pad.rjoy_v;
            case JSLeftX:   return pad.ljoy_h;
            case JSLeftY:   return pad.ljoy_v;
            default:        return 0x7F;
        }
    }

    float PadSummary::GetJoystickAxis(JoystickAxis axis) {
        uint8_t raw = GetJoystickAxisRaw(axis);
        return (raw / 255.f) * 2 - 1;
    }

    PadSummaryStatus PadSummary::GetStatus() {
        return status;
    }
}