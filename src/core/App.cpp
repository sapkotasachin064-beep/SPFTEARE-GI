#include "App.h"
#include "Logger.h"
#include "Utils.h"
#include "Hooks.h"
#include "../gui/gui.h"
#include <thread>

void App::Initialize(HMODULE hModule) {
    m_hModule = hModule;
    m_Running = true;

    Logger::Initialize();
    Logger::Log("Initializing SPFTEARE GI...");

    try {
        // Wait for game modules with a timeout
        Logger::Log("Waiting for game modules (UnityPlayer.dll or UserAssembly.dll)...");
        int attempts = 0;
        bool modulesLoaded = false;
        
        while (attempts < 200) { // Increase timeout to 100 seconds
            if (Utils::IsModuleLoaded("UserAssembly.dll") || Utils::IsModuleLoaded("UnityPlayer.dll")) {
                modulesLoaded = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            attempts++;
            
            if (attempts % 20 == 0) {
                Logger::Log("Still waiting for modules... (" + std::to_string(attempts / 2) + "s)");
            }
        }

        if (!modulesLoaded) {
            Logger::Error("Game modules not found after timeout. Aborting.");
            m_Running = false;
        } else {
            Logger::Success("Game modules detected!");
            
            // Reduced delay for faster loading
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            // Initialize Hooks
            if (Hooks::Initialize()) {
                Logger::Success("Hooks and GUI initialized successfully.");
            } else {
                Logger::Error("Failed to initialize hooks after retries.");
                m_Running = false;
            }
        }

        // Main loop
        while (m_Running) {
            if (GetAsyncKeyState(VK_END) & 0x8000) {
                Logger::Log("End key pressed. Shutting down...");
                m_Running = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const std::exception& e) {
        Logger::Error("Critical Exception in Main Loop: " + std::string(e.what()));
    }

    Shutdown();
}

void App::Shutdown() {
    Logger::Log("Shutting down...");
    Hooks::Shutdown();
    Logger::Log("Hooks detached.");
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Logger::Shutdown();
    
    FreeLibraryAndExitThread(m_hModule, 0);
}
