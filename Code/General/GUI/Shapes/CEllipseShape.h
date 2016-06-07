#ifndef CEllipseShape_H
#define CEllipseShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CEllipseShape : public CGUIShape
{
public:
	CEllipseShape(void);
	CEllipseShape(eGUIShape eShapeType);

	void					bindEvents(void);

	void					render(void);
};

#endif