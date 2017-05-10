#include "myWindow.h"
#include "../resources/resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

myWindow::myWindow()
{
    // append resource path to soundstrings
    for (unsigned int i = 0; i < 39; i++)
    {
        m_soundStrings[i] = "./resources/voices/andrew/" + m_soundStrings[i] + ".wav";
    }
}

void myWindow::processText()
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
        m_phonemes.push_back(m_dictionary.getPhonemes(m_words[currentPhoneme]));
    }

    // play audio
    for (unsigned int currentWord = 0; currentWord < m_words.size(); currentWord++)
    {
        for (unsigned int currentPhoneme = 0; currentPhoneme < m_phonemes[currentWord].size(); currentPhoneme++)
        {
            play(m_phonemes[currentWord][currentPhoneme]);
        }
    }

    // set focus back to button
    //SetFocus(m_button1);
}

// plays a wav file
void myWindow::play(int phoneme)
{
    if (!m_buffer.loadFromFile(m_soundStrings[phoneme].c_str()))
    {
        return;
    }
    m_sound.setBuffer(m_buffer);
    m_sound.play();
    while (m_sound.getStatus() == sf::Sound::Playing)
    {
        // don't continue until the sound finishes
    }
}

void myWindow::create(char appName[], char className[], RECT r)
{
    HINSTANCE hinst = GetModuleHandle(NULL);

    /*  Fill in WNDCLASSEX struct members  */
    m_wndclass.cbSize = sizeof(m_wndclass);
    m_wndclass.style = CS_HREDRAW | CS_VREDRAW;
    m_wndclass.lpfnWndProc = WndProc;
    m_wndclass.cbClsExtra = 0;
    m_wndclass.cbWndExtra = sizeof(void*);
    m_wndclass.hInstance = hinst;
    m_wndclass.hIcon = LoadIcon(hinst, MAKEINTRESOURCE(MIC_ICON));
    m_wndclass.hIconSm = LoadIcon(hinst, MAKEINTRESOURCE(MIC_ICON));
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

    // color window
    HBRUSH brush = CreateSolidBrush(RGB(30, 30, 30));
    SetClassLongPtr(m_hwnd, GCLP_HBRBACKGROUND, (LONG)brush);

    // store size of client rect along with width and height
    GetClientRect(m_hwnd, &m_clientRect);
    m_width = m_clientRect.right - m_clientRect.left;
    m_height = m_clientRect.bottom - m_clientRect.top;

    m_textBox1 = CreateWindowEx(
        NULL,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN,
        20, 20,
        m_width - 40, m_height / 2 - 40,
        m_hwnd, NULL,
        m_wndclass.hInstance,
        (LPVOID)this);

    Edit_SetCueBannerText(m_textBox1, L"Enter a word or phrase: ");
    // set focus to edit box
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

void myWindow::onCreate()
{

}

void myWindow::onDestroy()
{

}

void myWindow::onPaint()
{

}

void myWindow::onLeftMouseButtonDown(int xPos, int yPos)
{

}

void myWindow::onLeftClickButton(HWND buttonID)
{
    if (buttonID == m_button1)
    {
        processText();
    }
}

void myWindow::onResize()
{
    GetClientRect(m_hwnd, &m_clientRect);
    m_width = m_clientRect.right - m_clientRect.left;
    m_height = m_clientRect.bottom - m_clientRect.top;
    SetWindowPos(m_textBox1, NULL, 20, 20, m_width - 40, m_height / 2 - 40, SWP_NOZORDER);
    SetWindowPos(m_button1, NULL, 20, m_height / 2 + 20, m_width - 40, m_height / 2 - 80, SWP_NOZORDER);
    SetWindowPos(m_loadBar1, NULL, 20, m_clientRect.bottom - 40, m_width - 40, 20, SWP_NOZORDER);
}

void myWindow::onPressEnter()
{
    processText();
}