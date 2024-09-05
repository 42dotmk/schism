#include "VertexArray.h"

namespace Schism::Gl {
VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_ID);
}

Ref<VertexArray> VertexArray::Create() {
    return MakeRef<VertexArray>();
}

void VertexArray::Bind() const {
    if (m_IndexBuffer) {
        m_IndexBuffer->Bind();
    }
    if (m_VertexBuffer) {
        m_VertexBuffer->Bind();
    }
    glBindVertexArray(m_ID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer) {
    const auto& layout = buffer->GetLayout();
    SC_ASSERT(layout.GetElements().size(), "VertexBuffer with no layout");

    Bind();
    buffer->Bind();

    for (const auto& element : layout.GetElements()) {
        switch (element.Type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool: {
                glVertexAttribPointer(
                    m_BufferIndex, element.GetComponentCount(),
                    OpenGLBaseType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                    (const void*)element.Offset);
                glEnableVertexAttribArray(m_BufferIndex);
                m_BufferIndex++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: {
                uint8_t count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; i++) {
                    glVertexAttribPointer(
                        m_BufferIndex, count, OpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (const void*)(sizeof(float) * count * i));
                    glEnableVertexAttribArray(m_BufferIndex);
                    glVertexAttribDivisor(m_BufferIndex, 1);
                    m_BufferIndex++;
                }
                break;
            }
            default:
                SC_ASSERT(false, "Unknown ShaderDataType!");
        }
    }
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer) {
    Bind();
    buffer->Bind();
    m_IndexBuffer = buffer;
}
}  // namespace Schism::Gl
