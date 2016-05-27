#include "CCompressionUtility.h"
#include "zlib.h"
#include "lz4hc.h"
#include "lz4.h"
#include "lzo/lzo1x.h"

using namespace std;

string							CCompressionUtility::compressZLib(string& strData, uint32 uiCompressionLevel)
{
	uint32 uiUncompressedSize = strData.length();
	uint32 uiMaxCompressedSize = compressBound(uiUncompressedSize);

	const char *pInStr = strData.c_str();
	char *pOutStr = new char[uiMaxCompressedSize + 1];

	z_stream stream;
	stream.zalloc = NULL;
	stream.zfree = NULL;
	stream.opaque = NULL;

	stream.avail_in = uiUncompressedSize;
	stream.next_in = (unsigned __int8*)pInStr;
	stream.avail_out = uiMaxCompressedSize;
	stream.next_out = (unsigned __int8*)pOutStr;

	deflateInit(&stream, uiCompressionLevel);
	deflate(&stream, Z_FINISH);
	uint32 uiCompressedSize = stream.total_out;
	deflateEnd(&stream);

	string strOutData(pOutStr, uiCompressedSize);
	delete[] pOutStr;
	return strOutData;
}
string								CCompressionUtility::decompressZLib(string& strData, uint32 uiUncompressedDataLength)
{
	uint32 uiCompressedSize = strData.length();

	const char *pInStr = strData.c_str();
	char *pOutStr = new char[uiUncompressedDataLength];

	uncompress((unsigned __int8*)pOutStr, &uiUncompressedDataLength, (unsigned __int8*)pInStr, uiCompressedSize);

	string strOutData(pOutStr, uiUncompressedDataLength);
	delete[] pOutStr;
	return strOutData;
}

static bool bLZOInit = false;
string								CCompressionUtility::compressLZO1X999(string& strData)
{
	if (!bLZOInit)
	{
		bLZOInit = true;
		if (lzo_init() != LZO_E_OK)
		{
			return "";
		}
	}

	uint32 uiUncompressedSize = strData.length();
	uint32 uiCompressedSize = uiUncompressedSize + uiUncompressedSize / 16 + 64 + 3;

	lzo_uint iInBufferSize = uiUncompressedSize;
	lzo_uint iOutBufferSize = uiCompressedSize;
	lzo_bytep pInBuffer = new lzo_byte[uiUncompressedSize];
	lzo_bytep pOutBuffer = new lzo_byte[uiCompressedSize];
	lzo_uint uiOutBufferLength = uiCompressedSize;
	lzo_bytep pWorkingMemory = new lzo_byte[LZO1X_999_MEM_COMPRESS];

	memcpy(pInBuffer, strData.c_str(), uiUncompressedSize);

	lzo1x_999_compress(pInBuffer, iInBufferSize, pOutBuffer, &uiOutBufferLength, pWorkingMemory);

	string strOutData = "";
	strOutData.append((char*)pOutBuffer, uiOutBufferLength);

	delete[] pInBuffer;
	delete[] pOutBuffer;
	delete[] pWorkingMemory;

	return strOutData;
}
string								CCompressionUtility::decompressLZO1X(string& strData, uint32 uiUncompressedDataLength)
{
	//uint32 uiBlockSize = 2048; // guessed - todo - remove?

	uint32 uiCompressedSize = strData.length();
	uint32 uiUncompressedSize = uiUncompressedDataLength;
	//uint32 uiUncompressedSize = uiCompressedSize + uiCompressedSize / 16 + 64 + 3; - todo - remove?

	lzo_uint iInBufferSize = uiCompressedSize;
	lzo_uint iOutBufferSize = uiUncompressedSize;
	lzo_bytep pInBuffer = new lzo_byte[uiCompressedSize];
	lzo_bytep pOutBuffer = new lzo_byte[uiUncompressedSize];
	lzo_uint *pOutBufferLength = &iOutBufferSize;

	memcpy(pInBuffer, strData.c_str(), uiCompressedSize);

	int iResult = lzo1x_decompress_safe(pInBuffer, iInBufferSize, pOutBuffer, pOutBufferLength, 0);
	if (iResult != LZO_E_OK)
	{
		delete[] pInBuffer;
		delete[] pOutBuffer;
		return "";
	}

	string strOutData = "";
	strOutData.append((char*)pOutBuffer, uiUncompressedSize);
	
	delete[] pInBuffer;
	delete[] pOutBuffer;

	return strOutData;

	/*
	todo - remove?
	
	uint32 uiBlockCount = (uint32) ceil(((float32)iInBufferSize) / ((float32)uiBlockSize));
	string strEntryData = CFileParser::getInstance()->readString(iInBufferSize);

	for (uint32 i = 0; i < uiBlockCount; i++)
	{
	memcpy(pInBuffer, strEntryData.substr(i * uiBlockSize, uiBlockSize).c_str(), uiBlockSize);

	int iResult = lzo1x_decompress_safe(pInBuffer, iInBufferSize, pOutBuffer, pOutBufferLength);
	if (iResult == LZO_E_OK || new_len != out_len)
	{
	MessageBoxW(NULL, L"success!", L"success!", MB_OK);
	}
	//CDebugger::log("iResult: " + CStringUtility::toString(iResult));

	pIMGEntry->setRWVersionByVersionCC(0x00000302);
	}

	//CDebugger::log("FILE DATA LEN: " + CStringUtility::toString(strEntryData.size()));
	*/
}

static int LZ4IO_LZ4_compress(const char* src, char* dst, int srcSize, int dstSize, int cLevel)
{
	(void)cLevel;
	return LZ4_compress_fast(src, dst, srcSize, dstSize, 1);
}

string								CCompressionUtility::compressLZ4(string& strData, uint32 uiCompressionLevel)
{
	uint32 uiUncompressedSize = strData.length();
	uint32 uiCompressedSize;
	uint32 uiWorstCompressedSize = LZ4_compressBound(uiUncompressedSize);

	const char *pInStr = strData.c_str();
	char *pOutStr = new char[uiWorstCompressedSize + 1];

	if (uiCompressionLevel < 3)
	{
		uiCompressedSize = LZ4IO_LZ4_compress(pInStr, pOutStr, strData.length(), uiWorstCompressedSize, uiCompressionLevel);
	}
	else
	{
		uiCompressedSize = LZ4_compress_HC(pInStr, pOutStr, strData.length(), uiWorstCompressedSize, uiCompressionLevel);
	}

	string strOutData(pOutStr, uiCompressedSize);
	delete[] pOutStr;
	return strOutData;
}
string								CCompressionUtility::decompressLZ4(string& strData, uint32 uiUncompressedDataLength)
{
	uint32 uiCompressedSize = strData.length();

	const char *pInStr = strData.c_str();
	char *pOutStr = new char[uiUncompressedDataLength];

	LZ4_decompress_safe(pInStr, pOutStr, uiCompressedSize, uiUncompressedDataLength);

	string strOutData(pOutStr, uiUncompressedDataLength);
	delete[] pOutStr;
	return strOutData;
}