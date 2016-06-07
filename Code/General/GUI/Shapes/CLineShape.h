#ifndef CLineShape_H
#define CLineShape_H

#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shape/CGUIShape.h"

class CLineShape : public CGUIShape
{
public:
	CLineShape(void);

	void					bindEvents(void);

	void					render(void);
};

#endif