#include "Node2D.h"
#include <imgui.h>
#include "DocumentationManager.h"

Node2D::Node2D(const std::string &nodeName) : Node(nodeName, NodeType::Node2D)
{
    // Initialize documentation if not already done
    static bool documentationInitialized = false;
    if (!documentationInitialized)
    {
        documentationInitialized = true;
        InitializeDocumentation();
    }
}

Node2D::Node2D(const std::string &nodeName, NodeType nodeType) : Node(nodeName, nodeType)
{
    // Documentation is initialized in the other constructor
}

Node2D::~Node2D()
{
    // Nothing specific to clean up
}

void Node2D::SetPosition(float x, float y)
{
    transform.position[0] = x;
    transform.position[1] = y;
}

void Node2D::SetRotation(float degrees)
{
    transform.rotation[2] = degrees;
}

void Node2D::SetScale(float x, float y)
{
    transform.scale[0] = x;
    transform.scale[1] = y;
}

float *Node2D::GetPosition()
{
    return transform.position;
}

float Node2D::GetRotation() const
{
    return transform.rotation[2];
}

float *Node2D::GetScale()
{
    return transform.scale;
}

void Node2D::Update(float deltaTime)
{
    // Node2D specific update logic

    // Call base class update to update children
    Node::Update(deltaTime);
}

void Node2D::Render()
{
    // Node2D specific rendering logic

    // Call base class render to render children
    Node::Render();
}

std::string Node2D::GetTypeName() const
{
    return "Node2D";
}

void Node2D::RenderInspectorProperties()
{
    // Call base class implementation to render transform properties
    Node::RenderInspectorProperties();

    // Node2D-specific properties could be added here
    // For now, we just use the base class implementation
}

void Node2D::InitializeDocumentation()
{
    // Register node description
    RegisterNodeDescription("Node2D", "Base class for all 2D nodes in the scene. Provides functionality for 2D transformations.");

    // Register SetPosition method
    RegisterMethod("Node2D", {"SetPosition",
                              "Sets the 2D position of the node.",
                              "void",
                              "None",
                              {{"x", "X coordinate"}, {"y", "Y coordinate"}},
                              {"node2D->SetPosition(100.0f, 200.0f);"}});

    // Register SetRotation method
    RegisterMethod("Node2D", {"SetRotation",
                              "Sets the rotation of the node in degrees.",
                              "void",
                              "None",
                              {{"degrees", "Rotation angle in degrees"}},
                              {"node2D->SetRotation(45.0f);"}});

    // Register SetScale method
    RegisterMethod("Node2D", {"SetScale",
                              "Sets the scale of the node.",
                              "void",
                              "None",
                              {{"x", "X scale factor"}, {"y", "Y scale factor"}},
                              {"node2D->SetScale(2.0f, 2.0f);"}});

    // Register GetPosition method
    RegisterMethod("Node2D", {"GetPosition",
                              "Gets the position of the node.",
                              "float*",
                              "Pointer to an array of 3 floats representing the position [x, y, z]",
                              {},
                              {"float* position = node2D->GetPosition();\nfloat x = position[0];\nfloat y = position[1];"}});

    // Register GetRotation method
    RegisterMethod("Node2D", {"GetRotation",
                              "Gets the rotation of the node in degrees.",
                              "float",
                              "Rotation angle in degrees",
                              {},
                              {"float rotation = node2D->GetRotation();"}});

    // Register GetScale method
    RegisterMethod("Node2D", {"GetScale",
                              "Gets the scale of the node.",
                              "float*",
                              "Pointer to an array of 3 floats representing the scale [x, y, z]",
                              {},
                              {"float* scale = node2D->GetScale();\nfloat scaleX = scale[0];\nfloat scaleY = scale[1];"}});
}