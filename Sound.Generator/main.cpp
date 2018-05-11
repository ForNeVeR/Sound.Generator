#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <numeric>
#include <string>
#include <vector>

#include "synthesis.h"

void printUsage()
{
    std::wcout << LR"(Usage: Sound.Generator <input text file> <output WAV file>

Input file should be in UTF-8 encoding.

First line of text file should include voice name. Will write output audio
stream to stdout.

)";
}

std::vector<std::wstring> readUtf8(std::wstring path)
{
    auto emptyLocale = std::locale::empty();    
    auto converter = new std::codecvt_utf8<wchar_t>();
    auto utf8_locale = std::locale(emptyLocale, converter);
    std::wifstream stream(path);
    stream.imbue(utf8_locale);

    std::vector<std::wstring> result;
    while (!stream.eof())
    {
        std::wstring line;
        std::getline(stream, line);
        result.push_back(line);
    }

    return result;
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc != 3)
    {
        printUsage();
        SoundGenerator::printVoices();
        return 1;
    }

    try
    {
        std::wstring inputPath = argv[1];
        std::wstring outputPath = argv[2];

        auto lines = readUtf8(inputPath);
        if (lines.size() == 0)
        {
            throw std::exception("Voice name not found in the input file");
        }

        auto voiceName = lines[0];
        lines.erase(lines.begin(), lines.begin() + 1);
        auto text = std::accumulate(lines.begin(), lines.end(), std::wstring(L"\n"));

        SoundGenerator::synthesize(voiceName, text, outputPath);

        return 0;
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what() << "\n";
        return 2;
    }
}
