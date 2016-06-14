#ifndef CCheckControl_H
#define CCheckControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"

class CCheckControl : public CGUIControl, public CGUIControlComponent_Text
{
public:
	CCheckControl(void);

	void									bindEvents(void);

	void									onMouseUp(CVector2i32& vecCursorPosition);

	void									render(void);
	
	bool									isPointInControl(CVector2i32& vecPoint);

	CVector2ui32							getSizeWithText(void); // todo - duplicated code in check and radio controls

	void									setIconRightMargin(uint32 uiIconRightMargin) { m_uiIconRightMargin = uiIconRightMargin; }	// in pixels // todo - duplicated code in check and radio controls
	uint32									getIconRightMargin(void) { return m_uiIconRightMargin; }									// in pixels
	
	void									setMarked(bool bMarked) { m_bMarked = bMarked; } // todo - duplicated code in check and radio controls
	bool									isMarked(void) { return m_bMarked; }
	
private:
	uint32									m_uiIconRightMargin;	// in pixels
	uint8									m_bMarked			: 1;
};

#endif