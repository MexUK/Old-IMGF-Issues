#include "CRWEntry_2dEffect_SunGlare.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWEntry_2dEffect_SunGlare::CRWEntry_2dEffect_SunGlare(void) :
	C2dEffect(_2DFX_SUN_GLARE)
{
};

void							CRWEntry_2dEffect_SunGlare::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	// this 2d effect does not have body data
}

void							CRWEntry_2dEffect_SunGlare::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	// this 2d effect does not have body data
}