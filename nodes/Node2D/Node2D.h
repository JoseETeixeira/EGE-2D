#pragma once

#include "Node.h"

/**
 * @brief 2D Node with position, rotation, and scale
 *
 * The Node2D class is the base class for all 2D nodes in the scene.
 * It provides functionality for 2D transformations.
 */
class Node2D : public Node
{
public:
    Node2D(const std::string &nodeName);
    Node2D(const std::string &nodeName, NodeType nodeType);
    virtual ~Node2D();

    // Node2D specific methods
    void SetPosition(float x, float y);
    void SetRotation(float degrees);
    void SetScale(float x, float y);

    float *GetPosition();
    float GetRotation() const;
    float *GetScale();

    // Override base methods
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual std::string GetTypeName() const override;

    // Static documentation methods
    static void InitializeDocumentation();
};