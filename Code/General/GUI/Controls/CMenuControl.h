#ifndef CMenuControl_H
#define CMenuControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"
#include "GUI/Controls/Entries/CMenuControlEntry.h"
#include "Pool/CVectorPool.h"
#include <string>

class CMenuControl : public CGUIControl, public CGUIControlComponent_Text, public CVectorPool<CMenuControlEntry*>
{
public:
	CMenuControl(void) :
		CGUIControl(GUI_CONTROL_MENU)
	{};

	void									bindEvents(void);

	void									render(void);

	bool									isPointInControl(CVector2i32& vecPoint) { return false; }
};

#endif