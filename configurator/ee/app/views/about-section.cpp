#include "app.h"

#include "components/loading-progress.h"
#include "components/status.h"
#include "ui/fonts/playstation.h"

#include <math.h>

namespace PS2Plus::App::Views {
// Helper to generate the controlled "Video Mode" combo selector
void VideoModeComboBox() {
  PS2Plus::App::State& state = PS2Plus::App::GetState();
  PS2Plus::Graphics::VideoMode current_mode = state.video_mode();

  const char *mode_display = PS2Plus::Graphics::VideoModeStrings[current_mode];
  if (ImGui::BeginCombo("Video Mode", mode_display)) {
    for (auto const& entry : PS2Plus::Graphics::VideoModeStrings) {
      const bool is_selected = (entry.first == current_mode);
      if (ImGui::Selectable(entry.second, is_selected)) {
        state.set_video_mode(entry.first);
      }

      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
}

void AboutSection() {
  VideoModeComboBox();
  ImGui::Separator();
  PS2Plus::Components::StatusText("Firmware updated!", PS2Plus::Components::kStatusSuccess);
  PS2Plus::Components::StatusText("Update taking longer than expected", PS2Plus::Components::kStatusWarning);
  PS2Plus::Components::StatusText("Error occurred during update", PS2Plus::Components::kStatusError);
  PS2Plus::Components::StatusText("Rebooting controller", PS2Plus::Components::kStatusLoading);
  PS2Plus::Components::LoadingProgress("##Progress", fmodf(ImGui::GetTime(), 5.f) / 5.f, "%0.f%%");
  ImGui::Separator();
  ImGui::ShowStyleEditor();
}
} // namespace PS2Plus::App::Views