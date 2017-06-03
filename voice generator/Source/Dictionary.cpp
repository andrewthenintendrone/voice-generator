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
    std::vector<int> currentEntry;

    std::string currentLine;
    int currentWordLength = 0;

    unsigned int numLines = 0;
    dictionaryFile.unsetf(std::ios_base::skipws);

    while (std::getline(dictionaryFile, currentLine))
    {
        numLines++;
    }

    SendMessage(m_loadBar, PBM_SETRANGE32, 0, numLines);
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
            currentEntry.clear();

            // store the word
            currentWordLength = currentLine.find("  ");
            std::string word = currentLine.substr(0, currentWordLength);
            //currentEntry.word = currentLine.substr(0, currentWordLength);

            // add a space to stop overflows
            currentLine += " ";

            for (unsigned int currentChar = currentWordLength + 1; currentChar < currentLine.length() - 2; currentChar++)
            {
                std::string currentSlice = currentLine.substr(currentChar, 3);
                for (unsigned int soundIndex = 0; soundIndex < 39; soundIndex++)
                {
                    if (currentSlice == checkStrings[soundIndex])
                    {
                        currentEntry.push_back(soundIndex);
                        soundIndex = 39;
                        currentChar++;
                    }
                }
            }
            entries[word] = currentEntry;
        }
        SendMessage(m_loadBar, PBM_STEPIT, 0, 0);
        Sleep(0);
    }
    //std::cout << "Loaded " << entries.size() << " words from dictionary" << std::endl;
}