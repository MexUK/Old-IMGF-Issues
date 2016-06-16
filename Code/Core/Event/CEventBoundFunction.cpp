#include "CEventBoundFunction.h"

CEventBoundFunction::CEventBoundFunction(void) :
	m_uiEventTypeId(0),
	m_uiEventTypeIndex(0),
	m_uiEventId(0),
	m_iZIndex(0),
	m_eEventFunctionType(EVENT_FUNCTION_TYPE_1_ARG),
	m_pFunction_1Arg(nullptr),
	m_pFunction_2Args(nullptr),
	m_pFunctionBoundArgument(nullptr),
	m_pCallbackObject(nullptr)
{
}

void					CEventBoundFunction::onEventTriggered(uint32 uiEventId, void *pTriggerArgument)
{
	switch (m_eEventFunctionType)
	{
	case EVENT_FUNCTION_TYPE_1_ARG:
		m_pFunction_1Arg(m_pFunctionBoundArgument);
		break;
	case EVENT_FUNCTION_TYPE_2_ARGS:
		m_pFunction_2Args(m_pFunctionBoundArgument, pTriggerArgument);
		break;
	case EVENT_FUNCTION_TYPE_OBJECT_CALLBACK:
		triggerObjectCallback(uiEventId, pTriggerArgument);
		break;
	}
}

void					CEventBoundFunction::triggerObjectCallback(uint32 uiEventId, void *pTriggerArgument)
{
	switch (uiEventId)
	{
	case EVENT_onLeftMouseDown:
		m_pCallbackObject->onLeftMouseDown(*(CPoint2D*) pTriggerArgument);
		break;
	case EVENT_onLeftMouseUp:
		m_pCallbackObject->onLeftMouseUp(*(CPoint2D*) pTriggerArgument);
		break;
	case EVENT_onMouseMove:
		m_pCallbackObject->onMouseMove(*(CPoint2D*) pTriggerArgument);
		break;
	}
}