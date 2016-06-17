#include "CIPLFormat.h"
#include "IPL/CIPLEntry.h"
#include "Section Lines/eSectionLinesEntryType.h"
#include "Entry/CIPLEntry_Other.h"
#include "Entry/CIPLEntry_Section.h"
#include "Entry/CIPLEntry_Data.h"
#include "Entry/Data Entry/CIPLEntry_INST.h"
#include "Entry/Data Entry/CIPLEntry_ZONE.h"
#include "Entry/Data Entry/CIPLEntry_CULL.h"
#include "Entry/Data Entry/CIPLEntry_PICK.h"
#include "Entry/Data Entry/CIPLEntry_PATH.h"
#include "Entry/Data Entry/CIPLEntry_OCCL.h"
#include "Entry/Data Entry/CIPLEntry_MULT.h"
#include "Entry/Data Entry/CIPLEntry_GRGE.h"
#include "Entry/Data Entry/CIPLEntry_ENEX.h"
#include "Entry/Data Entry/CIPLEntry_CARS.h"
#include "Entry/Data Entry/CIPLEntry_JUMP.h"
#include "Entry/Data Entry/CIPLEntry_TCYC.h"
#include "Entry/Data Entry/CIPLEntry_AUZO.h"
#include "Entry/Data Entry/CIPLEntry_MZON.h"
#include "Entry/Data Entry/CIPLEntry_VNOD.h"
#include "Entry/Data Entry/CIPLEntry_LINK.h"
#include "Entry/Data Entry/CIPLEntry_BLOK.h"
#include "Entry/Data Entry/CIPLEntry_MLOPLUS.h"
#include "Entry/Data Entry/CIPLEntry_2DFX.h"
#include "Entry/Data Entry/CIPLEntry_LODM.h"
#include "Entry/Data Entry/CIPLEntry_SLOW.h"
#include "Entry/Data Entry/CIPLEntry_RTFX.h"
#include "Entry/Data Entry/CIPLEntry_UnknownSection.h"
#include "Entry/Data Entry/PATH/CIPLEntry_PATH_Group.h"
#include "Entry/Data Entry/PATH/CIPLEntry_PATH_Node.h"
#include "String/CStringUtility.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "eExceptionCode.h"

using namespace std;

CIPLFormat::CIPLFormat(void) :
	CSectionLinesFormat(),
	m_bIsBinary(false)
{
}

// unserialization & serialization
void		CIPLFormat::unserialize(void)
{
	if (isBinary())
	{
		// binary IPL file
		return unserializeBinary();
	}
	else
	{
		// text IPL file
		return unserializeText();
	}
}

void		CIPLFormat::serialize(void)
{
	if (isBinary())
	{
		// binary IPL file
		return serializeBinary();
	}
	else
	{
		// text IPL file
		return serializeText();
	}
}

// unserialize
void		CIPLFormat::unserializeBinary(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// header
	string strFourCC = pDataReader->readString(4);
	uint32 uiEntryCount_INST = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown1 = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown2 = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown3 = pDataReader->readUint32();
	uint32 uiEntryCount_CARS = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown4 = pDataReader->readUint32();
	uint32 uiSectionOffset_INST = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_INST = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown1 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown1 = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown2 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown2 = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown3 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown3 = pDataReader->readUint32();
	uint32 uiSectionOffset_CARS = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_CARS = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown4 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown4 = pDataReader->readUint32();

	// entries - INST
	pDataReader->setSeek(uiSectionOffset_INST);
	for (uint32 i = 0; i < uiEntryCount_INST; i++)
	{
		CIPLEntry_INST *pIPLEntry_INST = new CIPLEntry_INST(this);
		pIPLEntry_INST->unserialize();
		getSectionEntries()[IPL_SECTION_INST].push_back(pIPLEntry_INST);
	}

	//RG::CIPLEntry_INST RG_IPLEntries = pDataReader->readStruct<RG::CIPLEntry_INST>();

	// entries - CARS
	pDataReader->setSeek(uiSectionOffset_CARS);
	for (uint32 i = 0; i < uiSectionOffset_CARS; i++)
	{
		CIPLEntry_CARS *pIPLEntry_CARS = new CIPLEntry_CARS(this);
		pIPLEntry_CARS->unserialize();
		getSectionEntries()[IPL_SECTION_CARS].push_back(pIPLEntry_CARS);
	}
}

// serialize
void		CIPLFormat::serializeBinary(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// header
	string strFourCC = "BNRY";
	vector<vector<CIPLEntry*>> vecSections_INST = getSectionsBySection(IPL_SECTION_INST);
	vector<vector<CIPLEntry*>> vecSections_CARS = getSectionsBySection(IPL_SECTION_CARS);
	uint32
		uiEntryCount_INST = 0,
		uiEntryCount_CARS = 0;

	for (auto& vecIPLEntries : vecSections_INST)
	{
		uiEntryCount_INST += vecIPLEntries.size();
	}
	for (auto& vecIPLEntries : vecSections_CARS)
	{
		uiEntryCount_CARS += vecIPLEntries.size();
	}

	uint32
		uiSectionOffset_INST = 76,
		uiSectionOffset_CARS = uiSectionOffset_INST + (uiEntryCount_INST * 40);

	pDataWriter->writeStringRef(strFourCC);
	pDataWriter->writeUint32(uiEntryCount_INST);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(uiEntryCount_CARS);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(uiSectionOffset_INST);
	pDataWriter->writeUint32(uiSectionOffset_INST);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(uiSectionOffset_CARS);
	pDataWriter->writeUint32(uiSectionOffset_CARS);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);

	// entries - INST
	for (auto& vecIPLEntries : vecSections_INST)
	{
		for (auto pIPLEntry_INST : vecIPLEntries)
		{
			pIPLEntry_INST->serialize();
		}
	}

	// entries - CARS
	for (auto& vecIPLEntries : vecSections_CARS)
	{
		for (auto pIPLEntry_CARS : vecIPLEntries)
		{
			pIPLEntry_CARS->serialize();
		}
	}
}

CIPLEntry_Data*		CIPLFormat::unserializeDataEntry(eIPLSection eIPLSectionValue)
{
	CIPLEntry_Data *pIPLEntry = nullptr;
	switch (eIPLSectionValue)
	{
	case IPL_SECTION_PATH:
		pIPLEntry = createDataEntry(eIPLSectionValue, detectPATHType());
		break;
	default:
		pIPLEntry = createDataEntry(eIPLSectionValue);
		break;
	}
	if (!pIPLEntry)
	{
		throw EXCEPTION_UNKNOWN_FORMAT;
	}

	return pIPLEntry;
}

eIPLPathType		CIPLFormat::detectPATHType(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 2:		return IPL_PATH_GROUP;
	case 12:	return IPL_PATH_NODE;
	default:	return IPL_PATH_UNKNOWN;
	}
}

eIPLSection					CIPLFormat::getSectionFromText(string strIPLSectionText)
{
	strIPLSectionText = CStringUtility::zeroPad(strIPLSectionText, 4);

	const uint8 *pszData = (const uint8*) CStringUtility::toUpperCase(strIPLSectionText.substr(0, 4)).c_str();
	switch ((uint32)*pszData)
	{
	case 'INST':	return IPL_SECTION_INST;
	case 'ZONE':	return IPL_SECTION_ZONE;
	case 'CULL':	return IPL_SECTION_CULL;
	case 'PICK':	return IPL_SECTION_PICK;
	case 'PATH':	return IPL_SECTION_PATH;
	case 'OCCL':	return IPL_SECTION_OCCL;
	case 'MULT':	return IPL_SECTION_MULT;
	case 'GRGE':	return IPL_SECTION_GRGE;
	case 'ENEX':	return IPL_SECTION_ENEX;
	case 'CARS':	return IPL_SECTION_CARS;
	case 'JUMP':	return IPL_SECTION_JUMP;
	case 'TCYC':	return IPL_SECTION_TCYC;
	case 'AUZO':	return IPL_SECTION_AUZO;
	case 'MZON':	return IPL_SECTION_MZON;
	case 'VNOD':	return IPL_SECTION_VNOD;
	case 'LINK':	return IPL_SECTION_LINK;
	case 'BLOK':	return IPL_SECTION_BLOK;
	case 'MLO+':	return IPL_SECTION_MLOPLUS;
	case '2DFX':	return IPL_SECTION_2DFX;
	case 'LODM':	return IPL_SECTION_LODM;
	case 'SLOW':	return IPL_SECTION_SLOW;
	case 'RTFX':	return IPL_SECTION_RTFX;
	}
	return IPL_SECTION_UNKNOWN;
}

string						CIPLFormat::getSectionText(eIPLSection eIPLSectionValue)
{
	switch (eIPLSectionValue)
	{
	case IPL_SECTION_INST:		return "INST";
	case IPL_SECTION_ZONE:		return "ZONE";
	case IPL_SECTION_CULL:		return "CULL";
	case IPL_SECTION_PICK:		return "PICK";
	case IPL_SECTION_PATH:		return "PATH";
	case IPL_SECTION_OCCL:		return "OCCL";
	case IPL_SECTION_MULT:		return "MULT";
	case IPL_SECTION_GRGE:		return "GRGE";
	case IPL_SECTION_ENEX:		return "ENEX";
	case IPL_SECTION_CARS:		return "CARS";
	case IPL_SECTION_JUMP:		return "JUMP";
	case IPL_SECTION_TCYC:		return "TCYC";
	case IPL_SECTION_AUZO:		return "AUZO";
	case IPL_SECTION_MZON:		return "MZON";
	case IPL_SECTION_VNOD:		return "VNOD";
	case IPL_SECTION_LINK:		return "LINK";
	case IPL_SECTION_BLOK:		return "BLOK";
	case IPL_SECTION_MLOPLUS:	return "MLO+";
	case IPL_SECTION_2DFX:		return "2DFX";
	case IPL_SECTION_LODM:		return "LODM";
	case IPL_SECTION_SLOW:		return "SLOW";
	case IPL_SECTION_RTFX:		return "RTFX";
	}
	return "Unknown IPL Section";
}

CIPLEntry_Data*				CIPLFormat::createDataEntry(eIPLSection eIPLSectionValue, uint32 uiSectionSpecificType)
{
	switch (eIPLSectionValue)
	{
	case IPL_SECTION_INST:		return new CIPLEntry_INST(this);
	case IPL_SECTION_ZONE:		return new CIPLEntry_ZONE(this);
	case IPL_SECTION_CULL:		return new CIPLEntry_CULL(this);
	case IPL_SECTION_PICK:		return new CIPLEntry_PICK(this);
	case IPL_SECTION_PATH:
	{
		eIPLPathType eIPLPathTypeValue = (eIPLPathType)uiSectionSpecificType;
		switch (eIPLPathTypeValue)
		{
		case IPL_PATH_GROUP:	return new CIPLEntry_PATH_Group(this);
		case IPL_PATH_NODE:		return new CIPLEntry_PATH_Node(this);
		}
		break;
	}
	case IPL_SECTION_OCCL:		return new CIPLEntry_OCCL(this);
	case IPL_SECTION_MULT:		return new CIPLEntry_MULT(this);
	case IPL_SECTION_GRGE:		return new CIPLEntry_GRGE(this);
	case IPL_SECTION_ENEX:		return new CIPLEntry_ENEX(this);
	case IPL_SECTION_CARS:		return new CIPLEntry_CARS(this);
	case IPL_SECTION_JUMP:		return new CIPLEntry_JUMP(this);
	case IPL_SECTION_TCYC:		return new CIPLEntry_TCYC(this);
	case IPL_SECTION_AUZO:		return new CIPLEntry_AUZO(this);
	case IPL_SECTION_MZON:		return new CIPLEntry_MZON(this);
	case IPL_SECTION_VNOD:		return new CIPLEntry_VNOD(this);
	case IPL_SECTION_LINK:		return new CIPLEntry_LINK(this);
	case IPL_SECTION_BLOK:		return new CIPLEntry_BLOK(this);
	case IPL_SECTION_MLOPLUS:	return new CIPLEntry_MLOPLUS(this);
	case IPL_SECTION_2DFX:		return new CIPLEntry_2DFX(this);
	case IPL_SECTION_LODM:		return new CIPLEntry_LODM(this);
	case IPL_SECTION_SLOW:		return new CIPLEntry_SLOW(this);
	case IPL_SECTION_RTFX:		return new CIPLEntry_RTFX(this);
	default:					return new CIPLEntry_UnknownSection(this);
	}
	return nullptr;
}

uint32						CIPLFormat::detectSectionSpecificType(eIPLSection eIPLSectionValue)
{
	switch (eIPLSectionValue)
	{
	case IPL_SECTION_PATH:	return detectPATHType();
	}
	return 0;
}