#ifndef CDropControl_H
#define CDropControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Entries/CDropControlEntry.h"
#include "Pool/CVectorPool.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include <string>

class CDropControl : public CGUIControl, public CVectorPool<CDropControlEntry*>
{
public:
	CDropControl(void);

	void									bindEvents(void);

	void									onMouseUp(CVector2i32& vecCursorPosition);

	void									render(void);

	CDropControlEntry*						addItem(std::string strItemText);
	void									removeItem(CDropControlEntry *pDropEntry);

	bool									isPointInControl(CVector2i32& vecPoint);
	bool									isPointInSelectionList(CVector2i32& vecPoint);

	CVector2i32								getSelectionListPosition(void);
	CVector2ui32							getSelectionListSize(void);
	CVector2i32								getSelectionListEntryPosition(uint32 uiEntryIndex);
	CVector2ui32							getSelectionListEntrySize(void);
	uint32									getSelectionListEntryFromPoint(CVector2i32& vecCursorPosition);

	void									setSize(CVector2ui32& vecSize);

	void									setActiveItem(CDropControlEntry *pDropEntry) { m_pActiveItem = pDropEntry; }
	CDropControlEntry*						getActiveItem(void) { return m_pActiveItem; }
	
	void									setListWidth(uint32 uiListWidth) { m_uiListWidth = uiListWidth; }	// in pixels
	uint32									getListWidth(void) { return m_uiListWidth; }						// in pixels

	void									setListRowHeight(uint32 uiListRowHeight) { m_uiListRowHeight = uiListRowHeight; }	// in pixels
	uint32									getListRowHeight(void) { return m_uiListRowHeight; }								// in pixels

	void									setSelectionListOpen(bool bSelectionListOpen) { m_bSelectionListOpen = bSelectionListOpen; }
	bool									isSelectionListOpen(void) { return m_bSelectionListOpen; }

private:
	CVector2i32								getDropTrianglePosition(void);
	float32									getDropTriangleSideLength(void);
	float32									getDropTriangleSideHeight(void);

	void									recalculateListWidth(void);

private:
	uint32									m_uiSelectedIndex;
	uint32									m_uiListWidth;		// in pixels
	uint32									m_uiListRowHeight;	// in pixels
	uint8									m_bSelectionListOpen				: 1;
	CDropControlEntry*						m_pActiveItem;
};

#endif