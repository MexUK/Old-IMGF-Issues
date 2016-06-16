#include "CBMPFormat.h"
#include "Image/CImageUtility.h"
#include "Testing/CDebugger.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "Memory/CMemoryUtility.h"
#include "BMP/CBMPFormat_Header1.h"
#include "BMP/CBMPFormat_Header2_Version2.h"
#include "BMP/CBMPFormat_Header2_Version3.h"
#include "BMP/CBMPFormat_Header2_Version4.h"

using namespace std;

CBMPFormat::CBMPFormat(void) :
	CFormat(true, LITTLE_ENDIAN),
	m_bSkipBMPFileHeaderForSerialize(false),
	m_bHasPalette(false),
	m_uiBMPVersion(0),
	m_usFileType(0),
	m_usFileSize(0),
	m_usColourPlaneCount(0),
	m_uiWidth(0),
	m_uiHeight(0),
	m_usBPP(0)
{};

void			CBMPFormat::unserialize(void)
{
	m_uiBMPVersion = detectBMPVersion();
	switch (m_uiBMPVersion)
	{
	case 1:		return unserializeVersion1();
	case 2:		return unserializeVersion2();
	case 3:		return unserializeVersion3();
	case 4:		return unserializeVersion4();
	default:	throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
	}
}

void			CBMPFormat::serialize(void)
{
	switch (m_uiBMPVersion)
	{
	case 1:		return serializeVersion1();
	case 2:		return serializeVersion2();
	case 3:		return serializeVersion3();
	case 4:		return serializeVersion4();
	default:	throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
	}
}

uint8			CBMPFormat::detectBMPVersion(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	uint8 uiBMPVersion = 0;
	pDataReader->setPeek(true);
	string strHeader = pDataReader->readString(2);
	if (strHeader.c_str()[0] == '\0' && strHeader.c_str()[1] == '\0')
	{
		// BMP version 1.x
		uiBMPVersion = 1;
	}
	else if (strHeader.c_str()[0] == 'B' && strHeader.c_str()[1] == 'M')
	{
		// BMP version 2.x, 3.x or 4.x
		pDataReader->setSeek(12);
		switch (pDataReader->readUint32())
		{
		case 12: // BMP version 2.x
			uiBMPVersion = 2;
			break;
		case 40: // BMP version 3.x
			uiBMPVersion = 3;
			break;
		case 108: // BMP version 4.x
			uiBMPVersion = 4;
		}
	}
	pDataReader->setPeek(false);
	pDataReader->setSeek(0);
	return uiBMPVersion;
}

void			CBMPFormat::swapRows(void)
{
	m_strRasterData = CImageUtility::swapRows(m_strRasterData, m_uiWidth, m_uiHeight);
}

void			CBMPFormat::setRasterDataBGRA32(string& strRasterDataBGRA32)
{
	// todo
}
string			CBMPFormat::getRasterDataBGRA32(void)
{
	uint32
		uiSeek,
		uiRasterDataLength = m_strRasterData.length(),
		uiImageDataIndex = 0;
	const char *
		pRasterData = m_strRasterData.c_str();
	uint8
		uiCalculationPart1 = m_uiWidth * ((float32)m_usBPP / 8.0f);
	string
		strImageData;
	strImageData.resize((uiRasterDataLength / (m_usBPP / 4)) * 4);
	for (int i = 0; i < m_uiHeight; i++)
	{
		uiSeek = uiRasterDataLength - ((i + 1) * uiCalculationPart1);

		for (int i2 = 0; i2 < m_uiWidth; i2++)
		{
			strImageData[uiImageDataIndex++] = pRasterData[uiSeek++];
			strImageData[uiImageDataIndex++] = pRasterData[uiSeek++];
			strImageData[uiImageDataIndex++] = pRasterData[uiSeek++];
			strImageData[uiImageDataIndex++] = '\xff';

			if (m_usBPP == 32)
			{
				uiSeek++;
			}
		}
	}
	return strImageData;
}

void			CBMPFormat::unserializeVersion1(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
}

void			CBMPFormat::unserializeVersion2(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	uint32 uiByteCount;

	// read headers 1 & 2
	CBMPFormat_Header1 *pHeader1 = pDataReader->readStruct<CBMPFormat_Header1>();
	CBMPFormat_Header2_Version2 *pHeader2 = pDataReader->readStruct<CBMPFormat_Header2_Version2>();

	// palette
	m_bHasPalette = pHeader2->m_usBPP < 16;
	if (m_bHasPalette)
	{
		uiByteCount = 3 * (1 << pHeader2->m_usBPP);
		m_strPaletteData = pDataReader->readString(uiByteCount);
	}

	// raster data
	uiByteCount = pHeader2->m_uiWidth * pHeader2->m_uiHeight * ((float32)pHeader2->m_usBPP / 8.0f);
	m_strRasterData = pDataReader->readString(uiByteCount);

	// copy from raw structs to wrapper structs
	m_uiWidth = pHeader2->m_uiWidth;
	m_uiHeight = pHeader2->m_uiHeight;
	m_usColourPlaneCount = pHeader2->m_usPlaneCount;
	m_usBPP = pHeader2->m_usBPP;

	// clean up
	delete pHeader1;
	delete pHeader2;
}


void			CBMPFormat::unserializeVersion3(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	uint32 uiByteCount;

	// read headers 1 & 2
	CBMPFormat_Header1 *pHeader1 = pDataReader->readStruct<CBMPFormat_Header1>();
	CBMPFormat_Header2_Version3 *pHeader2 = pDataReader->readStruct<CBMPFormat_Header2_Version3>();

	// palette
	m_bHasPalette = pHeader2->m_usBPP < 16;
	if (m_bHasPalette)
	{
		uiByteCount = 4 * pHeader2->m_uiColoursUsed;
		m_strPaletteData = pDataReader->readString(uiByteCount);
	}

	// raster data
	uiByteCount = pHeader2->m_uiWidth * pHeader2->m_uiHeight * ((float32)pHeader2->m_usBPP / 8.0f);
	m_strRasterData = pDataReader->readString(uiByteCount);

	// copy from raw structs to wrapper structs
	m_uiWidth = pHeader2->m_uiWidth;
	m_uiHeight = pHeader2->m_uiHeight;
	m_usColourPlaneCount = pHeader2->m_usPlaneCount;
	m_usBPP = pHeader2->m_usBPP;

	// clean up
	delete pHeader1;
	delete pHeader2;
}

void			CBMPFormat::unserializeVersion4(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	uint32 uiByteCount;

	// read headers 1 & 2
	CBMPFormat_Header1 *pHeader1 = pDataReader->readStruct<CBMPFormat_Header1>();
	CBMPFormat_Header2_Version4 *pHeader2 = pDataReader->readStruct<CBMPFormat_Header2_Version4>();

	// palette
	m_bHasPalette = pHeader2->m_usBPP < 16;
	if (m_bHasPalette)
	{
		uiByteCount = 4 * pHeader2->m_uiColoursUsed;
		m_strPaletteData = pDataReader->readString(uiByteCount);
	}

	// raster data
	uiByteCount = pHeader2->m_uiWidth * pHeader2->m_uiHeight * ((float32)pHeader2->m_usBPP / 8.0f);
	m_strRasterData = pDataReader->readString(uiByteCount);

	// copy from raw structs to wrapper structs
	m_uiWidth = pHeader2->m_uiWidth;
	m_uiHeight = pHeader2->m_uiHeight;
	m_usColourPlaneCount = pHeader2->m_usPlaneCount;
	m_usBPP = pHeader2->m_usBPP;

	// clean up
	delete pHeader1;
	delete pHeader2;
}

void			CBMPFormat::serializeVersion1(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
}

void			CBMPFormat::serializeVersion2(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 16 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_strRasterData.length();

		pDataWriter->writeString("BM");
		pDataWriter->writeUint32(uiFileSize); // file size
		pDataWriter->writeUint16(0); // reserved 1
		pDataWriter->writeUint16(0); // reserved 2
		pDataWriter->writeUint32(54); // bitmap offset
	}

	pDataWriter->writeUint32(12);
	pDataWriter->writeUint32(m_uiWidth);
	pDataWriter->writeUint32(m_uiHeight);
	pDataWriter->writeUint16(1); // plane count
	pDataWriter->writeUint16(m_usBPP);

	if (m_bHasPalette)
	{
		pDataWriter->writeString(m_strPaletteData);
	}

	pDataWriter->writeString(m_strRasterData);
}

void			CBMPFormat::serializeVersion3(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 40 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_strRasterData.length();

		pDataWriter->writeString("BM");
		pDataWriter->writeUint32(uiFileSize); // file size
		pDataWriter->writeUint16(0); // reserved 1
		pDataWriter->writeUint16(0); // reserved 2
		pDataWriter->writeUint32(54); // bitmap offset
	}

	pDataWriter->writeUint32(40);
	pDataWriter->writeUint32(m_uiWidth);
	pDataWriter->writeUint32(m_uiHeight);
	pDataWriter->writeUint16(1); // plane count
	pDataWriter->writeUint16(m_usBPP);
	pDataWriter->writeUint32(0); // uiCompressionMethods
	pDataWriter->writeUint32(m_uiWidth * m_uiHeight * ((float32)m_usBPP / 8.0f)); // uiBitmapSize
	pDataWriter->writeUint32(0); // uiHorizontalResolution
	pDataWriter->writeUint32(0); // uiVerticalResolution
	pDataWriter->writeUint32(0); // uiColoursUsed
	pDataWriter->writeUint32(0); // uiColoursImportant

	if (m_bHasPalette)
	{
		pDataWriter->writeString(m_strPaletteData);
	}

	pDataWriter->writeString(m_strRasterData);
}

void			CBMPFormat::serializeVersion4(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 108 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_strRasterData.length();

		pDataWriter->writeString("BM", 2);
		pDataWriter->writeUint32(uiFileSize); // file size
		pDataWriter->writeUint16(0); // reserved 1
		pDataWriter->writeUint16(0); // reserved 2
		pDataWriter->writeUint32(54); // bitmap offset
	}

	pDataWriter->writeUint32(108);
	pDataWriter->writeUint32(m_uiWidth);
	pDataWriter->writeUint32(m_uiHeight);
	pDataWriter->writeUint16(1); // plane count
	pDataWriter->writeUint16(m_usBPP);
	pDataWriter->writeUint32(0); // uiCompressionMethods
	pDataWriter->writeUint32(m_uiWidth * m_uiHeight * ((float32)m_usBPP / 8.0f)); // uiBitmapSize
	pDataWriter->writeUint32(0); // uiHorizontalResolution
	pDataWriter->writeUint32(0); // uiVerticalResolution
	pDataWriter->writeUint32(0); // uiColoursUsed
	pDataWriter->writeUint32(0); // uiColoursImportant

	if (m_bHasPalette)
	{
		pDataWriter->writeString(m_strPaletteData);
	}

	pDataWriter->writeString(m_strRasterData);
}
