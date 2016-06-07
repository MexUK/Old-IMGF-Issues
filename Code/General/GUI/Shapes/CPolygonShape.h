#ifndef CPolygonShape_H
#define CPolygonShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_NxPoint.h"

class CPolygonShape : public CGUIShapeGeometry_NxPoint
{
public:
	CPolygonShape(void);

	void							bindEvents(void);
	void							render(void);
};

#endif