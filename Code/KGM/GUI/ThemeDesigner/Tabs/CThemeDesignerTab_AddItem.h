#ifndef CThemeDesignerTab_AddItem_H
#define CThemeDesignerTab_AddItem_H

#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Control/eGUIControl.h"
#include "GUI/ThemeDesigner/Tab/CThemeDesignerTab.h"

class CThemeDesignerTab_AddItem : public CThemeDesignerTab
{
public:
	CThemeDesignerTab_AddItem(CThemeDesigner *pThemeDesigner);

	void													bindEvents(void);
	void													initDesign(void);

	void													onLeftMouseDown(CPoint2D& vecCursorPosition);
	void													onMouseMove(CPoint2D& vecCursorPosition);
	void													onLeftMouseDown_MainWindow(CPoint2D& vecCursorPosition);
	void													onLeftMouseDown_MainWindow_WindowEventType(CPoint2D& vecCursorPosition);

	uint32													getTabShapeIndexFromPoint(CPoint2D& vecCursorPosition);
	uint32													getTabControlIndexFromPoint(CPoint2D& vecCursorPosition);
	CPoint2D												getShapeRowPoint(uint32 uiShapeRowIndex);
	CPoint2D												getControlRowPoint(uint32 uiControlRowIndex);
	eGUIShape												getShapeIdFromIndex(uint32 uiShapeIndex);
	eGUIControl												getControlIdFromIndex(uint32 uiControlIndex);

	CSize2D													getControlDefaultSize(eGUIControl eControlId);

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

	void													setShapeIconStartPosition(CPoint2D& vecShapeIconStartPosition) { m_vecShapeIconStartPosition = vecShapeIconStartPosition; }
	CPoint2D&											getShapeIconStartPosition(void) { return m_vecShapeIconStartPosition; }

	void													setShapeTextStartPosition(CPoint2D& vecShapeTextStartPosition) { m_vecShapeTextStartPosition = vecShapeTextStartPosition; }
	CPoint2D&											getShapeTextStartPosition(void) { return m_vecShapeTextStartPosition; }

	void													setControlIconStartPosition(CPoint2D& vecControlIconStartPosition) { m_vecControlIconStartPosition = vecControlIconStartPosition; }
	CPoint2D&											getControlIconStartPosition(void) { return m_vecControlIconStartPosition; }

	void													setControlTextStartPosition(CPoint2D& vecControlTextStartPosition) { m_vecControlTextStartPosition = vecControlTextStartPosition; }
	CPoint2D&											getControlTextStartPosition(void) { return m_vecControlTextStartPosition; }

	void													setItemRowSize(CSize2D& vecItemRowSize) { m_vecItemRowSize = vecItemRowSize; }
	CSize2D&												getItemRowSize(void) { return m_vecItemRowSize; }

	void													setItemSize(CSize2D& vecItemSize) { m_vecItemSize = vecItemSize; }
	CSize2D&												getItemSize(void) { return m_vecItemSize; }

private:
	uint32													m_uiHoveredItemType;
	uint32													m_uiActiveItemType;
	eGUIShape												m_eHoveredShapeId;
	eGUIControl												m_eHoveredControlId;
	eGUIShape												m_eActiveShapeId;
	eGUIControl												m_eActiveControlId;
	CPoint2D												m_vecShapeIconStartPosition;
	CPoint2D												m_vecShapeTextStartPosition;
	CPoint2D												m_vecControlIconStartPosition;
	CPoint2D												m_vecControlTextStartPosition;
	uint32													m_uiItemRowHeight;
	CSize2D													m_vecItemRowSize;
	CSize2D													m_vecItemSize;
};

#endif