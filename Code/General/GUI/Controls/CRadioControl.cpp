#include "CRadioControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/Window/CWindow.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Styles/CGUIStyles.h"

auto pOnMouseUp_Radio		= [](void *pControl, void *pTriggerArg) { ((CRadioControl*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
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
void		CRadioControl::onMouseUp(CVector2i32& vecCursorPosition)
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
	CVector2i32 vecCircleCenterPosition = getPosition();

	// draw circle
	getStyles()->setHasFillOverwrite(isMarked());
	pGFX->drawCircle(vecCircleCenterPosition, fRadius, getStyles());
	getStyles()->restoreStyleOverwrites();

	// reset
	getStyles()->restoreTemporaryStyleData();

	// draw text
	pGFX->drawText(getTextPosition(), getTextSize(), getText(), getStyles());
}

// cursor
bool			CRadioControl::isPointInControl(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getTotalSize()); // todo - also its repeated code for them 2 controls - make like CMarkableControlComponent
}

// position
CVector2i32		CRadioControl::getIconCenterPosition(void)
{
	return CMathUtility::getEllipseFromRectangle(getPosition(), getSize());
}

// size
CVector2ui32	CRadioControl::getTotalSize(void)
{
	return getSize() + getMarkableTextSpacing() + CVector2ui32(getTextWidth(), 0);
}

uint32			CRadioControl::getIconRadius(void)
{
	return CMathUtility::convertDiameterToRadius(getSize().m_x);
}

// text
CVector2i32		CRadioControl::getTextPosition(void)
{
	return CVector2i32(getPosition().m_x + getSize().m_x + getMarkableTextSpacing(), getPosition().m_y);
}

// styles
int32			CRadioControl::getMarkableTextSpacing(void)
{
	return getStyles()->getStyle<int32>("markable-text-spacing");
}