#include "CEventBoundFunction.h"

CEventBoundFunction::CEventBoundFunction(void) :
	m_uiEventTypeId(0),
	m_uiEventTypeIndex(0),
	m_uiEventId(0),
	m_iZIndex(0),
	m_eEventFunctionType(EVENT_FUNCTION_TYPE_1_ARG),
	m_pFunction_1Arg(nullptr),
	m_pFunction_2Args(nullptr),
	m_pFunctionArgument(nullptr)
{
}

void					CEventBoundFunction::onEventTriggered(void *pFunctionArgument)
{
	switch (m_eEventFunctionType)
	{
	case EVENT_FUNCTION_TYPE_1_ARG:
		m_pFunction_1Arg(pFunctionArgument);
		break;
	case EVENT_FUNCTION_TYPE_2_ARGS:
		m_pFunction_2Args(m_pFunctionArgument, pFunctionArgument);
		break;
	}
}