#ifndef CTabControlEntry_H
#define CTabControlEntry_H

#include "Types.h"
#include <string>

class CTabControl;

class CTabControlEntry
{
public:
	CTabControlEntry(void);
	
	void									unload(void) {}

	void									setActiveTab(void);
	bool									isActiveTab(void);

	void									setTabControl(CTabControl *pTabControl) { m_pTabControl = pTabControl; }
	CTabControl*							getTabControl(void) { return m_pTabControl; }

	void									setText(std::string& strText) { m_strText = strText; }
	std::string&							getText(void) { return m_strText; }
	
	void									setTextWidth(uint32 uiTextWidth) { m_uiTextWidth = uiTextWidth; }	// in pixels
	uint32									getTextWidth(void) { return m_uiTextWidth; }						// in pixels

private:
	CTabControl*							m_pTabControl;
	std::string								m_strText;
	uint32									m_uiTextWidth;
};

#endif