#include "EngineUI.h"
#include <iostream>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>

EngineUI::EngineUI() {}

EngineUI::~EngineUI() {}

bool EngineUI::Init()
{
    // Load fonts and setup theme
    if (!LoadFonts())
    {
        std::cerr << "Failed to load fonts" << std::endl;
        return false;
    }

    SetupTheme();
    return true;
}

bool EngineUI::LoadFonts()
{
    ImGuiIO &io = ImGui::GetIO();

    // Load Nunito Sans font
    std::string fontPath = "fonts/Nunito_Sans/NunitoSans-VariableFont_YTLC,opsz,wdth,wght.ttf";

    // Regular font (14px)
    mainFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 14.0f);
    if (mainFont == nullptr)
    {
        std::cerr << "Failed to load main font" << std::endl;
        return false;
    }

    // Bold font (15px) - using a slightly larger size to simulate bold
    boldFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 15.0f);

    // Title font (18px)
    titleFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 18.0f);

    return true;
}

void EngineUI::SetupTheme()
{
    // Godot-like dark theme
    ImGuiStyle &style = ImGui::GetStyle();

    // Colors based on Godot's dark theme
    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.08f, 0.50f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.37f, 0.37f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.39f, 0.39f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);

    // Adjust style
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.TabRounding = 3.0f;
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(5, 3);
    style.ItemSpacing = ImVec2(6, 6);
    style.ItemInnerSpacing = ImVec2(3, 3);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 10.0f;
}

void EngineUI::Render()
{
    // Set the main font for the UI
    ImGui::PushFont(mainFont);

    // Begin the main dockspace
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    // Render the top bar
    RenderTopBar();

    // Calculate content area (below top bar)
    float topBarHeight = ImGui::GetFrameHeight();
    ImVec2 contentPos = ImVec2(0, topBarHeight);
    ImVec2 contentSize = ImVec2(viewport->Size.x, viewport->Size.y - topBarHeight);

    // Create the three-panel layout
    ImGui::SetCursorPos(contentPos);

    // Left panel (Scene Hierarchy)
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, contentSize.y), true);
    RenderScenePanel();
    ImGui::EndChild();

    // Middle panel (Editor)
    ImGui::SameLine();
    ImGui::BeginChild("MiddlePanel", ImVec2(contentSize.x - leftPanelWidth - rightPanelWidth, contentSize.y), true);
    RenderEditorPanel();
    ImGui::EndChild();

    // Right panel (Inspector)
    ImGui::SameLine();
    ImGui::BeginChild("RightPanel", ImVec2(rightPanelWidth, contentSize.y), true);
    RenderInspectorPanel();
    ImGui::EndChild();

    ImGui::End(); // DockSpace

    // Show ImGui demo window if enabled
    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ImGui::PopFont();
}

void EngineUI::RenderTopBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene", "Ctrl+N"))
            {
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
            {
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
            {
            }
            if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S"))
            {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y"))
            {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X"))
            {
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C"))
            {
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Toggle ImGui Demo", nullptr, &showDemoWindow))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("Project Settings"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Documentation"))
            {
            }
            if (ImGui::MenuItem("About"))
            {
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void EngineUI::RenderScenePanel()
{
    ImGui::PushFont(titleFont);
    ImGui::Text("Scene");
    ImGui::PopFont();
    ImGui::Separator();

    // Scene hierarchy tree
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (ImGui::TreeNodeEx("Root", flags))
    {
        if (ImGui::TreeNodeEx("Sprite", flags))
        {
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Camera", flags))
        {
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Light", flags))
        {
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void EngineUI::RenderEditorPanel()
{
    ImGui::PushFont(titleFont);
    ImGui::Text("Editor");
    ImGui::PopFont();
    ImGui::Separator();

    // Editor viewport
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    ImGui::Text("Viewport Size: %.0f x %.0f", viewportSize.x, viewportSize.y);

    // Draw a placeholder for the viewport
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetCursorScreenPos(),
        ImVec2(ImGui::GetCursorScreenPos().x + viewportSize.x, ImGui::GetCursorScreenPos().y + viewportSize.y),
        IM_COL32(50, 50, 50, 255));

    // Draw a grid in the viewport
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 startPos = ImGui::GetCursorScreenPos();

    // Grid settings
    float gridSize = 20.0f;
    ImU32 gridColor = IM_COL32(100, 100, 100, 40);

    // Draw horizontal grid lines
    for (float y = 0; y < viewportSize.y; y += gridSize)
    {
        drawList->AddLine(
            ImVec2(startPos.x, startPos.y + y),
            ImVec2(startPos.x + viewportSize.x, startPos.y + y),
            gridColor);
    }

    // Draw vertical grid lines
    for (float x = 0; x < viewportSize.x; x += gridSize)
    {
        drawList->AddLine(
            ImVec2(startPos.x + x, startPos.y),
            ImVec2(startPos.x + x, startPos.y + viewportSize.y),
            gridColor);
    }

    // Draw origin lines (x and y axes)
    float centerX = startPos.x + viewportSize.x / 2;
    float centerY = startPos.y + viewportSize.y / 2;

    drawList->AddLine(
        ImVec2(centerX, startPos.y),
        ImVec2(centerX, startPos.y + viewportSize.y),
        IM_COL32(255, 0, 0, 100), 2.0f);

    drawList->AddLine(
        ImVec2(startPos.x, centerY),
        ImVec2(startPos.x + viewportSize.x, centerY),
        IM_COL32(0, 255, 0, 100), 2.0f);

    // Make the viewport area interactive
    ImGui::InvisibleButton("viewport", viewportSize);
}

void EngineUI::RenderInspectorPanel()
{
    ImGui::PushFont(titleFont);
    ImGui::Text("Inspector");
    ImGui::PopFont();
    ImGui::Separator();

    // Properties section
    ImGui::PushFont(boldFont);
    ImGui::Text("Transform");
    ImGui::PopFont();

    // Position
    static float position[3] = {0.0f, 0.0f, 0.0f};
    ImGui::Text("Position");
    ImGui::SameLine(100);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat3("##Position", position);
    ImGui::PopItemWidth();

    // Rotation
    static float rotation[3] = {0.0f, 0.0f, 0.0f};
    ImGui::Text("Rotation");
    ImGui::SameLine(100);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat3("##Rotation", rotation);
    ImGui::PopItemWidth();

    // Scale
    static float scale[3] = {1.0f, 1.0f, 1.0f};
    ImGui::Text("Scale");
    ImGui::SameLine(100);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat3("##Scale", scale);
    ImGui::PopItemWidth();

    ImGui::Separator();

    // Material section
    ImGui::PushFont(boldFont);
    ImGui::Text("Material");
    ImGui::PopFont();

    // Color
    static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    ImGui::Text("Color");
    ImGui::SameLine(100);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::ColorEdit4("##Color", color);
    ImGui::PopItemWidth();

    // Texture
    ImGui::Text("Texture");
    ImGui::SameLine(100);
    ImGui::Button("Select Texture", ImVec2(ImGui::GetContentRegionAvail().x, 0));

    ImGui::Separator();

    // Add Component button
    if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
    {
        // Component menu would appear here
    }
}