#ifndef CButtonControl_H
#define CButtonControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"
#include <string>
#include <vector>

class CButtonControl : public CGUIControl, public CGUIControlComponent_Text
{
public:
	CButtonControl(void);
	
	void									bindEvents(void);

	void									onMouseDown(CPoint2D& vecCursorPosition);
	void									onMouseUp(CPoint2D& vecCursorPosition);

	void									render(void);
};

#endif