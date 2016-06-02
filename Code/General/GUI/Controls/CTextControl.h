#ifndef CTextControl_H
#define CTextControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"
#include <string>

class CTextControl : public CGUIControl, public CGUIControlComponent_Text
{
public:
	CTextControl(void) :
		CGUIControl(GUI_CONTROL_TEXT)
	{};

	void									bindEvents(void);

	void									render(void);
};

#endif