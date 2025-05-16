#include "EngineUI.h"
#include <iostream>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include "Node.h"
#include "../nodes/Node2D/Node2D.h"
#include "../nodes/Sprite/Sprite.h"

EngineUI::EngineUI()
{
    // Initialize node counters
    for (int i = 0; i < static_cast<int>(NodeType::Panel) + 1; i++)
    {
        nodeCounters[static_cast<NodeType>(i)] = 0;
    }

    InitializeSceneHierarchy();
}

EngineUI::~EngineUI() {}

void EngineUI::InitializeSceneHierarchy()
{
    // Initialize node counters
    for (int i = 0; i < static_cast<int>(NodeType::Panel) + 1; i++)
    {
        nodeCounters[static_cast<NodeType>(i)] = 0;
    }

    // Create root node
    rootNode = std::make_shared<Node>(GenerateUniqueName(NodeType::Root), NodeType::Root);
    rootNode->expanded = true;

    // Add some example nodes with unique names
    AddChildNode(rootNode, NodeType::Node2D);
    auto node2D = rootNode->children.back();

    AddChildNode(node2D, NodeType::CharacterBody2D);
    auto characterBody = node2D->children.back();

    AddChildNode(characterBody, NodeType::Sprite);

    AddChildNode(rootNode, NodeType::Camera);

    // Create a UI node with custom name
    auto ui = std::make_shared<Node2D>("UI", NodeType::Node2D);
    nodeCounters[NodeType::Node2D]++; // Manually increment for this custom-named node
    rootNode->children.push_back(ui);

    AddChildNode(ui, NodeType::Label);
    AddChildNode(ui, NodeType::Button);
}

bool EngineUI::Init()
{
    // Load fonts and setup theme
    if (!LoadFonts())
    {
        std::cerr << "Failed to load fonts" << std::endl;
        return false;
    }

    SetupTheme();

    // Initialize documentation manager
    docManager.Init();

    return true;
}

void EngineUI::HandleDocumentationKeyPress(int key, int scancode, int action, int mods)
{
    docManager.HandleKeyPress(key, scancode, action, mods);

    // Handle gizmo operation shortcuts
    if (action == 1) // Key press
    {
        switch (key)
        {
        case 81: // Q key - Select
            currentGizmoOp = GizmoOperation::Select;
            break;

        case 87: // W key - Translate
            currentGizmoOp = GizmoOperation::Translate;
            break;

        case 69: // E key - Rotate
            currentGizmoOp = GizmoOperation::Rotate;
            break;

        case 82: // R key - Scale
            currentGizmoOp = GizmoOperation::Scale;
            break;
        }
    }
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

    // Load Font Awesome for icons
    static const ImWchar icons_ranges[] = {0xf000, 0xf999, 0}; // Font Awesome 5 ranges
    ImFontConfig icons_config;
    icons_config.MergeMode = false;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = 14.0f;
    icons_config.GlyphOffset = ImVec2(0, 0);

    std::string iconFontPath = "fonts/FontAwesome/fa-solid-900.ttf";
    iconFont = io.Fonts->AddFontFromFileTTF(iconFontPath.c_str(), 14.0f, &icons_config, icons_ranges);
    if (iconFont == nullptr)
    {
        std::cerr << "Failed to load icon font" << std::endl;
        // Continue anyway, we'll just have no icons
    }

    // Setup icon mappings
    SetupIcons();

    return true;
}

void EngineUI::SetupIcons()
{
    // Map node types to Font Awesome 5 icons with more appropriate choices
    // These are Unicode values for Font Awesome 5 icons
    nodeIcons[NodeType::Root] = "\xef\x81\x80";            // fa-sitemap
    nodeIcons[NodeType::Node2D] = "\xef\x81\x88";          // fa-square-o (empty square representing a 2D node)
    nodeIcons[NodeType::Sprite] = "\xef\x83\x87";          // fa-image
    nodeIcons[NodeType::CharacterBody2D] = "\xef\x86\x8e"; // fa-user
    nodeIcons[NodeType::Camera] = "\xef\x80\xbd";          // fa-camera
    nodeIcons[NodeType::Light] = "\xef\x83\x85";           // fa-lightbulb
    nodeIcons[NodeType::Label] = "\xef\x81\x9e";           // fa-font
    nodeIcons[NodeType::Button] = "\xef\x81\x95";          // fa-hand-pointer-o
    nodeIcons[NodeType::Panel] = "\xef\x84\xa6";           // fa-window-maximize
}

const char *EngineUI::GetNodeIcon(NodeType type)
{
    auto it = nodeIcons.find(type);
    if (it != nodeIcons.end())
    {
        return it->second;
    }
    return "\xef\x81\x90"; // fa-question-circle as fallback
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

    // Create the three-panel layout with resizable columns
    ImGui::SetCursorPos(contentPos);

    // Left panel (Scene Hierarchy)
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, contentSize.y), true);
    RenderScenePanel();
    ImGui::EndChild();

    // Left panel resize handle
    ImGui::SameLine();
    ImGui::SetCursorPosX(contentPos.x + leftPanelWidth);
    ImGui::InvisibleButton("LeftResizer", ImVec2(5.0f, contentSize.y));
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    if (ImGui::IsItemActive() || resizingLeftPanel)
    {
        resizingLeftPanel = ImGui::IsMouseDown(ImGuiMouseButton_Left);
        if (resizingLeftPanel)
        {
            float newWidth = ImGui::GetIO().MousePos.x - contentPos.x;
            // Clamp to reasonable values
            if (newWidth > 100.0f && newWidth < contentSize.x - rightPanelWidth - 100.0f)
                leftPanelWidth = newWidth;
        }
    }

    // Middle panel (Editor)
    ImGui::SameLine();
    ImGui::BeginChild("MiddlePanel", ImVec2(contentSize.x - leftPanelWidth - rightPanelWidth - 10.0f, contentSize.y), true);
    RenderEditorPanel();
    ImGui::EndChild();

    // Right panel resize handle
    ImGui::SameLine();
    ImGui::SetCursorPosX(contentPos.x + contentSize.x - rightPanelWidth - 5.0f);
    ImGui::InvisibleButton("RightResizer", ImVec2(5.0f, contentSize.y));
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    if (ImGui::IsItemActive() || resizingRightPanel)
    {
        resizingRightPanel = ImGui::IsMouseDown(ImGuiMouseButton_Left);
        if (resizingRightPanel)
        {
            float newWidth = contentPos.x + contentSize.x - ImGui::GetIO().MousePos.x;
            // Clamp to reasonable values
            if (newWidth > 100.0f && newWidth < contentSize.x - leftPanelWidth - 100.0f)
                rightPanelWidth = newWidth;
        }
    }

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

    // Render documentation if visible
    docManager.Render();

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
            if (ImGui::MenuItem("Documentation", "F1"))
            {
                docManager.ShowDocumentation();
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

    // Add node button
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60);
    if (ImGui::Button("+ Node"))
    {
        ImGui::OpenPopup("AddNodePopup");
    }

    if (ImGui::BeginPopup("AddNodePopup"))
    {
        // Get available node types from Node class
        std::vector<NodeType> nodeTypes = Node::GetAvailableNodeTypes();

        // Display menu items for each node type
        for (const auto &type : nodeTypes)
        {
            // Get name based on type
            std::string name;
            switch (type)
            {
            case NodeType::Node2D:
                name = "Node2D";
                break;
            case NodeType::Sprite:
                name = "Sprite";
                break;
            case NodeType::CharacterBody2D:
                name = "CharacterBody2D";
                break;
            case NodeType::Camera:
                name = "Camera";
                break;
            case NodeType::Light:
                name = "Light";
                break;
            case NodeType::Label:
                name = "Label";
                break;
            case NodeType::Button:
                name = "Button";
                break;
            case NodeType::Panel:
                name = "Panel";
                break;
            default:
                name = "Unknown";
                break;
            }

            if (ImGui::MenuItem(name.c_str()))
                AddChildNode(rootNode, type);
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();

    // Render the scene hierarchy
    if (rootNode)
    {
        RenderSceneNode(rootNode);
    }
}

void EngineUI::RenderSceneNode(std::shared_ptr<Node> node)
{
    if (!node)
        return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    // Add selection flag if this node is selected
    if (node->selected)
        flags |= ImGuiTreeNodeFlags_Selected;

    // If node has no children, make it a leaf node
    if (node->children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    // Use node's expanded state
    if (node->expanded)
        flags |= ImGuiTreeNodeFlags_DefaultOpen;

    // Push icon font for the icon
    ImGui::PushFont(iconFont);
    std::string icon = GetNodeIcon(node->type);
    ImGui::Text("%s", icon.c_str());
    ImGui::PopFont();

    ImGui::SameLine();

    // Display the node with its name
    bool nodeOpen = ImGui::TreeNodeEx(node->name.c_str(), flags);

    // Handle selection
    if (ImGui::IsItemClicked())
    {
        // Deselect previous node
        if (selectedNode)
            selectedNode->selected = false;

        // Select this node
        node->selected = true;
        selectedNode = node;
    }

    // Context menu for node operations
    if (ImGui::BeginPopupContextItem())
    {
        RenderNodeContextMenu(node);
        ImGui::EndPopup();
    }

    // Update expanded state
    node->expanded = nodeOpen;

    // Render children if node is open
    if (nodeOpen)
    {
        for (auto &child : node->children)
        {
            RenderSceneNode(child);
        }

        ImGui::TreePop();
    }
}

void EngineUI::RenderNodeContextMenu(std::shared_ptr<Node> node)
{
    if (ImGui::MenuItem("Add Child Node"))
    {
        ImGui::OpenPopup("AddChildNodePopup");
    }

    if (ImGui::BeginPopup("AddChildNodePopup"))
    {
        // Get available node types from Node class
        std::vector<NodeType> nodeTypes = Node::GetAvailableNodeTypes();

        // Display menu items for each node type
        for (const auto &type : nodeTypes)
        {
            // Get name based on type
            std::string name;
            switch (type)
            {
            case NodeType::Node2D:
                name = "Node2D";
                break;
            case NodeType::Sprite:
                name = "Sprite";
                break;
            case NodeType::CharacterBody2D:
                name = "CharacterBody2D";
                break;
            case NodeType::Camera:
                name = "Camera";
                break;
            case NodeType::Light:
                name = "Light";
                break;
            case NodeType::Label:
                name = "Label";
                break;
            case NodeType::Button:
                name = "Button";
                break;
            case NodeType::Panel:
                name = "Panel";
                break;
            default:
                name = "Unknown";
                break;
            }

            if (ImGui::MenuItem(name.c_str()))
                AddChildNode(node, type);
        }
        ImGui::EndPopup();
    }

    if (ImGui::MenuItem("Delete Node"))
    {
        // TODO: Implement node deletion
    }

    if (ImGui::MenuItem("Rename"))
    {
        // TODO: Implement node renaming
    }
}

// Generate a unique name for a node based on its type
std::string EngineUI::GenerateUniqueName(NodeType type)
{
    // Increment the counter for this node type
    nodeCounters[type]++;

    // Base name for each node type
    std::string baseName;
    switch (type)
    {
    case NodeType::Root:
        baseName = "Scene";
        break;
    case NodeType::Node2D:
        baseName = "Node2D";
        break;
    case NodeType::Sprite:
        baseName = "Sprite";
        break;
    case NodeType::CharacterBody2D:
        baseName = "CharacterBody2D";
        break;
    case NodeType::Camera:
        baseName = "Camera2D";
        break;
    case NodeType::Light:
        baseName = "Light2D";
        break;
    case NodeType::Label:
        baseName = "Label";
        break;
    case NodeType::Button:
        baseName = "Button";
        break;
    case NodeType::Panel:
        baseName = "Panel";
        break;
    default:
        baseName = "Node";
        break;
    }

    // For the first node of a type, don't add a number
    if (nodeCounters[type] == 1)
    {
        return baseName;
    }

    // For subsequent nodes, add the counter number
    return baseName + std::to_string(nodeCounters[type]);
}

void EngineUI::AddChildNode(std::shared_ptr<Node> parent, NodeType type)
{
    if (!parent)
        return;

    // Generate a unique name for the node
    std::string uniqueName = GenerateUniqueName(type);

    // Create a new node with the unique name based on type
    std::shared_ptr<Node> newNode;

    switch (type)
    {
    case NodeType::Node2D:
        newNode = std::make_shared<Node2D>(uniqueName);
        break;
    case NodeType::Sprite:
        newNode = std::make_shared<Sprite>(uniqueName);
        break;
    default:
        newNode = std::make_shared<Node>(uniqueName, type);
        break;
    }

    if (newNode)
    {
        // Add it to the parent's children
        parent->children.push_back(newNode);

        // Expand the parent to show the new child
        parent->expanded = true;
    }
}

std::shared_ptr<Node> EngineUI::CreateNodeOfType(NodeType type, const std::string &name)
{
    // Create node based on type
    switch (type)
    {
    case NodeType::Node2D:
        return std::make_shared<Node2D>(name);
    case NodeType::Sprite:
        return std::make_shared<Sprite>(name);
    default:
        return std::make_shared<Node>(name, type);
    }
}

void EngineUI::RenderEditorPanel()
{
    ImGui::PushFont(titleFont);
    ImGui::Text("Editor");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100);
    RenderModeToggle();
    ImGui::PopFont();

    ImGui::Separator();

    // Render the appropriate editor based on the current mode
    if (is3DMode)
    {
        Render3DEditor();
    }
    else
    {
        Render2DEditor();
    }
}

void EngineUI::RenderModeToggle()
{
    // Create a toggle button for 2D/3D mode
    ImGui::PushStyleColor(ImGuiCol_Button, is3DMode ? ImVec4(0.2f, 0.4f, 0.8f, 0.8f) : ImVec4(0.8f, 0.2f, 0.2f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, is3DMode ? ImVec4(0.3f, 0.5f, 0.9f, 1.0f) : ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, is3DMode ? ImVec4(0.4f, 0.6f, 1.0f, 1.0f) : ImVec4(1.0f, 0.4f, 0.4f, 1.0f));

    if (ImGui::Button(is3DMode ? "3D Mode" : "2D Mode", ImVec2(100, 0)))
    {
        ToggleEditorMode();
    }

    ImGui::PopStyleColor(3);

    // Show tooltip on hover
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Click to switch to %s mode", is3DMode ? "2D" : "3D");
        ImGui::EndTooltip();
    }
}

void EngineUI::ToggleEditorMode()
{
    is3DMode = !is3DMode;
    // Additional mode switching logic could be added here
}

void EngineUI::Render2DEditor()
{
    // Editor viewport
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    ImGui::Text("2D Viewport Size: %.0f x %.0f", viewportSize.x, viewportSize.y);

    // Draw a placeholder for the viewport
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetCursorScreenPos(),
        ImVec2(ImGui::GetCursorScreenPos().x + viewportSize.x, ImGui::GetCursorScreenPos().y + viewportSize.y),
        IM_COL32(50, 50, 50, 255));

    // Draw a grid in the viewport
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 startPos = ImGui::GetCursorScreenPos();

    // Add gizmo controls on top of the editor
    RenderGizmoControls();

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

    // Render nodes in the editor
    if (rootNode)
    {
        for (auto &node : rootNode->children)
        {
            RenderNodeInEditor(node, false);
        }
    }

    // Render gizmos for the selected node
    if (selectedNode)
    {
        RenderGizmos(startPos, viewportSize);
    }

    // Make the viewport area interactive
    ImGui::InvisibleButton("viewport2d", viewportSize);

    // Handle node selection when clicking in the viewport
    if (ImGui::IsItemClicked())
    {
        HandleNodeSelection(ImGui::GetIO().MousePos);
    }
}

void EngineUI::Render3DEditor()
{
    // Editor viewport
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    ImGui::Text("3D Viewport Size: %.0f x %.0f", viewportSize.x, viewportSize.y);

    // Draw a placeholder for the viewport
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetCursorScreenPos(),
        ImVec2(ImGui::GetCursorScreenPos().x + viewportSize.x, ImGui::GetCursorScreenPos().y + viewportSize.y),
        IM_COL32(30, 30, 40, 255));

    // Draw a 3D grid in the viewport
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 startPos = ImGui::GetCursorScreenPos();

    // Add gizmo controls on top of the editor
    RenderGizmoControls();

    // Grid settings
    float gridSize = 30.0f;
    ImU32 gridColor = IM_COL32(80, 80, 100, 40);

    // Center of the viewport
    float centerX = startPos.x + viewportSize.x / 2;
    float centerY = startPos.y + viewportSize.y / 2;

    // Draw 3D grid (perspective effect)
    // Horizontal lines with perspective
    for (int i = -10; i <= 10; i++)
    {
        float y = centerY + i * gridSize;
        float perspectiveScale = 0.7f + 0.3f * (float)(10 - abs(i)) / 10.0f;
        float xStart = centerX - (viewportSize.x / 2) * perspectiveScale;
        float xEnd = centerX + (viewportSize.x / 2) * perspectiveScale;

        drawList->AddLine(
            ImVec2(xStart, y),
            ImVec2(xEnd, y),
            gridColor);
    }

    // Vertical lines with perspective
    for (int i = -15; i <= 15; i++)
    {
        float x = centerX + i * gridSize;
        float perspectiveScale = 0.7f + 0.3f * (float)(15 - abs(i)) / 15.0f;
        float yStart = centerY - (viewportSize.y / 2) * perspectiveScale;
        float yEnd = centerY + (viewportSize.y / 2) * perspectiveScale;

        drawList->AddLine(
            ImVec2(x, yStart),
            ImVec2(x, yEnd),
            gridColor);
    }

    // Draw 3D axes
    // X axis (red)
    drawList->AddLine(
        ImVec2(centerX, centerY),
        ImVec2(centerX + viewportSize.x / 3, centerY),
        IM_COL32(255, 50, 50, 200), 2.0f);

    // Y axis (green)
    drawList->AddLine(
        ImVec2(centerX, centerY),
        ImVec2(centerX, centerY - viewportSize.y / 3),
        IM_COL32(50, 255, 50, 200), 2.0f);

    // Z axis (blue) - coming toward the viewer
    drawList->AddLine(
        ImVec2(centerX, centerY),
        ImVec2(centerX - viewportSize.x / 6, centerY + viewportSize.y / 6),
        IM_COL32(50, 50, 255, 200), 2.0f);

    // Draw axis labels directly with the draw list instead of using ImGui::Text
    // This avoids issues with cursor positioning
    ImGui::GetWindowDrawList()->AddText(
        ImVec2(centerX + viewportSize.x / 3 + 5, centerY),
        IM_COL32(255, 50, 50, 255), "X");

    ImGui::GetWindowDrawList()->AddText(
        ImVec2(centerX, centerY - viewportSize.y / 3 - 15),
        IM_COL32(50, 255, 50, 255), "Y");

    ImGui::GetWindowDrawList()->AddText(
        ImVec2(centerX - viewportSize.x / 6 - 15, centerY + viewportSize.y / 6 + 5),
        IM_COL32(50, 50, 255, 255), "Z");

    // Make the viewport area interactive
    ImGui::InvisibleButton("viewport3d", viewportSize);
}

void EngineUI::RenderInspectorPanel()
{
    ImGui::PushFont(titleFont);
    ImGui::Text("Inspector");
    ImGui::PopFont();
    ImGui::Separator();

    // If no node is selected, show a message
    if (!selectedNode)
    {
        ImGui::TextWrapped("Select a node to edit its properties");
        return;
    }

    // Show node type with icon
    ImGui::PushFont(iconFont);
    ImGui::Text("%s", GetNodeIcon(selectedNode->type));
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::Text("%s (%s)", selectedNode->name.c_str(),
                selectedNode->GetTypeName().c_str());

    ImGui::Separator();

    // Let the node render its own properties
    selectedNode->RenderInspectorProperties();
}

// Render gizmo control buttons
void EngineUI::RenderGizmoControls()
{
    // Define button size and spacing
    float buttonSize = 30.0f;
    ImVec2 buttonDim(buttonSize, buttonSize);

    // Create a small window for the gizmo controls at the top of the editor
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    ImVec2 windowPos = ImGui::GetCursorScreenPos();
    ImVec2 controlsPos = ImVec2(windowPos.x + viewportSize.x / 2 - (buttonSize * 4 + 12) / 2, windowPos.y + 5);

    ImGui::SetCursorScreenPos(controlsPos);

    // Background for the controls
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(controlsPos.x - 5, controlsPos.y - 5),
        ImVec2(controlsPos.x + buttonSize * 4 + 15, controlsPos.y + buttonSize + 5),
        IM_COL32(40, 40, 40, 200), 5.0f);

    // Select button
    bool isSelectActive = currentGizmoOp == GizmoOperation::Select;
    if (isSelectActive)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));

    ImGui::PushFont(iconFont);
    if (ImGui::Button("\xef\x80\xad", buttonDim)) // fa-mouse-pointer
    {
        currentGizmoOp = GizmoOperation::Select;
    }
    ImGui::PopFont();

    if (isSelectActive)
        ImGui::PopStyleColor();
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Select (Q)");

    // Translate button
    ImGui::SameLine();
    bool isTranslateActive = currentGizmoOp == GizmoOperation::Translate;
    if (isTranslateActive)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));

    ImGui::PushFont(iconFont);
    if (ImGui::Button("\xef\x81\x95", buttonDim)) // fa-arrows
    {
        currentGizmoOp = GizmoOperation::Translate;
    }
    ImGui::PopFont();

    if (isTranslateActive)
        ImGui::PopStyleColor();
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Translate (W)");

    // Rotate button
    ImGui::SameLine();
    bool isRotateActive = currentGizmoOp == GizmoOperation::Rotate;
    if (isRotateActive)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));

    ImGui::PushFont(iconFont);
    if (ImGui::Button("\xef\x81\x82", buttonDim)) // fa-refresh
    {
        currentGizmoOp = GizmoOperation::Rotate;
    }
    ImGui::PopFont();

    if (isRotateActive)
        ImGui::PopStyleColor();
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Rotate (E)");

    // Scale button
    ImGui::SameLine();
    bool isScaleActive = currentGizmoOp == GizmoOperation::Scale;
    if (isScaleActive)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));

    ImGui::PushFont(iconFont);
    if (ImGui::Button("\xef\x83\x91", buttonDim)) // fa-expand
    {
        currentGizmoOp = GizmoOperation::Scale;
    }
    ImGui::PopFont();

    if (isScaleActive)
        ImGui::PopStyleColor();
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Scale (R)");

    // Reset cursor position for the rest of the content
    ImGui::SetCursorScreenPos(windowPos);
}

// Render a node in the editor
void EngineUI::RenderNodeInEditor(std::shared_ptr<Node> node, bool is3D,
                                  float parentX, float parentY,
                                  float parentRotation,
                                  float parentScaleX, float parentScaleY)
{
    if (!node)
        return;

    // Skip rendering for certain node types
    if (node->type == NodeType::Root)
    {
        // Recursively render children
        for (auto &child : node->children)
        {
            RenderNodeInEditor(child, is3D, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        }
        return;
    }

    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();

    // Calculate node position in viewport
    float centerX = startPos.x + viewportSize.x / 2;
    float centerY = startPos.y + viewportSize.y / 2;

    // Apply parent transform to this node's position
    float worldX = parentX + node->transform.position[0];
    float worldY = parentY + node->transform.position[1];

    // Apply node transform (position is relative to center)
    float nodeX = centerX + worldX;
    float nodeY = centerY + worldY;

    // Node visual representation based on type
    ImU32 nodeColor = node->selected ? IM_COL32(255, 255, 0, 255) : IM_COL32(200, 200, 200, 255);
    float nodeSize = 10.0f;

    switch (node->type)
    {
    case NodeType::Node2D:
        // Draw a square
        drawList->AddRect(
            ImVec2(nodeX - nodeSize, nodeY - nodeSize),
            ImVec2(nodeX + nodeSize, nodeY + nodeSize),
            nodeColor, 0.0f, 0, 2.0f);
        break;

    case NodeType::Sprite:
        // Draw a filled square
        drawList->AddRectFilled(
            ImVec2(nodeX - nodeSize, nodeY - nodeSize),
            ImVec2(nodeX + nodeSize, nodeY + nodeSize),
            nodeColor);
        break;

    case NodeType::CharacterBody2D:
        // Draw a circle
        drawList->AddCircle(ImVec2(nodeX, nodeY), nodeSize, nodeColor, 0, 2.0f);
        break;

    case NodeType::Camera:
        // Draw a camera icon
        drawList->AddTriangleFilled(
            ImVec2(nodeX - nodeSize, nodeY - nodeSize),
            ImVec2(nodeX + nodeSize, nodeY),
            ImVec2(nodeX - nodeSize, nodeY + nodeSize),
            nodeColor);
        break;

    default:
        // Draw a circle for other types
        drawList->AddCircle(ImVec2(nodeX, nodeY), nodeSize, nodeColor, 0, 2.0f);
        break;
    }

    // Draw node name if selected
    if (node->selected)
    {
        drawList->AddText(ImVec2(nodeX + nodeSize + 5, nodeY - 10),
                          IM_COL32(255, 255, 255, 255), node->name.c_str());
    }

    // Calculate combined transform to pass to children
    float combinedX = worldX;
    float combinedY = worldY;
    float combinedRotation = parentRotation + node->transform.rotation[2];
    float combinedScaleX = parentScaleX * node->transform.scale[0];
    float combinedScaleY = parentScaleY * node->transform.scale[1];

    // Recursively render children with combined transform
    for (auto &child : node->children)
    {
        RenderNodeInEditor(child, is3D, combinedX, combinedY, combinedRotation, combinedScaleX, combinedScaleY);
    }
}

// Handle node selection in the editor
void EngineUI::HandleNodeSelection(ImVec2 mousePos)
{
    // Deselect current node
    if (selectedNode)
    {
        selectedNode->selected = false;
    }

    // Find node under mouse cursor
    std::shared_ptr<Node> clickedNode = nullptr;
    Node::Transform emptyTransform; // Default transform with no offset
    FindNodeAtPosition(rootNode, mousePos, clickedNode, emptyTransform);

    // Select the node
    if (clickedNode)
    {
        clickedNode->selected = true;
        selectedNode = clickedNode;
    }
}

// Helper function to find a node at a specific position
void EngineUI::FindNodeAtPosition(std::shared_ptr<Node> node, ImVec2 pos, std::shared_ptr<Node> &result,
                                  Node::Transform parentTransform)
{
    if (!node)
        return;

    // Skip root node
    if (node->type != NodeType::Root)
    {
        ImVec2 startPos = ImGui::GetCursorScreenPos();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        // Calculate node position in viewport
        float centerX = startPos.x + viewportSize.x / 2;
        float centerY = startPos.y + viewportSize.y / 2;

        // Apply parent position to this node's position
        float worldX = parentTransform.position[0] + node->transform.position[0];
        float worldY = parentTransform.position[1] + node->transform.position[1];

        // Apply node transform (position is relative to center)
        float nodeX = centerX + worldX;
        float nodeY = centerY + worldY;

        // Increase the hit area for better selection
        float nodeSize = 15.0f; // Increased from 10.0f for better selection

        // Adjust hit area based on node type
        switch (node->type)
        {
        case NodeType::Sprite:
            nodeSize = 20.0f; // Larger hit area for sprites
            break;
        case NodeType::CharacterBody2D:
            nodeSize = 20.0f; // Larger hit area for character bodies
            break;
        default:
            break;
        }

        // Apply scale to the hit area
        float worldScaleX = parentTransform.scale[0] * node->transform.scale[0];
        nodeSize *= worldScaleX; // Use X scale for simplicity

        // Check if mouse is over this node
        if (pos.x >= nodeX - nodeSize && pos.x <= nodeX + nodeSize &&
            pos.y >= nodeY - nodeSize && pos.y <= nodeY + nodeSize)
        {
            result = node;
            return;
        }

        // Create combined transform to pass to children
        Node::Transform combinedTransform;
        combinedTransform.position[0] = worldX;
        combinedTransform.position[1] = worldY;
        combinedTransform.rotation[2] = parentTransform.rotation[2] + node->transform.rotation[2];
        combinedTransform.scale[0] = worldScaleX;
        combinedTransform.scale[1] = parentTransform.scale[1] * node->transform.scale[1];

        // Pass the combined transform to children
        parentTransform = combinedTransform;
    }

    // Check children in reverse order (to select top-most node first)
    for (auto it = node->children.rbegin(); it != node->children.rend(); ++it)
    {
        FindNodeAtPosition(*it, pos, result, parentTransform);
        if (result)
            return;
    }
}

// Helper function to calculate a node's world position
void EngineUI::CalculateNodeWorldTransform(std::shared_ptr<Node> node, float &outWorldX, float &outWorldY)
{
    if (!node)
    {
        outWorldX = 0.0f;
        outWorldY = 0.0f;
        return;
    }

    // Start with the node's local position
    outWorldX = node->transform.position[0];
    outWorldY = node->transform.position[1];

    // Traverse up the parent hierarchy to calculate world position
    std::shared_ptr<Node> parent = nullptr;

    // Find the parent by searching through all nodes
    if (node != rootNode) // Skip if this is the root node
    {
        std::function<bool(std::shared_ptr<Node>, std::shared_ptr<Node>, std::shared_ptr<Node> &)> findParent;
        findParent = [&findParent](std::shared_ptr<Node> current, std::shared_ptr<Node> target, std::shared_ptr<Node> &result) -> bool
        {
            for (auto &child : current->children)
            {
                if (child == target)
                {
                    result = current;
                    return true;
                }
                if (findParent(child, target, result))
                    return true;
            }
            return false;
        };

        findParent(rootNode, node, parent);
    }

    // If we found a parent, add its world position
    if (parent)
    {
        float parentX, parentY;
        CalculateNodeWorldTransform(parent, parentX, parentY);
        outWorldX += parentX;
        outWorldY += parentY;
    }
}

// Render gizmos for the selected node
void EngineUI::RenderGizmos(ImVec2 startPos, ImVec2 viewportSize)
{
    if (!selectedNode)
        return;

    ImDrawList *drawList = ImGui::GetWindowDrawList();

    // Calculate node position in viewport
    float centerX = startPos.x + viewportSize.x / 2;
    float centerY = startPos.y + viewportSize.y / 2;

    // Calculate world position of the selected node
    float worldX, worldY;
    CalculateNodeWorldTransform(selectedNode, worldX, worldY);

    // Apply world transform to viewport coordinates
    float nodeX = centerX + worldX;
    float nodeY = centerY + worldY;

    // Gizmo colors
    ImU32 xAxisColor = IM_COL32(255, 0, 0, 255);
    ImU32 yAxisColor = IM_COL32(0, 255, 0, 255);
    ImU32 zAxisColor = IM_COL32(0, 0, 255, 255);

    // Gizmo size
    float gizmoSize = 50.0f;

    // Handle gizmo interaction
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    static bool isDraggingGizmo = false;
    static ImVec2 lastMousePos;
    static int activeAxis = -1; // 0 = X, 1 = Y, -1 = none

    // Check if mouse is over gizmo handles
    bool isMouseOverGizmo = false;

    switch (currentGizmoOp)
    {
    case GizmoOperation::Select:
        // Draw selection box
        drawList->AddRect(
            ImVec2(nodeX - 15, nodeY - 15),
            ImVec2(nodeX + 15, nodeY + 15),
            IM_COL32(255, 255, 0, 255), 0.0f, 0, 1.0f);
        break;

    case GizmoOperation::Translate:
        // Draw translation arrows
        // X-axis (right)
        drawList->AddLine(ImVec2(nodeX, nodeY), ImVec2(nodeX + gizmoSize, nodeY), xAxisColor, 2.0f);
        drawList->AddTriangleFilled(
            ImVec2(nodeX + gizmoSize, nodeY),
            ImVec2(nodeX + gizmoSize - 10, nodeY - 5),
            ImVec2(nodeX + gizmoSize - 10, nodeY + 5),
            xAxisColor);

        // Y-axis (up)
        drawList->AddLine(ImVec2(nodeX, nodeY), ImVec2(nodeX, nodeY - gizmoSize), yAxisColor, 2.0f);
        drawList->AddTriangleFilled(
            ImVec2(nodeX, nodeY - gizmoSize),
            ImVec2(nodeX - 5, nodeY - gizmoSize + 10),
            ImVec2(nodeX + 5, nodeY - gizmoSize + 10),
            yAxisColor);

        // Check if mouse is over X-axis handle
        if (mousePos.x >= nodeX && mousePos.x <= nodeX + gizmoSize &&
            mousePos.y >= nodeY - 5 && mousePos.y <= nodeY + 5)
        {
            isMouseOverGizmo = true;
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                isDraggingGizmo = true;
                lastMousePos = mousePos;
                activeAxis = 0; // X-axis
            }
        }
        // Check if mouse is over Y-axis handle
        else if (mousePos.x >= nodeX - 5 && mousePos.x <= nodeX + 5 &&
                 mousePos.y >= nodeY - gizmoSize && mousePos.y <= nodeY)
        {
            isMouseOverGizmo = true;
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                isDraggingGizmo = true;
                lastMousePos = mousePos;
                activeAxis = 1; // Y-axis
            }
        }
        break;

    case GizmoOperation::Rotate:
    {
        // Draw rotation circle
        drawList->AddCircle(ImVec2(nodeX, nodeY), gizmoSize / 2, IM_COL32(255, 255, 0, 255), 32, 2.0f);

        // Check if mouse is near the rotation circle
        float distToCenter = sqrt(pow(mousePos.x - nodeX, 2) + pow(mousePos.y - nodeY, 2));
        if (distToCenter >= gizmoSize / 2 - 5 && distToCenter <= gizmoSize / 2 + 5)
        {
            isMouseOverGizmo = true;
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                isDraggingGizmo = true;
                lastMousePos = mousePos;
                activeAxis = 2; // Rotation
            }
        }
        break;
    }

    case GizmoOperation::Scale:
        // Draw scale handles
        // X-axis
        drawList->AddLine(ImVec2(nodeX, nodeY), ImVec2(nodeX + gizmoSize, nodeY), xAxisColor, 2.0f);
        drawList->AddRectFilled(
            ImVec2(nodeX + gizmoSize - 5, nodeY - 5),
            ImVec2(nodeX + gizmoSize + 5, nodeY + 5),
            xAxisColor);

        // Y-axis
        drawList->AddLine(ImVec2(nodeX, nodeY), ImVec2(nodeX, nodeY - gizmoSize), yAxisColor, 2.0f);
        drawList->AddRectFilled(
            ImVec2(nodeX - 5, nodeY - gizmoSize - 5),
            ImVec2(nodeX + 5, nodeY - gizmoSize + 5),
            yAxisColor);

        // Check if mouse is over X-axis scale handle
        if (mousePos.x >= nodeX + gizmoSize - 5 && mousePos.x <= nodeX + gizmoSize + 5 &&
            mousePos.y >= nodeY - 5 && mousePos.y <= nodeY + 5)
        {
            isMouseOverGizmo = true;
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                isDraggingGizmo = true;
                lastMousePos = mousePos;
                activeAxis = 0; // X-axis scale
            }
        }
        // Check if mouse is over Y-axis scale handle
        else if (mousePos.x >= nodeX - 5 && mousePos.x <= nodeX + 5 &&
                 mousePos.y >= nodeY - gizmoSize - 5 && mousePos.y <= nodeY - gizmoSize + 5)
        {
            isMouseOverGizmo = true;
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                isDraggingGizmo = true;
                lastMousePos = mousePos;
                activeAxis = 1; // Y-axis scale
            }
        }
        break;
    }

    // Set cursor based on gizmo operation
    if (isMouseOverGizmo)
    {
        if (currentGizmoOp == GizmoOperation::Translate)
        {
            ImGui::SetMouseCursor(activeAxis == 0 ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS);
        }
        else if (currentGizmoOp == GizmoOperation::Rotate)
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
        }
        else if (currentGizmoOp == GizmoOperation::Scale)
        {
            ImGui::SetMouseCursor(activeAxis == 0 ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS);
        }
    }

    // Handle dragging
    if (isDraggingGizmo)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            ImVec2 mouseDelta = ImVec2(mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y);

            switch (currentGizmoOp)
            {
            case GizmoOperation::Translate:
                if (activeAxis == 0) // X-axis
                {
                    // Apply translation only to the selected node
                    selectedNode->transform.position[0] += mouseDelta.x;
                }
                else if (activeAxis == 1) // Y-axis
                {
                    // Apply translation only to the selected node
                    selectedNode->transform.position[1] += mouseDelta.y; // Y should move up when dragging up
                }
                break;

            case GizmoOperation::Rotate:
                if (activeAxis == 2) // Rotation
                {
                    // Calculate angle change based on mouse movement relative to node center
                    float prevAngle = atan2(lastMousePos.y - nodeY, lastMousePos.x - nodeX);
                    float newAngle = atan2(mousePos.y - nodeY, mousePos.x - nodeX);
                    float angleDelta = (newAngle - prevAngle) * 180.0f / 3.14159f; // Convert to degrees

                    // Apply rotation only to the selected node
                    selectedNode->transform.rotation[2] += angleDelta;
                }
                break;

            case GizmoOperation::Scale:
                if (activeAxis == 0) // X-axis scale
                {
                    float scaleFactor = 1.0f + mouseDelta.x / 100.0f;

                    // Apply X scale only to the selected node
                    selectedNode->transform.scale[0] *= scaleFactor;
                }
                else if (activeAxis == 1) // Y-axis scale
                {
                    float scaleFactor = 1.0f - mouseDelta.y / 100.0f; // Subtract because Y is inverted

                    // Apply Y scale only to the selected node
                    selectedNode->transform.scale[1] *= scaleFactor;
                }
                break;
            }

            lastMousePos = mousePos;
        }
        else
        {
            // Mouse released, stop dragging
            isDraggingGizmo = false;
            activeAxis = -1;
        }
    }
}