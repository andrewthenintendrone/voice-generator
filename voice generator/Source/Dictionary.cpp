#include "Dictionary.h"

// sets up when loaded
Dictionary::Dictionary()
{
    load();
}

// closes dictionary file when destroyed
Dictionary::~Dictionary()
{
    dictionaryFile.close();
}

// attempts to load dictionary file
void Dictionary::load()
{
    dictionaryFile.open("./resources/dictionary/extended dictionary.txt", std::ios_base::in | std::ios_base::_Nocreate);

    if (dictionaryFile.is_open())
    {
        //std::cout << "opened dictionary" << std::endl;
        fill();
        dictionaryFile.close();
    }
    else
    {
        std::cerr << "unable to open dictionary";
        throw;
    }
}

// fills in entries
void Dictionary::fill()
{
    Entry currentEntry;

    std::string currentLine;
    int currentWordLength = 0;

    // read every line of the file
    while (std::getline(dictionaryFile, currentLine))
    {
        // if the line is not a comment
        if (currentLine.substr(0, 3) != ";;;")
        {
            // clear the last entry
            currentEntry.phonemes.clear();

            // store the word
            currentWordLength = currentLine.find("  ");
            currentEntry.word = currentLine.substr(0, currentWordLength);

            // add a space to stop overflows
            currentLine += " ";

            for (unsigned int currentChar = currentWordLength + 1; currentChar < currentLine.length() - 2; currentChar++)
            {
                std::string currentSlice = currentLine.substr(currentChar, 3);
                for (unsigned int soundIndex = 0; soundIndex < 39; soundIndex++)
                {
                    if (currentSlice == checkStrings[soundIndex])
                    {
                        currentEntry.phonemes.push_back(soundIndex);
                        soundIndex = 39;
                        currentChar++;
                    }
                }
            }
        }
        entries.push_back(currentEntry);
    }
    //std::cout << "Loaded " << entries.size() << " words from dictionary" << std::endl;
}

// returns the phonemes for the given word
std::vector<int> Dictionary::getPhonemes(std::string word)
{
    std::string currentLine;

    // get the right line
    for (unsigned int entryIndex = 0; entryIndex < entries.size(); entryIndex++)
    {
        if (entries[entryIndex].word == word)
        {
            return entries[entryIndex].phonemes;
        }
    }

    std::vector<int> phonemes;
    return phonemes;
}