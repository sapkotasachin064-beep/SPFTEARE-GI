#include "Utils.h"
#include <thread>
#include <chrono>

namespace Utils {
    uintptr_t GetModuleBase(const std::string& moduleName) {
        return (uintptr_t)GetModuleHandleA(moduleName.c_str());
    }

    bool IsModuleLoaded(const std::string& moduleName) {
        return GetModuleHandleA(moduleName.c_str()) != nullptr;
    }

    void WaitForModule(const std::string& moduleName) {
        while (!IsModuleLoaded(moduleName)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
