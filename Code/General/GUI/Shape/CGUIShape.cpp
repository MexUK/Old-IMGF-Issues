#include "CGUIShape.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CGUIShape::CGUIShape(eGUIShapeGeometry eShapeGeometry, eGUIShape eShapeType) :
	m_eShapeGeometry(eShapeGeometry),
	m_eShapeType(eShapeType),
	m_uiShapeId(0)
{
}

// serialization
void									CGUIShape::unserialize(bool bSkipShapeId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	if (!bSkipShapeId)
	{
		setShapeType((eGUIShape) pDataReader->readUint32()); // shape type
	}
}

void									CGUIShape::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeUint32(getShapeType()); // shape type
}