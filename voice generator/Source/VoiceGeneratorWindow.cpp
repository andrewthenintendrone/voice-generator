#include "VoiceGeneratorWindow.h"
#include "../resources/resource.h"
#include <random>
#include <chrono>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// constructor
VoiceGeneratorWindow::VoiceGeneratorWindow()
{
    // seed RNG
    unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    //m_RNG = std::default_random_engine(seed);
	srand(seed);

    // append resource path to soundstrings
    for (unsigned int i = 0; i < 39; i++)
    {
        m_soundStrings[i] = "./resources/voices/michaelrosen/" + m_soundStrings[i] + ".wav";
    }
    m_audioPlaying = false;
}

// process the text in the text box
void VoiceGeneratorWindow::processText()
{
    // clear the previous data
    m_words.clear();
    m_phonemes.clear();

    // get input from text box
    int bufferLength = GetWindowTextLength(m_textBox1) + 1;
    m_inputLine.resize(bufferLength);
    GetWindowText(m_textBox1, &m_inputLine[0], bufferLength);
    m_inputLine.resize(bufferLength - 1);

    // convert line to upper case
    transform(m_inputLine.begin(), m_inputLine.end(), m_inputLine.begin(), toupper);

    // parse line
    std::istringstream ss(m_inputLine);
    copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), std::back_inserter(m_words));

    // get the phonemes for all the words
    for (unsigned int currentPhoneme = 0; currentPhoneme < m_words.size(); currentPhoneme++)
    {
        m_phonemes.push_back(m_dictionary.entries[(m_words[currentPhoneme])]);
    }

    m_audioThread = std::thread(&VoiceGeneratorWindow::play, this);
    m_audioThread.detach();

    // set focus back to text box
    SetFocus(m_textBox1);
}

// play phoneme sound effects
void VoiceGeneratorWindow::play()
{
    m_audioPlaying = true;
    // play audio
    for (unsigned int currentWord = 0; currentWord < m_words.size(); currentWord++)
    {
        for (unsigned int currentPhoneme = 0; currentPhoneme < m_phonemes[currentWord].size(); currentPhoneme++)
        {
            if (m_buffer.loadFromFile(m_soundStrings[m_phonemes[currentWord][currentPhoneme]].c_str()))
            {
                m_sound.setBuffer(m_buffer);
                m_sound.play();
                while (m_sound.getStatus() == sf::Sound::Playing)
                {
                    if (!m_audioPlaying)
                    {
                        return;
                    }
                }
            }
        }
    }
    m_audioPlaying = false;
}

// 
void VoiceGeneratorWindow::create(char appName[], char className[], RECT r)
{
    HINSTANCE hinst = GetModuleHandle(NULL);

    std::uniform_int_distribution<int> range(0, 255);
    m_randomColor = RGB(rand() % 256, rand() % 256, rand() % 256);
    LoadIcon(hinst, MAKEINTRESOURCE(APPLICATION_ICON));

    /*  Fill in WNDCLASSEX struct members  */
    m_wndclass.cbSize = sizeof(m_wndclass);
    m_wndclass.style = CS_HREDRAW | CS_VREDRAW;
    m_wndclass.lpfnWndProc = WndProc;
    m_wndclass.cbClsExtra = 0;
    m_wndclass.cbWndExtra = sizeof(void*);
    m_wndclass.hInstance = hinst;
    m_wndclass.hIcon = createColoredIcon(m_randomColor);
    m_wndclass.hIconSm = createColoredIcon(m_randomColor);
    m_wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    m_wndclass.hbrBackground = CreateSolidBrush(m_randomColor); // randomly color window
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

    // store size of client rect along with width and height
    GetClientRect(m_hwnd, &m_clientRect);
    m_width = m_clientRect.right - m_clientRect.left;
    m_height = m_clientRect.bottom - m_clientRect.top;

    // create text box
    m_textBox1 = CreateWindowEx(
        NULL,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL,
        20, 20,
        m_width - 40, m_height / 2 - 40,
        m_hwnd, NULL,
        m_wndclass.hInstance,
        (LPVOID)this);

    // add cue text
    //Edit_SetCueBannerText(m_textBox1, L"Enter a word or phrase: ");

    EnableScrollBar(m_textBox1, SB_VERT, ESB_ENABLE_BOTH);

    // set focus to text box
    SetFocus(m_textBox1);

    m_button1 = CreateWindowEx(
        NULL,
        "BUTTON",  // Predefined class; Unicode assumed 
        "Say it",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        20,         // x position 
        m_height / 2 + 20,         // y position 
        m_width - 40,        // Button width
        m_height / 2 - 80,        // Button height
        m_hwnd,     // Parent window
        NULL,       // menu.
        m_wndclass.hInstance,
        (LPVOID)this);      // Pointer not needed.

    m_loadBar1 = CreateWindowEx(
        NULL,
        PROGRESS_CLASS,
        "LOADING",
        WS_VISIBLE | WS_CHILD,
        20, m_clientRect.bottom - 40,
        m_width - 40, 20,
        m_hwnd,
        NULL,
        m_wndclass.hInstance,
        (LPVOID)this);
    //SendMessage(m_loadBar1, PBM_SETPOS, 100, 0);
}

/*  Show and update our window  */
void VoiceGeneratorWindow::show()
{
    ShowWindow(m_hwnd, SW_NORMAL);
    UpdateWindow(m_hwnd);
    m_dictionary.load(m_loadBar1);
}

// called after the window is created
void VoiceGeneratorWindow::onCreate()
{
    
}

// called when the window is being painted
void VoiceGeneratorWindow::onPaint()
{

}

// called when the left mouse button is held down
void VoiceGeneratorWindow::onLeftMouseButtonDown(int xPos, int yPos)
{

}

// called when a button recieves a left mouse click
void VoiceGeneratorWindow::onLeftClickButton(HWND buttonID)
{
    if (buttonID == m_button1 && !m_audioPlaying)
    {
        processText();
    }
}

// called when the window is resized
void VoiceGeneratorWindow::onResize()
{
    // resize all windows
    GetClientRect(m_hwnd, &m_clientRect);
    m_width = m_clientRect.right - m_clientRect.left;
    m_height = m_clientRect.bottom - m_clientRect.top;
    SetWindowPos(m_textBox1, NULL, 20, 20, m_width - 40, m_height / 2 - 40, SWP_NOZORDER);
    SetWindowPos(m_button1, NULL, 20, m_height / 2 + 20, m_width - 40, m_height / 2 - 80, SWP_NOZORDER);
    SetWindowPos(m_loadBar1, NULL, 20, m_clientRect.bottom - 40, m_width - 40, 20, SWP_NOZORDER);

    // set text box margins
    RECT margins;
    GetClientRect(m_textBox1, &margins);
    margins.left += 10;
    margins.right -= 10;
    margins.top += 10;
    margins.bottom -= 10;
    SendMessage(m_textBox1, EM_SETRECT, 0, (LPARAM)&margins);
}

// called when the enter key is pressed
void VoiceGeneratorWindow::onPressEnter()
{
    if (!m_audioPlaying)
    {
        processText();
    }
}

// called when the escape key is pressed
void VoiceGeneratorWindow::onPressEscape()
{
	// call onClose
	onClose();
}

// called when the window is closed
void VoiceGeneratorWindow::onClose()
{
    m_audioPlaying = false;
    PostQuitMessage(0);
}

HICON VoiceGeneratorWindow::createColoredIcon(COLORREF iconColor)
{
    // get the application icon
    HICON appIcon = LoadIcon(m_wndclass.hInstance, MAKEINTRESOURCE(APPLICATION_ICON));

    // get info about the application icon
    ICONINFO appIconInfo;
    GetIconInfo(appIcon, &appIconInfo);

    // store icon mask in a bitmap
    BITMAP iconMask;
    GetObject(appIconInfo.hbmMask, sizeof(iconMask), &iconMask);

    // get width and height
    int width = iconMask.bmWidth;
    int height = iconMask.bmHeight;

    // Obtain a handle to the screen device context.
    HDC hdcScreen = GetDC(NULL);

    // Create a memory device context, which we will draw into.
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    // Create the bitmap, and select it into the device context for drawing.
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, width, height);
    HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmp);

    // Draw your icon.
    // 
    // For this simple example, we're just drawing a solid color rectangle
    // in the specified color with the specified dimensions.
    HPEN hpen = CreatePen(PS_SOLID, 1, iconColor);
    HPEN hpenOld = (HPEN)SelectObject(hdcMem, hpen);
    HBRUSH hbrush = CreateSolidBrush(iconColor);
    HBRUSH hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrush);
    Rectangle(hdcMem, 0, 0, width, height);
    SelectObject(hdcMem, hbrushOld);
    SelectObject(hdcMem, hpenOld);
    DeleteObject(hbrush);
    DeleteObject(hpen);

    // Create an icon from the bitmap.
    // 
    // Icons require masks to indicate transparent and opaque areas. Since this
    // simple example has no transparent areas, we use a fully opaque mask.
    HBITMAP hbmpMask = CreateCompatibleBitmap(hdcScreen, width, height);
    ICONINFO ii;
    ii.fIcon = TRUE;
    ii.hbmMask = appIconInfo.hbmMask;
    ii.hbmColor = hbmp;
    HICON hIcon = CreateIconIndirect(&ii);
    DeleteObject(hbmpMask);

    // Clean-up.
    SelectObject(hdcMem, hbmpOld);
    DeleteObject(hbmp);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    // Return the icon.
    return hIcon;
}