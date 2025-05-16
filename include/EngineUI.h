#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include "imgui.h"
#include "DocumentationManager.h"

// Forward declaration
class Node;

class EngineUI
{
public:
    EngineUI();
    ~EngineUI();

    // Initialize the UI
    bool Init();

    // Render the UI
    void Render();

    // Documentation
    void HandleDocumentationKeyPress(int key, int scancode, int action, int mods);

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
    ImFont *iconFont = nullptr;

    // Icon mapping
    std::map<NodeType, const char *> nodeIcons;
    void SetupIcons();
    const char *GetNodeIcon(NodeType type);

    // Node counters for unique naming
    std::map<NodeType, int> nodeCounters;
    std::string GenerateUniqueName(NodeType type);

    // UI state
    float leftPanelWidth = 250.0f;
    float rightPanelWidth = 300.0f;
    bool showDemoWindow = false;
    bool is3DMode = false; // Default to 2D mode
    bool resizingLeftPanel = false;
    bool resizingRightPanel = false;

    // Gizmo state
    enum class GizmoOperation
    {
        Select,
        Translate,
        Rotate,
        Scale
    };
    GizmoOperation currentGizmoOp = GizmoOperation::Select;

    // Scene hierarchy
    std::shared_ptr<Node> rootNode;
    std::shared_ptr<Node> selectedNode;
    void InitializeSceneHierarchy();
    void RenderSceneNode(std::shared_ptr<Node> node);
    void RenderNodeContextMenu(std::shared_ptr<Node> node);
    void AddChildNode(std::shared_ptr<Node> parent, const std::string &name, NodeType type);

    // Editor functionality
    void RenderGizmoControls();
    void RenderNodeInEditor(std::shared_ptr<Node> node, bool is3D);
    void HandleNodeSelection(ImVec2 mousePos);

    // Node transforms are now part of the Node class

    // Mode switching
    void ToggleEditorMode();
    void RenderModeToggle();
    void Render2DEditor();
    void Render3DEditor();

    // Theme colors
    void SetupTheme();

    // Documentation
    DocumentationManager docManager;
};