#include "CWTDFormat.h"
#include "File/CFileUtility.h"
#include "Data Stream/CDataWriter.h"
#include "CWTDManager.h"
#include "String/CStringUtility.h"
#include "Texture/CIntermediateTextureFormat.h"
#include "Texture/CIntermediateTexture.h"
#include "Texture/Data/CIntermediateTextureMipmap.h"
#include "Compression/CCompressionUtility.h"
#include "Image/CImageManager.h"
#include "Image/CImageUtility.h"
#include "Data Stream/CDataReader.h"
#include "Memory/CMemoryUtility.h"
#include "Namespace.h"
#include "Raw/WTD/CWTDFormat_Header1.h"
#include "Raw/WTD/CWTDFormat_Header2.h"
#include "Raw/WTD/CWTDEntry.h"
#include "Engine/RAGE/CRageManager.h"

using namespace std;

void					CWTDFormat::unload(void)
{
	for (auto pWTDEntry : getEntries())
	{
		for (auto pWTDMipmap : pWTDEntry->getEntries())
		{
			delete pWTDMipmap;
		}
		//pWTDEntry->removeAllEntries();
		delete pWTDEntry;
	}
	getEntries().clear();
}

void					CWTDFormat::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// header 1 and decompress whole file except first 12 bytes
	uint32 uiSystemSegmentSize, uiGPUSegmentSize;
	string strData = decompressWTDFormatData(uiSystemSegmentSize, uiGPUSegmentSize);
	pDataReader->setStreamType(DATA_STREAM_MEMORY); // todo - make this call CDataReader::close() or something so that is closes the file handle
	pDataReader->setData(strData);
	pDataReader->setSeek(0);

	// header 2
	RG::CWTDFormat_Header2 *pHeader2 = pDataReader->readStruct<RG::CWTDFormat_Header2>();
	//header2.m_uiHashTableOffset = CStringUtility::swapEndian(header2.m_uiHashTableOffset); // todo - is this code needed?

	// texture hashes
	pDataReader->setSeek(pHeader2->m_uiHashTableOffset);
	vector<uint32> vecTextureHashes = pDataReader->readUint32ArrayAsStdVector(pHeader2->m_usTextureCount);

	// info offsets
	pDataReader->setSeek(pHeader2->m_uiTextureListOffset);
	vector<uint32> vecInfoOffsets = pDataReader->readUint32ArrayAsStdVector(pHeader2->m_usTextureCount);
	for (uint32& uiInt : vecInfoOffsets)
	{
		uiInt = convertULongToOffset(uiInt);
	}

	// prepare stored container
	m_vecEntries.clear();
	m_vecEntries.resize(pHeader2->m_usTextureCount);

	// RG structs
	RG::CWTDEntry *pWTDEntries_RG = new RG::CWTDEntry[pHeader2->m_usTextureCount];
	pDataReader->readCString((uint8*)pWTDEntries_RG, pHeader2->m_usTextureCount * sizeof(RG::CWTDEntry));

	// header for entries
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		RG::CWTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pDataReader->setSeek(vecInfoOffsets[i]);
		CMemoryUtility::copyAndDeleteUint8Array(pWTDEntry_RG, pDataReader->readCString(sizeof(RG::CWTDEntry)), sizeof(RG::CWTDEntry));
	}

	// convert ints to offsets
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		RG::CWTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pWTDEntry_RG->m_uiBlockMapOffset2 = convertULongToOffset(pWTDEntry_RG->m_uiBlockMapOffset2);
		pWTDEntry_RG->m_uiNameOffset = convertULongToOffset(pWTDEntry_RG->m_uiNameOffset);

		//D3DFORMAT eD3DFormat = getD3DFormatFromFourCC(CFileParser::getInstance()->readString(4));
		//eRasterDataFormat eRasterDataFormatValue = CImageUtility::getRasterDataFormatFromD3DFormat(eD3DFormat);

		pWTDEntry_RG->m_uiPrevTextureInfoOffset = convertULongToOffset(pWTDEntry_RG->m_uiPrevTextureInfoOffset);
		pWTDEntry_RG->m_uiNextTextureInfoOffset = convertULongToOffset(pWTDEntry_RG->m_uiNextTextureInfoOffset);

		pWTDEntry_RG->m_uiRawDataOffset = convertULongToDataOffset(pWTDEntry_RG->m_uiRawDataOffset);
	}

	// entry names
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		CWTDEntry *pWTDEntry = m_vecEntries[i]; 
		RG::CWTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pDataReader->setSeek(pWTDEntry_RG->m_uiNameOffset);
		pWTDEntry->setEntryName(pDataReader->readStringUntilZero());
	}

	// copy RG structs to wrapper structs
	for (uint32 i = 0, j = pHeader2->m_usTextureCount; i < j; i++)
	{
		CWTDEntry *pWTDEntry = m_vecEntries[i];
		RG::CWTDEntry *pWTDEntry_RG = &(pWTDEntries_RG[i]);

		pWTDEntry->stripNameHeaderAndFooter();
		pWTDEntry->setImageSize(true, pWTDEntry_RG->m_usWidth);
		pWTDEntry->setImageSize(false, pWTDEntry_RG->m_usHeight);
		pWTDEntry->setD3DFormat(getD3DFormatFromFourCC((char*)&pWTDEntry_RG->m_ucD3DFormat));
		pWTDEntry->setRasterDataFormat(CImageUtility::getRasterDataFormatFromD3DFormat(pWTDEntry->getD3DFormat()), false);
		pWTDEntry->setLevels(pWTDEntry_RG->m_ucLevels);
		pWTDEntry->setRawDataOffset(pWTDEntry_RG->m_uiRawDataOffset);
		pWTDEntry->setTextureHash(vecTextureHashes[i]);
	}

	// clean up
	delete[] pWTDEntries_RG;

	// raster data
	uint32 uiGraphicsStreamStartPosition = pDataReader->getSeek();
	for (uint32 i = 0; i < pHeader2->m_usTextureCount; i++)
	{
		CWTDEntry *pWTDEntry = m_vecEntries[i];

		uint32 uiDataSize = CWTDManager::getImageDataSize(pWTDEntry, false);

		uint32 uiSeek = uiSystemSegmentSize + pWTDEntry->getRawDataOffset();
		pDataReader->setSeek(uiSeek);

		uint32
			uiMipmapWidth = pWTDEntry->getImageSize(true),
			uiMipmapHeight = pWTDEntry->getImageSize(false);
		for (uint32 i = 0, j = pWTDEntry->getLevels(); i < j; i++)
		{
			// clamp to 16 bytes
			if (uiDataSize < 16)
			{
				if (pWTDEntry->getD3DFormat() == D3DFMT_DXT1 && uiDataSize < 8)
				{
					uiDataSize = 8;
				}
				else
				{
					uiDataSize = 16;
				}
			}

			// create mipmap
			CWTDMipmap *pMipmap = new CWTDMipmap(pWTDEntry);

			pMipmap->setRasterData(pDataReader->readString(uiDataSize));
			pMipmap->setImageSize(true, uiMipmapWidth);
			pMipmap->setImageSize(false, uiMipmapHeight);
			pWTDEntry->addEntry(pMipmap);

			// calculate data size and image size for next mipmap
			uiDataSize /= 4;
			uiMipmapWidth /= 2;
			uiMipmapHeight /= 2;
		}
	}
}

void					CWTDFormat::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	eDataStreamType ePreviousDataStreamType = pDataWriter->getStreamType();
	pDataWriter->setStreamType(DATA_STREAM_MEMORY);

	// store system stream
	uint32
		uiVTable = 0,
		uiBlockMapOffset = 0,
		uiParentDictionary = 0,
		uiUsageCount = 0,
		uiHashTableOffset = 0,
		uiTextureListOffset = 0
	;
	uint16
		usTextureCount = 0,
		usUnknown1 = 0,
		usUnknown2 = 0,
		usUnknown3 = 0
	;

	pDataWriter->writeUint32(uiVTable);

	pDataWriter->writeUint32(CRageManager::getPackedOffset(uiBlockMapOffset));
	pDataWriter->writeUint32(uiParentDictionary);
	pDataWriter->writeUint32(uiUsageCount);

	pDataWriter->writeUint32(CRageManager::getPackedOffset(uiHashTableOffset));
	pDataWriter->writeUint16(usTextureCount);
	pDataWriter->writeUint16(usUnknown1);

	pDataWriter->writeUint32(CRageManager::getPackedOffset(uiTextureListOffset));
	pDataWriter->writeUint16(usUnknown2);
	pDataWriter->writeUint16(usUnknown3);

	// store texture hashes
	for (auto pWTDEntry : getEntries())
	{
		pDataWriter->writeUint32(pWTDEntry->getTextureHash());
	}

	// store texture info
	for (uint32 i = 0; i < usTextureCount; i++)
	{
		uint32
			uiVTable2 = 0,
			uiBlockMapOffset2 = 0,
			uiUnknown4 = 0,
			uiUnknown5 = 0,
			uiUnknown6 = 0,
			uiNameOffset = 0,
			uiUnknown7 = 0,
			uiPrevTextureInfoOffset = 0,
			uiNextTextureInfoOffset = 0,
			uiRawDataOffset = 0,
			uiUnknown14 = 0
		;
		uint16
			usWidth = 0,
			usHeight = 0,
			usStrideSize = 0
		;
		uint8
			ucType = 0,
			ucLevels = 0
		;
		float
			fUnknown8 = 0.0f,
			fUnknown9 = 0.0f,
			fUnknown10 = 0.0f,
			fUnknown11 = 0.0f,
			fUnknown12 = 0.0f,
			fUnknown13 = 0.0f
		;
		D3DFORMAT eD3DFormat = D3DFMT_DXT1;

		pDataWriter->writeUint32(uiVTable2);

		pDataWriter->writeUint32(CRageManager::getPackedOffset(uiBlockMapOffset2));

		pDataWriter->writeUint32(uiUnknown4);
		pDataWriter->writeUint32(uiUnknown5);
		pDataWriter->writeUint32(uiUnknown6);

		pDataWriter->writeUint32(CRageManager::getPackedOffset(uiNameOffset));

		pDataWriter->writeUint32(uiUnknown7);

		pDataWriter->writeUint16(usWidth);
		pDataWriter->writeUint16(usHeight);
		pDataWriter->writeString(getFourCCFromD3DFormat(eD3DFormat));

		pDataWriter->writeUint16(usStrideSize);
		pDataWriter->writeUint8(ucType);
		pDataWriter->writeUint8(ucLevels);

		pDataWriter->writeFloat32(fUnknown8);
		pDataWriter->writeFloat32(fUnknown9);
		pDataWriter->writeFloat32(fUnknown10);
		pDataWriter->writeFloat32(fUnknown11);
		pDataWriter->writeFloat32(fUnknown12);
		pDataWriter->writeFloat32(fUnknown13);

		pDataWriter->writeUint32(CRageManager::getPackedOffset(uiPrevTextureInfoOffset));
		pDataWriter->writeUint32(CRageManager::getPackedOffset(uiNextTextureInfoOffset));

		pDataWriter->writeUint32(CRageManager::getPackedOffset(uiRawDataOffset));

		pDataWriter->writeUint32(uiUnknown14);
	}

	// store texture names
	for (auto pWTDEntry : getEntries())
	{
		string strEntryName = pWTDEntry->getEntryName();
		strEntryName.append("\0", 1);
		pDataWriter->writeStringRef(strEntryName);
	}

	// store graphics stream
	uint32 uiSystemStreamSize = pDataWriter->getData().length();
	for (auto pWTDEntry : getEntries())
	{
		for (auto pMipmap : pWTDEntry->getEntries())
		{
			pDataWriter->writeStringRef(pMipmap->getRasterData());
		}
	}
	uint32 uiGraphicsStreamSize = pDataWriter->getData().length() - uiSystemStreamSize;

	string strWTDFileData = pDataWriter->getData();
	pDataWriter->resetData();
	pDataWriter->setStreamType(ePreviousDataStreamType);
	pDataWriter->setSeek(0);

	uint32
		uiMagicNumber = 0x52534305, // R S C 0x05
		uiType = 0x08,
		uiFlags = getFileHeaderFlagsFromSystemAndGraphicsStreamSizes(uiSystemStreamSize, uiGraphicsStreamSize);
	;
	bool
		bBigEndian = false
	;
	string strHeader12B = 
		CStringUtility::packUint32(uiMagicNumber, bBigEndian) +
		CStringUtility::packUint32(uiType, bBigEndian) +
		CStringUtility::packUint32(uiFlags, bBigEndian);
	pDataWriter->writeString(strHeader12B + CCompressionUtility::compressZLib(strWTDFileData));
}

CIntermediateTextureFormat*		CWTDFormat::convertToIntermediateFormat(void)
{
	CIntermediateTextureFormat *pGeneralTextureFile = new CIntermediateTextureFormat;

	for (CWTDEntry *pWTDEntry : getEntries())
	{
		CIntermediateTexture *pGeneralTexture = new CIntermediateTexture;

		CVector2ui16 vecImageSize;
		vecImageSize.m_x = pWTDEntry->getImageSize(true);
		vecImageSize.m_y = pWTDEntry->getImageSize(false);

		pGeneralTexture->setName(pWTDEntry->getEntryName());
		pGeneralTexture->setRasterDataFormat(CImageUtility::getRasterDataFormatFromD3DFormat(pWTDEntry->getD3DFormat()));
		pGeneralTexture->setSize(vecImageSize);

		for (CWTDMipmap *pWTDMipmap : pWTDEntry->getEntries())
		{
			CIntermediateTextureMipmap *pGeneralMipmap = new CIntermediateTextureMipmap;

			CVector2ui16 vecMipmapSize;
			vecMipmapSize.m_x = pWTDMipmap->getImageSize(true);
			vecMipmapSize.m_y = pWTDMipmap->getImageSize(false);

			pGeneralMipmap->setRasterData(pWTDMipmap->getRasterData());
			pGeneralMipmap->setSize(vecMipmapSize);

			pGeneralTexture->addEntry(pGeneralMipmap);
		}

		pGeneralTextureFile->addEntry(pGeneralTexture);
	}

	return pGeneralTextureFile;
}

uint32				CWTDFormat::getFileHeaderFlagsFromSystemAndGraphicsStreamSizes(uint32 uiSystemStreamSize, uint32 uiGraphicsStreamSize)
{
	return (getCompactSize(uiSystemStreamSize) & 0x7FFF) | (getCompactSize(uiGraphicsStreamSize) & 0x7FFF) << 15 | 3 << 30;
}

uint32				CWTDFormat::getCompactSize(uint32 uiSize)
{
	// uiSize must be a multiple of 256
	uiSize >>= 8;
	uint32 i = 0;
	while ((uiSize % 2) == 0 && uiSize >= 32 && i < 15)
	{
		i++;
		uiSize >>= 1;
	}
	return ((i & 0x0F) << 11) | (uiSize & 0x7FF);
}

string						CWTDFormat::decompressWTDFormatData(uint32& uiSystemSegmentSize, uint32& uiGPUSegmentSize)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CWTDFormat_Header1 *pHeader1 = pDataReader->readStruct<CWTDFormat_Header1>();

	/*
	todo - is this code needed?
	header1.m_uiFlags = CStringUtility::swapEndian(header1.m_uiFlags);
	header1.m_uiMagicNumber = CStringUtility::swapEndian(header1.m_uiMagicNumber);
	header1.m_uiType = CStringUtility::swapEndian(header1.m_uiType);
	*/

	uiSystemSegmentSize = (pHeader1->m_uiFlags & 0x7FF) << (((pHeader1->m_uiFlags >> 11) & 0xF) + 8);
	uiGPUSegmentSize = ((pHeader1->m_uiFlags >> 15) & 0x7FF) << (((pHeader1->m_uiFlags >> 26) & 0xF) + 8);

	delete pHeader1;

	return CCompressionUtility::decompressZLib(pDataReader->readRemaining(), uiSystemSegmentSize + uiGPUSegmentSize);
}

uint32			CWTDFormat::convertULongToOffset(uint32 uiValue)
{
	if (uiValue == 0)
	{
		return 0;
	}

	return uiValue & 0x0FFFFFFF;
}

uint32			CWTDFormat::convertULongToDataOffset(uint32 uiValue)
{
	if (uiValue == 0)
	{
		return 0;
	}

	return uiValue & 0x0FFFFFFF;
}

D3DFORMAT				CWTDFormat::getD3DFormatFromFourCC(string strFourCC)
{
	if (strFourCC == "DXT1")
	{
		return D3DFMT_DXT1;
	}
	else if (strFourCC == "DXT3")
	{
		return D3DFMT_DXT3;
	}
	else if (strFourCC == "DXT5")
	{
		return D3DFMT_DXT5;
	}
	return D3DFMT_UNKNOWN;
}

string					CWTDFormat::getFourCCFromD3DFormat(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return "DXT1";
	case D3DFMT_DXT3:	return "DXT3";
	case D3DFMT_DXT5:	return "DXT5";
	}
	return "Unkn";
}