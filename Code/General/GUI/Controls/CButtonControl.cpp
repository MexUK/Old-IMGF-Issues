#include "CButtonControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Window/CWindow.h"
#include <gdiplus.h>

auto pOnMouseDown_Button	= [](void *pControl, void *pTriggerArg) { ((CButtonControl*) pControl)->onMouseDown(*(CPoint2D*) pTriggerArg); };
auto pOnMouseUp_Button		= [](void *pControl, void *pTriggerArg) { ((CButtonControl*) pControl)->onMouseUp(*(CPoint2D*) pTriggerArg); };
auto pOnRender_Button		= [](void *pControl) { ((CButtonControl*) pControl)->render(); };

CButtonControl::CButtonControl(void) :
	CGUIControl(GUI_CONTROL_BUTTON)
{
}

// event binding
void					CButtonControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnMouseDown_Button, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Button, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Button, this));
}

// input
void		CButtonControl::onMouseDown(CPoint2D& vecCursorPosition)
{
	if (isPointInItem(vecCursorPosition))
	{
		CEventManager::getInstance()->setEventHogged(true);
	}
}

void		CButtonControl::onMouseUp(CPoint2D& vecCursorPosition)
{
	if (isPointInItem(vecCursorPosition))
	{
		getWindow()->triggerEvent(EVENT_onPressButton, this);
	}
}

// render
void		CButtonControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	checkToRecalculateStringSize(getStyles());

	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
	pGFX->drawText(getPosition(), getSize(), getText(), getStyles());
}