#ifndef CRectangleShape_H
#define CRectangleShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x2DSize.h"

class CRectangleShape : public CGUIShapeGeometry_1xPoint_1x2DSize
{
public:
	CRectangleShape(void);

	void							bindEvents(void);
	void							render(void);
};

#endif