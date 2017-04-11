#pragma once
#include <cmft\image.h>
#include <cmft/cubemapfilter.h>
#include "ClrAllocator.h"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace cmft;
using CImage = cmft::Image;
using CTextureFormat = cmft::TextureFormat;
using CImageFileType = cmft::ImageFileType;
using COutputType = cmft::OutputType;
using CLightingModel = cmft::LightingModel;
using CEdgeFixup = cmft::EdgeFixup;

namespace cmftclr 
{
	public enum class FilterType
	{
		None,
		Radiance,
		Irradiance,
		ShCoeffs,
	};

	public enum class LightingModel
	{
		Phong,
		PhongBrdf,
		Blinn,
		BlinnBrdf,

		Count
	};

	public enum class EdgeFixup
	{
		None,
		Warp,
	};

	public enum class ImageFileType
	{
		DDS,
		KTX,
		TGA,
		HDR,

		Count
	};

	public enum class OutputType
	{
		LatLong,
		Cubemap,
		HCross,
		VCross,
		HStrip,
		VStrip,
		FaceList,
		Octant,

		Count,
		Null = -1,
	};

	public enum class TextureFormat
	{
		BGR8,
		RGB8,
		RGB16,
		RGB16F,
		RGB32F,
		RGBE,
		BGRA8,
		RGBA8,
		RGBA16,
		RGBA16F,
		RGBA32F,
		RGBM,
		Count,
		Null = -1,
	};

	public ref class CmftImage
	{
	private:
		CImage* image;
		bool imageLoaded;

	public:
		CmftImage(String^ filePath, TextureFormat format);
		CmftImage(String^ filePath);
		CmftImage(Stream^ stream);
		~CmftImage();

		bool IsCubemap();
		bool IsLatLong();
		bool IsHorizontalStrip();
		bool IsVerticalStrip();
		bool IsOctant();

		bool ToCubemapFromStrip();

		void DoRadianceFilter(Int32 faceSize, LightingModel lightModel, bool excludeBase, Byte mipCount, Byte glossScale, Byte glossBias, EdgeFixup edgeFixup, Byte numCPUThreads);
		void DoIrradianceFilterSh(Int32 faceSize);

		void GenerateMipMapChain();
		void ApplyGamma(float gamma);
		void EncodeRGBM();

		bool Save(String^ filePath, ImageFileType fileType, OutputType outType, TextureFormat format);

		static TextureFormat GetTextureFormat(CTextureFormat::Enum format)
		{
			return (TextureFormat)(Int32)(format);
		}
		static CTextureFormat::Enum GetCmftTextureFormat(TextureFormat format)
		{
			return (CTextureFormat::Enum)(Int32)(format);
		}

		Int32 Width()
		{
			return image->m_width;
		}
		Int32 Height()
		{
			return image->m_height;
		}
		Int32 DataSize()
		{
			return image->m_dataSize;
		}
		TextureFormat Format()
		{
			return static_cast<TextureFormat>(image->m_format);
		}

		Int32 NumFaces()
		{
			return image->m_numFaces;
		}
		Int32 NumMips()
		{
			return image->m_numMips;
		}

		IntPtr Data()
		{
			return IntPtr(image->m_data);
		}
	};
}
