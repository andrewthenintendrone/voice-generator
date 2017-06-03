#include "Header.h"
#include "Dictionary.h"
#include <SFML\Audio.hpp>


std::string soundStrings[39] = { "AA", "AE", "AH", "AO", "AW", "AY", "B", "CH", "D", "DH", "EH", "ER", "EY", "F", "G", "HH", "IH", "IY", "JH", "K", "L", "M", "N", "NG", "OW", "OY", "P", "R", "S", "SH", "T", "TH", "UH", "UW", "V", "W", "Y", "Z", "ZH" };
sf::SoundBuffer buffer;
sf::Sound sound;

// plays a wav file
void play(int phoneme)
{
    if (!buffer.loadFromFile(soundStrings[phoneme].c_str()))
    {
        std::cout << "Can't find file " << soundStrings[phoneme] << std::endl;
        return;
    }
    sound.setBuffer(buffer);
    sound.play();
    while (sound.getStatus() == sf::Sound::Playing)
    {
        // don't continue until the sound finishes
    }
}

// plays a wav file badly
void play_old(int phoneme)
{
    PlaySound(soundStrings[phoneme].c_str(), NULL, SND_SYNC | SND_FILENAME);
}

int main()
{
    // append resource path to soundstrings
    for (unsigned int i = 0; i < 39; i++)
    {
        soundStrings[i] = "./resources/voices/andrew/" + soundStrings[i] + ".wav";
    }

    // create dictionary
    Dictionary dictionary;

    std::string inputLine;

    std::vector<std::string> words;
    std::vector<std::vector<int>> phonemes;

    // Intro
    std::cout << "Voice Generator\n";

    // tel the user how to exit
    std::cout << "To exit the program type \"EXIT\"\n\n";

    // get words from user
    while (true)
    {
        // clear the previous data
        words.clear();
        phonemes.clear();

        // get line from user
        std::cout << "Enter a word or phrase: ";
        std::getline(std::cin, inputLine);

        // escape loop and close program if user types "EXIT"
        if (inputLine == "EXIT")
        {
            break;
        }

        // convert line to upper case
        transform(inputLine.begin(), inputLine.end(), inputLine.begin(), ::toupper);

        // parse line
        std::istringstream ss(inputLine);
        copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), std::back_inserter(words));

        // get the phonemes for all the words
        for (unsigned int currentPhoneme = 0; currentPhoneme < words.size(); currentPhoneme++)
        {
            phonemes.push_back(dictionary.entries[(words[currentPhoneme])]);
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