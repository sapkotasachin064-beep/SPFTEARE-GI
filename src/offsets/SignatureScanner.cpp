#include "SignatureScanner.h"
#include <psapi.h>

namespace SignatureScanner {
    uintptr_t Scan(const char* moduleName, const char* signature) {
        auto module = GetModuleHandleA(moduleName);
        if (!module) return 0;

        MODULEINFO moduleInfo;
        GetModuleInformation(GetCurrentProcess(), module, &moduleInfo, sizeof(MODULEINFO));

        auto start = (uintptr_t)module;
        auto end = start + moduleInfo.SizeOfImage;

        auto parseSignature = [](const char* sig) {
            std::vector<int> bytes;
            char* start = const_cast<char*>(sig);
            char* end = const_cast<char*>(sig) + strlen(sig);

            for (char* cur = start; cur < end; ++cur) {
                if (*cur == '?') {
                    ++cur;
                    if (*cur == '?') ++cur;
                    bytes.push_back(-1);
                } else {
                    bytes.push_back(strtoul(cur, &cur, 16));
                }
            }
            return bytes;
        };

        auto pattern = parseSignature(signature);
        auto data = (unsigned char*)start;

        for (size_t i = 0; i < moduleInfo.SizeOfImage - pattern.size(); ++i) {
            bool found = true;
            for (size_t j = 0; j < pattern.size(); ++j) {
                if (pattern[j] != -1 && data[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            if (found) return start + i;
        }

        return 0;
    }
}
