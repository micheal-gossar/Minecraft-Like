# Space Engine: DirectX 12 box

A small, independently written DirectX 12 starter rendering a colored 3D box.
It is not the source code from Frank Luna's book. Drag the left mouse button to
orbit the camera; use the mouse wheel to zoom. Close the window to exit.

## Requirements

- Windows 10 or later, a DirectX 12-capable graphics driver, and the Windows SDK.
- Visual Studio 2026 with the **Desktop development with C++** workload, MSVC
  v145 and CMake tools. The project selects the latest installed Windows SDK.
- Git and an internet connection during the initial CMake configure.

MSVC's `/std:c++latest` enables its latest available C++ language features,
including its implemented C++26 features. It does not imply complete C++26
conformance. DirectXMath, D3DCompiler, DXGI, and Direct3D 12 come from the
Windows SDK. CMake downloads Microsoft's MIT-licensed DirectX-Headers v1.620.2,
pinned to a commit, including the `d3dx12` helper library. DirectX 12 uses the
Windows runtime and installed driver; this project does not ship the optional
Agility SDK runtime.

## Build and run

From a Visual Studio Developer PowerShell or Developer Command Prompt:

```powershell
cmake --preset vs2026
cmake --build --preset debug
.\out\build\vs2026\Debug\SpaceEngine.exe
ctest --preset debug
```

Alternatively open the folder in Visual Studio or open the generated
`out\build\vs2026\SpaceEngine.slnx` solution. Select `SpaceEngine` as the startup
project if necessary.

In VS Code, use **Terminal > Run Task** to select **Build**, **Build and Run**,
or **Clean**. None is configured as the default build task. The tasks use the
Debug configuration and Visual Studio 2026's bundled CMake. Build and Build and
Run configure the project automatically; Build and Run opens the game in a
separate PowerShell window so logger output is visible there. Clean removes
Debug build outputs but keeps the configured build directory and downloaded
dependencies.

## Headers and IntelliSense

Put project headers under `headers`, including subdirectories. CMake adds
`.h`, `.hpp`, `.hxx`, and `.inl` files to the Visual Studio project and
automatically detects additions and removals on the next build. Include them
explicitly in your source, for example `#include "graphics/renderer.h"`.
Header files are not automatically included in every source file, and new
`.cpp` files must be added to `CMakeLists.txt`.

In VS Code, install the recommended C/C++ and CMake Tools extensions and select
the `vs2026` configure preset when prompted. CMake Tools supplies the target's
MSVC compiler, SDK, DirectX helper include paths and definitions to C/C++
IntelliSense. The workspace disables clangd to avoid conflicting C++ language
servers. The CMake path in `.vscode/settings.json` points to Visual Studio 2026
Community's default installation; adjust it if your installation differs.
Configure the project after changing its build settings.

The test creates a DirectX 12 device, renders the box into a back buffer, copies
the image back to the CPU, and checks that the box and background are present.
The test needs a working graphics device and driver; it fails explicitly on
initialization, shader compilation, rendering, or validation errors. It uses
the default hardware adapter rather than silently falling back to software.

To update Visual Studio and the Windows SDK, use Visual Studio Installer.
This project uses the versions installed on the machine rather than changing
system-wide tools automatically.
