#include "CDataReader.h"
#include "eExceptionCode.h"
#include "String/CStringUtility.h"
#include "Math/CMathUtility.h"

using namespace std;

CDataReader::CDataReader(void) :
	CDataStream(),
	m_uiUserData(0),
	m_uiCachedDataLength(0),
	m_pstrActiveLine(nullptr),
	m_uiActiveLineIndex(0),
	m_uiMaxLineIndex(0),
	m_bTokenModeEnabledState(false),
	m_uiActiveLineTokenIndex(0),
	m_bPeek(false)
{
}

// reset
void		CDataReader::reset(void)
{
	CDataStream::reset();
	resetLines();
	m_uiUserData = 0;
	m_uiCachedDataLength = 0;
}

// file processing
void		CDataReader::openFile(string& strFilePath, uint32 uiFlags)
{
	// always open with binary mode, even for text files, so that seek() and tellg() work.
	//m_file.open(strFilePath.c_str(), (ios_base::open_mode) uiFlags);
	m_file.open(strFilePath.c_str(), ios::binary);
}
bool		CDataReader::isFileOpen(void)
{
	return m_file.is_open();
}
void		CDataReader::resetFile(void)
{
	if (isFileOpen())
	{
		m_file.close();
	}
	m_file.clear();
}

// read string
void		CDataReader::readCString(uint8* pDestination, uint32 uiByteCount)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		if ((m_strData_Memory.length() - m_uiSeek_Memory) < uiByteCount)
		{
			throw EXCEPTION_FILE_READ_INSUFFICIENT_BYTES;
		}
		memcpy((char*)pDestination, m_strData_Memory.c_str() + m_uiSeek_Memory, uiByteCount);
		break;
	case DATA_STREAM_FILE:
		m_file.read((char*)pDestination, uiByteCount);
		if (m_file.fail()) // http://www.cplusplus.com/reference/ios/ios/good/
		{
			delete[] pDestination;
			throw EXCEPTION_FILE_READ_FAIL;
		}

		if (uiByteCount != m_file.gcount())
		{
			delete[] pDestination;
			throw EXCEPTION_FILE_READ_INSUFFICIENT_BYTES;
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
	restoreSeekForPeek(uiByteCount);
}

uint8*		CDataReader::readCString(uint32 uiByteCount)
{
	uint8 *pData = new uint8[uiByteCount];
	readCString(pData, uiByteCount);
	return pData;
}

string		CDataReader::readString(uint32 uiByteCount)
{
	string strOutputData = "";
	uint8 *pInputData = readCString(uiByteCount);
	strOutputData.append((char*)pInputData, uiByteCount);
	delete[] pInputData;
	return strOutputData;
}

string		CDataReader::readStringUntilZero(void)
{
	string strOutputData = "";
	for (;;)
	{
		string strChar = readString(1);
		if (strChar.c_str()[0] == 0)
		{
			break;
		}
		strOutputData += strChar;
	}
	return strOutputData;
}

string		CDataReader::readAll(void)
{
	uint64 uiDataLength = getProcessedDataLength();
	string strFileData = readString(uiDataLength);
	if (uiDataLength != strFileData.length())
	{
		throw EXCEPTION_FILE_READ_INSUFFICIENT_BYTES;
	}
	return strFileData;
}

string		CDataReader::readRemaining(void)
{
	return readString(getRemainingDataLength());
}

// read int
uint32		CDataReader::readUint32(void)
{
	return CStringUtility::unpackUint32(readString(4), m_eEndian == BIG_ENDIAN);
}

uint16		CDataReader::readUint16(void)
{
	return CStringUtility::unpackUint16(readString(2), m_eEndian == BIG_ENDIAN);
}

uint8		CDataReader::readUint8(void)
{
	return CStringUtility::unpackUint8(readString(1));
}

int32		CDataReader::readInt32(void)
{
	return CStringUtility::unpackInt32(readString(4), m_eEndian == BIG_ENDIAN);
}

int16		CDataReader::readInt16(void)
{
	return CStringUtility::unpackInt16(readString(2), m_eEndian == BIG_ENDIAN);
}

int8		CDataReader::readInt8(void)
{
	return CStringUtility::unpackInt8(readString(1));
}

CVector4ui8	CDataReader::readVector4ui8(void)
{
	CVector4ui8 vecVector;
	vecVector.m_x = readUint8();
	vecVector.m_y = readUint8();
	vecVector.m_z = readUint8();
	vecVector.m_w = readUint8();
	return vecVector;
}

vector<uint32>	CDataReader::readUint32ArrayAsStdVector(uint32 uiValueCount)
{
	uint32 uiByteCount = uiValueCount * sizeof(uint32);
	uint32 *pData = (uint32*) readCString(uiByteCount);
	vector<uint32> vecInts;
	vecInts.resize(uiValueCount);
	for (uint32 i = 0; i < uiValueCount; i++)
	{
		vecInts[i] = *(pData + i);
	}
	delete[] (uint8*) pData;
	if (m_eEndian == BIG_ENDIAN)
	{
		for (uint32& uiInt : vecInts)
		{
			uiInt = CStringUtility::swapEndian(uiInt);
		}
	}
	restoreSeekForPeek(uiByteCount);
	return vecInts;
}

vector<CVector2D>		CDataReader::readVector2DArrayAsStdVector(uint32 uiValueCount)
{
	vector<CVector2D> vecValues;
	vecValues.resize(uiValueCount);
	for (uint32 i = 0; i < uiValueCount; i++)
	{
		vecValues[i] = readVector2D();
	}
	return vecValues;
}

vector<CVector3D>		CDataReader::readVector3DArrayAsStdVector(uint32 uiValueCount)
{
	vector<CVector3D> vecValues;
	vecValues.resize(uiValueCount);
	for (uint32 i = 0; i < uiValueCount; i++)
	{
		vecValues[i] = readVector3D();
	}
	return vecValues;
}

vector<CVector4ui8>		CDataReader::readVector4ui8ArrayAsStdVector(uint32 uiValueCount)
{
	vector<CVector4ui8> vecValues;
	vecValues.resize(uiValueCount);
	for (uint32 i = 0; i < uiValueCount; i++)
	{
		vecValues[i] = CMathUtility::createVector4ui8(readUint8(), readUint8(), readUint8(), readUint8());
	}
	return vecValues;
}

vector<CVector4ui16>	CDataReader::readVector4ui16ArrayAsStdVector(uint32 uiValueCount)
{
	vector<CVector4ui16> vecValues;
	vecValues.resize(uiValueCount);
	for (uint32 i = 0; i < uiValueCount; i++)
	{
		vecValues[i] = CMathUtility::createVector4ui16(readUint16(), readUint16(), readUint16(), readUint16());
	}
	return vecValues;
}

// read float
float32		CDataReader::readFloat32(void)
{
	return CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN);
}

CVector2D	CDataReader::readVector2D(void)
{
	return{
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN),
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN)
	};
}

CVector3D	CDataReader::readVector3D(void)
{
	return{
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN),
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN),
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN)
	};
}

CVector4D	CDataReader::readVector4D(void)
{
	return{
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN),
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN),
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN),
		CStringUtility::unpackFloat32(readString(4), m_eEndian == BIG_ENDIAN)
	};
}

// read token
string					CDataReader::readTokenString(void)
{
	return readLineToken();
}
int32					CDataReader::readTokenInt32(void)
{
	return CStringUtility::toInt32(readLineToken());
}
uint32					CDataReader::readTokenUint32(void)
{
	return CStringUtility::toUint32(readLineToken());
}
float32					CDataReader::readTokenFloat32(void)
{
	return CStringUtility::toFloat32(readLineToken());
}
CVector2D				CDataReader::readTokenVector2D(void)
{
	return{
		CStringUtility::toFloat32(readLineToken()),
		CStringUtility::toFloat32(readLineToken())
	};
}
CVector3D				CDataReader::readTokenVector3D(void)
{
	return{
		CStringUtility::toFloat32(readLineToken()),
		CStringUtility::toFloat32(readLineToken()),
		CStringUtility::toFloat32(readLineToken())
	};
}
CVector4D				CDataReader::readTokenVector4D(void)
{
	return {
		CStringUtility::toFloat32(readLineToken()),
		CStringUtility::toFloat32(readLineToken()),
		CStringUtility::toFloat32(readLineToken()),
		CStringUtility::toFloat32(readLineToken())
	};
}

// seek
void		CDataReader::restoreSeekForPeek(uint32 uiByteCount)
{
	if (!getPeek())
	{
		return;
	}

	uint32 uiSeek = getSeek();
	if (uiSeek < uiByteCount)
	{
		throw EXCEPTION_CANT_RESTORE_PEEK;
	}
	setSeek(uiSeek - uiByteCount);
}

void		CDataReader::setSeek(uint64 uiByteIndex)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		if (uiByteIndex >= getProcessedDataLength())
		{
			throw EXCEPTION_CANT_SEEK_TO;
		}
		setSeek_Memory(uiByteIndex);
		break;
	case DATA_STREAM_FILE:
		m_file.seekg(uiByteIndex);
		if (m_file.eof() || m_file.fail())
		{
			throw EXCEPTION_CANT_SEEK_TO;
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
}

uint64		CDataReader::getSeek(void)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		return getSeek_Memory();
	case DATA_STREAM_FILE:
		return m_file.tellg();
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
	return 0;
}

// seek
bool		CDataReader::canSeekTo(uint64 uiByteIndex)
{
	return uiByteIndex < (getDataLength() - 1);
}

bool		CDataReader::isEOF(void)
{
	return getSeek() >= (getDataLength() - 1);
}

// data length
uint64		CDataReader::getDataLength(void)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		return m_strData_Memory.length();
	case DATA_STREAM_FILE:
	{
		uint32 uiSeekBefore = m_file.tellg();
		if (uiSeekBefore == -1)
		{
			return 0;
		}
		m_file.seekg(0, ios::end);
		uint32 uiDataLength = m_file.tellg();
		m_file.seekg(uiSeekBefore, ios::beg);
		return uiDataLength;
	}
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
	return 0;
}

uint64		CDataReader::getProcessedDataLength(void)
{
	if (getCachedDataLength() == 0)
	{
		return getDataLength();
	}
	else
	{
		return getCachedDataLength();
	}
}

uint64		CDataReader::getRemainingDataLength(void)
{
	uint64 uiDataLength = getDataLength();
	if (uiDataLength == 0)
	{
		return 0;
	}
	return (getDataLength() - 1) - getSeek();
}

bool		CDataReader::canRead(uint32 uiByteCount)
{
	return uiByteCount <= getRemainingDataLength();
}

// lines
void		CDataReader::readAndStoreLines(void)
{
	m_vecLines = CStringUtility::split(CStringUtility::fixEOLs(readAll(), "\n"), "\n");
	m_uiMaxLineIndex = m_vecLines.size() == 0 ? 0 : (m_vecLines.size() - 1);
}

uint32		CDataReader::getLineCount(void)
{
	return m_vecLines.size();
}

bool		CDataReader::iterateLines(void)
{
	if (m_uiActiveLineIndex < m_uiMaxLineIndex)
	{
		m_pstrActiveLine = &m_vecLines[m_uiActiveLineIndex];
		m_uiActiveLineIndex++;
		return true;
	}
	else
	{
		m_pstrActiveLine = nullptr;
		m_uiActiveLineIndex = 0;
		return false;
	}
}

void		CDataReader::resetLines(void)
{
	m_pstrActiveLine = nullptr;
	m_uiActiveLineIndex = 0;
	m_uiMaxLineIndex = 0;
	m_vecLines.clear();
	m_vecLineTokens.clear();
}

// line tokens
void		CDataReader::prepareLineTokens(void)
{
	setLineTokens(CStringUtility::split(CStringUtility::replace(*m_pstrActiveLine, ",", ""), " "));
	setActiveLineTokenIndex(0);
}

string		CDataReader::readLineToken(void)
{
	uint32 uiLineToken = m_uiActiveLineTokenIndex;
	if (uiLineToken >= m_vecLineTokens.size())
	{
		throw EXCEPTION_UNSERIALIZE_INSUFFICIENT_DATA;
	}
	m_uiActiveLineTokenIndex++;
	return m_vecLineTokens[uiLineToken];
}