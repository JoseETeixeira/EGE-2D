#pragma once

#include "Node.h"

/**
 * @brief Base Camera node for viewing scenes
 *
 * The Camera class is the base class for all camera nodes.
 * It provides common functionality for both 2D and 3D cameras.
 */
class Camera : public Node
{
public:
    Camera(const std::string &nodeName);
    Camera(const std::string &nodeName, NodeType nodeType);
    virtual ~Camera();

    // Camera specific methods
    void SetActive(bool active);
    bool IsActive() const;

    // Override base methods
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual std::string GetTypeName() const override;
    virtual void RenderInspectorProperties() override;

    // Static documentation methods
    static void InitializeDocumentation();

protected:
    bool isActive = false;
};