#pragma once
#include "wrappers/opengl_vertex_buffer.hpp"
#include "wrappers/opengl_index_buffer.hpp"
#include "graphics_handle.hpp"
#include "graphics_enums.hpp"
#include <glm/glm.hpp>
namespace tiny
{
	namespace graphics
	{
		struct VertexData
		{
			VertexData()
				: normalized(false)
			{}

			GraphicsHandle<VertexBuffer> vbo;
			const void* offset;
			size_t elem_size;
			size_t elem_count;
			size_t stride;
			enums::VertexDeclDataType::Enum data_type;
			int index;
			bool normalized;

			inline bool operator==(const VertexData& rhs) const
			{
				return
					vbo == rhs.vbo &&
					offset == rhs.offset &&
					elem_size == rhs.elem_size &&
					elem_count == rhs.elem_count &&
					stride == rhs.stride &&
					data_type == rhs.data_type &&
					index == rhs.index &&
					normalized == rhs.normalized;
			}

			inline bool operator!=(const VertexData& rhs) const
			{
				return !operator==(rhs);
			}
		};

		class VertexDecl
		{
		public:
			VertexDecl() = default;
			~VertexDecl() = default;

		public:
			void SetIndices(GraphicsHandle<IndexBuffer> ibo);
			bool HasIndices() const;
			GraphicsHandle<IndexBuffer> GetIndices() const;

			int AddElement(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_size, size_t elem_count, size_t stride, enums::VertexDeclDataType::Enum type, bool norm = false);

			template <typename T>
			int AddElement(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride)
			{
				return AddElement<T>(vbo, offset, elem_count, stride, false);
			}

			template <typename T>
			int AddElement(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
			{
				throw std::runtime_error("Attempted to add unrecognized type");
			}

			inline const std::vector<VertexData>& GetData() const
			{
				return m_data;
			}

			inline bool operator==(const VertexDecl& rhs) const
			{
				if (m_data.size() != rhs.m_data.size())
					return false;

				for (size_t i = 0; i < m_data.size(); ++i)
				{
					if (m_data[i] != rhs.m_data[i])
						return false;
				}

				return m_ibo == rhs.m_ibo;
			}

			inline bool operator!=(const VertexDecl& rhs) const
			{
				return !operator==(rhs);
			}

		private:
			std::vector<VertexData> m_data;
			GraphicsHandle<IndexBuffer> m_ibo;
		};

		template <>
		inline int VertexDecl::AddElement<uint8_t>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(uint8_t), elem_count, stride, UByte, norm);
		}

		template <>
		inline int VertexDecl::AddElement<int8_t>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(int8_t), elem_count, stride, Byte, norm);
		}

		template <>
		inline int VertexDecl::AddElement<uint16_t>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(uint16_t), elem_count, stride, UShort, norm);
		}

		template <>
		inline int VertexDecl::AddElement<uint32_t>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(uint32_t), elem_count, stride, UInt, norm);
		}

		template <>
		inline int VertexDecl::AddElement<float>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(float), elem_count, stride, Float, norm);
		}

		template <>
		inline int VertexDecl::AddElement<int16_t>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(int16_t), elem_count, stride, Short, norm);
		}

		template <>
		inline int VertexDecl::AddElement<int32_t>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(int32_t), elem_count, stride, Int, norm);
		}

		template <>
		inline int VertexDecl::AddElement<glm::vec2>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(float), elem_count * 2, stride, Float, norm);
		}

		template <>
		inline int VertexDecl::AddElement<glm::vec3>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(float), elem_count * 3, stride, Float, norm);
		}

		template <>
		inline int VertexDecl::AddElement<glm::vec4>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(float), elem_count * 4, stride, Float, norm);
		}

		template <>
		inline int VertexDecl::AddElement<glm::mat3>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(float), elem_count * 9, stride, Float, norm);
		}

		template <>
		inline int VertexDecl::AddElement<glm::mat4>(GraphicsHandle<VertexBuffer> vbo, const void* offset, size_t elem_count, size_t stride, bool norm)
		{
			using namespace graphics::enums::VertexDeclDataType;
			return AddElement(vbo, offset, sizeof(float), elem_count * 16, stride, Float, norm);
		}
	}
}