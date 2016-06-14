#ifndef CGUIShapeGeometry_2xPoints_H
#define CGUIShapeGeometry_2xPoints_H

#include "Types.h"
#include "CVector2i32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CGUIShapeGeometry_2xPoints : public CGUIShape
{
public:
	CGUIShapeGeometry_2xPoints(eGUIShape eShapeType);

	void							moveItem(CVector2i32& vecPositionChange);
	void							resizeItemViaOffsets(CVector2i32& vecItemSizeChange) {}

	void							setPoint1(CVector2i32& vecPoint1) { m_vecPoint1 = vecPoint1; }
	CVector2i32&					getPoint1(void) { return m_vecPoint1; }

	void							setPoint2(CVector2i32& vecPoint2) { m_vecPoint2 = vecPoint2; }
	CVector2i32&					getPoint2(void) { return m_vecPoint2; }
	
private:
	CVector2i32						m_vecPoint1;
	CVector2i32						m_vecPoint2;
};

#endif