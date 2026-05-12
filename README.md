# SPFTEARE GI v1.0

A clean, stable, and modern C++20 internal base for Genshin Impact.

## Project Structure

- `src/core/`: Core logic, initialization, hooking, and logging.
- `src/gui/`: ImGui menu and theme implementation.
- `src/features/`: Placeholder for future features.
- `src/offsets/`: Signature scanning and offset management.
- `src/sdk/`: Basic game SDK.
- `libs/`: Third-party libraries (MinHook, ImGui).

## Setup Instructions

1. **Visual Studio 2022**:
   - Create a new **Empty Project (C++)**.
   - Set Name to `SPFTEARE-GI`.
   - Change Project Type to **Dynamic Link Library (.dll)**.
   - Set Configuration to **x64 Release**.

2. **Project Properties**:
   - **General**:
     - C++ Language Standard: **ISO C++20 Standard (/std:c++20)**.
     - Character Set: **Use Multi-Byte Character Set**.
   - **C/C++ -> General -> Additional Include Directories**:
     - `$(ProjectDir)src`
     - `$(ProjectDir)libs`
     - `$(ProjectDir)include`
   - **Linker -> General -> Additional Library Directories**:
     - `$(ProjectDir)libs`
   - **Linker -> Input -> Additional Dependencies**:
     - `d3d11.lib`
     - `dxgi.lib`

3. **Dependencies**:
   - Download [MinHook](https://github.com/TsudaKageyu/minhook) and place `MinHook.h` and its library/source in `libs/MinHook/`.
   - Download [ImGui](https://github.com/ocornut/imgui) and place the core files + `imgui_impl_win32` and `imgui_impl_dx11` in `libs/ImGui/`.

## Build Instructions

1. Open the solution in Visual Studio 2022.
2. Add all `.cpp` and `.h` files from the `src` folder to the project.
3. Build the project in **Release x64**.
4. The resulting `SPFTEARE-GI.dll` can be injected into Genshin Impact using any standard injector.

## Safety Note

This base is designed for stability and safe rendering. It does not include any risky features like godmode or fly. Always use with caution and on an alt account if testing.
