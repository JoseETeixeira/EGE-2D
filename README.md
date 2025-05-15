# EGE-2D

Vulkan-based 2D Game Engine.

## Prerequisites

- Vulkan SDK (https://vulkan.lunarg.com/)
- SCons (https://scons.org)
- GLFW (https://www.glfw.org/)

Linux:

```
sudo apt install scons libglfw3-dev pkg-config
# Download and install Vulkan SDK, set VULKAN_SDK env var
export VULKAN_SDK=/opt/vulkan-sdk-1.4.313.0
source $VULKAN_SDK/setup-env.sh
source ~/.bashrc  # or ~/.zshrc

```

Windows:

```
# Install SCons (pip install scons)
# Install GLFW binaries and Vulkan SDK, set VULKAN_SDK env var
```

## Building

```
scons
```

## Running

```
./engine  # or engine.exe on Windows
```
