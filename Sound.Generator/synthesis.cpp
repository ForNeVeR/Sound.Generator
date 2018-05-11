#include <array>
#include <exception>
#include <functional>
#include <iostream>
#include <string>

#include <sapi.h>
#include <sphelper.h>

#include "synthesis.h"

void forEachVoice(
    std::function<bool (CComPtr<ISpVoice> &, CComPtr<ISpObjectToken> &)> action)
{
    if (FAILED(CoInitialize(nullptr)))
    {
        throw std::exception("Cannot call CoInitialize");
    }

    {
        CComPtr<ISpVoice> voice;
        if (FAILED(voice.CoCreateInstance(CLSID_SpVoice)))
        {
            throw std::exception("Cannot call CoCreateInstance(CLSID_SpVoice)");
        }

        CComPtr<IEnumSpObjectTokens> enumerator;
        if (FAILED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &enumerator)))
        {
            throw std::exception("Cannot call SpEnumTokens");
        }

        ULONG voiceCount = 0UL;
        if (FAILED(enumerator->GetCount(&voiceCount)))
        {
            throw std::exception("Cannot call IEnumSpObjectTokens::GetCount");
        }

        for (auto i = 0UL; i < voiceCount; ++i)
        {
            CComPtr<ISpObjectToken> voiceToken;
            if (FAILED(enumerator->Next(1, &voiceToken, nullptr)))
            {
                throw std::exception("Cannot call IEnumSpObjectTokens::Next");
            }

            if (!action(voice, voiceToken))
            {
                break;
            }
        }
    }

    CoUninitialize();
}

std::wstring getVoiceName(CComPtr<ISpObjectToken> &token)
{
    LPWSTR name = nullptr;
    if (FAILED(token->GetStringValue(nullptr, &name)))
    {
        throw std::exception("Cannot call ISpObjectToken::GetStringValue");
    }

    return name;
}

void generateSound(
    CComPtr<ISpVoice> &voice,
    CComPtr<ISpObjectToken> &token,
    std::wstring textString,
    std::wstring outputPath)
{
    auto name = getVoiceName(token);
    std::wcout << "Using " << name << " voice\n";

    if (FAILED(voice->SetVoice(token)))
    {
        throw std::exception("Cannot call ISpVoice::SetVoice");
    }

    CComPtr<ISpeechFileStream> outputStream;
    if (FAILED(outputStream.CoCreateInstance(CLSID_SpFileStream)))
    {
        throw std::exception("Cannot call CoCreateInstance(CLSID_SpFileStream)");
    }

    auto path = const_cast<wchar_t*>(outputPath.c_str());
    if (FAILED(outputStream->Open(path, SSFMCreateForWrite)))
    {
        throw std::exception("Cannot open the output file");
    }

    if (FAILED(voice->SetOutput(outputStream, true)))
    {
        throw std::exception("Cannot call ISpVoice::SetOutput");
    }

    auto text = const_cast<wchar_t*>(textString.c_str());
    if (FAILED(voice->Speak(text, SPF_DEFAULT, nullptr)))
    {
        throw std::exception("Cannot call ISpVoice::Speak");
    }
}

void SoundGenerator::printVoices()
{
    std::wcout << "Voice list:\n";

    forEachVoice([](auto voice, auto token)
    {
        auto name = getVoiceName(token);
        std::wcout << L"- " << name << "\n";

        return true;
    });
}

void SoundGenerator::synthesize(std::wstring voiceName, std::wstring text, std::wstring outputPath)
{
    auto voiceFound = false;
    forEachVoice([=, &voiceFound](auto voice, auto token)
    {
        if (getVoiceName(token) == voiceName)
        {
            generateSound(voice, token, text, outputPath);
            voiceFound = true;
            return false;
        }

        return true;
    });

    if (!voiceFound)
    {
        throw std::exception("Voice not found");
    }
}
