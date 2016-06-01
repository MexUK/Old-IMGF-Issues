#include "CWindowControl_Button.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"

auto pOnMouseDown_Button	= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Button*) pControl)->onMouseDown(*(CVector2i32*) pTriggerArg); };
auto pOnMouseUp_Button		= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Button*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Button		= [](void *pControl) { ((CWindowControl_Button*) pControl)->render(); };

// event binding
void					CWindowControl_Button::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnMouseDown_Button, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Button, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Button, this));
}

// input
void		CWindowControl_Button::onMouseDown(CVector2i32& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		CEventManager::getInstance()->setEventHogged(true);
	}
}

void		CWindowControl_Button::onMouseUp(CVector2i32& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		getWindow()->triggerEvent(EVENT_onPressButton, this);
	}
}

// render
void		CWindowControl_Button::render(void)
{
	if (doesHaveFill())
	{
		CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	}
	if (doesHaveBorder())
	{
		CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	}
	CGDIPlusUtility::drawText(getPosition(), getSize(), getText(), getTextColour(), getFontSize(), isBold());
}