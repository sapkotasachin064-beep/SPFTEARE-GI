#pragma once
#include <windows.h>
#include <string>
#include <psapi.h>

namespace Utils {
    uintptr_t GetModuleBase(const std::string& moduleName);
    bool IsModuleLoaded(const std::string& moduleName);
    void WaitForModule(const std::string& moduleName);
}
