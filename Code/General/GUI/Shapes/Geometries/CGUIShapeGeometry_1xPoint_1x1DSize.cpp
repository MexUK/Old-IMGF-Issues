#include "CGUIShapeGeometry_1xPoint_1x1DSize.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CGUIShapeGeometry_1xPoint_1x1DSize::CGUIShapeGeometry_1xPoint_1x1DSize(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_1_POINT_1_X_1D_SIZE, eShapeType)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_fSize = 0.0f;
}

// serialization
void							CGUIShapeGeometry_1xPoint_1x1DSize::unserialize(bool bSkipShapeId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIShape::unserialize(bSkipShapeId);
	setPosition(pDataReader->readPoint2D()); // position
	setSize(pDataReader->readFloat32()); // size
}

void							CGUIShapeGeometry_1xPoint_1x1DSize::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIShape::serialize();
	pDataWriter->writePoint2D(getPosition()); // position
	pDataWriter->writeFloat32(getSize()); // size
}

// center position
CPoint2D						CGUIShapeGeometry_1xPoint_1x1DSize::getCenterPosition(void)
{
	return getPosition() + CPoint2D(getSize(), getSize());
}