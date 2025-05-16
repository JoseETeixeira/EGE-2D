#pragma once

#include <string>
#include <vector>
#include <memory>
#include "imgui.h"

class EngineUI
{
public:
    EngineUI();
    ~EngineUI();

    // Initialize the UI
    bool Init();

    // Render the UI
    void Render();

private:
    // UI components
    void RenderTopBar();
    void RenderScenePanel();
    void RenderEditorPanel();
    void RenderInspectorPanel();

    // Font handling
    bool LoadFonts();
    ImFont *mainFont = nullptr;
    ImFont *boldFont = nullptr;
    ImFont *titleFont = nullptr;

    // UI state
    float leftPanelWidth = 250.0f;
    float rightPanelWidth = 300.0f;
    bool showDemoWindow = false;
    bool is3DMode = false; // Default to 2D mode

    // Mode switching
    void ToggleEditorMode();
    void RenderModeToggle();
    void Render2DEditor();
    void Render3DEditor();

    // Theme colors
    void SetupTheme();
};