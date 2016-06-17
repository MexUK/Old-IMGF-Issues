#include "CDATScriptPathFormat.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

CDATScriptPathFormat::CDATScriptPathFormat(void) :
	CFormat(false, LITTLE_ENDIAN)
{
}

void					CDATScriptPathFormat::unserialize(void)
{
	// todo CDataReader::getInstance()->unserializeLineEntries<CDATScriptPathEntry>(this);
}

void					CDATScriptPathFormat::serialize(void)
{
	// todo CDataWriter::getInstance()->serializeLineEntries<CDATScriptPathEntry>(this);
}