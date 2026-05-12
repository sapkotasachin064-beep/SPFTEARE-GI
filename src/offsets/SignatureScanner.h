#pragma once
#include <windows.h>
#include <vector>
#include <string>

namespace SignatureScanner {
    uintptr_t Scan(const char* module, const char* signature);
}
