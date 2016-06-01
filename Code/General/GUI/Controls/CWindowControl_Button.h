#ifndef CWindowControl_Button_H
#define CWindowControl_Button_H

#include "Types.h"
#include "GUI/Control/CWindowControl.h"
#include "GUI/Controls/Components/CWindowControlComponent_Text.h"
#include "GUI/Controls/Components/CWindowControlComponent_Rectangle.h"
#include <string>
#include <vector>

class CWindowControl_Button : public CWindowControl, public CWindowControlComponent_Text, public CWindowControlComponent_Rectangle
{
public:
	CWindowControl_Button(void) :
		CWindowControl(WINDOW_CONTROL_BUTTON)
	{};
	
	void									bindEvents(void);

	void									onMouseDown(CVector2ui32& vecCursorPosition);
	void									onMouseUp(CVector2ui32& vecCursorPosition);

	void									render(void);
};

#endif