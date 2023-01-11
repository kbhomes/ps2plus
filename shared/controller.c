#include "controller.h"

#include <string.h>

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
    case JSAxisRange_LeftXMin:     return "LXMin";
    case JSAxisRange_LeftXCenter:  return "LXCenter";
    case JSAxisRange_LeftXMax:     return "LXMax";
    case JSAxisRange_LeftYMin:     return "LYMin";
    case JSAxisRange_LeftYCenter:  return "LYCenter";
    case JSAxisRange_LeftYMax:     return "LYMax";
    case JSAxisRange_RightXMin:    return "RXMin";
    case JSAxisRange_RightXCenter: return "RXCenter";
    case JSAxisRange_RightXMax:    return "RXMax";
    case JSAxisRange_RightYMin:    return "RYMin";
    case JSAxisRange_RightYCenter: return "RYCenter";
    case JSAxisRange_RightYMax:    return "RYMax";
    default: return "<unknown>";
  }
}

#ifndef _IOP
ps2plus_controller_digital_button ps2plus_controller_digital_button_from_name(const char *name) {
  if (strcmp(name, "Select") == 0)    return DBSelect;
  if (strcmp(name, "L3") == 0)        return DBL3;
  if (strcmp(name, "R3") == 0)        return DBR3;
  if (strcmp(name, "Start") == 0)     return DBStart;
  if (strcmp(name, "Up") == 0)        return DDUp;
  if (strcmp(name, "Right") == 0)     return DDRight;
  if (strcmp(name, "Down") == 0)      return DDDown;
  if (strcmp(name, "Left") == 0)      return DDLeft;
  if (strcmp(name, "L2") == 0)        return DBL2;
  if (strcmp(name, "R2") == 0)        return DBR2;
  if (strcmp(name, "L1") == 0)        return DBL1;
  if (strcmp(name, "R1") == 0)        return DBR1;
  if (strcmp(name, "Triangle") == 0)  return DBTriangle;
  if (strcmp(name, "Circle") == 0)    return DBCircle;
  if (strcmp(name, "Cross") == 0)     return DBCross;
  if (strcmp(name, "Square") == 0)    return DBSquare;

  return (ps2plus_controller_digital_button)0;
}
#endif