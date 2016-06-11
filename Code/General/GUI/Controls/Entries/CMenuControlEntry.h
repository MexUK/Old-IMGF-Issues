#ifndef CMenuControlEntry_H
#define CMenuControlEntry_H

#include "Types.h"
#include "GUI/Control/e2DMirroredOrientation.h"
#include "Pool/CVectorPool.h"
#include <string>

class CMenuControlEntry;

class CMenuControlEntry : public CVectorPool<CMenuControlEntry*>
{
public:
	CMenuControlEntry(void);
	
	void									unload(void) {};

	void									setWindowControlOrientation(e2DMirroredOrientation eOrientation) { m_eOrientation = eOrientation; }
	e2DMirroredOrientation					getWindowControlOrientation(void) { return m_eOrientation; }

	void									setItemId(uint32 uiItemId) { m_uiItemId = uiItemId; }
	uint32									getItemId(void) { return m_uiItemId; }
	
	void									setText(std::string& strText) { m_strText = strText; }
	std::string&							getText(void) { return m_strText; }
	
private:
	e2DMirroredOrientation					m_eOrientation;
	uint32									m_uiItemId;
	std::string								m_strText;
};

#endif