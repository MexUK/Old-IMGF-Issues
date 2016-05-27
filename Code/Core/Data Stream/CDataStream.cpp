#include "CDataStream.h"
#include "eExceptionCode.h"

using namespace std;

CDataStream::CDataStream(void) :
	m_eDataStreamType(DATA_STREAM_UNKNOWN),
	m_eEndian(LITTLE_ENDIAN),
	m_uiSeek_Memory(0)
{
}

void		CDataStream::open(bool bBinaryMode)
{
	if (getFilePath() == "")
	{
		throw EXCEPTION_FILE_PATH_NOT_SET;
	}

	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		break;
	case DATA_STREAM_FILE:
	{
		openFile(getFilePath(), ios::in | (bBinaryMode ? ios::binary : 0));
		if (!isFileOpen())
		{
			throw EXCEPTION_CANT_OPEN_FILE;
		}
		break;
	}
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
}

void		CDataStream::close(void)
{
	reset();
}

void		CDataStream::reset(void)
{
	switch (getStreamType())
	{
	case DATA_STREAM_MEMORY:
		m_uiSeek_Memory = 0;
		break;
	case DATA_STREAM_FILE:
		resetFile();
		break;
	default:
		throw EXCEPTION_UNKNOWN_DATA_STREAM_TYPE;
	}
}