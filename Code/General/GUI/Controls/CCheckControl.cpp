#include "CCheckControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Window/CWindow.h"

auto pOnMouseUp_Check		= [](void *pControl, void *pTriggerArg) { ((CCheckControl*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Check		= [](void *pControl) { ((CCheckControl*) pControl)->render(); };

CCheckControl::CCheckControl(void) :
	CGUIControl(GUI_CONTROL_CHECK),
	m_uiIconRightMargin(5),
	m_bMarked(false)
{
}

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
			setMarked(!isMarked());
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CCheckControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
	if(isMarked())
	{
		pGFX->drawLine(CVector2i32(getPosition().m_x, getPosition().m_y + getSize().m_y), CVector2i32(getPosition().m_x + getSize().m_x, getPosition().m_y), getStyles());
	}
	pGFX->drawText(CVector2i32(getPosition().m_x + getSize().m_x + getIconRightMargin(), getPosition().m_y), getSize(), getText(), getStyles());
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