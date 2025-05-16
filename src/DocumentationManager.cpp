#include "DocumentationManager.h"
#include "imgui.h"
#include "Node.h"
#include <algorithm>
#include <cctype>
#include "../nodes/Camera/Camera.h"
#include "../nodes/Camera2D/Camera2D.h"
#include "../nodes/Camera3D/Camera3D.h"

// Initialize static members
std::map<std::string, std::vector<MethodDoc>> DocumentationManager::nodeDocumentation;
std::map<std::string, std::string> DocumentationManager::nodeDescriptions;

DocumentationManager::DocumentationManager()
{
    // Initialize with default values
    isVisible = false;
    selectedNodeType = "";
    selectedMethod = "";
}

DocumentationManager::~DocumentationManager()
{
    // Nothing to clean up
}

void DocumentationManager::Init()
{
    // Load documentation for all node types
    LoadDocumentation();
}

void DocumentationManager::ShowDocumentation()
{
    isVisible = true;
}

void DocumentationManager::HideDocumentation()
{
    isVisible = false;
}

void DocumentationManager::ToggleDocumentation()
{
    isVisible = !isVisible;
}

bool DocumentationManager::IsVisible() const
{
    return isVisible;
}

bool DocumentationManager::HandleKeyPress(int key, int scancode, int action, int mods)
{
    // F1 key to toggle documentation
    if (key == 290 && action == 1)
    { // F1 key pressed
        ToggleDocumentation();
        return true;
    }
    return false;
}

void DocumentationManager::RegisterNodeDocumentation(const std::string &nodeType, const std::vector<MethodDoc> &methods)
{
    // Add or replace the documentation for this node type
    nodeDocumentation[nodeType] = methods;
}

void DocumentationManager::RegisterNodeDescription(const std::string &nodeType, const std::string &description)
{
    // Add or replace the description for this node type
    nodeDescriptions[nodeType] = description;
}

std::map<std::string, std::vector<MethodDoc>> &DocumentationManager::GetNodeDocumentation()
{
    return nodeDocumentation;
}

std::map<std::string, std::string> &DocumentationManager::GetNodeDescriptions()
{
    return nodeDescriptions;
}

void DocumentationManager::LoadDocumentation()
{
    // Explicitly initialize documentation for all node types
    // This ensures documentation is available even if no instances have been created

    // Base node
    Node::InitializeDocumentation();

    // Camera nodes
    Camera::InitializeDocumentation();
    Camera2D::InitializeDocumentation();
    Camera3D::InitializeDocumentation();

    // Other node types will register their documentation when they are instantiated
}

void DocumentationManager::Render()
{
    if (!isVisible)
    {
        return;
    }

    // Set up the documentation window
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

    // Use a local copy of isVisible to prevent crashes when the window is closed
    bool visible = isVisible;

    // Begin the documentation window
    ImGui::Begin("Documentation", &visible, ImGuiWindowFlags_NoCollapse);

    // Check if the window should be closed
    if (!visible)
    {
        isVisible = false;
        ImGui::End();
        return;
    }

    // Only render content if the window is still visible

    // Only render content if the window is still visible
    if (isVisible)
    {
        // Render search bar at the top
        RenderSearchBar();

        ImGui::Separator();

        // If search query is not empty, show search results
        if (!searchQuery.empty())
        {
            RenderSearchResults();
        }
        else
        {
            // Otherwise show the normal documentation view
            // Split the window into two columns
            ImGui::Columns(2, "DocColumns", true);

            // Left column: Node list
            RenderNodeList();

            // Right column: Method list and details
            ImGui::NextColumn();

            if (!selectedNodeType.empty())
            {
                // Show node description
                ImGui::TextWrapped("%s", nodeDescriptions[selectedNodeType].c_str());
                ImGui::Separator();

                // Show method list for the selected node type
                RenderMethodList(selectedNodeType);

                // Show method details if a method is selected
                if (!selectedMethod.empty())
                {
                    ImGui::Separator();
                    RenderMethodDetails(selectedNodeType, selectedMethod);
                }
            }

            ImGui::Columns(1);
        }
    }

    // Always end the window
    ImGui::End();
}

void DocumentationManager::RenderSearchBar()
{
    // Search bar with clear button
    ImGui::Text("Search:");
    ImGui::SameLine();

    // Calculate width for search input
    float buttonWidth = 60.0f;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float inputWidth = ImGui::GetContentRegionAvail().x - buttonWidth - spacing;

    // Search input
    ImGui::PushItemWidth(inputWidth);

    // ImGui::InputText requires a char buffer, not a std::string
    static char searchBuffer[256] = "";
    if (ImGui::InputText("##SearchInput", searchBuffer, sizeof(searchBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        searchQuery = searchBuffer;
        UpdateSearchResults();
    }

    // Update searchQuery if buffer changes
    if (searchQuery != searchBuffer)
    {
        searchQuery = searchBuffer;
    }

    ImGui::PopItemWidth();

    // Clear button
    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(buttonWidth, 0)))
    {
        searchQuery.clear();
        searchResults.clear();
        // Also clear the buffer
        memset(searchBuffer, 0, sizeof(searchBuffer));
    }

    // Update search results as user types
    static std::string lastQuery;
    if (searchQuery != lastQuery)
    {
        UpdateSearchResults();
        lastQuery = searchQuery;
    }
}

void DocumentationManager::UpdateSearchResults()
{
    searchResults.clear();

    if (searchQuery.empty())
    {
        return;
    }

    // Convert search query to lowercase for case-insensitive search
    std::string lowerQuery = searchQuery;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    // Search through all node types and methods
    for (const auto &nodePair : nodeDocumentation)
    {
        const std::string &nodeType = nodePair.first;

        // Search in node type name
        std::string lowerNodeType = nodeType;
        std::transform(lowerNodeType.begin(), lowerNodeType.end(), lowerNodeType.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });

        if (lowerNodeType.find(lowerQuery) != std::string::npos)
        {
            // Add all methods of this node type to results
            for (const auto &method : nodePair.second)
            {
                searchResults.push_back(std::make_pair(nodeType, method));
            }
            continue; // Skip individual method search for this node
        }

        // Search in methods
        for (const auto &method : nodePair.second)
        {
            // Convert method name to lowercase
            std::string lowerMethodName = method.name;
            std::transform(lowerMethodName.begin(), lowerMethodName.end(), lowerMethodName.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });

            // Search in method name
            if (lowerMethodName.find(lowerQuery) != std::string::npos)
            {
                searchResults.push_back(std::make_pair(nodeType, method));
                continue;
            }

            // Search in method description
            std::string lowerDesc = method.description;
            std::transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });

            if (lowerDesc.find(lowerQuery) != std::string::npos)
            {
                searchResults.push_back(std::make_pair(nodeType, method));
                continue;
            }

            // Search in parameter names and descriptions
            bool foundInParams = false;
            for (const auto &param : method.params)
            {
                std::string lowerParamName = param.first;
                std::transform(lowerParamName.begin(), lowerParamName.end(), lowerParamName.begin(),
                               [](unsigned char c)
                               { return std::tolower(c); });

                std::string lowerParamDesc = param.second;
                std::transform(lowerParamDesc.begin(), lowerParamDesc.end(), lowerParamDesc.begin(),
                               [](unsigned char c)
                               { return std::tolower(c); });

                if (lowerParamName.find(lowerQuery) != std::string::npos ||
                    lowerParamDesc.find(lowerQuery) != std::string::npos)
                {
                    searchResults.push_back(std::make_pair(nodeType, method));
                    foundInParams = true;
                    break;
                }
            }

            if (foundInParams)
            {
                continue;
            }

            // Search in return type and description
            std::string lowerReturnType = method.returnType;
            std::transform(lowerReturnType.begin(), lowerReturnType.end(), lowerReturnType.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });

            std::string lowerReturnDesc = method.returnDesc;
            std::transform(lowerReturnDesc.begin(), lowerReturnDesc.end(), lowerReturnDesc.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });

            if (lowerReturnType.find(lowerQuery) != std::string::npos ||
                lowerReturnDesc.find(lowerQuery) != std::string::npos)
            {
                searchResults.push_back(std::make_pair(nodeType, method));
            }
        }
    }
}

void DocumentationManager::RenderSearchResults()
{
    ImGui::Text("Search Results (%d):", static_cast<int>(searchResults.size()));
    ImGui::Separator();

    if (searchResults.empty())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "No results found for '%s'", searchQuery.c_str());
        return;
    }

    // Display search results
    for (const auto &result : searchResults)
    {
        const std::string &nodeType = result.first;
        const MethodDoc &method = result.second;

        // Create a unique ID for this result
        std::string resultId = nodeType + "::" + method.name;

        if (ImGui::CollapsingHeader(resultId.c_str()))
        {
            ImGui::Indent();

            // Node type
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Node Type: %s", nodeType.c_str());

            // Method signature
            std::string signature = method.returnType + " " + method.name + "(";
            for (size_t i = 0; i < method.params.size(); ++i)
            {
                signature += method.params[i].first;
                if (i < method.params.size() - 1)
                {
                    signature += ", ";
                }
            }
            signature += ")";
            ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "%s", signature.c_str());

            // Method description
            ImGui::TextWrapped("%s", method.description.c_str());

            // Parameters
            if (!method.params.empty())
            {
                ImGui::Separator();
                ImGui::Text("Parameters:");
                for (const auto &param : method.params)
                {
                    ImGui::BulletText("%s: %s", param.first.c_str(), param.second.c_str());
                }
            }

            // Return value
            ImGui::Separator();
            ImGui::Text("Returns: %s", method.returnDesc.c_str());

            // Examples
            if (!method.examples.empty())
            {
                ImGui::Separator();
                ImGui::Text("Example:");
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.0f, 1.0f));
                for (const auto &example : method.examples)
                {
                    ImGui::TextWrapped("%s", example.c_str());
                }
                ImGui::PopStyleColor();
            }

            ImGui::Unindent();
        }
    }
}

void DocumentationManager::RenderNodeList()
{
    ImGui::Text("Node Types:");
    ImGui::BeginChild("NodeList", ImVec2(0, 0), true);

    for (const auto &pair : nodeDocumentation)
    {
        const std::string &nodeType = pair.first;

        bool isSelected = (selectedNodeType == nodeType);
        if (ImGui::Selectable(nodeType.c_str(), isSelected))
        {
            selectedNodeType = nodeType;
            selectedMethod = ""; // Clear selected method when changing node type
        }

        if (isSelected)
        {
            ImGui::SetItemDefaultFocus();
        }
    }

    ImGui::EndChild();
}

void DocumentationManager::RenderMethodList(const std::string &nodeType)
{
    ImGui::Text("Methods:");
    ImGui::BeginChild("MethodList", ImVec2(0, 150), true);

    // Safety check for empty node type
    if (nodeType.empty())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Select a node type first");
        ImGui::EndChild();
        return;
    }

    auto it = nodeDocumentation.find(nodeType);
    if (it != nodeDocumentation.end())
    {
        if (it->second.empty())
        {
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "No methods found for %s", nodeType.c_str());
        }
        else
        {
            for (const auto &method : it->second)
            {
                bool isSelected = (selectedMethod == method.name);
                if (ImGui::Selectable(method.name.c_str(), isSelected))
                {
                    selectedMethod = method.name;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
        }
    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Node type not found: %s", nodeType.c_str());
    }

    ImGui::EndChild();
}

void DocumentationManager::RenderMethodDetails(const std::string &nodeType, const std::string &methodName)
{
    // Safety check for empty inputs
    if (nodeType.empty() || methodName.empty())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Select a method to view details");
        return;
    }

    auto nodeIt = nodeDocumentation.find(nodeType);
    if (nodeIt == nodeDocumentation.end())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Node type not found: %s", nodeType.c_str());
        return;
    }

    // Find the method in the node's documentation
    const MethodDoc *methodDoc = nullptr;
    for (const auto &method : nodeIt->second)
    {
        if (method.name == methodName)
        {
            methodDoc = &method;
            break;
        }
    }

    if (!methodDoc)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Method not found: %s", methodName.c_str());
        return;
    }

    ImGui::Text("Method Details:");
    ImGui::BeginChild("MethodDetails", ImVec2(0, 0), true);

    // Method signature
    std::string signature = methodDoc->returnType + " " + methodDoc->name + "(";
    for (size_t i = 0; i < methodDoc->params.size(); ++i)
    {
        signature += methodDoc->params[i].first;
        if (i < methodDoc->params.size() - 1)
        {
            signature += ", ";
        }
    }
    signature += ")";
    ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "%s", signature.c_str());

    // Method description
    ImGui::TextWrapped("%s", methodDoc->description.c_str());

    // Parameters
    if (!methodDoc->params.empty())
    {
        ImGui::Separator();
        ImGui::Text("Parameters:");
        for (const auto &param : methodDoc->params)
        {
            ImGui::BulletText("%s: %s", param.first.c_str(), param.second.c_str());
        }
    }

    // Return value
    ImGui::Separator();
    ImGui::Text("Returns: %s", methodDoc->returnDesc.c_str());

    // Examples
    if (!methodDoc->examples.empty())
    {
        ImGui::Separator();
        ImGui::Text("Example:");
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.0f, 1.0f));
        for (const auto &example : methodDoc->examples)
        {
            ImGui::TextWrapped("%s", example.c_str());
        }
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();
}