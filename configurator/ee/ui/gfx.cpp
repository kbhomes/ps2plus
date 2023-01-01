#include "gfx.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_ps2sdk.h>
#include <imgui/backends/imgui_impl_ps2gskit.h>

#include "fonts/forkawesome.h"
#include "fonts/playstation.h"
#include "drawing/drawing.h"
#include "widgets/widget.h"
#include "../util/resource.h"

static GSGLOBAL *gsGlobal;
static ImFont *fontPlayStationLarge;

// External font resources
RESOURCE_EXTERNS(fonts_cousine_regular_ttf);
RESOURCE_EXTERNS(fonts_forkawesome_subset_ttf);
RESOURCE_EXTERNS(fonts_playstation_ttf);

void initializeImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGui::StyleColorsClassic();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.MousePos = ImVec2(0, 0);

    // NOTE: Adding fonts increases the font texture atlas size. At some point, if the font texture atlas
    // exceeds 512x512, the GSKit ImGui backend fails to render at all, leading to a black screen. To avoid
    // this, oversampling can be disabled on fonts, and fonts should have glyph ranges minimized to only
    // characters that are used.
    // See: https://github.com/ocornut/imgui/blob/master/docs/FONTS.md

    // Add the default font
    {
        ImFontConfig config;
        config.OversampleH = config.OversampleV = 1;
        io.Fonts->AddFontFromMemoryTTF(RESOURCE_POINTER(fonts_cousine_regular_ttf), RESOURCE_SIZE(fonts_cousine_regular_ttf), 18.f, &config);
    }

    // Add the PlayStation font
    {
        ImFontConfig config;
        config.OversampleH = config.OversampleV = 1;
        config.MergeMode = true;
        config.GlyphMinAdvanceX = 18.0f; // Use if you want to make the icon monospaced
        config.GlyphOffset = ImVec2(0.f, 3.f);
        static const ImWchar icon_ranges[] = { ICON_PLAYSTATION_GLYPH_MIN, ICON_PLAYSTATION_GLYPH_MAX, 0 };
        io.Fonts->AddFontFromMemoryTTF(RESOURCE_POINTER(fonts_playstation_ttf), RESOURCE_SIZE(fonts_playstation_ttf), 20.f, &config, icon_ranges);
    }

    // Add the ForkAwesome font
    {
        ImFontConfig config;
        config.OversampleH = config.OversampleV = 1;
        config.MergeMode = true;
        config.GlyphMinAdvanceX = 36.0f; // Use if you want to make the icon monospaced
        config.GlyphOffset = ImVec2(0.f, 3.f);
        static const ImWchar icon_ranges[] = { ICON_FK_GLYPH_MIN, ICON_FK_GLYPH_MAX, 0 };
        io.Fonts->AddFontFromMemoryTTF(RESOURCE_POINTER(fonts_forkawesome_subset_ttf), RESOURCE_SIZE(fonts_forkawesome_subset_ttf), 18.f, &config, icon_ranges);
    }

    // Add the PlayStation font in large format (this is a new font)
    {
        ImFontConfig config;
        config.OversampleH = config.OversampleV = 1;
        static const ImWchar icon_ranges[] = { ICON_PLAYSTATION_GLYPH_MIN, ICON_PLAYSTATION_GLYPH_MAX, 0 };
        fontPlayStationLarge = io.Fonts->AddFontFromMemoryTTF(RESOURCE_POINTER(fonts_playstation_ttf), RESOURCE_SIZE(fonts_playstation_ttf), 28.f, &config, icon_ranges);
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedLinesUseTex = false;
    style.CellPadding = ImVec2(6, 6);
    style.ItemSpacing = ImVec2(6, 6);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.FrameRounding = 4;
    style.FramePadding = ImVec2(10, 2);
    style.TabRounding = 2;
    style.ScrollbarSize = 20;
    style.GrabMinSize = 10;
    style.GrabRounding = 2;
    style.WindowBorderSize = 0;
    style.WindowRounding = 2;
    style.WindowPadding = ImVec2(6, 6);
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    style.TouchExtraPadding = ImVec2(8, 8);
    style.MouseCursorScale = 0.8;
    style.SelectableTextAlign = ImVec2(0, 0.5);
    style.Colors[ImGuiCol_Tab].w = 0.2f;

    // Setup ImGui backends
    ImGui_ImplPs2Sdk_InitForGsKit(gsGlobal);
    ImGui_ImplPs2GsKit_Init(gsGlobal);
}

void PS2Plus::Graphics::Initialize(PS2Plus::Graphics::VideoMode mode) {
    gsGlobal = gsKit_init_global();
    PS2Plus::Graphics::SetVideoMode(mode);
    initializeImGui();
}

void PS2Plus::Graphics::BeginFrame(uint64_t color) {
    // Clear the frame
    gsGlobal->PrimAlphaEnable = GS_SETTING_OFF;
    gsKit_clear(gsGlobal, color);
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

    // Start the Dear ImGui frame
    ImGui_ImplPs2Sdk_NewFrame();
    ImGui_ImplPs2GsKit_NewFrame();
    ImGui::NewFrame();
}

void PS2Plus::Graphics::EndFrame() {
    // Draw our custom mouse cursor for this frame; see `widgets/widget_cursor.cpp` for 
    // examples on how to draw a custom cursor depending on the cursor type. Must be 
    // called at the end of the frame so ImGui has time to update the cursor type.
    PS2Plus::UI::MouseCursor();
    ImGui::Render();

    ImGui_ImplPs2GsKit_RenderDrawData(ImGui::GetDrawData(), ImVec2(-0.5f, -0.5f));
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
    gsKit_TexManager_nextFrame(gsGlobal);
}

void PS2Plus::Graphics::UpdateGamepad(const PS2Plus::Gamepad::PadSummary& pad) {
    ImGui_ImplPs2Sdk_UpdateGamepad(pad.status == PS2Plus::Gamepad::PadSummaryActive ? &pad.pad : NULL);
}

ImFont* PS2Plus::Graphics::GetFontPlayStationLarge() {
    return fontPlayStationLarge;
}

void PS2Plus::Graphics::PauseGamepadNav() {
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
    ImGui::GetIO().MouseDrawCursor = false;
}

void PS2Plus::Graphics::ResumeGamepadNav() {
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

bool PS2Plus::Graphics::IsGamepadNavActive() {
    return ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_NavEnableGamepad;
}

void PS2Plus::Graphics::SetVideoMode(PS2Plus::Graphics::VideoMode mode) {
    if (mode == Interlaced) {
        gsGlobal->Mode = GS_MODE_NTSC;
        gsGlobal->Interlace = GS_INTERLACED;
        gsGlobal->Field = GS_FIELD;
        gsGlobal->Width = 640;
        gsGlobal->Height = 448;
    } else if (mode == Progressive480) {
        gsGlobal->Mode = GS_MODE_DTV_480P;
        gsGlobal->Interlace = GS_NONINTERLACED;
        gsGlobal->Field = GS_FRAME;
        gsGlobal->Width = 704;
        gsGlobal->Height = 480;
    }

    if ((gsGlobal->Interlace == GS_INTERLACED) && (gsGlobal->Field == GS_FRAME)) {
        gsGlobal->Height /= 2;
    }

    // Setup GS global settings
    gsGlobal->PSM = GS_PSM_CT32;
    gsGlobal->PSMZ = GS_PSMZ_16S;
    gsGlobal->Dithering = GS_SETTING_ON;
    gsGlobal->DoubleBuffering = GS_SETTING_ON;
    gsGlobal->ZBuffering = GS_SETTING_ON;
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    gsKit_set_test(gsGlobal, GS_ZTEST_ON);
    gsKit_set_test(gsGlobal, GS_ATEST_ON);
    gsKit_set_primalpha(gsGlobal, GS_SETREG_ALPHA(0, 1, 0, 1, 128), 0);
    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP);

    // Initialize DMA settings
    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsKit_init_screen(gsGlobal);
    gsKit_vram_clear(gsGlobal);
    gsKit_TexManager_init(gsGlobal);
}