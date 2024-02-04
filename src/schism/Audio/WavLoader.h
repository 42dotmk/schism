#pragma once

#include <cstdint>
#include <filesystem>
#include "AL/al.h"
#include <fstream>
#include <vector>

namespace Schism::Audio {
// subject to change a lot, just a basic way to load wav files
class WavLoader
{
public:
    struct Header 
    {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
    };

    WavLoader(std::filesystem::path filePath);
    ~WavLoader();
   
    bool Load();
    const Header& Header();
    const std::vector<char>& Data();
    char* data2() { return data; };
private:
    std::int32_t buffer_to_int(char* buffer, std::size_t size);
    void readIntoBuffer(char* buffer, std::size_t size);
    bool readHeader();
    bool readData();

    std::ifstream m_file;
    std::filesystem::path m_filepath;
    std::vector<char> m_data;
    char* data;
    struct Header m_header;
};

}
