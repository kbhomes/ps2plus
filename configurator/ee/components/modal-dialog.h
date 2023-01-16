#pragma once

#include <imgui/imgui.h>
#include <optional>
#include <string>

namespace PS2Plus::Components {
template <typename T> class ModalDialog {
public:
  ModalDialog(std::string id) : id_(id){};

  bool IsOpen() { return ImGui::IsPopupOpen(id_.c_str()); }

  void Open() { ImGui::OpenPopup(id_.c_str()); }

  std::optional<T> Render(ImGuiWindowFlags flags = ImGuiWindowFlags_None) {
    ImGui::SetNextWindowSize(GetPopupSize());
    ImGui::SetNextWindowPos(GetPopupPosCenter(), 0, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(id_.c_str(), NULL, flags | GetPopupFlags())) {
      auto result = RenderContents();
      ImGui::EndPopup();
      return result;
    }else {
      return {};
    }
  }

protected:
  virtual ImVec2 GetPopupSize() { return ImVec2(0, 0); }
  virtual ImVec2 GetPopupPosCenter() { return ImGui::GetMainViewport()->GetCenter(); }
  virtual ImGuiWindowFlags GetPopupFlags() { return ImGuiWindowFlags_None; }
  virtual std::optional<T> RenderContents() = 0;

private:
  std::string id_;
};
} // namespace PS2Plus::Components