#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "EngineUI.h"

class Engine
{
public:
    Engine();
    ~Engine();
    bool Init();
    void Run();

    // Keyboard callback
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
    // Thread functions
    void MainLoop();
    void RenderLoop();
    void Cleanup();

    // Thread control
    void StartThreads();
    void StopThreads();

    // Vulkan setup methods
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateDescriptorPool();
    void CreateSyncObjects();

    // Swapchain recreation handling
    void CleanupSwapChain();
    void RecreateSwapChain();

    // Thread synchronization
    std::atomic<bool> isRunning{false};
    std::atomic<bool> framebufferResized{false};
    std::mutex renderMutex;
    std::condition_variable renderCV;
    std::atomic<bool> frameReady{false};
    std::atomic<bool> frameRendered{false};

    // Threads
    std::thread mainThread;
    std::thread renderThread;

    // ImGui setup and rendering
    void SetupImGui();
    void CleanupImGui();
    void DrawFrame();

    // UI
    EngineUI ui;

    GLFWwindow *window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    VkDescriptorPool imguiDescriptorPool;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
};