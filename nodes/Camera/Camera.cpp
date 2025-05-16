#include "Camera.h"
#include <imgui.h>
#include "DocumentationManager.h"

Camera::Camera(const std::string &nodeName) : Node(nodeName, NodeType::Camera)
{
    // Initialize documentation if not already done
    static bool documentationInitialized = false;
    if (!documentationInitialized)
    {
        documentationInitialized = true;
        InitializeDocumentation();
    }
}

Camera::Camera(const std::string &nodeName, NodeType nodeType) : Node(nodeName, nodeType)
{
    // Documentation is initialized in the other constructor
}

Camera::~Camera()
{
    // Nothing specific to clean up
}

void Camera::SetActive(bool active)
{
    isActive = active;
}

bool Camera::IsActive() const
{
    return isActive;
}

void Camera::Update(float deltaTime)
{
    // Camera specific update logic

    // Call base class update to update children
    Node::Update(deltaTime);
}

void Camera::Render()
{
    // Camera specific rendering logic

    // Call base class render to render children
    Node::Render();
}

std::string Camera::GetTypeName() const
{
    return "Camera";
}

void Camera::RenderInspectorProperties()
{
    // Call base class implementation to render transform properties
    Node::RenderInspectorProperties();

    ImGui::Separator();

    // Camera-specific properties
    ImGui::Text("Camera Properties");

    // Active state
    bool active = isActive;
    if (ImGui::Checkbox("Active", &active))
    {
        SetActive(active);
    }
}

void Camera::InitializeDocumentation()
{
    // Register node description
    RegisterNodeDescription("Camera", "Base class for all camera nodes. Provides common functionality for viewing scenes.");

    // Register SetActive method
    RegisterMethod("Camera", {"SetActive",
                              "Sets whether this camera is active.",
                              "void",
                              "None",
                              {{"active", "Whether the camera should be active"}},
                              {"camera->SetActive(true);"}});

    // Register IsActive method
    RegisterMethod("Camera", {"IsActive",
                              "Checks if this camera is active.",
                              "bool",
                              "True if the camera is active, false otherwise",
                              {},
                              {"bool isActive = camera->IsActive();"}});
}