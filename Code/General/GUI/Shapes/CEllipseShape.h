#ifndef CEllipseShape_H
#define CEllipseShape_H

#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x2DSize.h"

class CEllipseShape : public CGUIShapeGeometry_1xPoint_1x2DSize
{
public:
	CEllipseShape(void);

	void							bindEvents(void);
	void							render(void);
	bool							isPointInItem(CVector2i32& vecPoint);
	CVector2i32						getBoundingRectanglePosition(void);
	CVector2ui32					getBoundingRectangleSize(void);
};

#endif