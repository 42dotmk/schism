#include "WavLoader.h"

#include "schism/System/Log.h"
#include <cstring>
#include <type_traits>

namespace Schism::Audio {

WavLoader::WavLoader(std::filesystem::path filePath)
    :
    m_filepath(filePath)
{

}

WavLoader::~WavLoader()
{
    if (m_file.is_open())
    {
        m_file.close();
    }
}

bool WavLoader::Load()
{
    m_file = std::ifstream(m_filepath, std::ios::binary);

    if (!m_file.is_open())
    {
        SC_CORE_ERROR("Cannot open wav file: {}", m_filepath.string());
        return false;
    }

    if (!readHeader())
    {
        return false;
    }
    SC_CORE_INFO("Reading data");
    return readData();
}

const struct WavLoader::Header& WavLoader::Header()
{
    return m_header;
}

const std::vector<char>& WavLoader::Data()
{
    return m_data;
}

std::int32_t WavLoader::buffer_to_int(char* buffer, std::size_t size)
{
    std::int32_t num = 0;
    if (std::endian::native == std::endian::little)
    {
        std::memcpy(&num, buffer, size);
    }
    else 
    {
        for (std::size_t i = 0; i < size; i++)
        {
            auto* ptr = reinterpret_cast<char*>(&num);
            ptr[3 - i] = buffer[i];
        }
    }

    return num;
}

void WavLoader::readIntoBuffer(char* buffer, std::size_t size)
{
    if (!m_file.read(buffer, size))
    {
        SC_CORE_TRACE("Error reading header from wav file");
    }
}

bool WavLoader::readHeader()
{
    char buffer[4];

    readIntoBuffer(buffer, 4);
    if (std::strncmp(buffer, "RIFF", 4) != 0)
    {
        SC_CORE_ERROR("Error reading wav file: {}", m_filepath.string());
        return false;
    }

    readIntoBuffer(buffer, 4);

    readIntoBuffer(buffer, 4);
    if (std::strncmp(buffer, "WAVE", 4) != 0)
    {
        SC_CORE_TRACE("Error reading wav file: {}", m_filepath.string());
        return false;
    }

    readIntoBuffer(buffer, 4);
    readIntoBuffer(buffer, 4);
    readIntoBuffer(buffer, 2);

    readIntoBuffer(buffer, 2);
    m_header.channels = buffer_to_int(buffer, 2);

    readIntoBuffer(buffer, 4);
    m_header.sampleRate = buffer_to_int(buffer, 4);

    readIntoBuffer(buffer, 4);
    readIntoBuffer(buffer, 2);

    readIntoBuffer(buffer, 2);
    m_header.bitsPerSample = buffer_to_int(buffer, 2);

    readIntoBuffer(buffer, 4);

    if (std::strncmp(buffer, "data", 4) != 0)
    {
        SC_CORE_TRACE("Error reading wav file : {}", m_filepath.string());
        return false;
    }

    readIntoBuffer(buffer, 4);
    m_header.size = buffer_to_int(buffer, 4);

    if (m_file.eof())
    {
        SC_CORE_TRACE("Error reading wav file : {}", m_filepath.string());
        return false; 
    }

    if (m_file.fail())
    {
        SC_CORE_TRACE("Error reading wav file : {}", m_filepath.string());
        return false; 
    }

    return true;
}

bool WavLoader::readData()
{
    m_data.resize(m_header.size);
    if (!m_file.read(&m_data[0], m_header.size))
    {
        SC_CORE_TRACE("Error reading wav file: {}", m_filepath.string());
    }

    return true;
}
} 

