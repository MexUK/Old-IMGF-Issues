#ifndef CTriangleShape_H
#define CTriangleShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_3xPoint.h"

class CTriangleShape : public CGUIShapeGeometry_3xPoint
{
public:
	CTriangleShape(void);

	void							bindEvents(void);
	void							render(void);
};

#endif