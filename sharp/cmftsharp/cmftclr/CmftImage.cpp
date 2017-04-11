#include "stdafx.h"
#include "CmftImage.h"

namespace cmftclr
{
	CmftImage::CmftImage(String^ filePath, TextureFormat format)
	{
		const char* fileName = (const char*)(void*)Marshal::StringToHGlobalAnsi(filePath);

		CTextureFormat::Enum aFormat = CmftImage::GetCmftTextureFormat(format);

		CImage img;
		imageLoaded = imageLoad(img, fileName, aFormat)
			|| imageLoadStb(img, fileName, aFormat);

		if (imageLoaded)
		{
			int size = sizeof(CImage);
			void* strPtr = malloc(sizeof(CImage));
			image = (CImage*)strPtr;
			image->m_width = 0;

			ClrAllocator allocator;
			imageMove(*image, img, &allocator);
		}
	}
	CmftImage::CmftImage(String^ filePath)
		: CmftImage(filePath, TextureFormat::Null)
	{
	}

	CmftImage::CmftImage(Stream^ stream)
	{

	}
	CmftImage::~CmftImage()
	{
		if (imageLoaded)
		{
			imageUnload(*image);
			free(image);
			imageLoaded = false;
		}
	}

	bool CmftImage::IsCubemap()
	{
		return imageIsCubemap(*image);
	}
	bool CmftImage::IsLatLong()
	{
		return imageIsLatLong(*image);
	}

	bool CmftImage::IsHorizontalStrip()
	{
		return imageIsHStrip(*image);
	}
	bool CmftImage::IsVerticalStrip()
	{
		return imageIsVStrip(*image);
	}

	bool CmftImage::IsOctant()
	{
		return imageIsOctant(*image);
	}

	bool CmftImage::ToCubemapFromStrip()
	{
		return imageCubemapFromStrip(*image);
	}


	void CmftImage::GenerateMipMapChain()
	{
		imageGenerateMipMapChain(*image);
	}

	void CmftImage::ApplyGamma(float gamma)
	{
		imageApplyGamma(*image, gamma);
	}
	void CmftImage::EncodeRGBM()
	{
		imageEncodeRGBM(*image);
	}

	bool CmftImage::Save(String^ filePath, ImageFileType fileType, OutputType outType, TextureFormat format)
	{
		COutputType::Enum    ot = (COutputType::Enum)outType;
		CImageFileType::Enum ft = (CImageFileType::Enum)fileType;
		CTextureFormat::Enum tf = (CTextureFormat::Enum)format;

		// Encode RGBM (using texture format)
		if (tf == CTextureFormat::RGBM)
		{
			imageEncodeRGBM(*image);
			tf = CTextureFormat::BGRA8;	// Change file format to BGRA8 for saving
		}

		const char* fileName = (const char*)(void*)Marshal::StringToHGlobalAnsi(filePath);
		return imageSave(*image, fileName, ft, ot, tf, false);
	}

	void CmftImage::DoRadianceFilter(Int32 faceSize, LightingModel lightModel, bool excludeBase, Byte mipCount, Byte glossScale, Byte glossBias, EdgeFixup edgeFixup, Byte numCPUThreads)
	{
		CLightingModel::Enum lmodel = (CLightingModel::Enum)lightModel;
		CEdgeFixup::Enum edge = (CEdgeFixup::Enum)edgeFixup;
		imageRadianceFilter(*image, faceSize, lmodel, excludeBase, mipCount, glossScale, glossBias, edge, numCPUThreads);
	}

	void CmftImage::DoIrradianceFilterSh(Int32 faceSize)
	{
		imageIrradianceFilterSh(*image, faceSize);
	}

}