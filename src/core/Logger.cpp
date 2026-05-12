#include "Logger.h"

namespace Logger {
    FILE* console_file = nullptr;
    std::mutex log_mutex;

    void Initialize() {
        AllocConsole();
        freopen_s(&console_file, "CONOUT$", "w", stdout);
        freopen_s(&console_file, "CONOUT$", "w", stderr);
        freopen_s(&console_file, "CONIN$", "r", stdin);
        SetConsoleTitleA("SPFTEARE GI - Debug Console");
        
        Success("Logger initialized.");
    }

    void Shutdown() {
        if (console_file) {
            fclose(console_file);
        }
        FreeConsole();
    }

    void Log(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cout << "[LOG] " << message << std::endl;
    }

    void Error(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "[ERROR] " << message << std::endl;
    }

    void Success(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cout << "[SUCCESS] " << message << std::endl;
    }
}
