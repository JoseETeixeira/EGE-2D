#include "Node.h"
#include <algorithm>

// Initialize static members
std::map<std::string, std::vector<MethodDoc>> Node::documentation;
std::map<std::string, std::string> Node::nodeDescriptions;

Node::Node(const std::string &nodeName, NodeType nodeType) : name(nodeName), type(nodeType)
{
    // Register base node documentation if not already done
    if (documentation.find("Node") == documentation.end())
    {
        // Register node description
        nodeDescriptions["Node"] = "Base class for all nodes in the scene hierarchy.";

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

std::map<std::string, std::vector<MethodDoc>> Node::GetDocumentation()
{
    return documentation;
}

std::string Node::GetNodeDescription()
{
    // This needs to be static since it's called without an instance
    std::string typeName = "Node";
    auto it = nodeDescriptions.find(typeName);
    if (it != nodeDescriptions.end())
    {
        return it->second;
    }
    return "No description available.";
}

std::string Node::GetNodeDescription(const std::string &nodeType)
{
    auto it = nodeDescriptions.find(nodeType);
    if (it != nodeDescriptions.end())
    {
        return it->second;
    }
    return "No description available for " + nodeType + ".";
}

std::string Node::GetTypeName() const
{
    return "Node";
}

void Node::RegisterMethod(const std::string &nodeType, const MethodDoc &methodDoc)
{
    documentation[nodeType].push_back(methodDoc);
}

void Node::InitializeDocumentation()
{
    // This is already done in the constructor, but we provide this method
    // for consistency with derived classes
    if (documentation.find("Node") == documentation.end())
    {
        // Register node description
        nodeDescriptions["Node"] = "Base class for all nodes in the scene hierarchy.";

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
}