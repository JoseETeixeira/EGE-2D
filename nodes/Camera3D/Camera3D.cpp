#include "Camera3D.h"
#include <imgui.h>
#include "DocumentationManager.h"

Camera3D::Camera3D(const std::string &nodeName) : Camera(nodeName, NodeType::Camera)
{
    // Initialize documentation if not already done
    static bool documentationInitialized = false;
    if (!documentationInitialized)
    {
        documentationInitialized = true;
        InitializeDocumentation();
    }
}

Camera3D::~Camera3D()
{
    // Nothing specific to clean up
}

void Camera3D::SetFOV(float fov)
{
    this->fov = fov;
}

float Camera3D::GetFOV() const
{
    return fov;
}

void Camera3D::SetNearClip(float nearClip)
{
    this->nearClip = nearClip;
}

float Camera3D::GetNearClip() const
{
    return nearClip;
}

void Camera3D::SetFarClip(float farClip)
{
    this->farClip = farClip;
}

float Camera3D::GetFarClip() const
{
    return farClip;
}

void Camera3D::Update(float deltaTime)
{
    // Camera3D specific update logic

    // Call base class update
    Camera::Update(deltaTime);
}

void Camera3D::Render()
{
    // Camera3D specific rendering logic

    // Call base class render
    Camera::Render();
}

std::string Camera3D::GetTypeName() const
{
    return "Camera3D";
}

void Camera3D::RenderInspectorProperties()
{
    // First render the transform properties from the base class
    Camera::RenderInspectorProperties();

    ImGui::Separator();

    // Camera3D-specific properties
    ImGui::Text("Camera3D Properties");

    // Field of View
    ImGui::Text("FOV");
    ImGui::SameLine(100);
    float fovValue = fov;
    if (ImGui::SliderFloat("##FOV", &fovValue, 10.0f, 120.0f, "%.1f°"))
    {
        SetFOV(fovValue);
    }

    // Near Clip
    ImGui::Text("Near Clip");
    ImGui::SameLine(100);
    float nearValue = nearClip;
    if (ImGui::InputFloat("##NearClip", &nearValue, 0.01f, 0.1f, "%.3f"))
    {
        if (nearValue > 0.0f && nearValue < farClip)
        {
            SetNearClip(nearValue);
        }
    }

    // Far Clip
    ImGui::Text("Far Clip");
    ImGui::SameLine(100);
    float farValue = farClip;
    if (ImGui::InputFloat("##FarClip", &farValue, 1.0f, 10.0f, "%.1f"))
    {
        if (farValue > nearClip)
        {
            SetFarClip(farValue);
        }
    }
}

void Camera3D::InitializeDocumentation()
{
    // Register node description
    RegisterNodeDescription("Camera3D", "A 3D camera node for viewing 3D scenes. Provides functionality for 3D camera operations like field of view and clipping planes.");

    // Register SetFOV method
    RegisterMethod("Camera3D", {"SetFOV",
                                "Sets the field of view of the camera in degrees.",
                                "void",
                                "None",
                                {{"fov", "Field of view in degrees"}},
                                {"camera3D->SetFOV(60.0f);"}});

    // Register GetFOV method
    RegisterMethod("Camera3D", {"GetFOV",
                                "Gets the field of view of the camera in degrees.",
                                "float",
                                "The current field of view in degrees",
                                {},
                                {"float fov = camera3D->GetFOV();"}});

    // Register SetNearClip method
    RegisterMethod("Camera3D", {"SetNearClip",
                                "Sets the near clipping plane distance.",
                                "void",
                                "None",
                                {{"nearClip", "Distance to the near clipping plane"}},
                                {"camera3D->SetNearClip(0.1f);"}});

    // Register GetNearClip method
    RegisterMethod("Camera3D", {"GetNearClip",
                                "Gets the near clipping plane distance.",
                                "float",
                                "The current near clipping plane distance",
                                {},
                                {"float nearClip = camera3D->GetNearClip();"}});

    // Register SetFarClip method
    RegisterMethod("Camera3D", {"SetFarClip",
                                "Sets the far clipping plane distance.",
                                "void",
                                "None",
                                {{"farClip", "Distance to the far clipping plane"}},
                                {"camera3D->SetFarClip(1000.0f);"}});

    // Register GetFarClip method
    RegisterMethod("Camera3D", {"GetFarClip",
                                "Gets the far clipping plane distance.",
                                "float",
                                "The current far clipping plane distance",
                                {},
                                {"float farClip = camera3D->GetFarClip();"}});
}