#pragma once

#include <imgui/imgui.h>
#include <optional>
#include <string>
#include <type_traits>

namespace PS2Plus::Components {
template <typename T> class ModalDialog {
// Conditional type for special handling of `void` return types
// If T is `void`, `Render` and `RenderContents` will have return type of `void`
// Otherwise, `Render` and `RenderContents` will have return type of `std::optional<T>`
using TReturn = typename std::conditional<std::is_void<T>::value, void, std::optional<T>>::type;

public:
  ModalDialog(std::string id) : id_(id){};

  bool IsOpen() { return ImGui::IsPopupOpen(id_.c_str()); }

  void Open() { ImGui::OpenPopup(id_.c_str()); }

  TReturn Render(ImGuiWindowFlags flags = ImGuiWindowFlags_None) {
    ImGui::SetNextWindowSize(GetPopupSize());
    ImGui::SetNextWindowPos(GetPopupPosCenter(), 0, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(id_.c_str(), NULL, flags | GetPopupFlags())) {
      if constexpr (std::is_void<TReturn>::value) {
        RenderContents();
        ImGui::EndPopup();
      } else {
        auto result = RenderContents();
        ImGui::EndPopup();
        return result;
      }
    } else {
      return TReturn();
    }
  }

protected:
  virtual ImVec2 GetPopupSize() { return ImVec2(0, 0); }
  virtual ImVec2 GetPopupPosCenter() { return ImGui::GetMainViewport()->GetCenter(); }
  virtual ImGuiWindowFlags GetPopupFlags() { return ImGuiWindowFlags_None; }
  virtual TReturn RenderContents() = 0;

private:
  std::string id_;
};
} // namespace PS2Plus::Components