#pragma once
#include <cassert>
#include "../glad.h"
#include "../graphics_enums.hpp"
#include <inttypes.h>

namespace tiny
{
	namespace graphics
	{
		namespace opengl
		{
			namespace enum_conversion
			{
				inline GLenum BufferEnum(
					graphics::enums::BufferUsage::Enum usage,
					graphics::enums::BufferMode::Enum mode = graphics::enums::BufferMode::Draw)
				{
					using namespace graphics::enums::BufferUsage;
					using namespace graphics::enums::BufferMode;
					switch (usage)
					{
					case Static:
						switch (mode)
						{
						case Draw:
							return GL_STATIC_DRAW;
						case Read:
							return GL_STATIC_READ;
						case Copy:
							return GL_STATIC_COPY;
						}
						break;
					case Dynamic:
						switch (mode)
						{
						case Draw:
							return GL_DYNAMIC_DRAW;
						case Read:
							return GL_DYNAMIC_READ;
						case Copy:
							return GL_DYNAMIC_COPY;
						}
						break;
					case Stream:
						switch (mode)
						{
						case Draw:
							return GL_STREAM_DRAW;
						case Read:
							return GL_STREAM_READ;
						case Copy:
							return GL_STREAM_COPY;
						}
						break;
					}

					// This should never happen
					assert(false && "Invalid OpenGL buffer mode");
					return 0;
				}

				inline GLenum ClearFlags(int value)
				{
					using namespace graphics::enums::ClearFlags;
					uint32_t ret = 0;

					// TODO: This can be optimized with better bitmath rather
					// than branches.  Happens so infrequently it probably
					// doesn't matter: but still...
					ret |= value & Color ? GL_COLOR_BUFFER_BIT : 0;
					ret |= value & Depth ? GL_DEPTH_BUFFER_BIT : 0;
					ret |= value & Stencil ? GL_STENCIL_BUFFER_BIT : 0;
					return ret;
				}

				inline GLenum FillMode(graphics::enums::FillMode::Enum mode)
				{
					using namespace graphics::enums::FillMode;
					switch (mode)
					{
					case Wireframe:
						return GL_LINE;
					case Solid:
						return GL_FILL;
					}

					assert(false && "Invalid OpenGL fill mode");
					return ~0;
				}

				inline GLenum CullMode(graphics::enums::CullMode::Enum mode)
				{
					using namespace graphics::enums::CullMode;
					switch (mode)
					{
					case Front:
						return GL_FRONT;
					case Back:
						return GL_BACK;
					case FrontAndBack:
						return GL_FRONT_AND_BACK;
					}

					assert(false && "Invalid OpenGL cull mode");
					return ~0;
				}

				inline GLenum ComparisonFunction(graphics::enums::ComparisonFunction::Enum mode)
				{
					using namespace graphics::enums::ComparisonFunction;
					switch (mode)
					{
					case Never:
						return GL_NEVER;
					case Less:
						return GL_LESS;
					case Equal:
						return GL_EQUAL;
					case LessEqual:
						return GL_LEQUAL;
					case Greater:
						return GL_GREATER;
					case NotEqual:
						return GL_NOTEQUAL;
					case GreaterEqual:
						return GL_GEQUAL;
					case Always:
						return GL_ALWAYS;
					}
					assert(false && "Invalid OpenGL comparison function");
					return ~0;
				}

				inline GLenum VertexDeclDataType(graphics::enums::VertexDeclDataType::Enum type)
				{
					using namespace graphics::enums::VertexDeclDataType;
					switch (type)
					{
					case UByte:return GL_UNSIGNED_BYTE;
					case UShort:return GL_UNSIGNED_SHORT;
					case UInt:return GL_UNSIGNED_INT;
					case Float:return GL_FLOAT;
					case Short:return GL_SHORT;
					case Int:return GL_INT;
					default:assert(false && "Trying to use undefined VertexDeclDataType");
					}
					return 0;
				}

				inline GLenum TextureFormat(enums::TextureFormat::Enum format)
				{
					using namespace enums::TextureFormat;
					switch (format)
					{
					case Red:
						return GL_RED;
						break;
					case RG:
						return GL_RG;
						break;
					case R8:
						return GL_RED;
						break;
					case R16:
						return GL_R16;
						break;
					case R16F:
						return GL_R16F;
						break;
					case R16UI:
						return GL_R16UI;
						break;
					case RG16UI:
						return GL_RG16UI;
						break;
					case R32F:
						return GL_R32F;
					case RGB:
						return GL_RGB;
						break;
					case RGBA:
						return GL_RGBA;
						break;
					case BGRA:
						return GL_BGRA;
						break;
					case RGB8:
						return GL_RGB8;
						break;
					case RGB16:
						return GL_RGB16;
						break;
					case RGBA8:
						return GL_RGBA8;
						break;
					case RGBA16:
						return GL_RGBA16;
						break;
					case RG16:
						return GL_RG16;
						break;
					case RGBA32F:
						return GL_RGBA32F;
						break;
					case RGB16F:
						return GL_RGB16F;
						break;
					case RGB32F:
						return GL_RGB32F;
						break;
						// Shared
					case RGB10_A2:
						return GL_RGB10_A2;
						break;
					case RGB10_A2UI:
						return GL_RGB10_A2UI;
						break;
					case R11F_G11F_G10F:
						return GL_R11F_G11F_B10F;
						break;
					case SRGB8_A8:
						return GL_SRGB8_ALPHA8;
						break;
					case DEPTH:
						return GL_DEPTH_COMPONENT;
						break;
					case DEPTH16:
						return GL_DEPTH_COMPONENT16;
						break;
					case DEPTH24:
						return GL_DEPTH_COMPONENT24;
						break;
					case DEPTH32:
						return GL_DEPTH_COMPONENT32;
						break;
					case DEPTH_STENCIL:
						return GL_DEPTH_STENCIL;
						break;
					case DEPTH24_STENCIL8:
						return GL_DEPTH_STENCIL;
						return GL_DEPTH24_STENCIL8;
						break;
					case DEPTH32F_STENCIL8:
						return GL_DEPTH32F_STENCIL8;
						break;
					case STENCIL_INDEX8:
						assert(false && "Texture format not supported");
						break;
					}
					assert(false && "Attempted to use unsupported texture format");
					return 0;
				}

				inline GLenum Blend(enums::Blend::Enum bl)
				{
					using namespace enums::Blend;
					switch (bl)
					{
					case Zero:
						return GL_ZERO;
						break;
					case One:
						return GL_ONE;
						break;
					case SrcColor:
						return GL_SRC_COLOR;
						break;
					case InvSrcColor:
						return GL_ONE_MINUS_SRC_COLOR;
						break;
					case SrcAlpha:
						return GL_SRC_ALPHA;
						break;
					case InvSrcAlpha:
						return GL_ONE_MINUS_SRC_ALPHA;
						break;
					case DestAlpha:
						return GL_DST_ALPHA;
						break;
					case InvDestAlpha:
						return GL_ONE_MINUS_DST_ALPHA;
						break;
					case DestColor:
						return GL_DST_COLOR;
						break;
					case InvDestColor:
						return GL_ONE_MINUS_DST_COLOR;
						break;
					case SrcAlphaSaturate:
						return GL_SRC_ALPHA_SATURATE;
						break;
					case BlendFactor:
						break;
					case InvBlendFactor:
						break;
					case Src1Color:
						return GL_SRC1_COLOR;
						break;
					case InvSrc1Color:
						return GL_ONE_MINUS_SRC1_COLOR;
						break;
					case Src1Alpha:
						return GL_SRC1_ALPHA;
						break;
					case InvSrc1Alpha:
						return GL_ONE_MINUS_SRC1_ALPHA;
						break;
					}
					assert(false && "Failed to convert to GL blend");
					return -1;
				}

				inline GLenum BlendOp(enums::BlendOp::Enum en)
				{
					using namespace enums::BlendOp;
					switch (en)
					{
					case Add:
						return GL_FUNC_ADD;
					case Subtract:
						return GL_FUNC_SUBTRACT;
					case RevSubtract:
						return GL_FUNC_REVERSE_SUBTRACT;
					case Min:
						return GL_MIN;
					case Max:
						return GL_MAX;
						break;
					}

					assert(false && "Failed to convert to GL blend op");
					return -1;
				}
				// enum_conversion
			}
		}
	}
}