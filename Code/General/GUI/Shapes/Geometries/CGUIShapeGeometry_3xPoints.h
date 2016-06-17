#ifndef CGUIShapeGeometry_3xPoints_H
#define CGUIShapeGeometry_3xPoints_H

#include "Types.h"
#include "CVector2i32.h"
#include "CPoint2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CGUIShapeGeometry_3xPoints : public CGUIShape
{
public:
	CGUIShapeGeometry_3xPoints(eGUIShape eShapeType);

	void							unserialize(bool bSkipShapeId = false);
	void							serialize(void);

	void							moveItem(CVector2i32& vecPositionChange);
	void							resizeItemViaOffsets(CVector2i32& vecItemSizeChange) {}

	void							setPoint1(CPoint2D& vecPoint1) { m_vecPoint1 = vecPoint1; }
	CPoint2D&						getPoint1(void) { return m_vecPoint1; }

	void							setPoint2(CPoint2D& vecPoint2) { m_vecPoint2 = vecPoint2; }
	CPoint2D&						getPoint2(void) { return m_vecPoint2; }
	
	void							setPoint3(CPoint2D& vecPoint3) { m_vecPoint3 = vecPoint3; }
	CPoint2D&						getPoint3(void) { return m_vecPoint3; }
	
private:
	CPoint2D						m_vecPoint1;
	CPoint2D						m_vecPoint2;
	CPoint2D						m_vecPoint3;
};

#endif