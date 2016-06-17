#include "CGUIShapeGeometry_2xPoints.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CGUIShapeGeometry_2xPoints::CGUIShapeGeometry_2xPoints(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_2_POINTS, eShapeType)
{
	m_vecPoint1.m_x = 0;
	m_vecPoint1.m_y = 0;
	m_vecPoint2.m_x = 0;
	m_vecPoint2.m_y = 0;
}

// serialization
void							CGUIShapeGeometry_2xPoints::unserialize(bool bSkipShapeId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIShape::unserialize(bSkipShapeId);
	setPoint1(pDataReader->readPoint2D()); // point 1
	setPoint2(pDataReader->readPoint2D()); // point 2
}

void							CGUIShapeGeometry_2xPoints::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIShape::serialize();
	pDataWriter->writePoint2D(getPoint1()); // point 1
	pDataWriter->writePoint2D(getPoint2()); // point 2
}

// position
void							CGUIShapeGeometry_2xPoints::moveItem(CVector2i32& vecPositionChange)
{
	m_vecPoint1 += vecPositionChange;
	m_vecPoint2 += vecPositionChange;
}