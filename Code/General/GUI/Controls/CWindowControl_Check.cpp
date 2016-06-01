#include "CWindowControl_Check.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"

auto pOnMouseUp_Check		= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Check*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Check		= [](void *pControl) { ((CWindowControl_Check*) pControl)->render(); };

// event binding
void					CWindowControl_Check::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Check, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Check, this));
}

// input
void		CWindowControl_Check::onMouseUp(CVector2i32& vecCursorPosition)
{
	if (CMathUtility::isPointInRectangle(vecCursorPosition, getPosition(), getSize() + CVector2ui32(getTextWidth(), 0)))
	{
		if (getWindow()->triggerEvent(EVENT_onMarkCheck, this))
		{
			setChecked(!isChecked());
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CWindowControl_Check::render(void)
{
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	if(isChecked())
	{
		CGDIPlusUtility::drawLine(CVector2i32(getPosition().m_x, getPosition().m_y + getSize().m_y), CVector2i32(getPosition().m_x + getSize().m_x, getPosition().m_y), getLineColour());
	}
	CGDIPlusUtility::drawText(CVector2i32(getPosition().m_x + getSize().m_x + getIconRightMargin(), getPosition().m_y), getSize(), getText(), getTextColour(), getFontSize(), isBold());
}

// cursor
bool		CWindowControl_Check::isPointInControl(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSizeWithText());
}

// size
CVector2ui32	CWindowControl_Check::getSizeWithText(void)
{
	return getSize() + CVector2ui32(getTextWidth(), 0);
}