#pragma once
#include <windows.h>
#include <string>

namespace SDK {
    class Entity {
    public:
        // Basic entity structure placeholder
        // In GI, this would be an Il2CppObject or similar.
        
        float GetHealth() {
            return *(float*)((uintptr_t)this + 0x100); // Dummy offset
        }

        std::string GetName() {
            return "Entity"; // Dummy name
        }
    };
}
