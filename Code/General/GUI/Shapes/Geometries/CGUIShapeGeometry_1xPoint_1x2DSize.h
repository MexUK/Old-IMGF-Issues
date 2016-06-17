#ifndef CGUIShapeGeometry_1xPoint_1x2DSize_H
#define CGUIShapeGeometry_1xPoint_1x2DSize_H

#include "Types.h"
#include "CVector2i32.h"
#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CGUIShapeGeometry_1xPoint_1x2DSize : public CGUIShape
{
public:
	CGUIShapeGeometry_1xPoint_1x2DSize(eGUIShape eShapeType);

	void							unserialize(bool bSkipShapeId = false);
	void							serialize(void);

	void							moveItem(CVector2i32& vecPositionChange) { m_vecPosition += vecPositionChange; }
	void							resizeItemViaOffsets(CVector2i32& vecItemSizeChange) { m_vecSize += CSize2D(vecItemSizeChange.m_x, vecItemSizeChange.m_y); }

	void							setPosition(CPoint2D& vecPosition) { m_vecPosition = vecPosition; }
	CPoint2D&						getPosition(void) { return m_vecPosition; }

	void							setSize(CSize2D& vecSize) { m_vecSize = vecSize; }
	CSize2D&						getSize(void) { return m_vecSize; }
	
private:
	CPoint2D						m_vecPosition;
	CSize2D							m_vecSize;
};

#endif