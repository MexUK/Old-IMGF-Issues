#ifndef CLineShape_H
#define CLineShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_2xPoint.h"

class CLineShape : public CGUIShapeGeometry_2xPoint
{
public:
	CLineShape(void);

	void							bindEvents(void);
	void							render(void);
};

#endif