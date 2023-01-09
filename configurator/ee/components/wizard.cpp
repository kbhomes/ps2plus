#include "wizard.h"

#include <map>
#include <string>
#include <vector>

namespace PS2Plus::Components {
    struct StepInfo {
        bool enabled;
        int index;
    };

    struct WizardInfo {
        int stepCount;
        int currentStep;
        int setupStepIndex;
        std::map<std::string, StepInfo> steps;
    };

    std::map<std::string, WizardInfo *> information;
    WizardInfo *wizard = nullptr;

    bool BeginWizard(const char *key, int stepCount, int *currentStep) {
        auto result = information.find(key);
        if (result != information.end()) {
            wizard = result->second;
            wizard->stepCount = stepCount;
        } else {
            wizard = new WizardInfo();
            wizard->stepCount = stepCount;
            wizard->currentStep = (currentStep ? *currentStep : 0);
            information.insert({ key, wizard });
        }

        *currentStep = wizard->currentStep;
        wizard->setupStepIndex = 0;
        wizard->steps.clear();

        ImGui::PushID(key);

        return true;
    }

    bool BeginWizardStepList() {
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
        bool value = ImGui::BeginTable("StepList", wizard->stepCount, ImGuiTableFlags_BordersOuter);
        ImGui::TableNextRow();
        return value;
    }

    void EndWizardStepList() {
        ImGui::EndTable();
        ImGui::PopStyleVar();
    }

    void SetupWizardStep(const char *name, bool enabled) {
        int index = wizard->setupStepIndex++;
        wizard->steps.insert({ name, { .enabled = enabled, .index = index } });
        
        ImGui::TableNextColumn();
        
        const ImVec2 p = ImGui::GetCursorScreenPos();
        float arrowSize = 10;
        float stepHeight = 30;
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::Dummy(ImVec2(1, stepHeight));
        ImGui::SetCursorScreenPos(p);
        ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_TableBorderStrong);
        ImU32 activeColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
        
        // Shade in previous/current steps
        if (index <= wizard->currentStep) {
            if (index < wizard->currentStep || index + 1 == wizard->stepCount) {
                // Previous steps and final step are simple rectangles
                drawList->AddRectFilled(p, ImVec2(p.x + size.x, p.y + stepHeight), activeColor);
            } else {
                // Current, non-final steps have the arrow
                std::vector<ImVec2> stepPoints;
                stepPoints.push_back(p);
                stepPoints.push_back(ImVec2(p.x + size.x - arrowSize, p.y));
                stepPoints.push_back(ImVec2(p.x + size.x, p.y + stepHeight / 2));
                stepPoints.push_back(ImVec2(p.x + size.x - arrowSize, p.y + stepHeight));
                stepPoints.push_back(ImVec2(p.x, p.y + stepHeight));
                drawList->AddConvexPolyFilled(stepPoints.data(), stepPoints.size(), activeColor);
            }
        }

        // Only draw the arrow if this isn't the last component
        if (index + 1 != wizard->stepCount) {
            drawList->AddLine(ImVec2(p.x + size.x, p.y + stepHeight / 2), ImVec2(p.x + size.x - arrowSize, p.y), borderColor);
            drawList->AddLine(ImVec2(p.x + size.x, p.y + stepHeight / 2), ImVec2(p.x + size.x - arrowSize, p.y + stepHeight), borderColor);
        }

        ImGui::SetCursorScreenPos(ImVec2(p.x + 8, p.y + 8));
        ImGui::Text(name);
    }

    bool BeginWizardStep(const char *name) {
        auto result = wizard->steps.find(name);
        if (result != wizard->steps.end()) {
            if (wizard->currentStep == result->second.index) {
                // ImGui::BeginChild(name, ImVec2(), true, ImGuiWindowFlags_NavFlattened);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    void EndWizardStep() {
        // ImGui::EndChild();
    }

    void EndWizard() {
        wizard = nullptr;
        ImGui::PopID();
    }

    void WizardPrevious() {
        if (wizard->currentStep != 0) {
            wizard->currentStep--;
        }
    }

    void WizardNext() {
        if (wizard->currentStep + 1 != wizard->stepCount) {
            wizard->currentStep++;
        }
    }

    void WizardGoToStep(const char *name) {
        auto result = wizard->steps.find(name);
        if (result != wizard->steps.end()) {
            wizard->currentStep = result->second.index;
        }
    }

}