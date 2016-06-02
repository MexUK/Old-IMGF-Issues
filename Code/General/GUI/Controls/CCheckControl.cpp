#include "CCheckControl.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"

auto pOnMouseUp_Check		= [](void *pControl, void *pTriggerArg) { ((CCheckControl*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Check		= [](void *pControl) { ((CCheckControl*) pControl)->render(); };

// event binding
void					CCheckControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Check, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Check, this));
}

// input
void		CCheckControl::onMouseUp(CVector2i32& vecCursorPosition)
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
void		CCheckControl::render(void)
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
bool		CCheckControl::isPointInControl(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getSizeWithText());
}

// size
CVector2ui32	CCheckControl::getSizeWithText(void)
{
	return getSize() + CVector2ui32(getTextWidth(), 0);
}