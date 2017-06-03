#pragma once
#include "Header.h"
#include <unordered_map>

// the dictionary contains entries for every word
class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    void load(HWND loadBar);

    //HashTable<std::vector<int>> entries = HashTable<std::vector<int>>(200000);
    std::unordered_map<std::string, std::vector<int>>entries;
private:
    HWND m_loadBar;

    std::string checkStrings[39] = { " AA", " AE", " AH", " AO", " AW", " AY", " B ", " CH", " D ", " DH", " EH", " ER", " EY", " F ", " G ", " HH", " IH", " IY", " JH", " K ", " L ", " M ", " N ", " NG", " OW", " OY", " P ", " R ", " S ", " SH", " T ", " TH", " UH", " UW", " V ", " W ", " Y ", " Z ", " ZH" };
    std::ifstream dictionaryFile;

    void fill();
};
