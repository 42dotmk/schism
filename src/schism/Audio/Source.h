#pragma once

#include "al.h"
#include <glm/glm.hpp>
#include "schism/System/Ptr.h"
#include <vector>
#include <cstdint>
#include <filesystem>

namespace Schism::Audio {

class Source 
{
public:
    Source(const std::vector<char>& data, std::uint32_t sampleRate, std::uint8_t bitsPerSample, std::uint8_t channels);

    static Ref<Source> Create(std::filesystem::path filePath);

    void SetPosition(const glm::vec3& position);
    void SetGain(float gain);
    void SetLooping(bool isLooping);
    void SetPitch(float pitch);

    bool IsPlaying();
    void Play();
    void Stop();
    void Pause();
    void Resume();
private:
    static ALenum getFormat(std::uint8_t bitsPerSample, std::uint8_t channels);

    // for future, not needed right now
    bool m_isInitialized;
    ALuint m_source;
    ALuint m_buffer;

    std::uint32_t m_sampleRate;
    std::uint8_t m_bitsPerSample;
    std::uint8_t m_channels;
};

}
