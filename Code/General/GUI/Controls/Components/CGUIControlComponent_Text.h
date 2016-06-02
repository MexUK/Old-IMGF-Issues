#ifndef CGUIControlComponent_Text_H
#define CGUIControlComponent_Text_H

#include "Types.h"
#include <string>

class CGUIControlComponent_Text
{
public:
	CGUIControlComponent_Text(void) :
		m_uiFontSize(13),
		m_uiTextColour(0x000000FF),
		m_bBold(false)
	{};
	
	uint32									getTextWidth(void) { return 500; } // todo

	void									setText(std::string& strText) { m_strText = strText; }
	std::string&							getText(void) { return m_strText; }
	
	void									setFontSize(uint32 uiFontSize) { m_uiFontSize = uiFontSize; }
	uint32									getFontSize(void) { return m_uiFontSize; }

	void									setTextColour(uint32 uiColour) { m_uiTextColour = uiColour; } // RGBA
	uint32									getTextColour(void) { return m_uiTextColour; } // RGBA
	
	void									setBold(bool bBold) { m_bBold = bBold; }
	bool									isBold(void) { return m_bBold; }
	
private:
	std::string								m_strText;
	uint32									m_uiFontSize; // in pixels
	uint32									m_uiTextColour; // RGBA
	uint8									m_bBold			: 1;
};

#endif