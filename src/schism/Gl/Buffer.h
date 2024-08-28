#pragma once
#include <numeric>
#include <string>
#include <vector>

#include "schism/System/System.h"

namespace Schism::Gl {
enum class ShaderDataType {
    None = 0,
    Int,
    Int2,
    Int3,
    Int4,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    static constexpr uint8_t dataTypeSize[] = {
        0,          //  None = 0,
        4,          //	Int,
        4 * 2,      //	Int2,
        4 * 3,      //	Int3,
        4 * 4,      //	Int4,
        4,          //	Float,
        4 * 2,      //	Float2,
        4 * 3,      //	Float3,
        4 * 4,      //	Float4,
        4 * 3 * 3,  //	Mat3, 3 * float3
        4 * 4 * 4,  //	Mat4, 4 * float4
        1,          //	Bool
    };

    if (type == ShaderDataType::None) {
        SC_CORE_ERROR("Invalid ShaderDataType!");
    }

    return dataTypeSize[static_cast<int>(type)];
};

struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string& name,
                      bool normalized = false)
            : Name(name),
              Type(type),
              Size(ShaderDataTypeSize(type)),
              Offset(0),
              Normalized(normalized) {}

        uint8_t GetComponentCount() const {
            static constexpr uint8_t componentCount[] = {
                0,  //	None = 0,
                1,  //	Int,
                2,  //	Int2,
                3,  //	Int3,
                4,  //	Int4,
                1,  //	Float,
                2,  //	Float2,
                3,  //	Float3,
                4,  //	Float4,
                3,  //	Mat3, 3 * float3
                4,  //	Mat4, 4 * float4
                1,  //	Bool
            };

            if (Type == ShaderDataType::None) {
                SC_CORE_ERROR("Invalid ShaderDataType!");
            }

            return componentCount[static_cast<int>(Type)];
        }
};

class BufferLayout {
    public:
        BufferLayout() {}

        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements) {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const { return m_Stride; }

        const auto& GetElements() const { return m_Elements; }

        uint32_t GetLength() const { return m_Length; }

    private:
        void CalculateOffsetsAndStride() {
            size_t offset = 0;
            m_Stride = 0;
            for (auto& el : m_Elements) {
                el.Offset = offset;
                offset += el.Size;
                m_Stride += el.Size;
            }
            m_Length = std::accumulate(m_Elements.begin(), m_Elements.end(), 0,
                                       [](uint32_t sum, BufferElement& el) {
                                           return el.GetComponentCount() + sum;
                                       });
        }

        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;
        uint32_t m_Length;
};

class Buffer {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
};

}  // namespace Schism::Gl
