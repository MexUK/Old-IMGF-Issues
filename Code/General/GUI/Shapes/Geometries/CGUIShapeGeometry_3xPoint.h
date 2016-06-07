#ifndef CGUIShapeGeometry_3xPoint_H
#define CGUIShapeGeometry_3xPoint_H

#include "Types.h"
#include "CVector2i32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CGUIShapeGeometry_3xPoint : public CGUIShape
{
public:
	CGUIShapeGeometry_3xPoint(eGUIShape eShapeType);

	void							setPoint1(CVector2i32& vecPoint1) { m_vecPoint1 = vecPoint1; }
	CVector2i32&					getPoint1(void) { return m_vecPoint1; }

	void							setPoint2(CVector2i32& vecPoint2) { m_vecPoint2 = vecPoint2; }
	CVector2i32&					getPoint2(void) { return m_vecPoint2; }
	
	void							setPoint3(CVector2i32& vecPoint3) { m_vecPoint3 = vecPoint3; }
	CVector2i32&					getPoint3(void) { return m_vecPoint3; }
	
private:
	CVector2i32						m_vecPoint1;
	CVector2i32						m_vecPoint2;
	CVector2i32						m_vecPoint3;
};

#endif