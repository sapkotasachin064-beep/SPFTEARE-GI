#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>

namespace Logger {
    void Initialize();
    void Shutdown();
    void Log(const std::string& message);
    void Error(const std::string& message);
    void Success(const std::string& message);
}
