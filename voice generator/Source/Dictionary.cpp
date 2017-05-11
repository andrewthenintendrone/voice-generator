#include "Dictionary.h"

// sets up when loaded
Dictionary::Dictionary()
{

}

// closes dictionary file when destroyed
Dictionary::~Dictionary()
{
    if (dictionaryFile)
    {
        dictionaryFile.close();
    }
}

// attempts to load dictionary file
void Dictionary::load(HWND loadBar)
{
    // store loadBar
    m_loadBar = loadBar;

    dictionaryFile.open("./resources/dictionary/dictionary.txt", std::ios_base::in | std::ios_base::_Nocreate);

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

    unsigned int numEntries = 0;

    while (std::getline(dictionaryFile, currentLine))
    {
        // if the line is not a comment
        if (currentLine.substr(0, 3) != ";;;")
        {
            numEntries++;
        }
    }

    SendMessage(m_loadBar, PBM_SETRANGE32, 0, numEntries);
    SendMessage(m_loadBar, PBM_SETSTEP, 1, 0);

    dictionaryFile.clear();
    dictionaryFile.seekg(0, std::ios::beg);

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
            entries.push_back(currentEntry);
            SendMessage(m_loadBar, PBM_STEPIT, 0, 0);
            Sleep(0);
        }
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