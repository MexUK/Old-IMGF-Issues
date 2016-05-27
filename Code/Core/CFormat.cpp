#include "CFormat.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

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
	pDataWriter->open(doesFormatUseBinaryData());
	_serialize(); // todo - have like seraizlize With Open Close like above coz of maybe havin an error with opening/closing file
	pDataWriter->close(doesHaveError());
}

void		CFormat::_serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	try
	{
		serialize(); // derived
	}
	catch (eExceptionCode eCode)
	{
		setErrorCode(eCode);
	};
}