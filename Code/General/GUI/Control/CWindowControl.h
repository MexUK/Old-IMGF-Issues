#ifndef CWindowControl_H
#define CWindowControl_H

#include "Types.h"
#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "eWindowControlType.h"
#include "Pool/CVectorPool.h"
#include "Event/CEventBoundFunction.h"
#include "Event/CEventBinder.h"
#include <string>
#include <vector>

class CWindow;
class CWindowScrollPool;

class CWindowControl : public CEventBinder
{
public:
	CWindowControl(eWindowControlType eControlType);
	~CWindowControl(void);

	void									unload(void) {};

	virtual void							bindEvents(void) = 0;
	virtual void							render(void) = 0;
	virtual bool							isPointInControl(CVector2i32& vecPoint);

	bool									doesControlHaveFocus(void);
	
	void									setControlType(eWindowControlType eControlType) { m_eControlType = eControlType; }
	eWindowControlType						getControlType(void) { return m_eControlType; }
	
	void									setControlId(uint32 uiControlId) { m_uiControlId = uiControlId; }
	uint32									getControlId(void) { return m_uiControlId; }

	void									setWindow(CWindow *pWindow) { m_pWindow = pWindow; }
	CWindow*								getWindow(void) { return m_pWindow; }
	
	void									setPosition(CVector2i32& vecPosition) { m_vecPosition = vecPosition; }
	CVector2i32&							getPosition(void) { return m_vecPosition; }
	
	void									setSize(CVector2ui32& vecSize) { m_vecSize = vecSize; }
	CVector2ui32&							getSize(void) { return m_vecSize; }
	
	void									setPointMarkedAsInControl(bool bPointMarkedAsInControl) { m_bPointMarkedAsInControl = bPointMarkedAsInControl; }
	bool									isPointMarkedAsInControl(void) { return m_bPointMarkedAsInControl; }

	CWindowScrollPool*						getScrolls(void) { return m_pScrolls; }

private:
	eWindowControlType						m_eControlType;
	CWindow*								m_pWindow;
	CVector2i32								m_vecPosition;
	CVector2ui32							m_vecSize;
	uint32									m_uiControlId;
	uint8									m_bPointMarkedAsInControl		: 1;
	CWindowScrollPool*						m_pScrolls;
};

#endif