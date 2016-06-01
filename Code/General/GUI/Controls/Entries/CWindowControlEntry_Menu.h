#ifndef CWindowControlEntry_Menu_H
#define CWindowControlEntry_Menu_H

#include "Types.h"
#include "GUI/Controls/Components/eWindowControlOrientation.h"
#include "Pool/CVectorPool.h"
#include <string>

class CWindowControlEntry_Menu;

class CWindowControlEntry_Menu : public CVectorPool<CWindowControlEntry_Menu*>
{
public:
	CWindowControlEntry_Menu(void) :
		m_eOrientation(WINDOW_CONTROL_ORIENTATION_HORIZONTAL),
		m_uiItemId(0)
	{};
	
	void									unload(void) {};

	void									setWindowControlOrientation(eWindowControlOrientation eOrientation) { m_eOrientation = eOrientation; }
	eWindowControlOrientation				getWindowControlOrientation(void) { return m_eOrientation; }

	void									setItemId(uint32 uiItemId) { m_uiItemId = uiItemId; }
	uint32									getItemId(void) { return m_uiItemId; }
	
	void									setText(std::string& strText) { m_strText = strText; }
	std::string&							getText(void) { return m_strText; }
	
private:
	eWindowControlOrientation				m_eOrientation;
	uint32									m_uiItemId;
	std::string								m_strText;
};

#endif