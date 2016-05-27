#include "CIPLEntry_PATH_Group.h"
#include "IPL/Entry/Data Entry/CIPLEntry_PATH.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_PATH_Group::unload(void)
{
	for (CIPLEntry *pIPLEntry : getEntries())
	{
		pIPLEntry->unload();
		delete pIPLEntry;
	}
}

void			CIPLEntry_PATH_Group::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 2:
		CIPLEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_strGroupType = pDataReader->readTokenString();
		m_iMinusOne = pDataReader->readTokenInt32();
		break;
	}

	setLatestPathGroup(this);
}

void			CIPLEntry_PATH_Group::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		CIPLEntry_PATH::serialize();

		pDataWriter->write(m_strGroupType);
		pDataWriter->write(m_iMinusOne);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}