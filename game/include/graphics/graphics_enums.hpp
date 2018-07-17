#pragma once
namespace tiny
{
	namespace graphics
	{
		namespace enums
		{
			// VBO/IBO
			namespace BufferUsage
			{
				enum Enum
				{
					Static,
					Dynamic,
					Stream
				};
			}

			namespace BufferMode
			{
				enum Enum
				{
					Draw,
					Read,
					Copy
				};
			}

			// General
			namespace ClearFlags
			{
				enum Enum
				{
					Color = 1 << 0,
					Depth = 1 << 1,
					Stencil = 1 << 2
				};
			}

			namespace FillMode
			{
				enum Enum
				{
					Wireframe = 1,
					Solid = 2
				};
			}

			namespace PrimitiveTopology
			{
				enum Enum
				{
					Points = 1,
					Lines = 2,
					Triangles = 3
				};
			}

			namespace CullMode
			{
				enum Enum
				{
					None = 1,
					Front = 2,
					Back = 3,
					FrontAndBack = 4	// why
				};
			}

			namespace StencilOp
			{
				enum Enum
				{
					// Keep the existing stencil data.
					Keep = 1,

					// Set the stencil data to 0.
					Zero = 2,

					// Set the stencil data to the reference value set by calling ID3D11DeviceContext::OMSetDepthStencilState.
					// TODO: Backend-agnostic
					Replace = 3,

					// Increment the stencil value by 1, and clamp the result.
					IncrSat = 4,

					// Decrement the stencil value by 1, and clamp the result.
					DecrSat = 5,

					// Invert the stencil data.
					Invert = 6,

					// Increment the stencil value by 1, and wrap the result if necessary.
					Incr = 7,

					// Decrement the stencil value by 1, and wrap the result if necessary.
					Decr = 8
				};
			}

			namespace ComparisonFunction
			{
				enum Enum
				{
					Never = 1,
					Less = 2,
					Equal = 3,
					LessEqual = 4,
					Greater = 5,
					NotEqual = 6,
					GreaterEqual = 7,
					Always = 8
				};
			}

			namespace DepthWriteMask
			{
				enum Enum
				{
					Zero = 0,
					All = 1
				};
			}

			// Shaders
			namespace ShaderType
			{
				enum Enum
				{
					Invalid,
					Vertex,
					Fragment,
					Geometry,
					Compute,

					Count
				};
			}

			// VBO/IBO/VAO
			namespace VertexDeclDataType
			{
				enum Enum
				{
					UByte,
					UShort,
					UInt,
					Float,
					Byte,
					Short,
					Int
				};
			}

			// Textures
			namespace TextureFilterMode
			{
				enum Enum
				{
					NearestNeighbor,
					Bilinear
				};
			}

			namespace TextureWrap
			{
				enum Enum
				{
					Repeat,
					Clamp
				};
			}

			namespace TextureDimension
			{
				enum Enum
				{
					Texture1D,
					Texture2D,
					Texture3D,
					TextureCube
				};
			}

			namespace TextureFormat
			{
				enum Enum
				{
					Invalid,
					Red,
					RG,

					R8,
					RG16,
					RG16UI,

					R16,
					R16F,
					R16UI,

					R32F,

					// Texture only, renderbuffer optional
					RGB,
					RGB8,
					RGB16,
					RGB16F,
					RGB10_A2,
					RGB10_A2UI,
					RGB32F,

					RGBA,
					BGRA,
					RGBA8,
					RGBA16,
					RGBA16F,
					RGBA32F,

					Luminance,
					LuminanceAlpha,

					// Shared
					R11F_G11F_G10F,
					SRGB8_A8,
					DEPTH,
					DEPTH16,
					DEPTH24,
					DEPTH32,
					DEPTH_STENCIL,
					DEPTH24_STENCIL8,
					DEPTH32F_STENCIL8,
					STENCIL_INDEX8
				};
			}

			namespace DepthFormat
			{
				enum Enum
				{
					Invalid,

					Count
				};
			}

			// Blending
			namespace Blend
			{
				enum Enum
				{
					Zero = 1,
					One = 2,
					SrcColor = 3,
					InvSrcColor = 4,
					SrcAlpha = 5,
					InvSrcAlpha = 6,
					DestAlpha = 7,
					InvDestAlpha = 8,
					DestColor = 9,
					InvDestColor = 10,
					SrcAlphaSaturate = 11,
					BlendFactor = 14,
					InvBlendFactor = 15,
					Src1Color = 16,
					InvSrc1Color = 17,
					Src1Alpha = 18,
					InvSrc1Alpha = 19
				};
			}

			namespace BlendOp
			{
				enum Enum
				{
					Add = 1,
					Subtract = 2,
					RevSubtract = 3,
					Min = 4,
					Max = 5
				};
			}
		}
		
		// Handy names
		typedef enums::BufferUsage::Enum			BufferUsage;
		typedef enums::BufferMode::Enum				BufferMode;
		typedef enums::ClearFlags::Enum				ClearFlags;
		typedef enums::FillMode::Enum				FillMode;
		typedef enums::PrimitiveTopology::Enum		PrimitiveTopology;
		typedef enums::CullMode::Enum				CullMode;
		typedef enums::StencilOp::Enum				StencilOp;
		typedef enums::ComparisonFunction::Enum		ComparisonFunction;
		typedef enums::DepthWriteMask::Enum			DepthWriteMask;
		typedef enums::ShaderType::Enum				ShaderType;
		typedef enums::VertexDeclDataType::Enum		VertexDeclDataType;
		typedef enums::TextureFilterMode::Enum		TextureFilterMode;
		typedef enums::TextureWrap::Enum			TextureWrap;
		typedef enums::TextureDimension::Enum		TextureDimension;
		typedef enums::TextureFormat::Enum			TextureFormat;
		typedef enums::Blend::Enum					Blend;
		typedef enums::BlendOp::Enum				BlendOp;
	}
}