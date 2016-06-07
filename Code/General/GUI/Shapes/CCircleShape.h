#ifndef CCircleShape_H
#define CCircleShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x1DSize.h"

class CCircleShape : public CGUIShapeGeometry_1xPoint_1x1DSize
{
public:
	CCircleShape(void);

	void							bindEvents(void);
	void							render(void);
};

#endif