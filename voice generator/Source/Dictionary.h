#pragma once
#include "Header.h"

class Entry
{
public:
    std::string word;
    std::vector<int> phonemes;
};

// the dictionary contains entries for every word
class Dictionary
{
public:
    Dictionary();
    ~Dictionary();

    std::vector<Entry> entries;

    std::vector<int> getPhonemes(const std::string words);
private:
    std::string checkStrings[39] = { " AA", " AE", " AH", " AO", " AW", " AY", " B ", " CH", " D ", " DH", " EH", " ER", " EY", " F ", " G ", " HH", " IH", " IY", " JH", " K ", " L ", " M ", " N ", " NG", " OW", " OY", " P ", " R ", " S ", " SH", " T ", " TH", " UH", " UW", " V ", " W ", " Y ", " Z ", " ZH" };
    std::ifstream dictionaryFile;

    void load();
    void fill();
};
