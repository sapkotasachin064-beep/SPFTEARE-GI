#pragma once
#include <windows.h>
#include <atomic>

class App {
public:
    static App& Get() {
        static App instance;
        return instance;
    }

    void Initialize(HMODULE hModule);
    void Shutdown();

    bool IsRunning() const { return m_Running; }
    HMODULE GetModule() const { return m_hModule; }

private:
    App() = default;
    ~App() = default;

    std::atomic<bool> m_Running{ false };
    HMODULE m_hModule{ nullptr };
};
