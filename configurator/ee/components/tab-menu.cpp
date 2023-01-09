#include "tab-menu.h"
#include "ui/fonts/playstation.h"
#include "ui/gfx.h"

#include <map>
#include <string>

namespace PS2Plus::Components {
    // Constant that indicates that L1/R1 buttons are not currently being used to navigate between tabs
    const int TAB_MENU_NO_NEXT_TAB = -1;

    struct TabMenuInfo {
        int tabCount;
        int activeTabIndex;
        int nextTabIndex;
    };

    std::map<std::string, TabMenuInfo> allTabMenus;
    TabMenuInfo *currentTabMenu = nullptr;

    bool BeginTabMenu(const char *key) {
        // Find or create the tab information structure for this key
        auto result = allTabMenus.find(key);
        if (result != allTabMenus.end()) {
            currentTabMenu = &result->second;
        } else {
            allTabMenus[key] = { .nextTabIndex = 0 };
            currentTabMenu = &allTabMenus[key];
        }

        // At the beginning of each frame, reset the tab count so that it can be automatically counted
        currentTabMenu->tabCount = 0;

        bool ret = ImGui::BeginTabBar("Sections");
        if (ret) {
            // Indicate disabled tabs
            ImGui::PushStyleColor(ImGuiCol_Tab, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_Tab) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabActive, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabActive) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabHovered, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabHovered) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabUnfocused, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocused) : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_TabUnfocusedActive) : ImVec4(0, 0, 0, 0));
        }

        return ret;
    }

    void EndTabMenu() {
        // Handle L1/R1 and calculations of which tab will be focused in the next frame
        {
            // Reset the "next tab" value from the previous frame (if any)
            currentTabMenu->nextTabIndex = TAB_MENU_NO_NEXT_TAB;

            // Control buttons on the ends have transparent backgrounds
            ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Text, PS2Plus::Graphics::IsGamepadNavActive() ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            ImGui::BeginDisabled(!PS2Plus::Graphics::IsGamepadNavActive());

            // Determine whether the user clicked the L1/R1 buttons in the tab bar
            bool move_previous = ImGui::TabItemButton(ICON_PLAYSTATION_L1_BUTTON_ALT, ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip);
            bool move_next = ImGui::TabItemButton(ICON_PLAYSTATION_R1_BUTTON_ALT, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip);

            // Determine whether the user clicked the L1/R1 buttons on their gamepad
            if (PS2Plus::Graphics::IsGamepadNavActive()) {
                move_previous = move_previous || ImGui::IsKeyPressed(ImGuiKey_GamepadL1);
                move_next = move_next || ImGui::IsKeyPressed(ImGuiKey_GamepadR1);
            }

            // If L1/R1 was pressed, calculate which tab should receive focus in the next frame
            if (move_previous) {
                currentTabMenu->nextTabIndex = (currentTabMenu->activeTabIndex > 0) 
                    ? currentTabMenu->activeTabIndex - 1 
                    : currentTabMenu->tabCount - 1;
            } else if (move_next) {
                currentTabMenu->nextTabIndex = (currentTabMenu->activeTabIndex < currentTabMenu->tabCount - 1) 
                    ? currentTabMenu->activeTabIndex + 1 
                    : 0;
            }

            ImGui::EndDisabled();
            ImGui::PopStyleColor(/* ImGuiCol_Text */);
            ImGui::PopStyleColor(/* ImGuiCol_Tab */);
        }

        ImGui::PopStyleColor(5 /* ImGuiCol_Tab/TabActive/TabHovered/TabUnfocused/TabUnfocusedActive */);
        ImGui::EndTabBar();
    }

    bool BeginTabMenuItem(const char *label) {
        // Create a new tab, and auto-select it if it was selected via L1/R1 in the previous frame
        int tabIndex = currentTabMenu->tabCount;
        bool ret = ImGui::BeginTabItem(label, NULL, currentTabMenu->nextTabIndex == tabIndex ? ImGuiTabItemFlags_SetSelected : 0);
        if (ret) {
            // Track this tab (currently being shown) to assist L1/R1 calculations
            currentTabMenu->activeTabIndex = tabIndex;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
        }

        currentTabMenu->tabCount += 1;
        return ret;
    }

    void EndTabMenuItem() {
        ImGui::PopStyleVar(/* ImGuiStyleVar_FramePadding */);
        ImGui::EndTabItem();
    }


    void TabMenu(const char *key, std::function<void()> code) {
        if (BeginTabMenu(key)) {
            code();
            EndTabMenu();
        }
    }

    void TabMenuItem(const char *label, std::function<void()> code) {
        if (BeginTabMenuItem(label)) {
            code();
            EndTabMenuItem();
        }
    }
}