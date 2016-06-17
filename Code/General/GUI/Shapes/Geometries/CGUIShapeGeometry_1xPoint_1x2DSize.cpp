#include "CGUIShapeGeometry_1xPoint_1x2DSize.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CGUIShapeGeometry_1xPoint_1x2DSize::CGUIShapeGeometry_1xPoint_1x2DSize(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_1_POINT_1_X_2D_SIZE, eShapeType)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_vecSize.m_x = 0;
	m_vecSize.m_y = 0;
}

// serialization
void							CGUIShapeGeometry_1xPoint_1x2DSize::unserialize(bool bSkipShapeId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIShape::unserialize(bSkipShapeId);
	setPosition(pDataReader->readPoint2D()); // position
	setSize(pDataReader->readSize2D()); // size
}

void							CGUIShapeGeometry_1xPoint_1x2DSize::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIShape::serialize();
	pDataWriter->writePoint2D(getPosition()); // position
	pDataWriter->writeSize2D(getSize()); // size
}