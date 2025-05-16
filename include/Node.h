#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>

// Node types for the scene hierarchy
enum class NodeType
{
    Root,
    Node2D,
    Sprite,
    CharacterBody2D,
    Camera,
    Light,
    Label,
    Button,
    Panel
};

// Forward declaration
class Node;

/**
 * @brief Documentation structure for node methods
 */
struct MethodDoc
{
    std::string name;                                        // Method name
    std::string description;                                 // Method description
    std::string returnType;                                  // Return type
    std::string returnDesc;                                  // Return value description
    std::vector<std::pair<std::string, std::string>> params; // Parameter name and description pairs
    std::vector<std::string> examples;                       // Usage examples
};

/**
 * @brief Base Node class that all node types will inherit from
 */
class Node
{
public:
    Node(const std::string &nodeName, NodeType nodeType);
    virtual ~Node();

    // Node properties
    std::string name;
    bool selected = false;
    bool expanded = false;
    std::vector<std::shared_ptr<Node>> children;
    NodeType type; // Store the node type

    // Node transform
    struct Transform
    {
        float position[3] = {0.0f, 0.0f, 0.0f};
        float rotation[3] = {0.0f, 0.0f, 0.0f};
        float scale[3] = {1.0f, 1.0f, 1.0f};
    };
    Transform transform;

    // Node methods
    virtual void Update(float deltaTime);
    virtual void Render();

    // Child management
    void AddChild(std::shared_ptr<Node> child);
    void RemoveChild(std::shared_ptr<Node> child);

    // Documentation methods
    static std::map<std::string, std::vector<MethodDoc>> GetDocumentation();
    static std::string GetNodeDescription();
    static std::string GetNodeDescription(const std::string &nodeType);
    static void InitializeDocumentation();

    // Type information
    virtual std::string GetTypeName() const;

protected:
    // Documentation registration
    static void RegisterMethod(const std::string &nodeType, const MethodDoc &methodDoc);
    static std::map<std::string, std::vector<MethodDoc>> documentation;
    static std::map<std::string, std::string> nodeDescriptions;
};