#include "Camera2D.h"
#include <imgui.h>
#include "DocumentationManager.h"

Camera2D::Camera2D(const std::string &nodeName) : Node2D(nodeName, NodeType::Camera)
{
    // Documentation is now initialized in DocumentationManager::LoadDocumentation
}

Camera2D::~Camera2D()
{
    // Nothing specific to clean up
}

void Camera2D::SetZoom(float zoom)
{
    this->zoom = zoom;
}

float Camera2D::GetZoom() const
{
    return zoom;
}

void Camera2D::SetActive(bool active)
{
    isActive = active;
}

bool Camera2D::IsActive() const
{
    return isActive;
}

void Camera2D::Update(float deltaTime)
{
    // Camera2D specific update logic

    // Call base class update
    Node2D::Update(deltaTime);
}

void Camera2D::Render()
{
    // Camera2D specific rendering logic

    // Call base class render
    Node2D::Render();
}

std::string Camera2D::GetTypeName() const
{
    return "Camera2D";
}

void Camera2D::RenderInspectorProperties()
{
    // First render the transform properties from the base class
    Node2D::RenderInspectorProperties();

    ImGui::Separator();

    // Camera2D-specific properties
    ImGui::Text("Camera2D Properties");

    // Zoom
    ImGui::Text("Zoom");
    ImGui::SameLine(100);
    float zoomValue = zoom;
    if (ImGui::SliderFloat("##Zoom", &zoomValue, 0.1f, 10.0f))
    {
        SetZoom(zoomValue);
    }

    // Active state
    bool active = isActive;
    if (ImGui::Checkbox("Active", &active))
    {
        SetActive(active);
    }
}

void Camera2D::InitializeDocumentation()
{
    // Register node description
    RegisterNodeDescription("Camera2D", "A 2D camera node for viewing 2D scenes. Inherits from Node2D and provides functionality for 2D camera operations.");

    // Register SetZoom method
    RegisterMethod("Camera2D", {"SetZoom",
                                "Sets the zoom level of the camera.",
                                "void",
                                "None",
                                {{"zoom", "Zoom level (1.0 is normal, greater than 1.0 is zoomed in, less than 1.0 is zoomed out)"}},
                                {"camera2D->SetZoom(2.0f); // Zoom in 2x"}});

    // Register GetZoom method
    RegisterMethod("Camera2D", {"GetZoom",
                                "Gets the current zoom level of the camera.",
                                "float",
                                "The current zoom level",
                                {},
                                {"float zoom = camera2D->GetZoom();"}});

    // Register SetActive method
    RegisterMethod("Camera2D", {"SetActive",
                                "Sets whether this camera is active.",
                                "void",
                                "None",
                                {{"active", "Whether the camera should be active"}},
                                {"camera2D->SetActive(true);"}});

    // Register IsActive method
    RegisterMethod("Camera2D", {"IsActive",
                                "Checks if this camera is active.",
                                "bool",
                                "True if the camera is active, false otherwise",
                                {},
                                {"bool isActive = camera2D->IsActive();"}});

    // These methods are already registered above, so we don't need to register them again
}