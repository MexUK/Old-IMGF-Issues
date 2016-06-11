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

	void													setItemType(uint32 uiItemType) { m_uiItemType = uiItemType; }
	uint32													getItemType(void) { return m_uiItemType; }

	void													setShapeId(eGUIShape eShapeId) { m_eShapeId = eShapeId; }
	eGUIShape												getShapeId(void) { return m_eShapeId; }

	void													setControlId(eGUIControl eControlId) { m_eControlId = eControlId; }
	eGUIControl												getControlId(void) { return m_eControlId; }

	void													setShapeIconStartPosition(CVector2i32& vecShapeIconStartPosition) { m_vecShapeIconStartPosition = vecShapeIconStartPosition; }
	CVector2i32&											getShapeIconStartPosition(void) { return m_vecShapeIconStartPosition; }

	void													setShapeTextStartPosition(CVector2i32& vecShapeTextStartPosition) { m_vecShapeTextStartPosition = vecShapeTextStartPosition; }
	CVector2i32&											getShapeTextStartPosition(void) { return m_vecShapeTextStartPosition; }

	void													setControlIconStartPosition(CVector2i32& vecControlIconStartPosition) { m_vecControlIconStartPosition = vecControlIconStartPosition; }
	CVector2i32&											getControlIconStartPosition(void) { return m_vecControlIconStartPosition; }

	void													setControlTextStartPosition(CVector2i32& vecControlTextStartPosition) { m_vecControlTextStartPosition = vecControlTextStartPosition; }
	CVector2i32&											getControlTextStartPosition(void) { return m_vecControlTextStartPosition; }

	void													setItemRowHeight(uint32 uiItemRowHeight) { m_uiItemRowHeight = uiItemRowHeight; }
	uint32													getItemRowHeight(void) { return m_uiItemRowHeight; }

	void													setItemSize(CVector2ui32& vecItemSize) { m_vecItemSize = vecItemSize; }
	CVector2ui32&											getItemSize(void) { return m_vecItemSize; }

private:
	uint32													m_uiItemType;
	eGUIShape												m_eShapeId;
	eGUIControl												m_eControlId;
	CVector2i32												m_vecShapeIconStartPosition;
	CVector2i32												m_vecShapeTextStartPosition;
	CVector2i32												m_vecControlIconStartPosition;
	CVector2i32												m_vecControlTextStartPosition;
	uint32													m_uiItemRowHeight;
	CVector2ui32											m_vecItemSize;
};

#endif