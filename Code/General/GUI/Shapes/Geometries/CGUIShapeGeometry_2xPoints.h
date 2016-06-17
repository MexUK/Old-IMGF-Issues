#ifndef CGUIShapeGeometry_2xPoints_H
#define CGUIShapeGeometry_2xPoints_H

#include "Types.h"
#include "CVector2i32.h"
#include "CPoint2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CGUIShapeGeometry_2xPoints : public CGUIShape
{
public:
	CGUIShapeGeometry_2xPoints(eGUIShape eShapeType);

	void							unserialize(bool bSkipShapeId = false);
	void							serialize(void);

	void							moveItem(CVector2i32& vecPositionChange);
	void							resizeItemViaOffsets(CVector2i32& vecItemSizeChange) {}

	void							setPoint1(CPoint2D& vecPoint1) { m_vecPoint1 = vecPoint1; }
	CPoint2D&						getPoint1(void) { return m_vecPoint1; }

	void							setPoint2(CPoint2D& vecPoint2) { m_vecPoint2 = vecPoint2; }
	CPoint2D&						getPoint2(void) { return m_vecPoint2; }
	
private:
	CPoint2D						m_vecPoint1;
	CPoint2D						m_vecPoint2;
};

#endif