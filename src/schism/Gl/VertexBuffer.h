#pragma once

#include <glad/glad.h>
#include <cstdint>

#include "Buffer.h"
#include "schism/System/System.h"

namespace Schism::Gl {
class VertexBuffer : public Buffer {
    public:
        VertexBuffer(const BufferLayout& layout);
        VertexBuffer(float* vertices, size_t size, const BufferLayout& layout,
                     bool dynamic = true);
        VertexBuffer(std::vector<float>& vertices, const BufferLayout& layout,
                     bool dynamic = true);
        virtual ~VertexBuffer();

        static Ref<VertexBuffer> Create(const BufferLayout& layout);
        static Ref<VertexBuffer> Create(void* vertices, size_t size,
                                        const BufferLayout& layout);

        const BufferLayout& GetLayout() const { return m_Layout; };

        void SetData(void* vertices, size_t size);
        void UpdateSubData(void* vertices, size_t size);

        template <typename T>
        void SetData(std::vector<T>& vertices, size_t count) {
            if (!m_Dynamic)
                return;
            Bind();
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), &vertices[0],
                         GL_DYNAMIC_DRAW);
        }

        void Bind() const override;
        void Unbind() const override;

        void Clear();

    private:
        void CreateBuffer();

        bool m_Dynamic{false};
        unsigned m_BufferID;
        BufferLayout m_Layout;
};
}  // namespace Schism::Gl
