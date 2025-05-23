#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "Node.h"

/**
 * @brief Manages documentation for all node types
 *
 * The DocumentationManager class is responsible for displaying
 * the documentation popup and handling search functionality.
 */
class DocumentationManager
{
public:
    DocumentationManager();
    ~DocumentationManager();

    // Initialize the documentation manager
    void Init();

    // Show/hide the documentation popup
    void ShowDocumentation();
    void HideDocumentation();
    void ToggleDocumentation();

    // Render the documentation popup
    void Render();

    // Handle keyboard shortcuts
    bool HandleKeyPress(int key, int scancode, int action, int mods);

    // Check if documentation is visible
    bool IsVisible() const;

    // Node documentation registration
    static void RegisterNodeDocumentation(const std::string &nodeType, const std::vector<MethodDoc> &methods);
    static void RegisterNodeDescription(const std::string &nodeType, const std::string &description);

    // Get documentation
    static std::map<std::string, std::vector<MethodDoc>> &GetNodeDocumentation();
    static std::map<std::string, std::string> &GetNodeDescriptions();

private:
    // Documentation state
    bool isVisible = false;

    // Search functionality
    std::string searchQuery;
    std::vector<std::pair<std::string, MethodDoc>> searchResults;
    void UpdateSearchResults();

    // Rendering helpers
    void RenderNodeList();
    void RenderMethodList(const std::string &nodeType);
    void RenderMethodDetails(const std::string &nodeType, const std::string &methodName);
    void RenderSearchBar();
    void RenderSearchResults();

    // Current selection
    std::string selectedNodeType;
    std::string selectedMethod;

    // Node documentation cache - now static so nodes can register themselves
    static std::map<std::string, std::vector<MethodDoc>> nodeDocumentation;
    static std::map<std::string, std::string> nodeDescriptions;

    // Load documentation for all node types
    void LoadDocumentation();
};