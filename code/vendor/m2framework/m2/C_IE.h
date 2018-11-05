#include <Common.h>

class C_IE {
public:
    static uintptr_t Alloc(uint32_t size) {
        return nio::call(0x401830, size);
    }
};
