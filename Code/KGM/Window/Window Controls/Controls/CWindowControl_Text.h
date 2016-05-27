#ifndef CWindowControl_Text_H
#define CWindowControl_Text_H

#include "Types.h"
#include "Window/Window Controls/CWindowControl.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Text.h"
#include <string>

class CWindowControl_Text : public CWindowControl, public CWindowControlComponent_Text
{
public:
	CWindowControl_Text(void) :
		CWindowControl(WINDOW_CONTROL_TEXT)
	{};
	
	void									render(void);
};

#endif