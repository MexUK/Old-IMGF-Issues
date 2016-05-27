#include "CRWSection_String.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

using namespace std;

CRWSection_String::CRWSection_String(void)
{
	setSectionId(RW_SECTION_STRING);
}

void							CRWSection_String::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	uint32 uiPreviousSeek = pDataReader->getSeek();
	pDataReader->setPeek(true);
	m_strData = pDataReader->readStringUntilZero();
	pDataReader->setPeek(false);
	pDataReader->setSeek(uiPreviousSeek);
}

void							CRWSection_String::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	uint32 uiStringLength = (uint32)(ceil(((float32)(m_strData.length() + 1)) / 4.0) * 4);
	string strData = CStringUtility::zeroPad(m_strData, uiStringLength);

	pDataWriter->write(strData);
}