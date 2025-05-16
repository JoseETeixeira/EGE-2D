#include "Sprite.h"

Sprite::Sprite(const std::string &nodeName) : Node2D(nodeName, NodeType::Sprite)
{
    // Initialize documentation if not already done
    InitializeDocumentation();
}

Sprite::Sprite(const std::string &nodeName, NodeType nodeType) : Node2D(nodeName, nodeType)
{
    // Initialize documentation if not already done
    InitializeDocumentation();
}

Sprite::~Sprite()
{
    // Nothing specific to clean up
}

void Sprite::SetTexture(const std::string &texturePath)
{
    this->texturePath = texturePath;
}

void Sprite::SetColor(float r, float g, float b, float a)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
}

const std::string &Sprite::GetTexturePath() const
{
    return texturePath;
}

const float *Sprite::GetColor() const
{
    return color;
}

void Sprite::Update(float deltaTime)
{
    // Sprite specific update logic

    // Call base class update
    Node2D::Update(deltaTime);
}

void Sprite::Render()
{
    // Sprite specific rendering logic
    // In a real implementation, this would render the texture with the specified color

    // Call base class render
    Node2D::Render();
}

std::string Sprite::GetTypeName() const
{
    return "Sprite";
}

void Sprite::InitializeDocumentation()
{
    // Register documentation if not already done
    if (documentation.find("Sprite") == documentation.end())
    {
        // Register node description
        nodeDescriptions["Sprite"] = "A 2D sprite node for displaying images. Inherits from Node2D and adds texture and color properties.";

        // Register SetTexture method
        RegisterMethod("Sprite", {"SetTexture",
                                  "Sets the texture to be displayed by the sprite.",
                                  "void",
                                  "None",
                                  {{"texturePath", "Path to the texture file"}},
                                  {"sprite->SetTexture(\"assets/textures/player.png\");"}});

        // Register SetColor method
        RegisterMethod("Sprite", {"SetColor",
                                  "Sets the color tint of the sprite.",
                                  "void",
                                  "None",
                                  {{"r", "Red component (0.0 to 1.0)"},
                                   {"g", "Green component (0.0 to 1.0)"},
                                   {"b", "Blue component (0.0 to 1.0)"},
                                   {"a", "Alpha component (0.0 to 1.0, default: 1.0)"}},
                                  {"sprite->SetColor(1.0f, 0.5f, 0.5f, 0.8f); // Pink with 80% opacity"}});

        // Register GetTexturePath method
        RegisterMethod("Sprite", {"GetTexturePath",
                                  "Gets the path to the texture being displayed.",
                                  "const std::string&",
                                  "Reference to the texture path string",
                                  {},
                                  {"std::string texturePath = sprite->GetTexturePath();"}});

        // Register GetColor method
        RegisterMethod("Sprite", {"GetColor",
                                  "Gets the color tint of the sprite.",
                                  "const float*",
                                  "Pointer to an array of 4 floats representing the color [r, g, b, a]",
                                  {},
                                  {"const float* color = sprite->GetColor();\nfloat r = color[0];\nfloat g = color[1];\nfloat b = color[2];\nfloat a = color[3];"}});
    }
}