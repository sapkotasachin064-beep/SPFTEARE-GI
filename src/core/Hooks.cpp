#include "Hooks.h"
#include "Logger.h"
#include "Renderer.h"
#include "../gui/gui.h"
#include <kiero.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <stdexcept>
#include <thread>

// Forward declare message handler from imgui_impl_win32.cpp
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks {
    Present OriginalPresent = nullptr;
    ResizeBuffers OriginalResizeBuffers = nullptr;
    WNDPROC OriginalWndProc = nullptr;
    HWND window = nullptr;

    bool Initialize() {
        Logger::Log("Initializing Kiero hooks...");
        
        int retries = 0;
        kiero::Status::Enum status = kiero::Status::UnknownError;
        
        while (retries < 10) {
            status = kiero::init(kiero::RenderType::D3D11);
            if (status == kiero::Status::Success) break;
            
            // Reduced retry delay for faster loading
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            retries++;
        }

        if (status != kiero::Status::Success) {
            Logger::Error("Kiero failed to initialize after retries.");
            return false;
        }

        Logger::Log("Kiero initialized. Binding Present and ResizeBuffers...");

        if (kiero::bind(8, (void**)&OriginalPresent, (void*)HookedPresent) != kiero::Status::Success ||
            kiero::bind(13, (void**)&OriginalResizeBuffers, (void*)HookedResizeBuffers) != kiero::Status::Success) {
            Logger::Error("Kiero failed to bind hooks.");
            return false;
        }

        Logger::Success("Hooks initialized via Kiero successfully.");
        return true;
    }

    void Shutdown() {
        Logger::Log("Shutting down hooks.");
        kiero::shutdown();

        if (OriginalWndProc && window) {
            SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)OriginalWndProc);
        }
    }

    HRESULT STDMETHODCALLTYPE HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
        static bool initialized = false;
        if (!initialized) {
            Logger::Log("HookedPresent: First call detected. Initializing ImGui...");
            
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Renderer::pDevice))) {
                Renderer::pDevice->GetImmediateContext(&Renderer::pContext);
                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                window = sd.OutputWindow;
                
                ID3D11Texture2D* pBackBuffer = nullptr;
                if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) {
                    Renderer::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &Renderer::pMainRenderTargetView);
                    pBackBuffer->Release();
                    
                    OriginalWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
                    
                    GUI::Initialize(window, Renderer::pDevice, Renderer::pContext);
                    initialized = true;
                    Logger::Success("GUI and ImGui initialized in HookedPresent callback.");
                } else {
                    Logger::Error("Failed to get SwapChain buffer.");
                }
            } else {
                Logger::Error("Failed to get D3D11 Device from SwapChain.");
            }
        }

        if (initialized && Renderer::pContext && Renderer::pMainRenderTargetView) {
            Renderer::pContext->OMSetRenderTargets(1, &Renderer::pMainRenderTargetView, NULL);
            GUI::Render();
        }

        return OriginalPresent(pSwapChain, SyncInterval, Flags);
    }

    HRESULT STDMETHODCALLTYPE HookedResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
        Logger::Log("HookedResizeBuffers called. Releasing RenderTargetView...");
        
        if (Renderer::pMainRenderTargetView) {
            Renderer::pContext->OMSetRenderTargets(0, 0, 0);
            Renderer::pMainRenderTargetView->Release();
            Renderer::pMainRenderTargetView = nullptr;
        }

        HRESULT hr = OriginalResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

        ID3D11Texture2D* pBackBuffer = nullptr;
        if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) {
            Renderer::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &Renderer::pMainRenderTargetView);
            pBackBuffer->Release();
            Logger::Log("RenderTargetView recreated after ResizeBuffers.");
        }

        return hr;
    }

    LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (GUI::isOpen && ::ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;

        return CallWindowProc(OriginalWndProc, hWnd, uMsg, wParam, lParam);
    }
}
