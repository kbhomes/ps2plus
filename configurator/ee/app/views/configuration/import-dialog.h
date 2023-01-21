#pragma once

#include "app/controller-configuration.h"
#include "components/modal-dialog.h"

#include <imgui/imgui.h>
#include <vector>

namespace PS2Plus::App::Views::Configuration {
enum ImportDialogState {
  kImportDialogSelectingPath,
  kImportDialogSuccessfulRead,
  kImportDialogFailedRead,
  kImportDialogSuccessfulFinished,
  kImportDialogFailedFinished,
  kImportDialogCancelledFinished
};

class ImportDialog : public PS2Plus::Components::ModalDialog<void> {
public:
  ImportDialog(PS2Plus::App::Configuration& target_config)
      : ModalDialog("Import Configuration"), state_(kImportDialogSelectingPath), selected_path_{}, target_config_(target_config) {}

protected:
  virtual ImVec2 GetPopupSize();
  virtual ImGuiWindowFlags GetPopupFlags();
  virtual void RenderContents();

private:
  void RenderState_SelectingPath();
  void RenderState_SuccessfulRead();
  void RenderState_FailedRead();

  ImportDialogState state_;
  char selected_path_[255];
  PS2Plus::App::Configuration& target_config_;
};
} // namespace PS2Plus::App::Views::Configuration