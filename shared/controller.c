#include "controller.h"

const char *ps2plus_controller_digital_button_name(ps2plus_controller_digital_button button) {
  switch (button) {
    case DBSelect:   return "Select";
    case DBL3:       return "L3";
    case DBR3:       return "R3";
    case DBStart:    return "Start";
    case DDUp:       return "Up";
    case DDRight:    return "Right";
    case DDDown:     return "Down";
    case DDLeft:     return "Left";
    case DBL2:       return "L2";
    case DBR2:       return "R2";
    case DBL1:       return "L1";
    case DBR1:       return "R1";
    case DBTriangle: return "Triangle";
    case DBCircle:   return "Circle";
    case DBCross:    return "Cross";
    case DBSquare:   return "Square";
    default:         return "<unknown>";
  }
}

const char *ps2plus_controller_joystick_axis_name(ps2plus_controller_joystick_axis axis) {
  switch (axis) {
    case JSRightX: return "RX";
    case JSRightY: return "RY";
    case JSLeftX:  return "LX";
    case JSLeftY:  return "LY";
    default:       return "<unknown>";
  }
}

const char *ps2plus_controller_joystick_axis_range_name(ps2plus_controller_joystick_axis_range axis_range) {
  switch (axis_range) {
    case JSAxisRange_LeftXMin:     return "LX-Min";
    case JSAxisRange_LeftXCenter:  return "LX-Center";
    case JSAxisRange_LeftXMax:     return "LX-Max";
    case JSAxisRange_LeftYMin:     return "LY-Min";
    case JSAxisRange_LeftYCenter:  return "LY-Center";
    case JSAxisRange_LeftYMax:     return "LY-Max";
    case JSAxisRange_RightXMin:    return "RX-Min";
    case JSAxisRange_RightXCenter: return "RX-Center";
    case JSAxisRange_RightXMax:    return "RX-Max";
    case JSAxisRange_RightYMin:    return "RY-Min";
    case JSAxisRange_RightYCenter: return "RY-Center";
    case JSAxisRange_RightYMax:    return "RY-Max";
    default: return "<unknown>";
  }
}