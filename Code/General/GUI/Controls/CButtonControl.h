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
	CButtonControl(void) :
		CGUIControl(GUI_CONTROL_BUTTON)
	{};
	
	void									bindEvents(void);

	void									onMouseDown(CVector2i32& vecCursorPosition);
	void									onMouseUp(CVector2i32& vecCursorPosition);

	void									render(void);
};

#endif