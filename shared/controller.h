#ifndef SHARED_CONTROLLER_H
#define SHARED_CONTROLLER_H

typedef enum {
  // First byte
  DBSelect,
  DBL3,
  DBR3,
  DBStart,
  DDUp,
  DDRight,
  DDDown,
  DDLeft,

  // Second byte
  DBL2,
  DBR2,
  DBL1,
  DBR1,
  DBTriangle,
  DBCircle,
  DBCross,
  DBSquare,

  // Total number of buttons
  NUM_DIGITAL_BUTTONS
} ps2plus_controller_digital_button;

typedef enum {
  JSRightX,
  JSRightY,
  JSLeftX,
  JSLeftY,
  NUM_JOYSTICK_AXES
} ps2plus_controller_joystick_axis;

typedef enum {
  JSAxisRange_LeftXMin,
  JSAxisRange_LeftXCenter,
  JSAxisRange_LeftXMax,
  JSAxisRange_LeftYMin,
  JSAxisRange_LeftYCenter,
  JSAxisRange_LeftYMax,
  JSAxisRange_RightXMin,
  JSAxisRange_RightXCenter,
  JSAxisRange_RightXMax,
  JSAxisRange_RightYMin,
  JSAxisRange_RightYCenter,
  JSAxisRange_RightYMax,
  NUM_JOYSTICK_AXIS_RANGES,
} ps2plus_controller_joystick_axis_range;

const char *ps2plus_controller_digital_button_name(ps2plus_controller_digital_button button);
const char *ps2plus_controller_joystick_axis_name(ps2plus_controller_joystick_axis axis);
const char *ps2plus_controller_joystick_axis_range_name(ps2plus_controller_joystick_axis_range axis_range);

#ifndef _IOP
ps2plus_controller_digital_button ps2plus_controller_digital_button_from_name(const char *name);
#endif

#endif /* SHARED_CONTROLLER_H */