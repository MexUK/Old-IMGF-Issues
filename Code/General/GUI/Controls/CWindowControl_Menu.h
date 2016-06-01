#ifndef CWindowControl_Menu_H
#define CWindowControl_Menu_H

#include "Types.h"
#include "GUI/Control/CWindowControl.h"
#include "GUI/Controls/Components/CWindowControlComponent_Text.h"
#include "GUI/Controls/Entries/CWindowControlEntry_Menu.h"
#include "Pool/CVectorPool.h"
#include <string>

class CWindowControl_Menu : public CWindowControl, public CWindowControlComponent_Text, public CVectorPool<CWindowControlEntry_Menu*>
{
public:
	CWindowControl_Menu(void) :
		CWindowControl(WINDOW_CONTROL_MENU)
	{};

	void									bindEvents(void);

	void									render(void);

	bool									isPointInControl(CVector2i32& vecPoint) { return false; }
};

#endif