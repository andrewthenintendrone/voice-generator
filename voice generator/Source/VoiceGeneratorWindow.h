#pragma once
#include "Header.h"
#include "Dictionary.h"
#include <SFML\Audio.hpp>
#include "Window.h"

class VoiceGeneratorWindow : public Window
{
public:

	VoiceGeneratorWindow();
    ~VoiceGeneratorWindow() {};

    void create(char appName[], char className[], RECT r);
    void show();

    virtual void onCreate();
    virtual void onPaint();
    virtual void onLeftMouseButtonDown(int xPos, int yPos);
    virtual void onLeftClickButton(HWND buttonID);
    virtual void onResize();
    virtual void onPressEnter();
	virtual void onPressEscape();
    virtual void onClose();

    void processText();
    void play();

private:

    HWND m_textBox1;
    HWND m_button1;
    HWND m_loadBar1;
    RECT m_clientRect;
    int m_width;
    int m_height;

    COLORREF m_randomColor;
    HICON m_applicationIcon;
    HICON createColoredIcon(COLORREF iconColor);

    std::string m_soundStrings[39] = { "AA", "AE", "AH", "AO", "AW", "AY", "B", "CH", "D", "DH", "EH", "ER", "EY", "F", "G", "HH", "IH", "IY", "JH", "K", "L", "M", "N", "NG", "OW", "OY", "P", "R", "S", "SH", "T", "TH", "UH", "UW", "V", "W", "Y", "Z", "ZH" };
    std::thread m_audioThread;
    sf::SoundBuffer m_buffer;
    sf::Sound m_sound;
    bool m_audioPlaying = false;

    std::string m_inputLine;
    std::vector<std::string> m_words;
    std::vector<std::vector<int>> m_phonemes;

    Dictionary m_dictionary;
};