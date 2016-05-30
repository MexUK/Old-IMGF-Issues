#ifndef CEventBoundFunction_H
#define CEventBoundFunction_H

#include "Types.h"
#include "eEventFunctionType.h"

class CEventBoundFunction
{
public:
	CEventBoundFunction(void);

	void					onEventTriggered(void *pFunctionArgument);

	void					setEventTypeId(uint32 uiEventTypeId) { m_uiEventTypeId = uiEventTypeId; }
	uint32					getEventTypeId(void) { return m_uiEventTypeId; }

	void					setEventTypeIndex(uint32 uiEventTypeIndex) { m_uiEventTypeIndex = uiEventTypeIndex; }
	uint32					getEventTypeIndex(void) { return m_uiEventTypeIndex; }

	void					setEventId(uint32 uiEventId) { m_uiEventId = uiEventId; }
	uint32					getEventId(void) { return m_uiEventId; }

	void					setZIndex(int32 iZIndex) { m_iZIndex = iZIndex; }
	int32					getZIndex(void) { return m_iZIndex; }

	void					setEventFunctionType(eEventFunctionType eFunctionType) { m_eEventFunctionType = eFunctionType; }
	eEventFunctionType		getEventFunctionType(void) { return m_eEventFunctionType; }

	void					setFunction(void(*fFunction)(void*)) { m_pFunction_1Arg = fFunction; }
	void					setFunction(void(*fFunction)(void*,void*)) { m_pFunction_2Args = fFunction; }
	void					(*getFunction1Arg(void))(void*) { return m_pFunction_1Arg; }
	void					(*getFunction2Args(void))(void*,void*) { return m_pFunction_2Args; }

	void					setFunctionArgument(void* pFunctionArgument) { m_pFunctionArgument = pFunctionArgument; }
	void*					getFunctionArgument(void) { return m_pFunctionArgument; }

private:
	uint32					m_uiEventTypeId;
	uint32					m_uiEventTypeIndex;
	uint32					m_uiEventId;
	int32					m_iZIndex;
	eEventFunctionType		m_eEventFunctionType;
	void					(*m_pFunction_1Arg)(void*);
	void					(*m_pFunction_2Args)(void*,void*);
	void*					m_pFunctionArgument;
};

#endif