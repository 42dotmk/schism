#include "Source.h"
#include "al.h"
#include "schism/Audio/WavLoader.h"
#include "schism/System/Ptr.h"
#include <glm/glm.hpp>

namespace Schism::Audio {

Source::Source(const std::vector<char>& data, std::uint32_t sampleRate, std::uint8_t bitsPerSample, std::uint8_t channels)
{
    // need to check for errors
    alGenSources(1, &m_source);

    SetPitch(1);
    SetGain(1);
    SetPosition(glm::vec3{ 0.f, 0.f, 0.f });
    SetLooping(false);

    alGenBuffers(1, &m_buffer);
    alBufferData(m_buffer, getFormat(bitsPerSample, channels), &data[0], data.size(), sampleRate);
    alSourcei(m_source, AL_BUFFER, m_buffer);
}

Ref<Source> Source::Create(std::filesystem::path filePath)
{
    WavLoader wav(filePath);

    if (!wav.Load())
    {
        SC_CORE_ERROR("Error creating audio source, with file: ", filePath.string());
        return nullptr;
    }
    auto& header = wav.Header();
    auto& data = wav.Data();
    SC_CORE_INFO("Wav file loaded!");
    return MakeRef<Source>(data, header.sampleRate, header.bitsPerSample, header.channels);
}

void Source::SetPosition(const glm::vec3& position)
{
    alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
}

void Source::SetGain(float gain) 
{
    alSourcef(m_source, AL_GAIN, gain);
}

void Source::SetLooping(bool isLooping)
{
    alSourcei(m_source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
}

void Source::SetPitch(float pitch)
{
    alSourcef(m_source, AL_PITCH, pitch);
}

bool Source::IsPlaying()
{
    return false;
}

void Source::Play()
{
    alSourcePlay(m_source);
}

void Source::Stop()
{
    alSourceStop(m_source);
}

void Source::Pause()
{
    alSourcePause(m_source);
}

void Source::Resume()
{
    Play();
}


ALenum Source::getFormat(std::uint8_t bitsPerSample, std::uint8_t channels)
{
    switch (bitsPerSample)
    {
        case 16:
            {
                switch (channels) 
                {
                    case 1:
                        return AL_FORMAT_MONO16;
                    case 2:
                        return AL_FORMAT_STEREO16;
                    default:
                        SC_CORE_ERROR("Not supporting more than 2 channels");
                }
                
                break;
            }
        case 8:
            {
                switch (channels)
                {
                    case 1:
                        return AL_FORMAT_MONO8;
                    case 2:
                        return AL_FORMAT_STEREO8;
                    default:
                        SC_CORE_ERROR("Not supporting more than 2 channels");
                }
                break;
            }
        default:
            SC_CORE_ERROR("Not supporting more than 32 bits per channel");
    };

    return -1;
}
}
