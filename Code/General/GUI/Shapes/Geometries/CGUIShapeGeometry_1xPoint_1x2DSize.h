#ifndef CGUIShapeGeometry_1xPoint_1x2DSize_H
#define CGUIShapeGeometry_1xPoint_1x2DSize_H

#include "Types.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CGUIShapeGeometry_1xPoint_1x2DSize : public CGUIShape
{
public:
	CGUIShapeGeometry_1xPoint_1x2DSize(eGUIShape eShapeType);

	void							setPosition(CVector2i32& vecPosition) { m_vecPosition = vecPosition; }
	CVector2i32&					getPosition(void) { return m_vecPosition; }

	void							setSize(CVector2ui32& vecSize) { m_vecSize = vecSize; }
	CVector2ui32&					getSize(void) { return m_vecSize; }
	
private:
	CVector2i32						m_vecPosition;
	CVector2ui32					m_vecSize;
};

#endif