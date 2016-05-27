#include "CRWEntry_2dEffect_SlotmachineWheel.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWEntry_2dEffect_SlotmachineWheel::CRWEntry_2dEffect_SlotmachineWheel(void) :
	C2dEffect(_2DFX_SLOT_MACHINE_WHEEL),
	m_iWheelIndex(0)
{
};

void							CRWEntry_2dEffect_SlotmachineWheel::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_iWheelIndex = pDataReader->readInt32();
}

void							CRWEntry_2dEffect_SlotmachineWheel::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->write(m_iWheelIndex);
}