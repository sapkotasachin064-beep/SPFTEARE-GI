#pragma once
#include <windows.h>
#include <d3d11.h>
#include <imgui.h>

namespace GUI {
    extern bool isOpen;
    
    void Initialize(HWND window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Render();
    void Toggle();
}
