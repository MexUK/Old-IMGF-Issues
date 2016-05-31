#ifndef CWindowControl_Check_H
#define CWindowControl_Check_H

#include "Types.h"
#include "Window/Window Controls/CWindowControl.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Text.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Rectangle.h"

class CWindowControl_Check : public CWindowControl, public CWindowControlComponent_Text, public CWindowControlComponent_Rectangle
{
public:
	CWindowControl_Check(void) :
		CWindowControl(WINDOW_CONTROL_CHECK),
		m_uiIconRightMargin(5),
		m_bChecked(false)
	{};

	void									onMouseUp(CVector2ui32& vecCursorPosition);

	void									render(void);
	
	bool									isPointInControl(CVector2ui32& vecPoint);

	CVector2ui32							getSizeWithText(void);

	void									setIconRightMargin(uint32 uiIconRightMargin) { m_uiIconRightMargin = uiIconRightMargin; }	// in pixels
	uint32									getIconRightMargin(void) { return m_uiIconRightMargin; }									// in pixels
	
	void									setChecked(bool bChecked) { m_bChecked = bChecked; }
	bool									isChecked(void) { return m_bChecked; }
	
private:
	uint32									m_uiIconRightMargin;	// in pixels
	uint8									m_bChecked			: 1;
};

#endif