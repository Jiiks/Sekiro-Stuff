#include <Windows.h>

DWORD64 TUTORIAL_MSG_DIALOG = 0x140DC7A8E;
DWORD64 CS_TUTORIAL_DIALOG = 0x140D6EB98;
DWORD64 CS_MENU_TUTORIAL_DIALOG = 0x140D6D51C;
const char NEW_BYTES[] = { 0x75 };

DWORD WINAPI run_thread(LPVOID param) {
    Sleep(10000);

    auto addr = reinterpret_cast<void *>(TUTORIAL_MSG_DIALOG);
    const auto size = 1;
    DWORD old;
    VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &old);
    memcpy(addr, NEW_BYTES, size);
    VirtualProtect(addr, size, old, &old);

    addr = reinterpret_cast<void *>(CS_TUTORIAL_DIALOG);
    VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &old);
    memcpy(addr, NEW_BYTES, size);
    VirtualProtect(addr, size, old, &old);

    addr = reinterpret_cast<void *>(CS_MENU_TUTORIAL_DIALOG);
    VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &old);
    memcpy(addr, NEW_BYTES, size);
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

