#pragma once

#include "app/controller-configuration.h"
#include "components/modal-dialog.h"

#include <imgui/imgui.h>
#include <variant>

namespace PS2Plus::App::Views::Configuration {
enum OptionsDialogResult {
  kOptionsResultSave,
  kOptionsResultReload,
  kOptionsResultResetDefaults,
  kOptionsResultImport,
  kOptionsResultExport,
};

class OptionsDialog : public PS2Plus::Components::ModalDialog<OptionsDialogResult> {
public:
  OptionsDialog(PS2Plus::App::Configuration& config) : ModalDialog("Options##Configuration"), config_(config) {}

protected:
  virtual ImVec2 GetPopupSize();
  virtual ImGuiWindowFlags GetPopupFlags();
  virtual std::optional<OptionsDialogResult> RenderContents();

private:
  PS2Plus::App::Configuration& config_;
};
} // namespace PS2Plus::App::Views::Configuration