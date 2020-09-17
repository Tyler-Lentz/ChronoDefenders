#include "soundfile.h"
#include "utilities.h"

#include <string>
#include <Windows.h>
#include <codecvt>
#include <locale>

#pragma comment(lib, "Winmm.lib")

void playSound(const SoundFile& sound)
{
    sound.play();
}

void stopSound(SoundType type)
{
    if (type == SoundType::MP3)
    {
        mciSendString(L"stop mp3", NULL, 0, NULL);
        mciSendString(L"close mp3", NULL, 0, NULL);
    }
    else if (type == SoundType::WAV)
    {
        PlaySound(NULL, 0, 0);
    }
}

SoundFile::SoundFile(const std::string filename, SoundType type)
{
    this->filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(filename);
    this->type = type;
}

SoundFile::SoundFile()
{
    filename = L"NULL";
    type = SoundType::INVALID;
}

std::wstring SoundFile::getFilename() const
{
    return filename;
}

Mp3File::Mp3File(const std::string filename) :
    SoundFile(filename, SoundType::MP3)
{
    hasVariant = false;
}

Mp3File::Mp3File(const std::string filename, const std::string variant) :
    SoundFile(filename, SoundType::MP3)
{
    hasVariant = true;
    this->variant = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(variant);
}

Mp3File::Mp3File() :
    SoundFile()
{
    hasVariant = false;
}

std::wstring Mp3File::getFilename() const
{
    if (hasVariant && useVariant())
    {
        return variant;
    }
    else
    {
        return SoundFile::getFilename();
    }
}

bool Mp3File::useVariant() const
{
    return (ddutil::random(0, 1) == 0);
}

void Mp3File::play() const
{
    std::wstring fullFilename = getFilename();

    fullFilename = L"Music/" + fullFilename + L".mp3";

    std::wstring file = L"open \"" + fullFilename + L"\" type mpegvideo alias mp3";

    mciSendString(file.c_str(), NULL, 0, NULL);
    mciSendString(L"play mp3 repeat", NULL, 0, NULL);
}

void Mp3File::stop() const
{
    stopSound(SoundType::MP3);
}

WavFile::WavFile(std::string filename, bool loop, bool async) :
    SoundFile(filename, SoundType::WAV)
{
    this->loop = loop;
    this->async = async;
}

WavFile::WavFile() :
    SoundFile()
{
    loop = false;
    async = false;
}

void WavFile::play() const
{
    std::wstring fullFileName = getFilename() + L".wav";

    fullFileName = L"Music/" + fullFileName;

    if (loop)
    {
        PlaySound(LPCTSTR(fullFileName.c_str()), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    }
    else if (async)
    {
        PlaySound(LPCTSTR(fullFileName.c_str()), NULL, SND_FILENAME | SND_ASYNC);
    }
    else
    {
        PlaySound(LPCTSTR(fullFileName.c_str()), NULL, SND_FILENAME);
    }
}

void WavFile::stop() const
{
    stopSound(SoundType::WAV);
}