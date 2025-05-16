#pragma once

#include "../Node2D/Node2D.h"

/**
 * @brief 2D Camera for viewing 2D scenes
 *
 * The Camera2D class is used for viewing 2D scenes.
 * It inherits from Node2D and provides functionality for 2D camera operations.
 */
class Camera2D : public Node2D
{
public:
    Camera2D(const std::string &nodeName);
    virtual ~Camera2D();

    // Camera2D specific methods
    void SetZoom(float zoom);
    float GetZoom() const;
    void SetActive(bool active);
    bool IsActive() const;

    // Override base methods
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual std::string GetTypeName() const override;
    virtual void RenderInspectorProperties() override;

    // Static documentation methods
    static void InitializeDocumentation();

private:
    float zoom = 1.0f;
    bool isActive = false;
};