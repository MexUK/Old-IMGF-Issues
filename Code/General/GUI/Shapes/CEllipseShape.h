#ifndef CEllipseShape_H
#define CEllipseShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x2DSize.h"

class CEllipseShape : public CGUIShapeGeometry_1xPoint_1x2DSize
{
public:
	CEllipseShape(void);

	void							bindEvents(void);
	void							render(void);
};

#endif