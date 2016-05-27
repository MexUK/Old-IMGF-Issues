#include "CIPLEntry_PATH.h"
#include "IPL/Entry/Data Entry/PATH/CIPLEntry_PATH_Group.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CIPLEntry_PATH_Group*	CIPLEntry_PATH::m_pLatestPathGroup = nullptr;

void			CIPLEntry_PATH::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
}

void			CIPLEntry_PATH::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
}