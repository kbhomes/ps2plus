#pragma once

#include "app/controller-configuration.h"
#include "components/modal-dialog.h"

#include <imgui/imgui.h>
#include <vector>

namespace PS2Plus::App::Views::Configuration {
class ExportDialog : public PS2Plus::Components::ModalDialog<std::string> {
public:
  ExportDialog(PS2Plus::App::Configuration& config) : ModalDialog("Export Configuration"), config_(config), selected_path_{} {}

protected:
  virtual ImVec2 GetPopupSize();
  virtual ImGuiWindowFlags GetPopupFlags();
  virtual std::optional<std::string> RenderContents();

private:
  PS2Plus::App::Configuration& config_;
  char selected_path_[255];
};
} // namespace PS2Plus::App::Views::Configuration