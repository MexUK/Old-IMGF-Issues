#include "CRWSection_TextureNative.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "TXD/eTXDRasterDataFormat.h"
#include "TXD/CTXDManager.h"
#include "String/CStringUtility.h"
#include "Image/CImageUtility.h"
#include "eExceptionCode.h"

using namespace std;

CRWSection_TextureNative::CRWSection_TextureNative(void) :
	m_ePlatform(_PLATFORM_UNKNOWN),
	m_bHasDiffuse(false),
	m_bHasAlpha(false),
	m_bPaletteIsUsed(false),
	m_uiPlatformId(0),
	m_ucFilterFlags(0),
	m_ucTextureWrapUV(0),
	m_uiTXDRasterDataFormat(0),
	m_eRasterDataFormat(RASTERDATAFORMAT_UNKNOWN),
	m_uiAlpha(0),
	m_ucBPP(0),
	m_ucRasterType(0),
	m_eDXTCompressionType(DXT_NOT_COMPRESSED),
	m_bCubeTexture(false),
	m_bAutoMipMaps(false),
	m_bIsNotRWCompatible(false),
	m_ucOriginalBPP(0)
{
	setSectionId(RW_SECTION_TEXTURE_DICTIONARY);
	m_vecImageSize.m_x = 0;
	m_vecImageSize.m_y = 0;
}

void							CRWSection_TextureNative::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance(); // todo - make better code for peek and seek-restore
	uint32 uiPreviousSeek = pDataReader->getSeek();
	pDataReader->setPeek(true);
	uint32 uiPlatformId = pDataReader->readUint32();
	pDataReader->setPeek(false);
	pDataReader->setSeek(uiPreviousSeek);

	m_ePlatform = CTXDManager::getPlatformFromTXDPlatformId(uiPlatformId);

	unserializeHeader();
	unserializeBody();
}

void							CRWSection_TextureNative::serialize(void)
{
	serializeHeader();
	serializeBody();
}

void							CRWSection_TextureNative::unserializeHeader(void)
{
	switch (m_ePlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		unserializeHeader_Direct3D();
	case PLATFORM_PS2:
		unserializeHeader_PS2();
	case PLATFORM_ANDROID:
		unserializeHeader_Android();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
	setRasterDataFormat(detectRasterDataFormat(), false);
}

void							CRWSection_TextureNative::unserializeBody(void)
{
	switch (m_ePlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		return unserializeBody_Direct3D();
	case PLATFORM_PS2:
		return unserializeBody_PS2();
	case PLATFORM_ANDROID:
		return unserializeBody_Android();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void							CRWSection_TextureNative::serializeHeader(void)
{
	switch (m_ePlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		return serializeHeader_Direct3D();
	case PLATFORM_PS2:
		return serializeHeader_PS2();
	case PLATFORM_ANDROID:
		return serializeHeader_Android();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void							CRWSection_TextureNative::serializeBody(void)
{
	switch (m_ePlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		return serializeBody_Direct3D();
	case PLATFORM_PS2:
		return serializeBody_PS2();
	case PLATFORM_ANDROID:
		return serializeBody_Android();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void							CRWSection_TextureNative::unserializeHeader_Direct3D(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// struct TextureFormat
	m_uiPlatformId = pDataReader->readUint32();
	m_ucFilterFlags = pDataReader->readUint8();
	m_ucTextureWrapUV = pDataReader->readUint8();
	string strPadding = pDataReader->readString(2);
	setDiffuseName(CStringUtility::rtrimFromLeft(pDataReader->readString(32)));
	if (getDiffuseName() != "")
	{
		setHasDiffuse(true);
	}
	setAlphaName(CStringUtility::rtrimFromLeft(pDataReader->readString(32)));
	if (getAlphaName() != "")
	{
		setHasAlpha(true);
	}

	// struct RasterFormat
	setTXDRasterDataFormat(pDataReader->readUint32());
	if (m_uiPlatformId == 9)
	{
		// GTA SA
		setDXTCompressionType(CImageUtility::getDXTCompressionTypeFromFourCC(pDataReader->readString(4)));
		setAlpha(0);
	}
	else
	{
		// GTA III & VC
		setAlpha(pDataReader->readUint32());
	}
	CSize2D vecImageSize;
	vecImageSize.m_x = pDataReader->readUint16();
	vecImageSize.m_y = pDataReader->readUint16();
	setImageSize(vecImageSize);
	setBPP(pDataReader->readUint8());
	uint8 uiMipMapCount = pDataReader->readUint8();
	setRasterType(pDataReader->readUint8());
	if (getPlatformId() == 9)
	{
		// GTA SA
		uint8 ucSAData = pDataReader->readUint8();
		setAlpha((bool)(ucSAData & 1));
		setCubeTexture((bool)(ucSAData & 2));
		setAutoMipMaps((bool)(ucSAData & 4));
		setIsNotRWCompatible((bool)(ucSAData & 8));
	}
	else
	{
		// GTA III & VC
		setDXTCompressionType((eDXTCompressionType)pDataReader->readUint8());
		setCubeTexture(false);
		setAutoMipMaps(false);
		setIsNotRWCompatible(false);
	}
}

void							CRWSection_TextureNative::unserializeHeader_PS2(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// struct TextureFormat
	setPlatformId(pDataReader->readUint32());
	setFilterFlags(pDataReader->readUint8());
	setTextureWrapUV(pDataReader->readUint8());
	string strPadding = pDataReader->readString(2);

	setDiffuseName(unserializeString());
	if (getDiffuseName() != "")
	{
		setHasDiffuse(true);
	}
	setAlphaName(unserializeString());
	if (getAlphaName() != "")
	{
		setHasAlpha(true);
	}

	// struct RasterFormat
	pDataReader->readUint32(); // RW section header
	pDataReader->readUint32();
	pDataReader->readUint32();

	pDataReader->readUint32(); // RW section header
	pDataReader->readUint32();
	pDataReader->readUint32();

	CSize2D vecImageSize;
	m_vecImageSize.m_x = pDataReader->readUint32();
	m_vecImageSize.m_y = pDataReader->readUint32();
	setImageSize(vecImageSize);
	setBPP((uint8)pDataReader->readUint32());
	setTXDRasterDataFormat(pDataReader->readUint32());
	uint32 uiTex0_a = pDataReader->readUint32();
	uint32 uiTex0_b = pDataReader->readUint32();
	uint32 uiTex1_a = pDataReader->readUint32();
	uint32 uiTex1_b = pDataReader->readUint32();
	uint32 miptbp1_a = pDataReader->readUint32();
	uint32 miptbp1_b = pDataReader->readUint32();
	uint32 miptbp2_a = pDataReader->readUint32();
	uint32 miptbp2_b = pDataReader->readUint32();
	uint32 uiTexelDataSectionSize = pDataReader->readUint32();
	uint32 uiPaletteDataSectionSize = pDataReader->readUint32();
	uint32 uiGpuDataAlignedSize = pDataReader->readUint32();
	uint32 uiSkyMipmapVal = pDataReader->readUint32();
}

void							CRWSection_TextureNative::unserializeHeader_Android(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// struct TextureFormat
	setPlatformId(pDataReader->readUint32());
	setFilterFlags(pDataReader->readUint8());
	setTextureWrapUV(pDataReader->readUint8());
	string strPadding = pDataReader->readString(2);
	pDataReader->readUint32();
	pDataReader->readUint32();
	pDataReader->readString(16);
	setDiffuseName(CStringUtility::rtrimFromLeft(pDataReader->readString(32)));
	if (getDiffuseName() != "")
	{
		setHasDiffuse(true);
	}
	setAlphaName(CStringUtility::rtrimFromLeft(pDataReader->readString(32)));
	if (getAlphaName() != "")
	{
		setHasAlpha(true);
	}

	// struct RasterFormat
	pDataReader->readUint32();
	m_vecImageSize.m_x = pDataReader->readUint16();
	m_vecImageSize.m_y = pDataReader->readUint16();
	pDataReader->readUint32();
	pDataReader->readUint32();

	setBPP(8);
	setTXDRasterDataFormat(TXDRASTERDATAFORMAT_EXT_PAL8 | TXDRASTERDATAFORMAT_8888);
}

void							CRWSection_TextureNative::unserializeBody_Direct3D(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// texture native struct body - palette
	bool bPaletteUsed = false;
	uint32 uiPaletteSize = 0;
	if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL4) == TXDRASTERDATAFORMAT_EXT_PAL4)
	{
		bPaletteUsed = true;
		uiPaletteSize = 16;
	}
	else if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8) == TXDRASTERDATAFORMAT_EXT_PAL8)
	{
		bPaletteUsed = true;
		uiPaletteSize = 256;
	}

	if (bPaletteUsed)
	{
		uint32 uiPaletteDataSize = uiPaletteSize * 4;
		setPaletteUsed(true);
		setPaletteData(pDataReader->readString(uiPaletteDataSize));
	}

	// texture native struct body - mipmap image data
	uint32
		uiMipmapWidth = getImageSize().m_x,
		uiMipmapHeight = getImageSize().m_y;
	for (uint8 ucMipmapIndex = 0, j = getMipMaps().getEntryCount(); ucMipmapIndex < j; ucMipmapIndex++)
	{
		uint32 uiImageDataSize = pDataReader->readUint32();

		CSize2D vecImageSize;
		vecImageSize.m_x = uiMipmapWidth;
		vecImageSize.m_y = uiMipmapHeight;

		CRWEntry_TextureNative_MipMap *pMipmap = new CRWEntry_TextureNative_MipMap(this);
		pMipmap->setImageSize(vecImageSize);
		pMipmap->setRasterData(pDataReader->readString(uiImageDataSize));
		getMipMaps().addEntry(pMipmap);

		uiMipmapWidth /= 2;
		uiMipmapHeight /= 2;
	}
}

void							CRWSection_TextureNative::unserializeBody_PS2(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	pDataReader->readUint32(); // RW section header
	pDataReader->readUint32();
	pDataReader->readUint32();

	uint32 uiMipmapWidth = getImageSize().m_x;
	uint32 uiMipmapHeight = getImageSize().m_y;
	uint32 j = getMipMaps().getEntryCount();
	if (j == 0)
	{
		j = 1;
	}
	for (uint32 uiMipmapIndex = 0; uiMipmapIndex < j; uiMipmapIndex++)
	{
		uint32 uiImageDataSize;
		uint32 uiSwizzledWidth;
		uint32 uiSwizzledHeight;

		if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_PS2_SWIZZLEDWITHHEADER)
		{
			pDataReader->readString(8 * 4);
			uiSwizzledWidth = pDataReader->readUint32();
			uiSwizzledHeight = pDataReader->readUint32();
			pDataReader->readString(6 * 4);
			uiImageDataSize = pDataReader->readUint32() * 0x10;
			pDataReader->readString(3 * 4);
		}
		else
		{
			uiSwizzledWidth = uiMipmapWidth;
			uiSwizzledHeight = uiMipmapHeight;
			if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_PS2_SWIZZLEDNOHEADER)
			{
				uiSwizzledWidth /= 2;
				uiSwizzledHeight /= 2;
			}

			uiImageDataSize = uiMipmapWidth * uiMipmapHeight * ((float32)getBPP() / 8.0f);
		}

		CRWEntry_TextureNative_MipMap *pMipmap = new CRWEntry_TextureNative_MipMap(this);

		CSize2D vecImageSize, vecSwizzledImageSize;
		vecImageSize.m_x = uiMipmapWidth;
		vecImageSize.m_y = uiMipmapHeight;
		vecSwizzledImageSize.m_x = uiSwizzledWidth;
		vecSwizzledImageSize.m_y = uiSwizzledHeight;
		pMipmap->setImageSize(vecImageSize);
		pMipmap->setSwizzledImageSize(vecSwizzledImageSize);
		pMipmap->setRasterData(pDataReader->readString(uiImageDataSize));
		getMipMaps().addEntry(pMipmap);

		uiMipmapWidth /= 2;
		uiMipmapHeight /= 2;
	}

	if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL4) || (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8))
	{
		uint32 uiUnknown1, uiUnknown2, uiUnknown3, uiUnknown4;
		uiUnknown1 = 0;
		uiUnknown2 = 0;
		uiUnknown3 = 0;
		uiUnknown4 = 0;
		if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_PS2_SWIZZLEDWITHHEADER)
		{
			pDataReader->readString(5 * 4);
			uiUnknown1 = pDataReader->readUint32();
			pDataReader->readString(2 * 4);
			uiUnknown2 = pDataReader->readUint32();
			uiUnknown3 = pDataReader->readUint32();
			pDataReader->readString(6 * 4);
			uiUnknown4 = pDataReader->readUint32();
			pDataReader->readString(3 * 4);
		}

		uint32 uiPaletteSize;
		if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8)
		{
			uiPaletteSize = 0x100;
		}
		else
		{
			uiPaletteSize = 0x10;
		}

		setPaletteUsed(true);
		setPaletteData(pDataReader->readString(uiPaletteSize * 4));

		// need to work on 4bit palettes in vc & sa
		if (uiUnknown2 == 8 && uiUnknown3 == 3 && uiUnknown4 == 6)
		{
			pDataReader->readString(32);
		}
	}

	unswizzlePS2Format();
}

void							CRWSection_TextureNative::unserializeBody_Android(void)
{
	// same body data as texture native for PC
	return unserializeBody_Direct3D();
}

void							CRWSection_TextureNative::serializeHeader_Direct3D(void)
{
	// texture native struct header - 86 bytes

	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// struct TextureFormat
	pDataWriter->writeUint32(m_uiPlatformId);
	pDataWriter->writeUint8(m_ucFilterFlags);
	pDataWriter->writeUint8(m_ucTextureWrapUV);
	pDataWriter->writeString(2); // 2 bytes padding
	
	if (doesHaveDiffuse())
	{
		pDataWriter->writeStringRef(m_strDiffuseName, 32);
	}
	else
	{
		pDataWriter->writeString(32);
	}

	if (doesHaveAlpha())
	{
		pDataWriter->writeStringRef(m_strAlphaName, 32);
	}
	else
	{
		pDataWriter->writeString(32);
	}

	// struct RasterFormat
	pDataWriter->writeUint32(getTXDRasterDataFormat() | (getMipMaps().getEntryCount() > 1 ? TXDRASTERDATAFORMAT_EXT_MIPMAP : 0));
	
	if (m_uiPlatformId == 9)
	{
		// GTA SA
		pDataWriter->writeString(CImageUtility::getD3DFormatToPack(CImageUtility::getD3DFormatFromRasterDataFormat(m_eRasterDataFormat)));
	}
	else
	{
		// GTA III & VC
		pDataWriter->writeUint32((doesHaveAlpha() ? 1 : 0));
	}

	pDataWriter->writeUint16(getImageSize().m_x);
	pDataWriter->writeUint16(getImageSize().m_y);
	pDataWriter->writeUint8(getBPP());
	pDataWriter->writeUint32(getMipMaps().getEntryCount());
	pDataWriter->writeUint8(getRasterType());
	if (m_uiPlatformId == 9)
	{
		// GTA SA
		uint8 ucSAData = 0;
		if (doesHaveAlpha())
		{
			ucSAData |= 1;
		}
		if (getCubeTexture())
		{
			ucSAData |= 2;
		}
		if (getAutoMipMaps())
		{
			ucSAData |= 4;
		}
		if (getIsNotRWCompatible())
		{
			ucSAData |= 8;
		}
		pDataWriter->writeUint8(ucSAData);
	}
	else
	{
		// GTA III & VC
		pDataWriter->writeUint8(getDXTCompressionType());
	}
}

void							CRWSection_TextureNative::serializeHeader_PS2(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// todo
}

void							CRWSection_TextureNative::serializeHeader_Android(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// todo
}

void							CRWSection_TextureNative::serializeBody_Direct3D(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	for (auto pMipmap : getMipMaps().getEntries())
	{
		pDataWriter->writeUint32(pMipmap->getRasterData().length());
		pDataWriter->writeStringRef(pMipmap->getRasterData());
	}
}

void							CRWSection_TextureNative::serializeBody_PS2(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// todo
}

void							CRWSection_TextureNative::serializeBody_Android(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// todo
}

string							CRWSection_TextureNative::unserializeString(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	pDataReader->readUint32(); // RW section header
	uint32 uiSectionSize = pDataReader->readUint32();
	pDataReader->readUint32();

	pDataReader->setPeek(true);
	uint32 uiPreviousSeek = pDataReader->getSeek();
	string strData = pDataReader->readStringUntilZero();
	pDataReader->setPeek(false);
	pDataReader->setSeek(uiPreviousSeek);

	uint32 uiStringLength;
	if (uiSectionSize == 0)
	{
		uiStringLength = (uint32)(ceil(((float32)(strData.length() + 1)) / 4.0) * 4);
	}
	else
	{
		uiStringLength = uiSectionSize;
	}

	pDataReader->readString(uiStringLength);
	return strData;
}

void							CRWSection_TextureNative::setRasterDataFormat(eRasterDataFormat eRasterDataFormatValue, bool bUpdateTXDRasterDataFormat)
{
	m_eRasterDataFormat = eRasterDataFormatValue;
	if (bUpdateTXDRasterDataFormat)
	{
		m_uiTXDRasterDataFormat = CTXDManager::getTXDRasterDataFormatFromRasterDataFormat(eRasterDataFormatValue);
		m_eDXTCompressionType = CImageUtility::getDXTCompressionTypeFromRasterDataFormat(eRasterDataFormatValue);
	}
}

bool							CRWSection_TextureNative::doesHaveValidTXDRasterDataFormat(void)
{
	return CRWSection_TextureNative::doesTXDRasterDataFormatExist(getTXDRasterDataFormat());
}

bool							CRWSection_TextureNative::doesTXDRasterDataFormatExist(uint32 uiTXDRasterDataFormat)
{
	switch (uiTXDRasterDataFormat & 0xFFB) // (0xFFF - 4) As TXD format versions for PS2 can have a value 4 OR'd into the TXD raster data format value.
	{
	case TXDRASTERDATAFORMAT_4444:
	case TXDRASTERDATAFORMAT_1555:
	case TXDRASTERDATAFORMAT_565:
	case TXDRASTERDATAFORMAT_8888:
	case TXDRASTERDATAFORMAT_888:
		return true;
	}
	return false;
}

eRasterDataFormat				CRWSection_TextureNative::detectRasterDataFormat(void)
{
	if (m_uiTXDRasterDataFormat & TXDRASTERDATAFORMAT_EXT_PAL4)
	{
		return RASTERDATAFORMAT_PAL4;
	}
	else if (m_uiTXDRasterDataFormat & TXDRASTERDATAFORMAT_EXT_PAL8)
	{
		return RASTERDATAFORMAT_PAL8;
	}

	switch (m_uiTXDRasterDataFormat & 0xFFB)
	{
	case TXDRASTERDATAFORMAT_8888:
		return RASTERDATAFORMAT_BGRA32;
	case TXDRASTERDATAFORMAT_888:
		switch (m_ucBPP)
		{
		case 32:		return RASTERDATAFORMAT_BGR32;
		case 24:		return RASTERDATAFORMAT_BGR24;
		}
		break;
	case TXDRASTERDATAFORMAT_565:
	case TXDRASTERDATAFORMAT_1555:
	case TXDRASTERDATAFORMAT_4444:
		switch (m_eDXTCompressionType)
		{
		case DXT_1:		return RASTERDATAFORMAT_DXT1;
		case DXT_2:		return RASTERDATAFORMAT_DXT2;
		case DXT_3:		return RASTERDATAFORMAT_DXT3;
		case DXT_4:		return RASTERDATAFORMAT_DXT4;
		case DXT_5:		return RASTERDATAFORMAT_DXT5;
		}
		break;
	}

	return RASTERDATAFORMAT_UNKNOWN;
}

void							CRWSection_TextureNative::unswizzlePS2Format(void)
{
	uint32 uiWidth = getImageSize().m_x;
	uint32 uiHeight = getImageSize().m_y;
	uint32 k = getMipMaps().getEntryCount() == 0 ? 1 : getMipMaps().getEntryCount();
	for (uint32 j = 0; j < k; j++)
	{
		// skip mipmaps - as undocumented
		if (j > 0)
		{
			break;
		}
		bool swizzled = (getMipMaps().getEntryByIndex(j)->getSwizzledImageSize().m_y != uiHeight);

		string strRasterData = getMipMaps().getEntryByIndex(0)->getRasterData();
		const char *pRasterData = strRasterData.c_str();
		uint32 uiImageDataSize = strRasterData.length();
		string strNewRasterData;
		uint8 *oldtexels;
		uint8 *newtexels;
		switch (getBPP())
		{
		case 4:
			// convert from 4 BPP to 8 BPP

			oldtexels = (uint8*)pRasterData;
			uiImageDataSize *= 2;
			newtexels = new uint8[uiImageDataSize];
			for (uint32 i = 0; i < (uiImageDataSize / 2); i++)
			{
				newtexels[i * 2 + 0] = oldtexels[i] & 0x0F;
				newtexels[i * 2 + 1] = oldtexels[i] >> 4;
			}
			strNewRasterData.append((char*)newtexels, uiImageDataSize);
			delete[] newtexels;
			getMipMaps().getEntryByIndex(0)->setRasterData(strNewRasterData);
			setBPP(8);
			//setTXDRasterDataFormat(getTXDRasterDataFormat() | ~TXDRASTERDATAFORMAT_EXT_PAL4);
			//setTXDRasterDataFormat(getTXDRasterDataFormat() | TXDRASTERDATAFORMAT_EXT_PAL8);
			setRasterDataFormat(RASTERDATAFORMAT_PAL8, true);
			setOriginalBPP(4);

			if (swizzled)
			{
				processPs2Swizzle();
			}
			break;
		case 8:
			if (swizzled)
			{
				processPs2Swizzle();
			}
			unclut(getImageSize().m_x, getImageSize().m_y);
			break;
		case 32:
			newtexels = new uint8[uiImageDataSize];
			for (uint32 i = 0; i < uiWidth * uiHeight; i++)
			{
				// swap R and B
				uint8 tmp = pRasterData[i * 4 + 0];
				newtexels[i * 4 + 0] = pRasterData[i * 4 + 2];
				newtexels[i * 4 + 1] = pRasterData[i * 4 + 1];
				newtexels[i * 4 + 2] = tmp;
				newtexels[i * 4 + 3] = pRasterData[i * 4 + 3];
				// fix alpha
				uint32 newval = pRasterData[i * 4 + 3] * 0xff;
				newval /= 0x80;
				newtexels[i * 4 + 3] = newval;
				//				if (texels[j][i*4+3] != 0xFF)
				//					hasAlpha = true;
			}
			strNewRasterData.append((char*)newtexels, uiImageDataSize);
			delete[] newtexels;
			getMipMaps().getEntryByIndex(0)->setRasterData(strNewRasterData);
			break;
		}
	}

	// can't understand ps2 mipmaps
	if (getMipMaps().getEntryCount() > 1)
	{
		auto pMipMap = getMipMaps().getEntryByIndex(0);
		getMipMaps().removeAllEntries();
		getMipMaps().addEntry(pMipMap);

		setTXDRasterDataFormat(~(TXDRASTERDATAFORMAT_EXT_AUTO_MIPMAP | TXDRASTERDATAFORMAT_EXT_MIPMAP));
	}

	if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL4) || (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8))
	{
		uint32 uiPaletteSize = m_strPaletteData.length() / 4;
		uint32 uiAlphaDistribution = 0;
		uint32 aref = 0x40;
		for (uint32 i = 0; i < uiPaletteSize; i++)
		{
			if ((uiAlphaDistribution & 0x1) == 0 && (m_strPaletteData.c_str()[i * 4 + 3] & 0xFF) >= aref)
			{
				uiAlphaDistribution |= 0x1;
			}
			else if ((uiAlphaDistribution & 0x2) == 0 && (m_strPaletteData.c_str()[i * 4 + 3] & 0xFF) < aref)
			{
				uiAlphaDistribution |= 0x2;
			}
			uint32 uiNewAlpha = (m_strPaletteData.c_str()[i * 4 + 3] & 0xFF) * 0xff;
			uiNewAlpha /= 0x80;
			m_strPaletteData[i * 4 + 3] = uiNewAlpha;
		}
	}

	setDXTCompressionType(DXT_NOT_COMPRESSED);
}

void							CRWSection_TextureNative::processPs2Swizzle(void)
{
	auto pMipmap = getMipMaps().getEntryByIndex(0);
	uint32 uiSwizzledWidth = pMipmap->getSwizzledImageSize().m_x;
	uint32 uiSwizzledHeight = pMipmap->getSwizzledImageSize().m_y;

	//dataSizes[i] = swizzleWidth[i] * swizzleHeight[i] * 4;
	uint32 uiDataSize = uiSwizzledWidth * uiSwizzledHeight * 4;
	uint8 *pNewtexels = new uint8[uiDataSize];
	unswizzle8(pNewtexels, (uint8*)pMipmap->getRasterData().c_str(), uiSwizzledWidth * 2, uiSwizzledHeight * 2);
	string strRasterData;
	strRasterData.append((char*)pNewtexels, uiDataSize);
	delete[] pNewtexels;
	pMipmap->setRasterData(strRasterData);

	const char *pRasterData = pMipmap->getRasterData().c_str();
	uint32 uiStride = uiSwizzledWidth * 2;
	if (uiStride != pMipmap->getImageSize().m_x)
	{
		//dataSizes[i] = width[i] * height[i];
		uint32 uiDataSize2 = pMipmap->getImageSize().m_x * pMipmap->getImageSize().m_y;
		uint8 *pNewtexels2 = new uint8[uiDataSize2];
		for (uint32 y = 0; y < pMipmap->getImageSize().m_y; y++)
		{
			for (uint32 x = 0; x < pMipmap->getImageSize().m_x; x++)
			{
				pNewtexels2[y*pMipmap->getImageSize().m_x + x] = pRasterData[y*uiStride + x];
			}
		}
		string strRasterData2;
		strRasterData2.append((char*)pNewtexels2, uiDataSize2);
		delete[] pNewtexels2;
		pMipmap->setRasterData(strRasterData2);
	}
}

void							CRWSection_TextureNative::unswizzle8(uint8 *pTexels, uint8 *pRawIndices, uint32 uiWidth, uint32 uiHeight)
{
	for (uint32 y = 0; y < uiHeight; y++)
	{
		for (uint32 x = 0; x < uiWidth; x++)
		{
			int32 block_loc = (y&(~0x0F))*uiWidth + (x&(~0x0F)) * 2;
			uint32 swap_sel = (((y + 2) >> 2) & 0x01) * 4;
			int32 ypos = (((y&(~3)) >> 1) + (y & 1)) & 0x07;
			int32 column_loc = ypos*uiWidth * 2 + ((x + swap_sel) & 0x07) * 4;
			int32 byte_sum = ((y >> 1) & 1) + ((x >> 2) & 2);
			uint32 swizzled = block_loc + column_loc + byte_sum;
			//			cout << swizzled << endl;
			pTexels[y*uiWidth + x] = pRawIndices[swizzled];
		}
	}
}

void							CRWSection_TextureNative::unclut(uint32 uiWidth, uint32 uiHeight)
{
	auto pMipmap = getMipMaps().getEntryByIndex(0);
	string strRasterData = pMipmap->getRasterData();
	uint8 map[4] = { 0, 16, 8, 24 };
	for (uint32 i = 0; i < (uiWidth*uiHeight); i++)
	{
		strRasterData[i] = (strRasterData.c_str()[i] & ~0x18) | map[(strRasterData.c_str()[i] & 0x18) >> 3];
	}
	pMipmap->setRasterData(strRasterData);
}

void							CRWSection_TextureNative::convertToGame(ePlatformedGame ePlatformedGame, vector<string>& vecMipmapsRemoved)
{
	switch (ePlatformedGame)
	{
	case PLATFORMED_GAME_PC_GTA_III:
		setPlatformId(8);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_565);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(16);
		setDXTCompressionType(DXT_1);

		convertToRasterDataFormat(RASTERDATAFORMAT_DXT1, vecMipmapsRemoved);
		break;
	case PLATFORMED_GAME_PC_GTA_VC:
		setPlatformId(8);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_565);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(16);
		setDXTCompressionType(DXT_1);

		convertToRasterDataFormat(RASTERDATAFORMAT_DXT1, vecMipmapsRemoved);
		break;
	case PLATFORMED_GAME_PC_GTA_SA:
		setPlatformId(9);
		setFilterFlags(1);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_565);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(16);
		setDXTCompressionType(DXT_1);

		convertToRasterDataFormat(RASTERDATAFORMAT_DXT1, vecMipmapsRemoved);
		break;
	case PLATFORMED_GAME_ANDROID_GTA:
		setPlatformId(8);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_888);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(32);
		setDXTCompressionType(DXT_NOT_COMPRESSED);

		convertToRasterDataFormat(RASTERDATAFORMAT_BGR32, vecMipmapsRemoved);
		break;
	case PLATFORMED_GAME_XBOX_GTA:
		setPlatformId(5);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_888);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(32);
		setDXTCompressionType(DXT_NOT_COMPRESSED);

		convertToRasterDataFormat(RASTERDATAFORMAT_BGR32, vecMipmapsRemoved);
		break;
	}
}

void							CRWSection_TextureNative::convertToRasterDataFormat(eRasterDataFormat eRasterDataFormatValue, vector<string>& vecMipmapsRemoved)
{
	vector<CRWEntry_TextureNative_MipMap*> vecMipmapsToRemove;
	uint32 uiMipmapIndex = 0;
	for (auto pMipmap : getMipMaps().getEntries())
	{
		if (!pMipmap->canRasterDataBeConverted())
		{
			vecMipmapsToRemove.push_back(pMipmap);
			vecMipmapsRemoved.push_back("Texture (" + getDiffuseName() + ") - Mipmap #" + CStringUtility::toString(uiMipmapIndex + 1));
			continue;
		}

		pMipmap->convertToRasterDataFormat(eRasterDataFormatValue);
		uiMipmapIndex++;
	}
	for (auto pMipmap : vecMipmapsToRemove)
	{
		getMipMaps().removeEntry(pMipmap);
	}

	setRasterDataFormat(eRasterDataFormatValue, true);
	//setTXDRasterDataFormat(CTXDManager::getTXDRasterDataFormatFromRasterDataFormat(eRasterDataFormatValue));
	setBPP(CImageUtility::getBPPFromRasterDataFormat(eRasterDataFormatValue));
	setDXTCompressionType(CImageUtility::getDXTCompressionTypeFromRasterDataFormat(eRasterDataFormatValue));

	if (eRasterDataFormatValue != RASTERDATAFORMAT_PAL4 && eRasterDataFormatValue != RASTERDATAFORMAT_PAL8)
	{
		string strPaletteData = "";

		setPaletteUsed(false);
		setPaletteData(strPaletteData);
	}
}