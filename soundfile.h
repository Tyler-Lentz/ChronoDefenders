#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <string>

enum class SoundType
{
    MP3, WAV, INVALID
};

class SoundFile
{
public:
    SoundFile(const std::string filename, SoundType type);
    SoundFile();

    virtual void play() const = 0;
    virtual void stop() const = 0;

    virtual std::wstring getFilename() const;

private:
    std::wstring filename;
    SoundType type;
};

class Mp3File : public SoundFile
{
public:
    Mp3File(const std::string filename);
    Mp3File(const std::string filename, const std::string variant);
    Mp3File();

    virtual void play() const override;
    void stop() const override;
    std::wstring getFilename() const override;

private:
    std::wstring variant;
    bool hasVariant;
    bool useVariant() const;
};

class WavFile : public SoundFile
{
public:
    WavFile(const std::string filename, bool loop, bool async);
    WavFile();

    void play() const override;
    void stop() const override;

private:
    bool loop;
    bool async;
};

void playSound(const SoundFile& sound);
void stopSound(SoundType type);

#endif