#pragma once

#include "../Node2D/Node2D.h"
#include <string>

/**
 * @brief 2D Sprite node for displaying images
 *
 * The Sprite class is used to display 2D images in the scene.
 * It inherits from Node2D and adds texture and color properties.
 */
class Sprite : public Node2D
{
public:
    Sprite(const std::string &nodeName);
    Sprite(const std::string &nodeName, NodeType nodeType);
    virtual ~Sprite();

    // Sprite specific methods
    void SetTexture(const std::string &texturePath);
    void SetColor(float r, float g, float b, float a = 1.0f);

    const std::string &GetTexturePath() const;
    const float *GetColor() const;

    // Override base methods
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual std::string GetTypeName() const override;

    // Static documentation methods
    static void InitializeDocumentation();

private:
    std::string texturePath;
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // RGBA
};