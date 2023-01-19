#include "footer-command-menu.h"

#include "ui/gfx.h"

#include <map>
#include <stack>

namespace PS2Plus::Components {
struct FooterCommandMenu_ {
  FooterCommandMenuId id;
  bool has_previous_command = false;
  bool is_disabled = false;
};

std::map<FooterCommandMenuId, FooterCommandMenu_> menus;
std::stack<FooterCommandMenu_ *> menu_stack;

FooterCommandMenu_* GetMenu(FooterCommandMenuId id) {
  if (menus.find(id) != menus.end()) {
    menus[id] = { .id = id };
  }

  return &menus[id];
}

void BeginFooterCommandMenu(FooterCommandMenuId id, FooterCommandMenuCond cond) {
  // Push this menu onto the menu stack
  FooterCommandMenu_* menu = GetMenu(id);
  menu_stack.push(menu);

  // Reset footer state for this new footer
  menu->has_previous_command = false;

  // Determine whether the menu is disabled
  switch (cond) {
    case kFooterCommandMenuEnabledWithGlobalNav:
      menu->is_disabled = !PS2Plus::Graphics::IsGamepadGlobalNavActive();
      break;

    case kFooterCommandMenuDisabled:
      menu->is_disabled = true;
      break;

    case kFooterCommandMenuEnabledAlways:
    default:
      menu->is_disabled = false;
      break;
  }

  // Begin footer rendering
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.f); // Slightly move footer down due to icon heights
  ImGui::BeginDisabled(menu->is_disabled);
}

void EndFooterCommandMenu() {
  // End footer rendering
  ImGui::EndDisabled();

  // Pop the menu off the stack
  menu_stack.pop();
}

bool FooterCommand(const char *label, const char *icon, ImColor icon_color, ImGuiKey key) {
  // Get the menu that this command is a part of
  FooterCommandMenu_ *menu = menu_stack.top();

  // SameLine() on all but the first command
  if (menu->has_previous_command) {
    ImGui::SameLine();
  } else {
    menu->has_previous_command = true;
  }

  ImGui::TextColored(menu->is_disabled ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : (const ImVec4)icon_color, icon);
  ImGui::SameLine();
  ImGui::Text(label);
  ImGui::SameLine();
  ImGui::Text(" ");

  return !menu->is_disabled && ImGui::IsKeyPressed(key);
}
} // namespace PS2Plus::Components