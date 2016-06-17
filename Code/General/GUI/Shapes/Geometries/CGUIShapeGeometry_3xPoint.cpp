#include "CGUIShapeGeometry_3xPoints.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CGUIShapeGeometry_3xPoints::CGUIShapeGeometry_3xPoints(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_3_POINTS, eShapeType)
{
	m_vecPoint1.m_x = 0;
	m_vecPoint1.m_y = 0;
	m_vecPoint2.m_x = 0;
	m_vecPoint2.m_y = 0;
	m_vecPoint3.m_x = 0;
	m_vecPoint3.m_y = 0;
}

// serialization
void							CGUIShapeGeometry_3xPoints::unserialize(bool bSkipShapeId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIShape::unserialize(bSkipShapeId);
	setPoint1(pDataReader->readPoint2D()); // point 1
	setPoint2(pDataReader->readPoint2D()); // point 2
	setPoint3(pDataReader->readPoint2D()); // point 3
}

void							CGUIShapeGeometry_3xPoints::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIShape::serialize();
	pDataWriter->writePoint2D(getPoint1()); // point 1
	pDataWriter->writePoint2D(getPoint2()); // point 2
	pDataWriter->writePoint2D(getPoint3()); // point 3
}

// position
void							CGUIShapeGeometry_3xPoints::moveItem(CVector2i32& vecPositionChange)
{
	m_vecPoint1 += vecPositionChange;
	m_vecPoint2 += vecPositionChange;
	m_vecPoint3 += vecPositionChange;
}