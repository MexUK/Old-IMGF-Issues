#ifndef CRadioControl_H
#define CRadioControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"

class CRadioControl : public CGUIControl, public CGUIControlComponent_Text
{
public:
	CRadioControl(void) :
		CGUIControl(GUI_CONTROL_RADIO),
		CGUIControlComponent_Text(),
		m_uiIconRightMargin(5),
		m_uiGroupId(0),
		m_bMarked(false)
	{};

	void									bindEvents(void);

	void									onMouseUp(CVector2i32& vecCursorPosition);

	void									render(void);

	bool									isPointInControl(CVector2i32& vecPoint);

	CVector2i32								getIconCenterPosition(void);

	CVector2ui32							getSizeWithText(void); // todo - duplicated code in check and radio controls
	uint32									getIconRadius(void);

	CVector2i32								getTextPosition(void);
	CVector2ui32							getTextSize(void);

	void									setIconRightMargin(uint32 uiIconRightMargin) { m_uiIconRightMargin = uiIconRightMargin; }	// in pixels // todo - duplicated code in check and radio controls
	uint32									getIconRightMargin(void) { return m_uiIconRightMargin; }									// in pixels
	
	void									setMarked(bool bMarked) { m_bMarked = bMarked; } // todo - duplicated code in check and radio controls
	bool									isMarked(void) { return m_bMarked; }
	
	void									setGroupId(uint32 uiGroupId) { m_uiGroupId = uiGroupId; }
	uint32									getGroupId(void) { return m_uiGroupId; }
	
private:
	uint32									m_uiIconRightMargin;	// in pixels
	uint32									m_uiGroupId;
	uint8									m_bMarked			: 1;
};

#endif