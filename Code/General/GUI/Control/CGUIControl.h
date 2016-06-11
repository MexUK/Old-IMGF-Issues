#ifndef CGUIControl_H
#define CGUIControl_H

#include "Types.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
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

class CGUIControl : public CGUIItem, public CEventBinder
{
public:
	CGUIControl(eGUIControl eControlType);
	~CGUIControl(void);

	void									unload(void) {};

	virtual void							bindEvents(void) = 0;
	virtual void							render(void) = 0;
	virtual bool							isPointInControl(CVector2i32& vecPoint);

	CWindow*								getWindow(void);

	bool									doesControlHaveFocus(void);

	void									setLayer(CGUILayer* pLayer) { m_pLayer = pLayer; }
	CGUILayer*							getLayer(void) { return m_pLayer; }

	void									setControlType(eGUIControl eControlType) { m_eControlType = eControlType; }
	eGUIControl								getControlType(void) { return m_eControlType; }
	
	void									setControlId(uint32 uiControlId) { m_uiControlId = uiControlId; }
	uint32									getControlId(void) { return m_uiControlId; }
	
	void									setPosition(CVector2i32& vecPosition) { m_vecPosition = vecPosition; }
	CVector2i32&							getPosition(void) { return m_vecPosition; }
	
	void									setSize(CVector2ui32& vecSize) { m_vecSize = vecSize; }
	CVector2ui32&							getSize(void) { return m_vecSize; }
	
	void									setPointMarkedAsInControl(bool bPointMarkedAsInControl) { m_bPointMarkedAsInControl = bPointMarkedAsInControl; }
	bool									isPointMarkedAsInControl(void) { return m_bPointMarkedAsInControl; }

	CGUIScrollPool*							getScrolls(void) { return m_pScrolls; }

private:
	CGUILayer*							m_pLayer;
	eGUIControl								m_eControlType;
	CVector2i32								m_vecPosition;
	CVector2ui32							m_vecSize;
	uint32									m_uiControlId;
	uint8									m_bPointMarkedAsInControl		: 1;
	CGUIScrollPool*							m_pScrolls;
};

#endif