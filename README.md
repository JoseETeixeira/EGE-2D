# EGE-2D Engine

A 2D game engine built with C++ using Vulkan for rendering and ImGui for the UI.

## Node System

The engine uses a node-based architecture similar to Godot, with different node types:

- **Node**: Base class for all nodes
- **Node2D**: Base class for all 2D nodes with transform capabilities
- **Sprite**: 2D sprite node for displaying images
- **CharacterBody2D**: Physics-based character controller
- **Camera**: Camera node for controlling the view
- **Light**: Light node for 2D lighting
- **Label**: UI text label
- **Button**: UI button
- **Panel**: UI panel container

Each node type has its own folder and files in the `nodes` directory, making it easy to extend the engine with new node types.

## Documentation System

The engine includes a built-in documentation system that auto-generates documentation for node methods. The documentation can be accessed in two ways:

1. Press **F1** to open the documentation popup
2. Go to **Help -> Documentation** in the top menu

### Documentation Features

- **Auto-generated documentation** from node class definitions
- **Search functionality** to quickly find methods and properties
- **Organized by node type** for easy navigation
- **Examples** for each method to help understand usage

### Documentation Structure

Each node method's documentation includes:

- Method name and signature
- Description
- Parameters with descriptions
- Return value description
- Usage examples

## Building the Engine

The engine uses SCons as its build system. To build the engine:

```bash
scons
```

## Running the Engine

After building, run the engine with:

```bash
./engine
```
