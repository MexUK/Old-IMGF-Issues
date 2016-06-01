#ifndef CWindowControl_Drop_H
#define CWindowControl_Drop_H

#include "Types.h"
#include "GUI/Control/CWindowControl.h"
#include "GUI/Controls/Components/CWindowControlComponent_Text.h"
#include "GUI/Controls/Components/CWindowControlComponent_Rectangle.h"
#include "GUI/Controls/Entries/CWindowControlEntry_Drop.h"
#include "Pool/CVectorPool.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
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

	void									bindEvents(void);

	void									onMouseUp(CVector2ui32& vecCursorPosition);

	void									render(void);

	bool									isPointInControl(CVector2ui32& vecPoint);
	bool									isPointInSelectionList(CVector2ui32& vecPoint);

	CVector2ui32							getSelectionListPosition(void);
	CVector2ui32							getSelectionListSize(void);
	CVector2ui32							getSelectionListEntryPosition(uint32 uiEntryIndex);
	CVector2ui32							getSelectionListEntrySize(void);
	uint32									getSelectionListEntryFromPoint(CVector2ui32& vecCursorPosition);

	void									setSelectedIndex(uint32 uiSelectedIndex) { m_uiSelectedIndex = uiSelectedIndex; }
	uint32									getSelectedIndex(void) { return m_uiSelectedIndex; }
	
	void									setListWidth(uint32 uiListWidth) { m_uiListWidth = uiListWidth; }	// in pixels
	uint32									getListWidth(void) { return m_uiListWidth; }						// in pixels

	void									setListRowHeight(uint32 uiListRowHeight) { m_uiListRowHeight = uiListRowHeight; }	// in pixels
	uint32									getListRowHeight(void) { return m_uiListRowHeight; }								// in pixels

	void									setSelectionListOpen(bool bSelectionListOpen) { m_bSelectionListOpen = bSelectionListOpen; }
	bool									isSelectionListOpen(void) { return m_bSelectionListOpen; }
	
private:
	uint32									m_uiSelectedIndex;
	uint32									m_uiListWidth;		// in pixels
	uint32									m_uiListRowHeight;	// in pixels
	uint8									m_bSelectionListOpen	: 1;
};

#endif