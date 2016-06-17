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

	void									unserialize(bool bSkipControlId = false);
	void									serialize(void);

	void									render(void);

	bool									isPointInControl(CPoint2D& vecPoint) { return false; }
};

#endif