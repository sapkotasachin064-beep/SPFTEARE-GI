#pragma once
#include <windows.h>
#include <d3d11.h>

namespace Hooks {
    bool Initialize();
    void Shutdown();

    // Hooked functions
    typedef HRESULT(STDMETHODCALLTYPE* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    typedef HRESULT(STDMETHODCALLTYPE* ResizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

    extern Present OriginalPresent;
    extern ResizeBuffers OriginalResizeBuffers;
    extern WNDPROC OriginalWndProc;

    HRESULT STDMETHODCALLTYPE HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    HRESULT STDMETHODCALLTYPE HookedResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}
