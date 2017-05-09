#include "Window.h"
#include "myWindow.h"

/*  Window procedure  */
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = (Window*)GetWindowLong(hwnd, 0);

    /*  Switch according to what type of message we have received  */
    switch (iMsg)
    {
    case WM_NCCREATE:
    {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        SetWindowLong(hwnd, 0, (LONG)cs->lpCreateParams);
        break;
    }
    case WM_PAINT:
    {
        /*  We receive WM_PAINT every time window is updated  */
        if (window)
        {
            window->onPaint();
        }
        break;
    }

    case WM_LBUTTONDOWN:
    {

        if (window)
        {
            window->onLeftMouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        break;
    }

    case WM_COMMAND:
    {
        if (window)
        {
            if (((HWND)lParam) && (HIWORD(wParam) == BN_CLICKED))
            {
                window->onLeftClickButton((HWND)lParam);
            }
        }
        break;
    }

    case WM_SIZE:
    {
        if (window)
        {
            window->onResize();
        }
        break;
    }

    case WM_DESTROY:
        /*  Window has been destroyed, so exit cleanly  */
        if (window)
        {
            window->destroy();
        }
        return 0;
    }

    /*  Send any messages we don't handle to default window procedure  */
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void Window::create(char appName[], char className[], RECT r)
{
    HINSTANCE hinst = GetModuleHandle(NULL);

    /*  Fill in WNDCLASSEX struct members  */
    m_wndclass.cbSize = sizeof(m_wndclass);
    m_wndclass.style = CS_HREDRAW | CS_VREDRAW;
    m_wndclass.lpfnWndProc = WndProc;
    m_wndclass.cbClsExtra = 0;
    m_wndclass.cbWndExtra = sizeof(void*);
    m_wndclass.hInstance = hinst;
    m_wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    m_wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    m_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    m_wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    m_wndclass.lpszClassName = className;
    m_wndclass.lpszMenuName = NULL;

    /*  Register a new window class with Windows  */
    if (!RegisterClassEx(&m_wndclass)) return;

    /*  Create a window based on our new class  */
    m_hwnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        className,
        appName,
        WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_MAXIMIZEBOX,
        r.left, r.top,
        r.right - r.left, r.bottom - r.top,
        NULL, NULL,
        m_wndclass.hInstance,
        (LPVOID)this);
}

/*  Show and update our window  */
void Window::show()
{
    ShowWindow(m_hwnd, SW_NORMAL);
    UpdateWindow(m_hwnd);
}

/*  Retrieve and process messages until we get WM_QUIT  */
void Window::run()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void Window::destroy()
{
    PostQuitMessage(0);
}

HWND Window::getHWND()
{
    return m_hwnd;
}