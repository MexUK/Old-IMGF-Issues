#include "CIDEEntry_2DFX_SwayableAttr.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_SwayableAttr::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_SwayableAttr::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}