#include "CDATAnimgrpFormat.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

CDATAnimgrpFormat::CDATAnimgrpFormat(void) :
	CFormat(false, LITTLE_ENDIAN)
{
}

void					CDATAnimgrpFormat::unserialize(void)
{
	// todo CDataReader::getInstance()->readSectionLineEntries<eDATAnimgrpLineType, CDATAnimgrpSection, CDATAnimgrpEntry>(this);
}

void					CDATAnimgrpFormat::serialize(void)
{
	// todo CDataWriter::getInstance()->writeSectionLineEntries<CDATAnimgrpSection, CDATAnimgrpEntry>(this);
}