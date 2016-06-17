#include "CDataWriter.h"
#include "eExceptionCode.h"
#include "String/CStringUtility.h"
#include "File/CFileUtility.h"
#include "Path/CPathUtility.h"

using namespace std;

vector<CDataWriter*>	CDataWriter::m_vecDataWriters;

CDataWriter::CDataWriter(void) :
	CDataStream()
{
}

// reset
void					CDataWriter::reset(bool bHasError)
{
	if (bHasError)
	{
		CFileUtility::removeFile(getTemporaryFilePath());
	}
	else
	{
		CFileUtility::moveFile(getTemporaryFilePath(), getFilePath());
	}

	CDataStream::reset();
}

void					CDataWriter::close(bool bHasError)
{
	CDataWriter::reset(bHasError);
}

void					CDataWriter::resetData(void)
{
	m_strData_Memory = "";
}

// file processing
void					CDataWriter::openFile(string& strFilePath, uint32 uiFlags)
{
	string strTemporaryFilePath = getTemporaryFilePath();
	CFileUtility::createFoldersForPath(strTemporaryFilePath);
	m_file.open(strTemporaryFilePath.c_str(), (ios_base::open_mode) uiFlags);
}
bool					CDataWriter::isFileOpen(void)
{
	return m_file.is_open();
}
void					CDataWriter::resetFile(void)
{
	m_file.close();
	m_file.clear();
}

// write string
void					CDataWriter::writeStringRef(string& strData)
{
	writeCString((char*) strData.c_str(), strData.length());
}

void					CDataWriter::writeStringRef(string& strData, uint32 uiTotalByteCountPadded)
{
	writeCString((char*) CStringUtility::zeroPad(CStringUtility::capLength(strData, uiTotalByteCountPadded), uiTotalByteCountPadded).c_str(), uiTotalByteCountPadded);
}

void					CDataWriter::writeString(string strData)
{
	writeCString((char*) strData.c_str(), strData.length());
}

void					CDataWriter::writeString(string strData, uint32 uiTotalByteCountPadded)
{
	writeCString((char*)CStringUtility::zeroPad(CStringUtility::capLength(strData, uiTotalByteCountPadded), uiTotalByteCountPadded).c_str(), uiTotalByteCountPadded);
}

void					CDataWriter::writeString(uint32 uiZeroByteCount)
{
	writeCString((char*)CStringUtility::zeroPad(uiZeroByteCount).c_str(), uiZeroByteCount);
}

void					CDataWriter::writeCString(char *pData, uint32 uiByteCount)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		m_strData_Memory.append(pData, uiByteCount);
		m_uiSeek_Memory += uiByteCount;
		break;
	case DATA_STREAM_FILE:
		m_file.write(pData, uiByteCount);
		if (m_file.fail())
		{
			throw EXCEPTION_FILE_WRITE_FAIL;
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
}

// write int
void					CDataWriter::writeUint8(uint8 uiInt)
{
	writeString(CStringUtility::packUint8(uiInt));
}

void					CDataWriter::writeUint16(uint16 uiInt)
{
	writeString(CStringUtility::packUint16(uiInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeUint32(uint32 uiInt)
{
	writeString(CStringUtility::packUint32(uiInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeUint64(uint64 uiInt)
{
	writeString(CStringUtility::packUint64(uiInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeInt8(int8 iInt)
{
	writeString(CStringUtility::packInt8(iInt));
}

void					CDataWriter::writeInt16(int16 iInt)
{
	writeString(CStringUtility::packInt16(iInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeInt32(int32 iInt)
{
	writeString(CStringUtility::packInt32(iInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeInt64(int64 iInt)
{
	writeString(CStringUtility::packInt64(iInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeVector2ui8(CVector2ui8& vecUints)
{
	writeUint8(vecUints.m_x);
	writeUint8(vecUints.m_y);
}

void					CDataWriter::writeVector3ui8(CVector3ui8& vecUints)
{
	writeUint8(vecUints.m_x);
	writeUint8(vecUints.m_y);
	writeUint8(vecUints.m_z);
}

void					CDataWriter::writeVector3ui32(CVector3ui32& vecUints)
{
	writeUint32(vecUints.m_x);
	writeUint32(vecUints.m_y);
	writeUint32(vecUints.m_z);
}

void					CDataWriter::writeVector4ui8(CVector4ui8& vecUints)
{
	writeUint8(vecUints.m_x);
	writeUint8(vecUints.m_y);
	writeUint8(vecUints.m_z);
	writeUint8(vecUints.m_w);
}

void					CDataWriter::writeStdVectorUint32(vector<uint32>& vecUints)
{
	string strData = "";
	for (uint32 uiInt : vecUints)
	{
		strData += CStringUtility::packUint32(uiInt, m_eEndian == BIG_ENDIAN);
	}
	writeString(strData);
}

void					CDataWriter::writeStdVector4ui8(vector<CVector4ui8>& vecVectors)
{
	string strData = "";
	for (CVector4ui8& vecVector : vecVectors)
	{
		strData += CStringUtility::packVector4ui8(vecVector);
	}
	writeString(strData);
}

void					CDataWriter::writeStdVector4ui16(vector<CVector4ui16>& vecVectors)
{
	string strData = "";
	for (CVector4ui16& vecVector : vecVectors)
	{
		strData += CStringUtility::packVector4ui16(vecVector, m_eEndian == BIG_ENDIAN);
	}
	writeString(strData);
}

// write float
void					CDataWriter::writeFloat32(float32 fFloat)
{
	writeString(CStringUtility::packFloat32(fFloat, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeFloat64(float64 fFloat)
{
	writeString(CStringUtility::packFloat64(fFloat, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::writeVector2D(CVector2D& vecVector)
{
	string strData = "";
	strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN);
	writeString(strData);
}

void					CDataWriter::writeVector3D(CVector3D& vecVector)
{
	string strData = "";
	strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_z, m_eEndian == BIG_ENDIAN);
	writeString(strData);
}

void					CDataWriter::writeVector4D(CVector4D& vecVector)
{
	string strData = "";
	strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_z, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_w, m_eEndian == BIG_ENDIAN);
	writeString(strData);
}

void					CDataWriter::writeStdVector2D(vector<CVector2D>& vecVectors)
{
	string strData = "";
	for (CVector2D& vecVector : vecVectors)
	{
		strData += CStringUtility::packVector2D(vecVector, m_eEndian == BIG_ENDIAN);
	}
	writeString(strData);
}

void					CDataWriter::writeStdVector3D(vector<CVector3D>& vecVectors)
{
	string strData = "";
	for (CVector3D& vecVector : vecVectors)
	{
		strData += CStringUtility::packVector3D(vecVector, m_eEndian == BIG_ENDIAN);
	}
	writeString(strData);
}

void					CDataWriter::writeStdVector4D(vector<CVector4D>& vecVectors)
{
	string strData = "";
	for (CVector4D& vecVector : vecVectors)
	{
		strData += CStringUtility::packVector4D(vecVector, m_eEndian == BIG_ENDIAN);
	}
	writeString(strData);
}

// write token
void					CDataWriter::writeTokenString(string& strString)
{
	m_vecLineTokens.push_back(strString);
}

void					CDataWriter::writeTokenInt32(int32 iInt)
{
	m_vecLineTokens.push_back(CStringUtility::toString(iInt));
}

void					CDataWriter::writeTokenUint32(uint32 uiInt)
{
	m_vecLineTokens.push_back(CStringUtility::toString(uiInt));
}

void					CDataWriter::writeTokenFloat32(float32 fFloat)
{
	m_vecLineTokens.push_back(CStringUtility::toString(fFloat));
}

void					CDataWriter::writeTokenVector2D(CVector2D& vecVector)
{
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_x));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_y));
}

void					CDataWriter::writeTokenVector3D(CVector3D& vecVector)
{
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_x));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_y));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_z));
}

void					CDataWriter::writeTokenVector4D(CVector4D& vecVector)
{
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_x));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_y));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_z));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_w));
}

// write string - type guessed
void					CDataWriter::write(string& strData)
{
	writeString(strData);
}

void					CDataWriter::write(string& strData, uint32 uiTotalByteCountPadded)
{
	writeString(strData, uiTotalByteCountPadded);
}

void					CDataWriter::write(char *pData, uint32 uiByteCount)
{
	writeCString(pData, uiByteCount);
}

// write int - type guessed
void					CDataWriter::write(uint8 uiInt)
{
	writeUint8(uiInt);
}

void					CDataWriter::write(uint16 uiInt)
{
	writeUint16(uiInt);
}

void					CDataWriter::write(uint32 uiInt)
{
	writeUint32(uiInt);
}

void					CDataWriter::write(uint64 uiInt)
{
	writeUint64(uiInt);
}

void					CDataWriter::write(int8 iInt)
{
	writeInt8(iInt);
}

void					CDataWriter::write(int16 iInt)
{
	writeInt16(iInt);
}

void					CDataWriter::write(int32 iInt)
{
	writeInt32(iInt);
}

void					CDataWriter::write(int64 iInt)
{
	writeInt64(iInt);
}

void					CDataWriter::write(CVector4ui8& vecUints)
{
	writeVector4ui8(vecUints);
}

void					CDataWriter::write(vector<uint32>& vecUints)
{
	writeStdVectorUint32(vecUints);
}

void					CDataWriter::write(vector<CVector2D>& vecVectors)
{
	writeStdVector2D(vecVectors);
}

void					CDataWriter::write(vector<CVector3D>& vecVectors)
{
	writeStdVector3D(vecVectors);
}

void					CDataWriter::write(vector<CVector4ui8>& vecVectors)
{
	writeStdVector4ui8(vecVectors);
}

void					CDataWriter::write(vector<CVector4ui16>& vecVectors)
{
	writeStdVector4ui16(vecVectors);
}

// write float - type guessed
void					CDataWriter::write(float32 fFloat)
{
	writeFloat32(fFloat);
}

void					CDataWriter::write(float64 fFloat)
{
	writeFloat64(fFloat);
}

void					CDataWriter::write(CVector2D& vecVector)
{
	writeVector2D(vecVector);
}

void					CDataWriter::write(CVector3D& vecVector)
{
	writeVector3D(vecVector);
}

void					CDataWriter::write(CVector4D& vecVector)
{
	writeVector4D(vecVector);
}

// write token - type guessed
void					CDataWriter::writeToken(string& strString)
{
	writeTokenString(strString);
}

void					CDataWriter::writeToken(int32 iInt)
{
	writeTokenInt32(iInt);
}

void					CDataWriter::writeToken(uint32 uiInt)
{
	writeTokenUint32(uiInt);
}

void					CDataWriter::writeToken(float32 fFloat)
{
	writeTokenFloat32(fFloat);
}

void					CDataWriter::writeToken(CVector2D& vecVector)
{
	writeTokenVector2D(vecVector);
}

void					CDataWriter::writeToken(CVector3D& vecVector)
{
	writeTokenVector3D(vecVector);
}

void					CDataWriter::writeToken(CVector4D& vecVector)
{
	writeTokenVector4D(vecVector);
}

// seek
void					CDataWriter::setSeek(uint64 uiByteIndex)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		setSeek_Memory(uiByteIndex);
		break;
	case DATA_STREAM_FILE:
		m_file.seekp(uiByteIndex);
		if (m_file.eof() || m_file.fail())
		{
			throw EXCEPTION_CANT_SEEK_TO;
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
}

uint64					CDataWriter::getSeek(void)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		return getSeek_Memory();
	case DATA_STREAM_FILE:
		return m_file.tellp();
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
	return 0;
}

string					CDataWriter::generateTemporaryFilePath(string& strFilePath)
{
	return CFileUtility::getNextIncrementingFileName(CPathUtility::replaceFileExtension(strFilePath, "temp"));
}