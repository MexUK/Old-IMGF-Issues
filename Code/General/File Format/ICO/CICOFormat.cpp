#include "CICOFormat.h"
#include "BMP/CBMPFormat.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

using namespace std;

CICOFormat::CICOFormat(void) :
	CBMPFormat()
{
	setSkipBMPFileHeaderForSerialize(true);
	setBMPVersion(3);
}

void			CICOFormat::unserialize(void)
{
	// todo - unserialize ICO format
}

void			CICOFormat::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	string strCURHeaderData = "";
	strCURHeaderData += CStringUtility::packUint16(0, false);
	strCURHeaderData += CStringUtility::packUint16(1, false); // 1 for icon, 2 for cursor
	strCURHeaderData += CStringUtility::packUint16(1, false); // image count
	strCURHeaderData += CStringUtility::packUint8((uint8)getWidth());
	strCURHeaderData += CStringUtility::packUint8((uint8)getHeight());
	strCURHeaderData += CStringUtility::packUint8(0); // palette colour count
	strCURHeaderData += CStringUtility::packUint8(0); // reserved
	strCURHeaderData += CStringUtility::packUint16((uint8)getWidth() / 2, false); // x hotspot
	strCURHeaderData += CStringUtility::packUint16((uint8)getHeight() / 2, false); // y hotspot
	strCURHeaderData += CStringUtility::packUint32((getWidth() * getHeight()) * (32 / 8), false); // BPP
	strCURHeaderData += CStringUtility::packUint32(22, false); // offset to BMP data from beginning of file
	
	pDataWriter->writeString(strCURHeaderData + serializeViaMemory());
}