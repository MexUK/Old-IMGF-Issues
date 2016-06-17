#include "CGUIShapeGeometry_NxPoints.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CGUIShapeGeometry_NxPoints::CGUIShapeGeometry_NxPoints(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_N_POINTS, eShapeType)
{
}

// serialization
void							CGUIShapeGeometry_NxPoints::unserialize(bool bSkipShapeId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIShape::unserialize(bSkipShapeId);
	uint32 uiPointCount = pDataReader->readUint32(); // point count
	getPoints().resize(uiPointCount);
	for (uint32 i = 0; i < uiPointCount; i++)
	{
		getPoints()[i] = pDataReader->readPoint2D(); // point N
	}
}

void							CGUIShapeGeometry_NxPoints::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIShape::serialize();
	pDataWriter->writeUint32(getPoints().size()); // point count // todo - add getPointCount
	for (CPoint2D& vecPoint : getPoints())
	{
		pDataWriter->writePoint2D(vecPoint); // point N
	}
}

// position
void							CGUIShapeGeometry_NxPoints::moveItem(CVector2i32& vecPositionChange)
{
	for (CPoint2D& vecPoint : m_vecPoints)
	{
		vecPoint += vecPositionChange;
	}
}