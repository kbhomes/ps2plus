#pragma once

#include <imgui/imgui.h>
#include <string>

namespace PS2Plus::Components {
  using FooterCommandMenuId = const char *;
  enum FooterCommandMenuCond {
    kFooterCommandMenuDisabled,
    kFooterCommandMenuEnabledWithGlobalNav,
    kFooterCommandMenuEnabledAlways,
  };
  
  void BeginFooterCommandMenu(FooterCommandMenuId id, FooterCommandMenuCond cond = kFooterCommandMenuEnabledAlways);
  void EndFooterCommandMenu();
  bool FooterCommand(const char *label, const char *icon, ImColor icon_color, ImGuiKey key);
}