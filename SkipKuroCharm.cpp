#include <Windows.h>
#include <vector>

DWORD64 ORIG_PTR = 0x1408F947B;

const char proxy[] = {
    0x81, 0xFB, 0xE4, 0x0C, 0x00, 0x00,
    0x74, 0x0E,
    0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x90, 0x7B, 0x68, 0x40, 0x01, 0x00, 0x00, 0x00,
    0xC3
};

template<typename T>
std::vector<unsigned char> NumToBytesReverse(T value) {
    std::vector<unsigned char> result;
    const int size = sizeof(T) * 8;
    for(auto i = 0; i < size; i += 8)
        result.push_back(value >> i);
    return result;
}

DWORD WINAPI run_thread(LPVOID param) {
    Sleep(10000);
    auto ptr = VirtualAlloc(reinterpret_cast<LPVOID>(0x13FFF0000), 256, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(ptr, proxy, 256);

    const auto size = 5;

    const auto addr = reinterpret_cast<void *>(ORIG_PTR);
    auto bytes = NumToBytesReverse(static_cast<int>(reinterpret_cast<int>(ptr)) - (ORIG_PTR + size));
    bytes.insert(bytes.begin(), 0xE8);

    DWORD old;
    VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &old);
    memcpy(addr, bytes.data(), size);
    VirtualProtect(addr, size, old, &old);

    return 0;
}

extern "C" {
    int __declspec(dllexport) __stdcall postAttach() {
        DWORD tmp;
        CreateThread(NULL, 0, run_thread, NULL, 0, &tmp);
        return 0;
    }
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
) {
    return TRUE;
}

