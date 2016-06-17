#ifndef CGUIControl_H
#define CGUIControl_H

#include "Types.h"
#include "CPoint2D.h"
#include "CSize2D.h"
#include "eGUIControl.h"
#include "Pool/CVectorPool.h"
#include "Event/CEventBoundFunction.h"
#include "Event/CEventBinder.h"
#include "GUI/Item/CGUIItem.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <gdiplus.h>

class CWindow;
class CGUIScrollPool;
class CGUILayer;
class CGUIStyles;
class CScrollControl;

class CGUIControl : public CGUIItem, public CEventBinder
{
public:
	CGUIControl(eGUIControl eControlType);
	~CGUIControl(void);

	void									unload(void) {}

	virtual void							unserialize(bool bSkipControlId = false);
	virtual void							serialize(void);

	virtual bool							isPointInItem(CPoint2D& vecPoint);
	CPoint2D								getBoundingRectanglePosition(void) { return m_vecPosition; }
	CSize2D									getBoundingRectangleSize(void) { return m_vecSize; }
	void									moveItem(CVector2i32& vecPositionChange) { m_vecPosition += vecPositionChange; }
	void									resizeItemViaOffsets(CVector2i32& vecItemSizeChange) { m_vecSize += CSize2D(vecItemSizeChange.m_x, vecItemSizeChange.m_y); }

	bool									doesControlHaveFocus(void);

	void									addScroll(CScrollControl *pScroll);

	void									setControlType(eGUIControl eControlType) { m_eControlType = eControlType; }
	eGUIControl								getControlType(void) { return m_eControlType; }
	
	void									setControlId(uint32 uiControlId) { m_uiControlId = uiControlId; }
	uint32									getControlId(void) { return m_uiControlId; }
	
	void									setPosition(CPoint2D& vecPosition) { m_vecPosition = vecPosition; }
	CPoint2D&								getPosition(void) { return m_vecPosition; }
	
	void									setSize(CSize2D& vecSize) { m_vecSize = vecSize; }
	CSize2D&								getSize(void) { return m_vecSize; }
	
	void									setPointMarkedAsInControl(bool bPointMarkedAsInControl) { m_bPointMarkedAsInControl = bPointMarkedAsInControl; }
	bool									isPointMarkedAsInControl(void) { return m_bPointMarkedAsInControl; }

	CGUIScrollPool*							getScrolls(void) { return m_pScrolls; }

private:
	eGUIControl								m_eControlType;
	CPoint2D								m_vecPosition;
	CSize2D									m_vecSize;
	uint32									m_uiControlId;
	uint8									m_bPointMarkedAsInControl		: 1;
	CGUIScrollPool*							m_pScrolls;
};

#endif