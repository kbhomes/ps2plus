#ifndef UI_PAD_H
#define UI_PAD_H

#include <shared/controller.h>

#include <libpad.h>
#include <stdint.h>
#include <tamtypes.h>

namespace PS2Plus::Gamepad {
    typedef ps2plus_controller_joystick_axis JoystickAxis; // Friendlier class naming
    enum PadPortStatus {
        PadPortStopped,
        PadPortOpening,
        PadPortSettingMode,
        PadPortSettingActuators,
        PadPortReady,
        PadPortError,
        PadPortDisconnected,
    };

    enum PadSummaryStatus {
        PadSummaryInactive,
        PadSummaryActive,
    };

    /**
     * Represents the current status of a single gamepad (e.g., controller 1)
     */
    struct PadStatus {
        PadPortStatus status;
        int port;
        int slot;
        padButtonStatus pad;

        u16 rawButtons; // Current status of the buttons
        u16 rawButtonsPrevious; // Status of the buttons in the previous frame
        u16 rawButtonsNew; // Only buttons which are new since the previous frame

        void Reset();
        bool Update();
        bool IsButtonDown(int button);
        bool IsButtonPressed(int button);
        uint8_t GetJoystickAxisRaw(JoystickAxis axis);
        float GetJoystickAxis(JoystickAxis axis);
        PadPortStatus GetStatus();
        void SetRumbleActuatorSmall(bool active);
        void SetRumbleActuatorLargePower(uint8_t power);
        bool IsRumbleActuatorSmallActive();
        uint8_t GetRumbleActuatorLargePower();
    };

    /**
     * Represents the aggregated button/joystick status across all connected gamepads
     */
    struct PadSummary {
        PadSummaryStatus status;
        PadPortStatus ports[2];
        padButtonStatus pad;
        u16 rawButtons;
        u16 rawButtonsPrevious;
        u16 rawButtonsNew;

        bool IsButtonDown(int button);
        bool IsButtonPressed(int button);
        uint8_t GetJoystickAxisRaw(JoystickAxis axis);
        float GetJoystickAxis(JoystickAxis axis);
        PadSummaryStatus GetStatus();
    };

    void Initialize();
    void StartAll();
    void StopAll();
    const PadSummary& ReadSummary();

    void Start(int port = 0);
    void Stop(int port = 0);
    const PadStatus& Read(int port = 0); 
}

#endif /* UI_PAD_H */