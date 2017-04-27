#include "Header.h"
#include "Dictionary.h"

std::string soundStrings[39] = { "AA", "AE", "AH", "AO", "AW", "AY", "B", "CH", "D", "DH", "EH", "ER", "EY", "F", "G", "HH", "IH", "IY", "JH", "K", "L", "M", "N", "NG", "OW", "OY", "P", "R", "S", "SH", "T", "TH", "UH", "UW", "V", "W", "Y", "Z", "ZH" };

// plays a wav file
void play(int phoneme)
{
    std::string filename = "./rosen/" + soundStrings[phoneme] + ".wav";
    PlaySound(filename.c_str(), NULL, SND_SYNC | SND_FILENAME);
}

int main()
{
    // create dictionary
    Dictionary dictionary;

    std::string inputLine;

    std::vector<std::string> words;
    std::vector<std::vector<int>> phonemes;

    // get words from user
    while (true)
    {
        // clear the previous data
        words.clear();
        phonemes.clear();

        // get line from user
        std::cout << "Enter a word or phrase: ";
        std::getline(std::cin, inputLine);

        // convert line to upper case
        transform(inputLine.begin(), inputLine.end(), inputLine.begin(), ::toupper);

        // parse line
        std::istringstream ss(inputLine);
        copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), std::back_inserter(words));

        // get the phonemes for all the words
        for (unsigned int currentPhoneme = 0; currentPhoneme < words.size(); currentPhoneme++)
        {
            phonemes.push_back(dictionary.getPhonemes(words[currentPhoneme]));
        }

        // play audio
        for (unsigned int currentWord = 0; currentWord < words.size(); currentWord++)
        {
            for (unsigned int currentPhoneme = 0; currentPhoneme < phonemes[currentWord].size(); currentPhoneme++)
            {
                //std::cout << soundStrings[phonemes[currentWord][currentPhoneme]] + " ";
                play(phonemes[currentWord][currentPhoneme]);
            }
            //std::cout << std::endl;
        }
    }

    return 0;
}