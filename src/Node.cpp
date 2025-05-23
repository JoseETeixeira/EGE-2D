#include "Node.h"
#include <algorithm>
#include <imgui.h>
#include "DocumentationManager.h"

Node::Node(const std::string &nodeName, NodeType nodeType) : name(nodeName), type(nodeType)
{
    // Register base node documentation if not already done
    static bool documentationInitialized = false;
    if (!documentationInitialized)
    {
        documentationInitialized = true;
        InitializeDocumentation();
    }
}

Node::~Node()
{
    // Clean up children
    children.clear();
}

void Node::Update(float deltaTime)
{
    // Base implementation does nothing
    // Update all children
    for (auto &child : children)
    {
        child->Update(deltaTime);
    }
}

void Node::Render()
{
    // Base implementation does nothing
    // Render all children
    for (auto &child : children)
    {
        child->Render();
    }
}

void Node::RenderInspectorProperties()
{
    // Base implementation just shows transform properties
    ImGui::Text("Transform");

    // Position
    ImGui::Text("Position");
    ImGui::SameLine(100);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat3("##Position", transform.position);
    ImGui::PopItemWidth();

    // Rotation
    ImGui::Text("Rotation");
    ImGui::SameLine(100);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat3("##Rotation", transform.rotation);
    ImGui::PopItemWidth();

    // Scale
    ImGui::Text("Scale");
    ImGui::SameLine(100);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat3("##Scale", transform.scale);
    ImGui::PopItemWidth();
}

std::vector<NodeType> Node::GetAvailableNodeTypes()
{
    // Return all available node types
    std::vector<NodeType> types;

    // Add all node types except Root
    for (int i = 0; i <= static_cast<int>(NodeType::Panel); i++)
    {
        NodeType type = static_cast<NodeType>(i);
        if (type != NodeType::Root) // Skip Root type as it's special
        {
            types.push_back(type);
        }
    }

    return types;
}

void Node::AddChild(std::shared_ptr<Node> child)
{
    children.push_back(child);
}

void Node::RemoveChild(std::shared_ptr<Node> child)
{
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
    {
        children.erase(it);
    }
}

std::string Node::GetTypeName() const
{
    return "Node";
}

void Node::RegisterMethod(const std::string &nodeType, const MethodDoc &methodDoc)
{
    // Use DocumentationManager to register the method
    auto &nodeDocs = DocumentationManager::GetNodeDocumentation();
    nodeDocs[nodeType].push_back(methodDoc);
}

void Node::RegisterNodeDescription(const std::string &nodeType, const std::string &description)
{
    // Use DocumentationManager to register the node description
    DocumentationManager::RegisterNodeDescription(nodeType, description);
}

void Node::InitializeDocumentation()
{
    // Register node description
    RegisterNodeDescription("Node", "Base class for all nodes in the scene hierarchy.");

    // Register Update method
    RegisterMethod("Node", {"Update",
                            "Updates the node state based on the elapsed time.",
                            "void",
                            "None",
                            {{"deltaTime", "Time elapsed since the last frame in seconds"}},
                            {"node->Update(deltaTime);"}});

    // Register Render method
    RegisterMethod("Node", {"Render",
                            "Renders the node to the screen.",
                            "void",
                            "None",
                            {},
                            {"node->Render();"}});

    // Register AddChild method
    RegisterMethod("Node", {"AddChild",
                            "Adds a child node to this node.",
                            "void",
                            "None",
                            {{"child", "Shared pointer to the child node to add"}},
                            {"auto childNode = std::make_shared<Node2D>(\"Child\");\nparentNode->AddChild(childNode);"}});

    // Register RemoveChild method
    RegisterMethod("Node", {"RemoveChild",
                            "Removes a child node from this node.",
                            "void",
                            "None",
                            {{"child", "Shared pointer to the child node to remove"}},
                            {"parentNode->RemoveChild(childNode);"}});

    // Register GetTypeName method
    RegisterMethod("Node", {"GetTypeName",
                            "Returns the type name of the node.",
                            "std::string",
                            "The type name as a string",
                            {},
                            {"std::string typeName = node->GetTypeName();"}});
}