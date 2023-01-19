#pragma once

#include "app/controller-configuration.h"
#include "components/modal-dialog.h"

#include <imgui/imgui.h>
#include <vector>

namespace PS2Plus::App::Views::Configuration {
enum ExportDialogState { kExportDialogSelectingPath, kExportDialogSuccessfulWrite, kExportDialogFailedWrite, kExportDialogFinished };

class ExportDialog : public PS2Plus::Components::ModalDialog<bool> {
public:
  ExportDialog(PS2Plus::App::Configuration& config)
      : ModalDialog("Export Configuration"), state_(kExportDialogSelectingPath), config_(config), selected_path_{} {}

protected:
  virtual ImVec2 GetPopupSize();
  virtual ImGuiWindowFlags GetPopupFlags();
  virtual std::optional<bool> RenderContents();

private:
  void RenderState_SelectingPath();
  void RenderState_SuccessfulWrite();
  void RenderState_FailedWrite();

  PS2Plus::App::Configuration& config_;
  ExportDialogState state_;
  char selected_folder_[255];
  std::string selected_path_;
};
} // namespace PS2Plus::App::Views::Configuration