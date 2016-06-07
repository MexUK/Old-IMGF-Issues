#ifndef CSquareShape_H
#define CSquareShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x1DSize.h"

class CSquareShape : public CGUIShapeGeometry_1xPoint_1x1DSize
{
public:
	CSquareShape(void);

	void							bindEvents(void);
	void							render(void);
};

#endif