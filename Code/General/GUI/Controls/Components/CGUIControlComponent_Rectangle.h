#ifndef CGUIControlComponent_Rectangle_H
#define CGUIControlComponent_Rectangle_H

#include "Types.h"

class CGUIControlComponent_Rectangle
{
public:
	CGUIControlComponent_Rectangle(void) :
		m_bHasFill(false),
		m_bHasBorder(false),
		m_uiFillColour(0x0000AAFF),
		m_uiLineColour(0x000000FF)
	{};
	
	void									setHasFill(bool bHasFill) { m_bHasFill = bHasFill; }
	bool									doesHaveFill(void) { return m_bHasFill; }

	void									setHasBorder(bool bHasBorder) { m_bHasBorder = bHasBorder; }
	bool									doesHaveBorder(void) { return m_bHasBorder; }

	void									setFillColour(uint32 uiFillColour) { m_uiFillColour = uiFillColour; m_bHasFill = true; } // RGBA
	uint32									getFillColour(void) { return m_uiFillColour; } // RGBA
	
	void									setLineColour(uint32 uiLineColour) { m_uiLineColour = uiLineColour; m_bHasBorder = true; } // RGBA
	uint32									getLineColour(void) { return m_uiLineColour; } // RGBA
	
private:
	uint8									m_bHasFill		: 1;
	uint8									m_bHasBorder	: 1;
	uint32									m_uiFillColour; // RGBA
	uint32									m_uiLineColour; // RGBA
};

#endif