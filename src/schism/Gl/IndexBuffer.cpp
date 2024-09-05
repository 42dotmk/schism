#include "IndexBuffer.h"

namespace Schism::Gl {
IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_DYNAMIC_DRAW);
}

IndexBuffer::IndexBuffer(std::vector<uint32_t>& indices) {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
                 &indices[0], GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_BufferID);
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    return MakeRef<IndexBuffer>(indices, count);
}

Ref<IndexBuffer> IndexBuffer::Create() {
    return MakeRef<IndexBuffer>();
}

void IndexBuffer::SetData(uint32_t* indices, uint32_t count) const {
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), &indices[0],
                 GL_DYNAMIC_DRAW);
}

void IndexBuffer::SetData(std::vector<uint32_t>& indices,
                          uint32_t count) const {
    if (indices.size() == 0)
        return;
    SC_ASSERT(indices.size(), "Empty index buffer data");
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), &indices[0],
                 GL_DYNAMIC_DRAW);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Clear() const {
    glDeleteBuffers(1, &m_BufferID);
}
}  // namespace Schism::Gl
