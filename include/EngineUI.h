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

    // No node type registry needed - nodes will be auto-discovered

    // UI state
    float leftPanelWidth = 250.0f;
    float rightPanelWidth = 300.0f;
    bool showDemoWindow = false;
    bool is3DMode = false; // Default to 2D mode
    bool resizingLeftPanel = false;
    bool resizingRightPanel = false;

    // Editor camera state
    struct EditorCamera2D
    {
        float zoom = 1.0f;
        float posX = 0.0f;
        float posY = 0.0f;
        bool isDragging = false;
        ImVec2 lastMousePos = ImVec2(0, 0);
    } camera2D;

    struct EditorCamera3D
    {
        float zoom = 5.0f;
        float posX = 0.0f;
        float posY = 0.0f;
        float posZ = 0.0f;
        float rotX = 0.0f;
        float rotY = 0.0f;
        bool isDragging = false;
        bool isRotating = false;
        ImVec2 lastMousePos = ImVec2(0, 0);
    } camera3D;

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
    void AddChildNode(std::shared_ptr<Node> parent, NodeType type);
    std::shared_ptr<Node> CreateNodeOfType(NodeType type, const std::string &name);

    // Editor functionality
    void RenderGizmoControls();
    void RenderNodeInEditor(std::shared_ptr<Node> node, bool is3D,
                            float parentX = 0.0f, float parentY = 0.0f,
                            float parentRotation = 0.0f,
                            float parentScaleX = 1.0f, float parentScaleY = 1.0f);
    void HandleNodeSelection(ImVec2 mousePos);
    void RenderGizmos(ImVec2 startPos, ImVec2 viewportSize);
    void FindNodeAtPosition(std::shared_ptr<Node> node, ImVec2 pos, std::shared_ptr<Node> &result, Node::Transform parentTransform = Node::Transform());

    // Helper function to calculate a node's world position
    void CalculateNodeWorldTransform(std::shared_ptr<Node> node, float &outWorldX, float &outWorldY);

    // Helper function to apply a transform operation to a node and all its children
    template <typename TransformFunc>
    void ApplyTransformToNodeAndChildren(std::shared_ptr<Node> node, TransformFunc transformFunc)
    {
        if (!node)
            return;

        // Apply the transform to this node
        transformFunc(node->transform);

        // Apply the same transform to all children recursively
        for (auto &child : node->children)
        {
            ApplyTransformToNodeAndChildren(child, transformFunc);
        }
    }

    // Node transforms are now part of the Node class

    // Mode switching
    void ToggleEditorMode();
    void RenderModeToggle();
    void Render2DEditor();
    void Render3DEditor();

    // Camera controls
    void HandleEditorCameraControls(ImVec2 viewportSize, ImVec2 viewportPos);
    void Handle2DCameraControls(ImVec2 viewportSize, ImVec2 viewportPos);
    void Handle3DCameraControls(ImVec2 viewportSize, ImVec2 viewportPos);
    ImVec2 WorldToScreen2D(float worldX, float worldY, ImVec2 viewportSize, ImVec2 viewportPos);
    ImVec2 ScreenToWorld2D(float screenX, float screenY, ImVec2 viewportSize, ImVec2 viewportPos);

    // Theme colors
    void SetupTheme();

    // Documentation
    DocumentationManager docManager;
};