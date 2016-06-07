#ifndef CPolygonShape_H
#define CPolygonShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CPolygonShape : public CGUIShape
{
public:
	CPolygonShape(void);
	CPolygonShape(eGUIShape eShapeType);

	void					bindEvents(void);

	void					render(void);
};

#endif