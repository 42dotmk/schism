#pragma once

#include <glad/glad.h>

#include "Buffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "schism/System/System.h"

namespace Schism::Gl {
static GLenum OpenGLBaseType(ShaderDataType type) {
    static constexpr GLenum glDataType[] = {
        0,         //  None = 0,
        GL_INT,    //	Int,
        GL_INT,    //	Int2,
        GL_INT,    //	Int3,
        GL_INT,    //	Int4,
        GL_FLOAT,  //	Float,
        GL_FLOAT,  //	Float2,
        GL_FLOAT,  //	Float3,
        GL_FLOAT,  //	Float4,
        GL_FLOAT,  //	Mat3, 3 * float3
        GL_FLOAT,  //	Mat4, 4 * float4
        GL_BOOL,   //	Bool
    };

    if (type == ShaderDataType::None) {
        SC_CORE_TRACE("Invalid shader data type");
    }

    return glDataType[static_cast<int>(type)];
}

class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        static Ref<VertexArray> Create();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(const Ref<VertexBuffer>& buf);
        void SetIndexBuffer(const Ref<IndexBuffer>& buffer);

    private:
        uint32_t m_ID;
        uint32_t m_BufferIndex{0};
        uint32_t m_Stride;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
};
}  // namespace Schism::Gl
