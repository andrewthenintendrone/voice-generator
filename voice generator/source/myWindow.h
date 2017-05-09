#pragma once
#include "Window.h"

class myWindow : public Window
{
public:
    myWindow() {};
    ~myWindow() {};

    void create(char appName[], char className[], RECT r);

    virtual void onCreate();
    virtual void onDestroy();
    virtual void onPaint();
    virtual void onLeftMouseButtonDown(int xPos, int yPos);
    virtual void onLeftClickButton(HWND buttonID);
    virtual void onResize();
protected:
private:
    HWND m_textBox1;
    HWND m_button1;
    HWND m_loadBar1;
    RECT m_clientRect;;
    int m_width;
    int m_height;
};