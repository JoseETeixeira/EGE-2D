#pragma once

#include "../Camera/Camera.h"

/**
 * @brief 3D Camera for viewing 3D scenes
 *
 * The Camera3D class is used for viewing 3D scenes.
 * It inherits from Camera and provides functionality for 3D camera operations.
 */
class Camera3D : public Camera
{
public:
    Camera3D(const std::string &nodeName);
    virtual ~Camera3D();

    // Camera3D specific methods
    void SetFOV(float fov);
    float GetFOV() const;
    void SetNearClip(float nearClip);
    float GetNearClip() const;
    void SetFarClip(float farClip);
    float GetFarClip() const;

    // Override base methods
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual std::string GetTypeName() const override;
    virtual void RenderInspectorProperties() override;

    // Static documentation methods
    static void InitializeDocumentation();

private:
    float fov = 70.0f;       // Field of view in degrees
    float nearClip = 0.1f;   // Near clipping plane
    float farClip = 1000.0f; // Far clipping plane
};