#include "Window.h"

// window callback function
// this is called by windows when events occur
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// retrieve Window pointer from the HWND
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	// branch based on the message we have received
	switch (iMsg)
	{
		// WM_CREATE is the first message received from windows
		case WM_NCCREATE:
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			window = static_cast<Window*>(lpcs->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			break;
		}

		// WM_CREATE is received after the window is created by windows
		case WM_CREATE:
		{
			window->onCreate();
			break;
		}

		// WM_PAINT is received when the window is going to be repainted
		case WM_PAINT:
		{
			window->onPaint();
			break;
		}

		// WM_LBUTTONDOWN is recieved when the left mouse button is pressed
		case WM_LBUTTONDOWN:
		{
			window->onLeftMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}

		//  WM_COMMAND is received whenever the user interacts with the window
		case WM_COMMAND:
		{
			// a button has been clicked
			if (((HWND)lParam) && (HIWORD(wParam) == BN_CLICKED))
			{
				window->onLeftClickButton((HWND)lParam);
			}
			break;
		}

		// WM_SIZE is received whenever the window is resized
		case WM_SIZE:
		{
			window->onResize();
			break;
		}

		// WM_CLOSE is received when the window is closed
		case WM_CLOSE:
		{
			window->onClose();
			break;
		}
	}
    
    // Send any messages we don't handle to default window procedure
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

// show and update our window
void Window::show()
{
    ShowWindow(m_hwnd, SW_NORMAL);
    UpdateWindow(m_hwnd);
}

// Retrieve and process messages until we get WM_QUIT
void Window::run()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN)
            {
                onPressEnter();
            }
            else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
            {
                onPressEscape();
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}