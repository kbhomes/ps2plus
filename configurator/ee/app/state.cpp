#include "state.h"

namespace PS2Plus::App {
State state_;

State& GetState() { return state_; }

State::State() {
  for (int i = 0; i < 2; i++) {
    controllers_.push_back(new Controller(i));
  }
}

void State::UpdateGamepads() {
  current_gamepad_ = PS2Plus::Gamepad::Read();
  gamepad_summary_ = PS2Plus::Gamepad::ReadSummary();
}

void State::SelectController(int port) { current_controller_ = controllers_[port]; }

void State::MockController(int port) { controllers_[port]->MockData(); }

void State::set_video_mode(PS2Plus::Graphics::VideoMode mode) {
  if (video_mode_ != mode) {
    PS2Plus::Graphics::SetVideoMode(mode);
  }

  video_mode_ = mode;
}
} // namespace PS2Plus::App