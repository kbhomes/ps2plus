#pragma once

#include "app/controller-configuration.h"
#include "components/modal-dialog.h"

#include <imgui/imgui.h>

namespace PS2Plus::App::Views::Configuration {
class ResetDefaultsDialog : public PS2Plus::Components::ModalDialog<void> {
public:
  ResetDefaultsDialog(PS2Plus::App::Configuration& config) : ModalDialog("Reset Configuration"), config_(config) {}

protected:
  virtual ImVec2 GetPopupSize();
  virtual ImGuiWindowFlags GetPopupFlags();
  virtual void RenderContents();

private:
  void ResetConfiguration();

  PS2Plus::App::Configuration& config_;
};
} // namespace PS2Plus::App::Views::Configuration