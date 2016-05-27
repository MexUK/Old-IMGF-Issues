#include "CIDEEntry_PATH.h"
#include "IDE/Entry/Data Entry/PATH/CIDEEntry_PATH_Group.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CIDEEntry_PATH_Group*				CIDEEntry_PATH::m_pLatestPathGroup = nullptr;

void			CIDEEntry_PATH::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
}

void			CIDEEntry_PATH::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
}