#pragma once

// include windows headers
#include <Windows.h>
#include <windowsx.h>
#include <Commctrl.h>

// enables fancy windows 7 buttons
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

class Window
{
    public:

        virtual void create(char appName[], char className[], RECT r) = 0 {};
        void show();
        void run();

        virtual void onCreate() = 0;
        virtual void onPaint() = 0;
        virtual void onLeftMouseButtonDown(int xPos, int yPos) = 0;
        virtual void onLeftClickButton(HWND buttonID) = 0;
        virtual void onResize() = 0;
        virtual void onPressEnter() = 0;
		virtual void onPressEscape() = 0;
		virtual void onClose() = 0;

		HWND getHWND() { return m_hwnd; }

    protected:
        Window() {};
        virtual ~Window() = 0 {};

        HWND m_hwnd;
        WNDCLASSEX  m_wndclass;
};