import os, sys
from SCons.Script import Environment, Glob, Default, Exit

env = Environment(ENV=os.environ)
env.Append(CPPPATH=['include'])
env.Append(CPPPATH=['vendor/imgui', 'vendor/imgui/backends'])

if sys.platform.startswith('win'):
    vulkan_sdk = env['ENV'].get('VULKAN_SDK')
    if not vulkan_sdk:
        print("Please set VULKAN_SDK environment variable to Vulkan SDK path.")
        Exit(1)
    env.Append(CPPPATH=[os.path.join(vulkan_sdk, 'Include')])
    env.Append(LIBPATH=[os.path.join(vulkan_sdk, 'Lib')])
    env.Append(LIBS=['vulkan-1', 'glfw3'])
elif sys.platform.startswith('linux'):
    # GLFW: compile flags and link flags via pkg-config
    import subprocess
    try:
        flags = subprocess.check_output(['pkg-config', '--cflags', '--libs', 'glfw3']).decode().split()
    except Exception as e:
        print("pkg-config for glfw3 failed:", e)
        Exit(1)
    for flag in flags:
        if flag.startswith('-I'):
            env.Append(CPPPATH=[flag[2:]])
        elif flag.startswith('-L'):
            env.Append(LIBPATH=[flag[2:]])
        elif flag.startswith('-l'):
            env.Append(LIBS=[flag[2:]])
    # Vulkan
    env.Append(LIBS=['vulkan'])
else:
    print("Unsupported platform:", sys.platform)
    Exit(1)

# Add nodes directory to include path
env.Append(CPPPATH=['nodes'])

sources = Glob('src/*.cpp')
sources += Glob('nodes/*/*.cpp')  # Include all node implementation files
sources += Glob('vendor/imgui/*.cpp')
sources += ['vendor/imgui/backends/imgui_impl_vulkan.cpp', 'vendor/imgui/backends/imgui_impl_glfw.cpp']
program = env.Program('engine', sources)
Default(program)