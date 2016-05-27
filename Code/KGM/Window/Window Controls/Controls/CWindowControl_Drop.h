#ifndef CWindowControl_Drop_H
#define CWindowControl_Drop_H

#include "Types.h"
#include "Window/Window Controls/CWindowControl.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Text.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Rectangle.h"
#include "Window/Window Controls/Entries/CWindowControlEntry_Drop.h"
#include "Pool/CVectorPool.h"
#include <string>

class CWindowControl_Drop : public CWindowControl, public CWindowControlComponent_Text, public CWindowControlComponent_Rectangle, public CVectorPool<CWindowControlEntry_Drop*>
{
public:
	CWindowControl_Drop(void) :
		CWindowControl(WINDOW_CONTROL_DROP),
		m_uiSelectedIndex(0),
		m_uiListWidth(100),
		m_uiListRowHeight(30),
		m_bSelectionListOpen(false)
	{};
	
	void									onMouseUp(CVector2ui32& vecCursorPosition);

	void									render(void);

	bool									isPointInControl(CVector2ui32& vecPoint);
	
	void									setSelectedIndex(uint32 uiSelectedIndex) { m_uiSelectedIndex = uiSelectedIndex; }
	uint32									getSelectedIndex(void) { return m_uiSelectedIndex; }
	
	void									setListWidth(uint32 uiListWidth) { m_uiListWidth = uiListWidth; }
	uint32									getListWidth(void) { return m_uiListWidth; }

	void									setListRowHeight(uint32 uiListRowHeight) { m_uiListRowHeight = uiListRowHeight; }
	uint32									getListRowHeight(void) { return m_uiListRowHeight; }

	void									setSelectionListOpen(bool bSelectionListOpen) { m_bSelectionListOpen = bSelectionListOpen; }
	bool									isSelectionListOpen(void) { return m_bSelectionListOpen; }
	
private:
	uint32									m_uiSelectedIndex;
	uint32									m_uiListWidth;
	uint32									m_uiListRowHeight;
	uint8									m_bSelectionListOpen	: 1;
};

#endif