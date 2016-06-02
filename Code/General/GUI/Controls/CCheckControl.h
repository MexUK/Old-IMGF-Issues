#ifndef CCheckControl_H
#define CCheckControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"
#include "GUI/Controls/Components/CGUIControlComponent_Rectangle.h"

class CCheckControl : public CGUIControl, public CGUIControlComponent_Text, public CGUIControlComponent_Rectangle
{
public:
	CCheckControl(void) :
		CGUIControl(GUI_CONTROL_CHECK),
		m_uiIconRightMargin(5),
		m_bChecked(false)
	{};

	void									bindEvents(void);

	void									onMouseUp(CVector2i32& vecCursorPosition);

	void									render(void);
	
	bool									isPointInControl(CVector2i32& vecPoint);

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