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

// static
CDataWriter*			CDataWriter::getInstance(void)
{
	uint32 uiInstanceCount = m_vecDataWriters.size();
	if (uiInstanceCount == 0)
	{
		return addInstance();
	}
	else
	{
		return m_vecDataWriters[uiInstanceCount - 1];
	}
}
CDataWriter*			CDataWriter::addInstance(void)
{
	CDataWriter *pDataWriter = new CDataWriter;
	m_vecDataWriters.push_back(pDataWriter);
	return pDataWriter;
}
void					CDataWriter::removeLatestInstance(void)
{
	CDataWriter *pDataWriter = m_vecDataWriters[m_vecDataWriters.size() - 1];
	m_vecDataWriters.pop_back();
	delete pDataWriter;
}
void					CDataWriter::moveLatestInstanceDown(void)
{
	uint32 uiCurrentInstanceIndex = m_vecDataWriters.size() - 1;

	CDataWriter *pDataWriterLatest = m_vecDataWriters[uiCurrentInstanceIndex];
	CDataWriter *pDataWriterSecondLatest = m_vecDataWriters[uiCurrentInstanceIndex - 1];

	pDataWriterSecondLatest->write(pDataWriterLatest->getData());

	m_vecDataWriters.pop_back();
	delete pDataWriterLatest;
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
void					CDataWriter::write(string& strData)
{
	write((char*) strData.c_str(), strData.length());
}

void					CDataWriter::write(string& strData, uint32 uiByteCount)
{
	write((char*)CStringUtility::zeroPad(CStringUtility::capLength(strData, uiByteCount), uiByteCount).c_str(), uiByteCount);
}

void					CDataWriter::write(char *pData, uint32 uiByteCount)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		m_strData_Memory.append(pData, uiByteCount);
		m_uiSeek_Memory += uiByteCount;
		break;
	case DATA_STREAM_FILE:
		m_file.write(pData, uiByteCount);
		// todo - check for errors
		break;
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
}

// write int
void					CDataWriter::write(uint8 uiInt)
{
	write(CStringUtility::packUint8(uiInt));
}

void					CDataWriter::write(uint16 uiInt)
{
	write(CStringUtility::packUint16(uiInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::write(uint32 uiInt)
{
	write(CStringUtility::packUint32(uiInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::write(uint64 uiInt)
{
	//write(CStringUtility::packUint64(uiInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::write(int8 iInt)
{
	write(CStringUtility::packInt8(iInt));
}

void					CDataWriter::write(int16 iInt)
{
	write(CStringUtility::packInt16(iInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::write(int32 iInt)
{
	write(CStringUtility::packInt32(iInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::write(int64 iInt)
{
	//write(CStringUtility::packInt64(iInt, m_eEndian == BIG_ENDIAN));
}

void					CDataWriter::write(CVector4ui8 vecUint)
{
	write(vecUint.m_x);
	write(vecUint.m_y);
	write(vecUint.m_z);
	write(vecUint.m_w);
}

void					CDataWriter::write(vector<uint32>& vecUints)
{
	string strData = "";
	for (uint32 uiInt : vecUints)
	{
		strData += CStringUtility::packUint32(uiInt, m_eEndian == BIG_ENDIAN);
	}
	write(strData);
}

void					CDataWriter::write(vector<CVector2D>& vecVectors)
{
	string strData = "";
	for (CVector2D& vecVector : vecVectors)
	{
		strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector2D and 3D
		strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector2D and 3D
	}
	write(strData);
}

void					CDataWriter::write(vector<CVector3D>& vecVectors)
{
	string strData = "";
	for (CVector3D& vecVector : vecVectors)
	{
		strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector2D and 3D
		strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector2D and 3D
		strData += CStringUtility::packFloat32(vecVector.m_z, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector2D and 3D
	}
	write(strData);
}

void					CDataWriter::write(vector<CVector4ui8>& vecVectors)
{
	string strData = "";
	for (CVector4ui8& vecVector : vecVectors)
	{
		strData += CStringUtility::packUint8(vecVector.m_x); // todo - change to CStringUtility::packVector4ui8
		strData += CStringUtility::packUint8(vecVector.m_y); // todo - change to CStringUtility::packVector4ui8
		strData += CStringUtility::packUint8(vecVector.m_z); // todo - change to CStringUtility::packVector4ui8
		strData += CStringUtility::packUint8(vecVector.m_w); // todo - change to CStringUtility::packVector4ui8
	}
	write(strData);
}

void					CDataWriter::write(vector<CVector4ui16>& vecVectors)
{
	string strData = "";
	for (CVector4ui16& vecVector : vecVectors)
	{
		strData += CStringUtility::packUint16(vecVector.m_x, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector4ui16
		strData += CStringUtility::packUint16(vecVector.m_y, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector4ui16
		strData += CStringUtility::packUint16(vecVector.m_z, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector4ui16
		strData += CStringUtility::packUint16(vecVector.m_w, m_eEndian == BIG_ENDIAN); // todo - change to CStringUtility::packVector4ui16
	}
	write(strData);
}

// write float
void					CDataWriter::write(float32 fFloat)
{
	write(CStringUtility::packFloat32(fFloat, m_eEndian == BIG_ENDIAN));
}
void					CDataWriter::write(float64 fFloat)
{
	//write(CStringUtility::packFloat64(fFloat, m_eEndian == BIG_ENDIAN));
}
void					CDataWriter::write(float80 fFloat)
{
	//write(CStringUtility::packFloat80(fFloat, m_eEndian == BIG_ENDIAN));
}
void					CDataWriter::write(CVector2D& vecVector)
{
	string strData = "";
	strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN);
	write(strData);
}
void					CDataWriter::write(CVector3D& vecVector)
{
	string strData = "";
	strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_z, m_eEndian == BIG_ENDIAN);
	write(strData);
}
void					CDataWriter::write(CVector4D& vecVector)
{
	string strData = "";
	strData += CStringUtility::packFloat32(vecVector.m_x, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_y, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_z, m_eEndian == BIG_ENDIAN);
	strData += CStringUtility::packFloat32(vecVector.m_w, m_eEndian == BIG_ENDIAN);
	write(strData);
}

// write token
void					CDataWriter::writeToken(string& strString)
{
	m_vecLineTokens.push_back(strString);
}
void					CDataWriter::writeToken(int32 iInt)
{
	m_vecLineTokens.push_back(CStringUtility::toString(iInt));
}
void					CDataWriter::writeToken(uint32 uiInt)
{
	m_vecLineTokens.push_back(CStringUtility::toString(uiInt));
}
void					CDataWriter::writeToken(float32 fFloat)
{
	m_vecLineTokens.push_back(CStringUtility::toString(fFloat));
}
void					CDataWriter::writeToken(CVector2D& vecVector)
{
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_x));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_y));
}
void					CDataWriter::writeToken(CVector3D& vecVector)
{
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_x));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_y));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_z));
}
void					CDataWriter::writeToken(CVector4D& vecVector)
{
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_x));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_y));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_z));
	m_vecLineTokens.push_back(CStringUtility::toString(vecVector.m_w));
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
	}
	return 0;
}

string					CDataWriter::generateTemporaryFilePath(string& strFilePath)
{
	return CFileUtility::getNextIncrementingFileName(CPathUtility::replaceFileExtension(strFilePath, "temp"));
}