#ifndef CMenuControl_H
#define CMenuControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Entries/CMenuControlEntry.h"
#include "Pool/CVectorPool.h"
#include <string>

class CMenuControl : public CGUIControl, public CVectorPool<CMenuControlEntry*>
{
public:
	CMenuControl(void);

	void									bindEvents(void);

	void									render(void);

	bool									isPointInControl(CVector2i32& vecPoint) { return false; }
};

#endif