#pragma once

namespace SoundGenerator
{
    void printVoices();
    void synthesize(
        std::wstring voiceName,
        std::wstring text,
        std::wstring outputPath);
}
