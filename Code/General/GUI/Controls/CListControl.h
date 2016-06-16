#ifndef CListControl_H
#define CListControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Styles/CGUIStyles.h"
#include "GUI/Controls/Entries/CListControlEntry.h"
#include "Pool/CVectorPool.h"
#include "CVector2i32.h"
#include <string>
#include <vector>

class CListControl : public CGUIControl, public CVectorPool<CListControlEntry*>
{
public:
	CListControl(void);

	void									bindEvents(void);

	void									onMouseDown(CVector2i32& vecCursorPosition);

	void									render(void);

	CVector2i32								getRowPosition(uint32 uiRowIndex);	// in pixels
	CVector2ui32							getRowSize(void);					// in pixels

	CVector2i32								getCellTextPosition(uint32 uiRowIndex, uint32 uiTextRowIndex, uint32 uiColumnIndex);	// in pixels
	CVector2ui32							getCellTextSize(uint32 uiRowIndex, uint32 uiTextRowIndex, uint32 uiColumnIndex);		// in pixels

	void									setRowHeight(uint32 uiRowHeight) { m_uiRowHeight = uiRowHeight; }	// in pixels
	uint32									getRowHeight(void) { return m_uiRowHeight; }						// in pixels

	uint32									getRowFillColour1(void) { return getStyles()->getStyle<uint32>("row-fill-colour-1"); }	// RGBA
	uint32									getRowFillColour2(void) { return getStyles()->getStyle<uint32>("row-fill-colour-2"); }	// RGBA
	
	void									setColumnWidth(uint32 uiColumnWidth) { m_uiColumnWidth = uiColumnWidth; }	// in pixels
	uint32									getColumnWidth(void) { return m_uiColumnWidth; }							// in pixels

	void									setRowTextHeight(uint32 uiRowTextHeight) { m_uiRowTextHeight = uiRowTextHeight; }	// in pixels
	uint32									getRowTextHeight(void) { return m_uiRowTextHeight; }								// in pixels

	void									setHasVerticalScrollBar(bool bHasVerticalScrollBar) { m_bHasVerticalScrollBar = bHasVerticalScrollBar; }
	bool									doesHaveVerticalScrollBar(void) { return m_bHasVerticalScrollBar; }
	
private:
	uint32									m_uiRowHeight;				// in pixels
	uint32									m_uiColumnWidth;			// in pixels
	uint32									m_uiRowTextHeight;			// in pixels
	uint8									m_bHasVerticalScrollBar		: 1;
};

#endif