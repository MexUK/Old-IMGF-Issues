#include "CFormat.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

using namespace std;

CFormat::CFormat(bool bFormatUsesBinaryFile, eEndian eBinaryDataByteEndian) :
	m_ucErrorCode(0),
	m_bFormatUsesBinaryData(bFormatUsesBinaryFile),
	m_eEndian(eBinaryDataByteEndian)
{
}

// unserialize
void		CFormat::unserializeViaMemory(string& strData)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	pDataReader->setStreamType(DATA_STREAM_MEMORY);
	pDataReader->setData(strData);
	_unserialize();
	pDataReader->reset();
}

void		CFormat::unserializeViaFile(string& strFilePath)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	pDataReader->setStreamType(DATA_STREAM_FILE);
	pDataReader->setFilePath(strFilePath);
	_unserializeWithOpenClose();
}

void		CFormat::_unserialize(void)
{
	try
	{
		unserialize(); // derived
	}
	catch (eExceptionCode eCode)
	{
		setErrorCode(eCode);
	};
}

void		CFormat::_unserializeWithOpenClose(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	try
	{
		pDataReader->open(doesFormatUseBinaryData());
		unserialize(); // derived
		pDataReader->close();
	}
	catch (eExceptionCode eCode)
	{
		setErrorCode(eCode);
	};
}

// serialize
string		CFormat::serializeViaMemory(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	pDataWriter->setStreamType(DATA_STREAM_MEMORY);
	_serialize();
	string strOutputData = pDataWriter->getData();
	pDataWriter->reset();
	return strOutputData;
}

void		CFormat::serializeViaFile(void)
{
	serializeViaFile(getFilePath());
}

void		CFormat::serializeViaFile(string& strFilePath)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	string strTemporaryFilePath = pDataWriter->generateTemporaryFilePath(strFilePath);

	pDataWriter->setStreamType(DATA_STREAM_FILE);
	pDataWriter->setFilePath(strFilePath);
	pDataWriter->setTemporaryFilePath(strTemporaryFilePath);
	_serializeWithOpenClose();
}

void		CFormat::_serialize(void)
{
	try
	{
		serialize(); // derived
	}
	catch (eExceptionCode eCode)
	{
		setErrorCode(eCode);
	};
}

void		CFormat::_serializeWithOpenClose(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	try
	{
		pDataWriter->open(doesFormatUseBinaryData());
		serialize(); // derived
		pDataWriter->close(doesHaveError());
	}
	catch (eExceptionCode eCode)
	{
		setErrorCode(eCode);
	};
}

string		CFormat::getErrorReason(uint8 uiErrorCode)
{
	switch(uiErrorCode)
	{
	case EXCEPTION_INVALID_DATA_SIZE_MULTIPLE:			return "Invalid data size multiple";
	case EXCEPTION_UNSUPPORTED_FORMAT_VERSION:			return "Unsupported format version";
	case EXCEPTION_UNKNOWN_ENCRYPTION_ALGORITHM_USED:	return "Unknown encryption algorithm used";
	case EXCEPTION_UNSUPPORTED_GAME_USED:				return "Unsupported game used";
	case EXCEPTION_CANT_OPEN_FILE:						return "Can't open file";
	case EXCEPTION_FILE_READ_INSUFFICIENT_BYTES:		return "File read insufficient bytes";
	case EXCEPTION_CANT_SEEK_TO:						return "Can't seek to";
	case EXCEPTION_FILE_PATH_NOT_SET:					return "File path not set";
	case EXCEPTION_FILE_INVALID:						return "File invalid";
	case EXCEPTION_CANT_RESTORE_PEEK:					return "Can't restore peek";
	case EXCEPTION_UNKNOWN_FORMAT:						return "Unknown format";
	case EXCEPTION_FILE_READ_FAIL:						return "File read fail";
	case EXCEPTION_FILE_WRITE_FAIL:						return "File write fail";
	case EXCEPTION_UNKNOWN_FORMAT_TYPE:					return "Unknown format type";
	case EXCEPTION_UNSERIALIZE_INSUFFICIENT_DATA:		return "Unserialize insufficient data";
	case EXCEPTION_INVALID_DATA_ORDER:					return "Invalid data order";
	case EXCEPTION_UNKNOWN_DATA_STREAM_TYPE:			return "Unknown data stream type";
	case EXCEPTION_UNKNOWN_ENTRY_TYPE:					return "Unknown entry type";
	}
	return "Unknown error [Error Id = " + CStringUtility::toString(uiErrorCode) + "]";
}