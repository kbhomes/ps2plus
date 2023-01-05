#include <imgui/imgui.h>
#include <functional>

// Tab menu widget, currently designed to be a top-level menu interface with
// buttons and L1/R1 gamepad support to move between the tabs.
namespace PS2Plus::UI {
    bool BeginTabMenu(const char *key);
    void EndTabMenu();
    bool BeginTabMenuItem(const char *label);
    void EndTabMenuItem();

    /**
     * Functional interface around the TabMenu's Begin/End methods. Equivalent to:
     *   if (PS2Plus::UI::BeginTabMenu(key)) { code(); PS2Plus::UI::EndTabMenu(); }
     */
    void TabMenu(const char *key, std::function<void()> code);

    /**
     * Functional interface around the TabMenu's Begin/End methods. Equivalent to:
     *   if (PS2Plus::UI::BeginTabMenuItem(label)) { code(); PS2Plus::UI::EndTabMenuItem(); }
     */
    void TabMenuItem(const char *label, std::function<void()> code);
}