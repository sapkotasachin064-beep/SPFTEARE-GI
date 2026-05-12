#pragma once
#include <d3d11.h>

namespace Renderer {
    inline ID3D11Device* pDevice = nullptr;
    inline ID3D11DeviceContext* pContext = nullptr;
    inline ID3D11RenderTargetView* pMainRenderTargetView = nullptr;
}
