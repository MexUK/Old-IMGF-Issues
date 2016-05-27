#include "CImageUtility.h"
#include "String/CStringUtility.h"
#include "squish.h"

using namespace std;

string					CImageUtility::convertBGRA32ToRasterDataFormat(string& strRasterData, eRasterDataFormat eRasterDataFormatValue, string* strPaletteDataOut, uint32 uiWidth, uint32 uiHeight)
{
	switch (eRasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:		return convertBGRA32ToDXT(strRasterData, DXT_1, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT2:		return convertBGRA32ToDXT(strRasterData, DXT_2, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT3:		return convertBGRA32ToDXT(strRasterData, DXT_3, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT4:		return convertBGRA32ToDXT(strRasterData, DXT_4, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT5:		return convertBGRA32ToDXT(strRasterData, DXT_5, uiWidth, uiHeight);
	case RASTERDATAFORMAT_BGRA32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGBA32:	return convertBGRA32ToRGBA32(strRasterData);
	case RASTERDATAFORMAT_BGR32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGB32:	return convertBGRA32ToRGBA32(strRasterData);
	case RASTERDATAFORMAT_PAL4:		return convertBGRA32ToPAL4(strRasterData);
	case RASTERDATAFORMAT_PAL8:		return convertBGRA32ToPAL8(strRasterData);
	case RASTERDATAFORMAT_BGR24:	return convertBGRA32ToBGR24(strRasterData);
	case RASTERDATAFORMAT_RGB24:	return convertBGR24ToRGB24(convertBGRA32ToBGR24(strRasterData));
	}
	return "";
}
string					CImageUtility::convertRasterDataFormatToBGRA32(string& strRasterData, eRasterDataFormat eRasterDataFormatValue, string& strPaletteDataIn, uint32 uiWidth, uint32 uiHeight)
{
	switch (eRasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:		return convertDXTToBGRA32(strRasterData, DXT_1, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT2:		return convertDXTToBGRA32(strRasterData, DXT_2, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT3:		return convertDXTToBGRA32(strRasterData, DXT_3, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT4:		return convertDXTToBGRA32(strRasterData, DXT_4, uiWidth, uiHeight);
	case RASTERDATAFORMAT_DXT5:		return convertDXTToBGRA32(strRasterData, DXT_5, uiWidth, uiHeight);
	case RASTERDATAFORMAT_BGRA32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGBA32:	return convertRGBA32ToBGRA32(strRasterData);
	case RASTERDATAFORMAT_BGR32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGB32:	return convertRGBA32ToBGRA32(strRasterData);
	case RASTERDATAFORMAT_PAL4:		return convertPAL4ToBGRA32(strRasterData, strPaletteDataIn, uiWidth, uiHeight);
	case RASTERDATAFORMAT_PAL8:		return convertPAL8ToBGRA32(strRasterData, strPaletteDataIn, uiWidth, uiHeight);
	case RASTERDATAFORMAT_BGR24:	return convertBGR24ToBGRA32(strRasterData);
	case RASTERDATAFORMAT_RGB24:	return convertBGR24ToBGRA32(convertRGB24ToBGR24(strRasterData));
	}
	return "";
}

string					CImageUtility::convertBGRA32ToRGBA32(string& strRasterData)
{
	for (uint32 uiIndex32 = 0, j = strRasterData.length(); uiIndex32 < j; uiIndex32 += 4)
	{
		uint8 ucValue = strRasterData.c_str()[uiIndex32];
		strRasterData[uiIndex32] = strRasterData.c_str()[uiIndex32 + 2];
		strRasterData[uiIndex32 + 2] = ucValue;
	}
	return strRasterData;
}
string					CImageUtility::convertRGBA32ToBGRA32(string& strRasterData)
{
	return convertBGRA32ToRGBA32(strRasterData);
}

string					CImageUtility::convertBGRA32ToBGR24(string& strRasterData)
{
	uint32 uiIndex24 = 0;
	string strNewRasterData = "";
	strNewRasterData.resize((strRasterData.length() / 4) * 3);
	for (uint32 uiIndex32 = 0, j = strRasterData.length(); uiIndex32 < j; uiIndex32 += 4)
	{
		strNewRasterData[uiIndex24] = strRasterData.c_str()[uiIndex32];
		strNewRasterData[uiIndex24 + 1] = strRasterData.c_str()[uiIndex32 + 1];
		strNewRasterData[uiIndex24 + 2] = strRasterData.c_str()[uiIndex32 + 2];
		uiIndex24 += 3;
	}
	return strNewRasterData;
}
string					CImageUtility::convertBGR24ToBGRA32(string& strRasterData)
{
	uint32 uiIndex32 = 0;
	string strNewRasterData = "";
	strNewRasterData.resize((strRasterData.length() / 3) * 4);
	for (uint32 uiIndex24 = 0, j = strRasterData.length(); uiIndex24 < j; uiIndex24 += 3)
	{
		strNewRasterData[uiIndex32] = strRasterData.c_str()[uiIndex24];
		strNewRasterData[uiIndex32 + 1] = strRasterData.c_str()[uiIndex24 + 1];
		strNewRasterData[uiIndex32 + 2] = strRasterData.c_str()[uiIndex24 + 2];
		strNewRasterData[uiIndex32 + 3] = 255;
		uiIndex32 += 4;
	}
	return strNewRasterData;
}

string					CImageUtility::convertBGR24ToRGB24(string& strRasterData)
{
	for (uint32 uiIndex24 = 0, j = strRasterData.length(); uiIndex24 < j; uiIndex24 += 3)
	{
		uint8 ucValue = strRasterData.c_str()[uiIndex24];
		strRasterData[uiIndex24] = strRasterData.c_str()[uiIndex24 + 2];
		strRasterData[uiIndex24 + 2] = ucValue;
	}
	return strRasterData;
}
string					CImageUtility::convertRGB24ToBGR24(string& strRasterData)
{
	return convertBGR24ToRGB24(strRasterData);
}

string					CImageUtility::convertBGRA32ToDXT(string& strRasterData, eDXTCompressionType eDXTCompressionTypeValue, uint32 uiWidth, uint32 uiHeight)
{
	if (uiWidth < 4 || uiHeight < 4)
	{
		return "";
	}

	string
		strCompressedData;
	uint32
		uiSeek = 0,
		uiWidthX4 = uiWidth * 4,
		uiWidthD4 = uiWidth / 4;
	const uint8
		ucTexelByteCount = CImageUtility::getTexelByteCountFromDXTCompressionType(eDXTCompressionTypeValue),
		ucTexelPixelCount = 16;

	//strCompressedData.resize(uiWidth * uiHeight * 4);

	for (uint32 i = 0, j = (strRasterData.length() / 4) / ucTexelPixelCount; i < j; i++)
	{
		squish::u8 ucPixels[16 * 4];	// 16 pixels of input
		squish::u8 ucBlock[16];			// 8 or 16 bytes of output

		for (int i2 = 0; i2 < 16; i2++)
		{
			uint32 uiPixelKey = (uint32)((floor(i / uiWidthD4) * uiWidthX4) + ((i % uiWidthD4) * 4) + (floor(i2 / 4) * uiWidth) + (i2 % 4));
			uiPixelKey *= 4;

			if (uiPixelKey < strRasterData.size()) // this checks if the height has a remainder when dividing by 4 (as the iteration does 4x4 block of pixels)
			{
				ucPixels[(i2 * 4) + 2] = strRasterData.c_str()[uiPixelKey];
				ucPixels[(i2 * 4) + 1] = strRasterData.c_str()[uiPixelKey + 1];
				ucPixels[(i2 * 4) + 0] = strRasterData.c_str()[uiPixelKey + 2];
				ucPixels[(i2 * 4) + 3] = strRasterData.c_str()[uiPixelKey + 3];
			}
		}

		// compress the 4x4 block using DXT compression
		squish::Compress(ucPixels, ucBlock, CImageUtility::getSquishCompressionFlagsForDXTCompressionType(eDXTCompressionTypeValue));

		char *pData = new char[ucTexelByteCount];
		memcpy(pData, ucBlock, ucTexelByteCount);
		strCompressedData.append((char*)&ucBlock, ucTexelByteCount);
		delete[] pData;
	}

	return strCompressedData;
}
string					CImageUtility::convertDXTToBGRA32(string& strRasterData, eDXTCompressionType eDXTCompressionTypeValue, uint32 uiWidth, uint32 uiHeight)
{
	if (uiWidth < 4 || uiHeight < 4)
	{
		return "";
	}

	string
		strPixels;
	uint32
		uiSeek = 0,
		uiWidthX4 = uiWidth * 4,
		uiWidthD4 = uiWidth / 4;
	const uint8
		ucTexelByteCount = CImageUtility::getTexelByteCountFromDXTCompressionType(eDXTCompressionTypeValue);

	strPixels.resize(uiWidth * uiHeight * 4);

	for (uint32 i = 0, j = strRasterData.length() / ucTexelByteCount; i < j; i++)
	{
		string strTexel = strRasterData.substr(uiSeek + (i * ucTexelByteCount), ucTexelByteCount);

		squish::u8 ucBlock[16];			// 8 or 16 bytes of input
		squish::u8 ucPixels[16 * 4];	// 16 pixels of output

		memcpy(ucBlock, strTexel.c_str(), ucTexelByteCount);

		// decompress the 4x4 block using DXT compression
		squish::Decompress(ucPixels, ucBlock, CImageUtility::getSquishCompressionFlagsForDXTCompressionType(eDXTCompressionTypeValue));

		for (int i2 = 0; i2 < 16; i2++)
		{
			uint32 uiPixelKey = (uint32)((floor(i / uiWidthD4) * uiWidthX4) + ((i % uiWidthD4) * 4) + (floor(i2 / 4) * uiWidth) + (i2 % 4));
			uiPixelKey *= 4;

			if (uiPixelKey < strPixels.size()) // this checks if the height has a remainder when dividing by 4 (as the iteration does 4x4 block of pixels)
			{
				strPixels[uiPixelKey] = ucPixels[(i2 * 4) + 2];
				strPixels[uiPixelKey + 1] = ucPixels[(i2 * 4) + 1];
				strPixels[uiPixelKey + 2] = ucPixels[(i2 * 4) + 0];
				strPixels[uiPixelKey + 3] = ucPixels[(i2 * 4) + 3];
			}
		}
	}
	return strPixels;
}

string					CImageUtility::convertBGRA32ToPAL4(string& strRasterData)
{
	return "";
}
string					CImageUtility::convertPAL4ToBGRA32(string& strRasterData, string& strPaletteData, uint32 uiWidth, uint32 uiHeight)
{
	string strPixels;
	strPixels.resize(uiWidth * uiHeight * 4);

	uint32 uiPixelIndex = 0;
	for (uint32 i = 0, j = (uint32)((uiWidth * uiHeight) / 2); i < j; i++)
	{
		uint8 ucPaletteIndexes = strRasterData.c_str()[i] & 0xFF;
		uint8 ucPaletteIndex1 = (ucPaletteIndexes >> 4) & 0x0F;
		uint8 ucPaletteIndex2 = ucPaletteIndexes & 0x0F;

		strPixels[uiPixelIndex] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 2] & 0xFF;
		strPixels[uiPixelIndex + 1] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 1] & 0xFF;
		strPixels[uiPixelIndex + 2] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 0] & 0xFF;
		strPixels[uiPixelIndex + 3] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 3] & 0xFF;
		//strPixels[uiPixelIndex + 3] = ((getTexture()->getRasterFormat() & FORMAT_888) == FORMAT_888) ? 0xFF : (strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 3] & 0xFF);
		uiPixelIndex += 4;

		strPixels[uiPixelIndex] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 2] & 0xFF;
		strPixels[uiPixelIndex + 1] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 1] & 0xFF;
		strPixels[uiPixelIndex + 2] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 0] & 0xFF;
		strPixels[uiPixelIndex + 3] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 3] & 0xFF;
		//strPixels[uiPixelIndex + 3] = ((getTexture()->getRasterFormat() & FORMAT_888) == FORMAT_888) ? 0xFF : (strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 3] & 0xFF);
		uiPixelIndex += 4;
	}

	return strPixels;
}

string					CImageUtility::convertBGRA32ToPAL8(string& strRasterData)
{
	return "";
}
string					CImageUtility::convertPAL8ToBGRA32(string& strRasterData, string& strPaletteData, uint32 uiWidth, uint32 uiHeight)
{
	string strPixels;
	strPixels.resize(uiWidth * uiHeight * 4);

	uint32 uiPixelIndex = 0;
	for (uint32 i = 0, j = (uint32)(uiWidth * uiHeight); i < j; i++)
	{
		uint8 ucPaletteIndex = strRasterData.c_str()[i] & 0xFF;

		strPixels[uiPixelIndex] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 2] & 0xFF;
		strPixels[uiPixelIndex + 1] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 1] & 0xFF;
		strPixels[uiPixelIndex + 2] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 0] & 0xFF;
		strPixels[uiPixelIndex + 3] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 3] & 0xFF;
		//strPixels[uiPixelIndex + 3] = ((getTexture()->getRasterFormat() & FORMAT_888) == FORMAT_888) ? 0xFF : (strPaletteData.c_str()[(ucPaletteIndex * 4) + 3] & 0xFF);

		uiPixelIndex += 4;
	}

	return strPixels;
}

string					CImageUtility::swapRows(string& strRasterData, uint32 uiWidth, uint32 uiHeight)
{
	for (uint32 y = 0, j = (uint32)floor(uiHeight / 2); y < j; y++)
	{
		for (uint32 x = 0; x < uiWidth; x++)
		{
			int iPixelKey1 = ((y * uiWidth) + x) * 4;
			int iPixelKey2 = ((((uiHeight - 1) - y) * uiWidth) + x) * 4;

			uint8 ucBytes1[4], ucBytes2[4];

			// read
			ucBytes1[0] = strRasterData.c_str()[iPixelKey1];
			ucBytes1[1] = strRasterData.c_str()[iPixelKey1 + 1];
			ucBytes1[2] = strRasterData.c_str()[iPixelKey1 + 2];
			ucBytes1[3] = strRasterData.c_str()[iPixelKey1 + 3];
			ucBytes2[0] = strRasterData.c_str()[iPixelKey2];
			ucBytes2[1] = strRasterData.c_str()[iPixelKey2 + 1];
			ucBytes2[2] = strRasterData.c_str()[iPixelKey2 + 2];
			ucBytes2[3] = strRasterData.c_str()[iPixelKey2 + 3];

			// write
			strRasterData[iPixelKey1] = ucBytes2[0];
			strRasterData[iPixelKey1 + 1] = ucBytes2[1];
			strRasterData[iPixelKey1 + 2] = ucBytes2[2];
			strRasterData[iPixelKey1 + 3] = ucBytes2[3];
			strRasterData[iPixelKey2] = ucBytes1[0];
			strRasterData[iPixelKey2 + 1] = ucBytes1[1];
			strRasterData[iPixelKey2 + 2] = ucBytes1[2];
			strRasterData[iPixelKey2 + 3] = ucBytes1[3];
		}
	}
	return strRasterData;
}

string					CImageUtility::swapColumns(string& strRasterData, uint32 uiWidth, uint32 uiHeight)
{
	for (uint32 x = 0, j = (uint32)floor(uiWidth / 2); x < j; x++)
	{
		for (uint32 y = 0; y < uiHeight; y++)
		{
			int iPixelKey1 = ((y * uiWidth) + x) * 4;
			int iPixelKey2 = ((y * uiWidth) + ((uiWidth - 1) - x)) * 4;

			uint8 ucBytes1[4], ucBytes2[4];

			// read
			ucBytes1[0] = strRasterData.c_str()[iPixelKey1];
			ucBytes1[1] = strRasterData.c_str()[iPixelKey1 + 1];
			ucBytes1[2] = strRasterData.c_str()[iPixelKey1 + 2];
			ucBytes1[3] = strRasterData.c_str()[iPixelKey1 + 3];
			ucBytes2[0] = strRasterData.c_str()[iPixelKey2];
			ucBytes2[1] = strRasterData.c_str()[iPixelKey2 + 1];
			ucBytes2[2] = strRasterData.c_str()[iPixelKey2 + 2];
			ucBytes2[3] = strRasterData.c_str()[iPixelKey2 + 3];

			// write
			strRasterData[iPixelKey1] = ucBytes2[0];
			strRasterData[iPixelKey1 + 1] = ucBytes2[1];
			strRasterData[iPixelKey1 + 2] = ucBytes2[2];
			strRasterData[iPixelKey1 + 3] = ucBytes2[3];
			strRasterData[iPixelKey2] = ucBytes1[0];
			strRasterData[iPixelKey2 + 1] = ucBytes1[1];
			strRasterData[iPixelKey2 + 2] = ucBytes1[2];
			strRasterData[iPixelKey2 + 3] = ucBytes1[3];
		}
	}
	return strRasterData;
}
string					CImageUtility::swapRowsAndColumns(string& strRasterData, uint32 uiWidth, uint32 uiHeight)
{
	return swapColumns(swapRows(strRasterData, uiWidth, uiHeight), uiWidth, uiHeight);
}

D3DFORMAT				CImageUtility::getD3DFormatFromRasterDataFormat(eRasterDataFormat eRasterDataFormatValue)
{
	switch (eRasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:	return D3DFMT_DXT1;
	case RASTERDATAFORMAT_DXT2:	return D3DFMT_DXT2;
	case RASTERDATAFORMAT_DXT3:	return D3DFMT_DXT3;
	case RASTERDATAFORMAT_DXT4:	return D3DFMT_DXT4;
	case RASTERDATAFORMAT_DXT5:	return D3DFMT_DXT5;
	case RASTERDATAFORMAT_BGRA32:
	case RASTERDATAFORMAT_RGBA32:
	case RASTERDATAFORMAT_BGR32:
	case RASTERDATAFORMAT_RGB32:
	case RASTERDATAFORMAT_PAL8:
	case RASTERDATAFORMAT_PAL4:
		return D3DFMT_A8B8G8R8;
	case RASTERDATAFORMAT_BGR24:
	case RASTERDATAFORMAT_RGB24:
		return D3DFMT_R8G8B8;
	}
	return D3DFMT_UNKNOWN;
}
eRasterDataFormat		CImageUtility::getRasterDataFormatFromD3DFormat(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return RASTERDATAFORMAT_DXT1;
	case D3DFMT_DXT2:	return RASTERDATAFORMAT_DXT2;
	case D3DFMT_DXT3:	return RASTERDATAFORMAT_DXT3;
	case D3DFMT_DXT4:	return RASTERDATAFORMAT_DXT4;
	case D3DFMT_DXT5:	return RASTERDATAFORMAT_DXT5;
	case D3DFMT_A8B8G8R8:
		return RASTERDATAFORMAT_BGRA32;
	case D3DFMT_R8G8B8:
		return RASTERDATAFORMAT_BGR24;
	}
	return RASTERDATAFORMAT_UNKNOWN;
}

eDXTCompressionType		CImageUtility::getDXTCompressionTypeFromRasterDataFormat(eRasterDataFormat eRasterDataFormatValue)
{
	switch (eRasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:	return DXT_1;
	case RASTERDATAFORMAT_DXT2:	return DXT_2;
	case RASTERDATAFORMAT_DXT3:	return DXT_3;
	case RASTERDATAFORMAT_DXT4:	return DXT_4;
	case RASTERDATAFORMAT_DXT5:	return DXT_5;
	}
	return DXT_NOT_COMPRESSED;
}

uint8			CImageUtility::getBPPFromRasterDataFormat(eRasterDataFormat eRasterDataFormatValue)
{
	switch (eRasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:	return 16;
	case RASTERDATAFORMAT_DXT2:	return 16;
	case RASTERDATAFORMAT_DXT3:	return 32;
	case RASTERDATAFORMAT_DXT4:	return 32;
	case RASTERDATAFORMAT_DXT5:	return 32;
	case RASTERDATAFORMAT_BGRA32:
	case RASTERDATAFORMAT_RGBA32:
	case RASTERDATAFORMAT_BGR32:
	case RASTERDATAFORMAT_RGB32:
		return 32;
	case RASTERDATAFORMAT_PAL4:
		return 4;
	case RASTERDATAFORMAT_PAL8:
		return 8;
	case RASTERDATAFORMAT_BGR24:
	case RASTERDATAFORMAT_RGB24:
		return 24;
	}
	return 0;
}

string					CImageUtility::getD3DFormatToPack(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return "DXT1";
	case D3DFMT_DXT2:	return "DXT2";
	case D3DFMT_DXT3:	return "DXT3";
	case D3DFMT_DXT4:	return "DXT4";
	case D3DFMT_DXT5:	return "DXT5";
	case D3DFMT_A8B8G8R8:
		return "8888";
	case D3DFMT_R8G8B8:
		return "8880";
	}
	return "0000";
}

string					CImageUtility::getD3DFormatText(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return "DXT1";
	case D3DFMT_DXT2:	return "DXT2";
	case D3DFMT_DXT3:	return "DXT3";
	case D3DFMT_DXT4:	return "DXT4";
	case D3DFMT_DXT5:	return "DXT5";
	case D3DFMT_A8B8G8R8:
		return "8888";
	case D3DFMT_R8G8B8:
		return "8880";
	}
	return "UnknownD3DFormat";
}

string					CImageUtility::getFourCCFromDXTCompressionType(eDXTCompressionType eDXTCompressionTypeValue)
{
	switch (eDXTCompressionTypeValue)
	{
	case DXT_1:		return "DXT1";
	case DXT_2:		return "DXT2";
	case DXT_3:		return "DXT3";
	case DXT_4:		return "DXT4";
	case DXT_5:		return "DXT5";
	}
	return CStringUtility::packUint32(0);
}
eDXTCompressionType		CImageUtility::getDXTCompressionTypeFromFourCC(string strFourCC)
{
	if (strFourCC == "DXT1")
	{
		return DXT_1;
	}
	else if (strFourCC == "DXT2")
	{
		return DXT_2;
	}
	else if (strFourCC == "DXT3")
	{
		return DXT_3;
	}
	else if (strFourCC == "DXT4")
	{
		return DXT_4;
	}
	else if (strFourCC == "DXT5")
	{
		return DXT_5;
	}
	return DXT_NOT_COMPRESSED;
}

uint8			CImageUtility::getTexelByteCountFromDXTCompressionType(eDXTCompressionType eDXTCompressionTypeValue)
{
	switch (eDXTCompressionTypeValue)
	{
	case DXT_1:		return 8;
	case DXT_2:		return 16;
	case DXT_3:		return 16;
	case DXT_4:		return 16;
	case DXT_5:		return 16;
	}
	return 0;
}

uint8			CImageUtility::getSquishCompressionFlagsForDXTCompressionType(eDXTCompressionType eDXTCompressionTypeValue)
{
	switch (eDXTCompressionTypeValue)
	{
	case DXT_1:		return squish::kDxt1;
	case DXT_2:		return squish::kDxt3;
	case DXT_3:		return squish::kDxt3;
	case DXT_4:		return squish::kDxt5;
	case DXT_5:		return squish::kDxt5;
	}
	return 0;
}