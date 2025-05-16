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

    // Theme colors
    void SetupTheme();
};