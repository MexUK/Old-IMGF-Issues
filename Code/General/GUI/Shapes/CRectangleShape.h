#ifndef CRectangleShape_H
#define CRectangleShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CRectangleShape : public CGUIShape
{
public:
	CRectangleShape(void);
	CRectangleShape(eGUIShape eShapeType);

	void					bindEvents(void);

	void					render(void);
};

#endif