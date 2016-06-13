#ifndef CThemeDesignerTab_AddItem_H
#define CThemeDesignerTab_AddItem_H

#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Control/eGUIControl.h"
#include "GUI/ThemeDesigner/Tab/CThemeDesignerTab.h"

class CThemeDesignerTab_AddItem : public CThemeDesignerTab
{
public:
	CThemeDesignerTab_AddItem(CThemeDesigner *pThemeDesigner);

	void													bindEvents(void);
	void													initDesign(void);

	void													onLeftMouseDown(CVector2i32& vecCursorPosition);
	void													onMouseMove(CVector2i32& vecCursorPosition);
	void													onLeftMouseDown_MainWindow(CVector2i32& vecCursorPosition);
	void													onLeftMouseDown_MainWindow_WindowEventType(CVector2i32& vecCursorPosition);

	uint32													getTabShapeIndexFromPoint(CVector2i32& vecCursorPosition);
	uint32													getTabControlIndexFromPoint(CVector2i32& vecCursorPosition);
	CVector2i32												getShapeRowPoint(uint32 uiShapeRowIndex);
	CVector2i32												getControlRowPoint(uint32 uiControlRowIndex);
	eGUIShape												getShapeIdFromIndex(uint32 uiShapeIndex);
	eGUIControl												getControlIdFromIndex(uint32 uiControlIndex);

	CVector2ui32											getControlDefaultSize(eGUIControl eControlId);

	void													setHoveredItemType(uint32 uiHoveredItemType) { m_uiHoveredItemType = uiHoveredItemType; }
	uint32													getHoveredItemType(void) { return m_uiHoveredItemType; }

	void													setActiveItemType(uint32 uiActiveItemType) { m_uiActiveItemType = uiActiveItemType; }
	uint32													getActiveItemType(void) { return m_uiActiveItemType; }

	void													setHoveredShapeId(eGUIShape eHoveredShapeId) { m_eHoveredShapeId = eHoveredShapeId; }
	eGUIShape												getHoveredShapeId(void) { return m_eHoveredShapeId; }

	void													setHoveredControlId(eGUIControl eHoveredControlId) { m_eHoveredControlId = eHoveredControlId; }
	eGUIControl												getHoveredControlId(void) { return m_eHoveredControlId; }

	void													setActiveShapeId(eGUIShape eActiveShapeId) { m_eActiveShapeId = eActiveShapeId; }
	eGUIShape												getActiveShapeId(void) { return m_eActiveShapeId; }

	void													setActiveControlId(eGUIControl eActiveControlId) { m_eActiveControlId = eActiveControlId; }
	eGUIControl												getActiveControlId(void) { return m_eActiveControlId; }

	void													setShapeIconStartPosition(CVector2i32& vecShapeIconStartPosition) { m_vecShapeIconStartPosition = vecShapeIconStartPosition; }
	CVector2i32&											getShapeIconStartPosition(void) { return m_vecShapeIconStartPosition; }

	void													setShapeTextStartPosition(CVector2i32& vecShapeTextStartPosition) { m_vecShapeTextStartPosition = vecShapeTextStartPosition; }
	CVector2i32&											getShapeTextStartPosition(void) { return m_vecShapeTextStartPosition; }

	void													setControlIconStartPosition(CVector2i32& vecControlIconStartPosition) { m_vecControlIconStartPosition = vecControlIconStartPosition; }
	CVector2i32&											getControlIconStartPosition(void) { return m_vecControlIconStartPosition; }

	void													setControlTextStartPosition(CVector2i32& vecControlTextStartPosition) { m_vecControlTextStartPosition = vecControlTextStartPosition; }
	CVector2i32&											getControlTextStartPosition(void) { return m_vecControlTextStartPosition; }

	void													setItemRowSize(CVector2ui32& vecItemRowSize) { m_vecItemRowSize = vecItemRowSize; }
	CVector2ui32&											getItemRowSize(void) { return m_vecItemRowSize; }

	void													setItemSize(CVector2ui32& vecItemSize) { m_vecItemSize = vecItemSize; }
	CVector2ui32&											getItemSize(void) { return m_vecItemSize; }

private:
	uint32													m_uiHoveredItemType;
	uint32													m_uiActiveItemType;
	eGUIShape												m_eHoveredShapeId;
	eGUIControl												m_eHoveredControlId;
	eGUIShape												m_eActiveShapeId;
	eGUIControl												m_eActiveControlId;
	CVector2i32												m_vecShapeIconStartPosition;
	CVector2i32												m_vecShapeTextStartPosition;
	CVector2i32												m_vecControlIconStartPosition;
	CVector2i32												m_vecControlTextStartPosition;
	uint32													m_uiItemRowHeight;
	CVector2ui32											m_vecItemRowSize;
	CVector2ui32											m_vecItemSize;
};

#endif