#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <windows.h>
#include <stdexcept>
#include "Logger/logger.h"

static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
class Window {
public:
    [[nodiscard]] HWND Handle() {
        return hwnd;
    }
    Window(HINSTANCE hI);
    void RenderBasicWindow(LPCWSTR windowName);

private:
Logger logger;
LogStream windowLogger;
HWND hwnd{};
HINSTANCE hInstance;
};

Window::Window(HINSTANCE hI) :
windowLogger(logger.CreateStream("Window")),
hInstance(hI) {}

void Window::RenderBasicWindow(LPCWSTR windowName)
{
    const wchar_t CLASS_NAME[] = L"Main Window";

    WNDCLASS wc = {};

    wc.lpfnWndProc = MainWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0, // window styles
        CLASS_NAME,
        windowName,
        WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, //Parent Window
        NULL, //Menu
        hInstance,
        NULL); //Additional ApplicationData
        if (hwnd == NULL) {
            throw std::runtime_error("Failed to create the window.");
        }
        windowLogger.Debug("Window Handle successfully created");
}
//hardcoded proc for now
static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);


            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif