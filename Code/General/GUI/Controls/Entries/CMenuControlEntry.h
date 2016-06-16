#ifndef CMenuControlEntry_H
#define CMenuControlEntry_H

#include "Types.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"
#include "GUI/Control/e2DMirroredOrientation.h"
#include "Pool/CVectorPool.h"

class CMenuControlEntry;

class CMenuControlEntry : public CGUIControlComponent_Text, public CVectorPool<CMenuControlEntry*>
{
public:
	CMenuControlEntry(void);
	
	void									unload(void) {}

	void									setWindowControlOrientation(e2DMirroredOrientation eOrientation) { m_eOrientation = eOrientation; }
	e2DMirroredOrientation					getWindowControlOrientation(void) { return m_eOrientation; }

	void									setItemId(uint32 uiItemId) { m_uiItemId = uiItemId; }
	uint32									getItemId(void) { return m_uiItemId; }
	
private:
	e2DMirroredOrientation					m_eOrientation;
	uint32									m_uiItemId;
};

#endif