#include <imgui/imgui.h>

namespace PS2Plus::UI {
    bool BeginWizard(const char *key, int stepCount, int *currentStep = nullptr);
    bool BeginWizardStepList();
    void EndWizardStepList();
    void SetupWizardStep(const char *name, bool enabled);
    bool BeginWizardStep(const char *name);
    void EndWizardStep();
    void EndWizard();
    void WizardPrevious();
    void WizardNext();
    void WizardGoToStep(const char *name);
}