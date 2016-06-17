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

	void									unserialize(bool bSkipControlId = false);
	void									serialize(void);

	void									onMouseUp(CPoint2D& vecCursorPosition);

	void									render(void);
	
	bool									isPointInControl(CPoint2D& vecPoint);

	CSize2D									getTotalSize(void); // todo - duplicated code in check and radio controls

	CPoint2D								getTextPosition(void);

	int32									getMarkableTextSpacing(void); // todo - duplicated code in check and radio controls
	
	void									setMarked(bool bMarked) { m_bMarked = bMarked; } // todo - duplicated code in check and radio controls
	bool									isMarked(void) { return m_bMarked; }
	
private:
	uint8									m_bMarked			: 1;
};

#endif