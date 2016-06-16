#include "CRadioControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Styles/CGUIStyles.h"

auto pOnMouseUp_Radio		= [](void *pControl, void *pTriggerArg) { ((CRadioControl*) pControl)->onMouseUp(*(CPoint2D*) pTriggerArg); };
auto pOnRender_Radio		= [](void *pControl) { ((CRadioControl*) pControl)->render(); };

CRadioControl::CRadioControl(void) :
	CGUIControl(GUI_CONTROL_RADIO),
	CGUIControlComponent_Text(),
	m_uiGroupId(0),
	m_bMarked(false)
{
}

// event binding
void		CRadioControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Radio, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Radio, this));
}

// input
void		CRadioControl::onMouseUp(CPoint2D& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		if (getWindow()->triggerEvent(EVENT_onMarkRadio, this))
		{
			getWindow()->unmarkRadios(this);
			setMarked(true);
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CRadioControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	getStyles()->setItemStatus(isMarked() ? "marked" : "unmarked");

	checkToRecalculateStringSize(getStyles());

	float32 fRadius = getIconRadius();
	CPoint2D vecCircleCenterPosition = getPosition();

	// draw circle
	getStyles()->setHasFillOverwrite(isMarked());
	pGFX->drawCircle(vecCircleCenterPosition, fRadius, getStyles());
	getStyles()->restoreStyleOverwrites();

	// draw text
	pGFX->drawText(getTextPosition(), getTextSize(), getText(), getStyles());

	// reset
	getStyles()->restoreTemporaryStyleData();
}

// cursor
bool			CRadioControl::isPointInControl(CPoint2D& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getTotalSize()); // todo - also its repeated code for them 2 controls - make like CMarkableControlComponent
}

// position
CPoint2D		CRadioControl::getIconCenterPosition(void)
{
	return CMathUtility::getEllipseFromRectangle(getPosition(), getSize());
}

// size
CSize2D			CRadioControl::getTotalSize(void)
{
	return getSize() + CSize2D(getMarkableTextSpacing() + getTextWidth(), 0);
}

uint32			CRadioControl::getIconRadius(void)
{
	return CMathUtility::convertDiameterToRadius(getSize().m_x);
}

// text
CPoint2D		CRadioControl::getTextPosition(void)
{
	return CPoint2D(getPosition().m_x + getSize().m_x + getMarkableTextSpacing(), getPosition().m_y);
}

// styles
int32			CRadioControl::getMarkableTextSpacing(void)
{
	return getStyles()->getStyle<int32>("markable-text-spacing");
}