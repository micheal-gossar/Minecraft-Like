#include <windows.h>
#include "Window/window.hpp"



int main() {
    Window window(GetModuleHandleW(nullptr));

    window.RenderBasicWindow(L"First Window");

    ShowWindow(window.Handle(), SW_SHOWDEFAULT);

    MSG msg = {};
    while (true) {
        const BOOL result = GetMessage(&msg, nullptr, 0, 0);
        if (result == -1) {
            return 1;
        }
        if (result == 0) {
            return static_cast<int>(msg.wParam);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
