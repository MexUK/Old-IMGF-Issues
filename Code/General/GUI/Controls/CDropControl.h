#ifndef CDropControl_H
#define CDropControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"
#include "GUI/Controls/Entries/CDropControlEntry.h"
#include "Pool/CVectorPool.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include <string>

class CDropControl : public CGUIControl, public CGUIControlComponent_Text, public CVectorPool<CDropControlEntry*>
{
public:
	CDropControl(void) :
		CGUIControl(GUI_CONTROL_DROP),
		m_uiSelectedIndex(-1),
		m_uiListWidth(100),
		m_uiListRowHeight(30),
		m_bSelectionListOpen(false)
	{};

	void									bindEvents(void);

	void									onMouseUp(CVector2i32& vecCursorPosition);

	void									render(void);

	bool									isPointInControl(CVector2i32& vecPoint);
	bool									isPointInSelectionList(CVector2i32& vecPoint);

	CVector2i32								getSelectionListPosition(void);
	CVector2ui32							getSelectionListSize(void);
	CVector2i32								getSelectionListEntryPosition(uint32 uiEntryIndex);
	CVector2ui32							getSelectionListEntrySize(void);
	uint32									getSelectionListEntryFromPoint(CVector2i32& vecCursorPosition);

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