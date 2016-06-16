#include "CScrollControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Math/CMathUtility.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnMouseDown_Scroll	= [](void *pControl, void *pTriggerArg) { ((CScrollControl*) pControl)->onMouseDown(*(CPoint2D*) pTriggerArg); };
auto pOnMouseUp_Scroll		= [](void *pControl, void *pTriggerArg) { ((CScrollControl*) pControl)->onMouseUp(*(CPoint2D*) pTriggerArg); };
auto pOnMouseMove_Scroll	= [](void *pControl, void *pTriggerArg) { ((CScrollControl*) pControl)->onMouseMove(*(CPoint2D*) pTriggerArg); };
auto pOnRender_Scroll		= [](void *pControl) { ((CScrollControl*) pControl)->render(); };

CScrollControl::CScrollControl(void) :
	m_eScrollOrientation(_2D_MIRRORED_ORIENTATION_VERTICAL),
	CGUIControl(GUI_CONTROL_SCROLL),
	m_uiSeekBarLength(50),
	m_uiSeekBarFillColour(0x008800FF),
	m_uiSeekBarLineColour(0x000000FF),
	m_fProgress(0.0f),
	m_bSeekBarIsMoving(false)
{
}

// event binding
void									CScrollControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnMouseDown_Scroll, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Scroll, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onMouseMove, pOnMouseMove_Scroll, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Scroll, this));
}

// input
void									CScrollControl::onMouseDown(CPoint2D& vecCursorPosition)
{
	if (isPointInSeekBar(vecCursorPosition))
	{
		if (getWindow()->triggerEvent(EVENT_onStartMovingSeekBar, this))
		{
			setSeekBarMoving(true);
		}
	}
}

void									CScrollControl::onMouseUp(CPoint2D& vecCursorPosition)
{
	if (isSeekBarMoving())
	{
		if (getWindow()->triggerEvent(EVENT_onStopMovingSeekBar, this))
		{
			setSeekBarMoving(false);
		}
	}
}

void									CScrollControl::onMouseMove(CPoint2D& vecCursorPosition)
{
	if (isSeekBarMoving())
	{
		if (getWindow()->triggerEvent(EVENT_onMoveSeekBar, this))
		{
			increaseProgress(getProgressIncreaseForLength(CEventManager::getInstance()->getScreenCursorMoveDifference().m_y));
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void									CScrollControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	// background bar
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
	
	// seek bar
	pGFX->drawRectangle(getSeekBarPosition(), getSeekBarSize(), getStyles());
}

// seek bar
CPoint2D								CScrollControl::getSeekBarPosition(void)
{
	return getPosition() + CPoint2D(0, getSeekBarPositionOffset());
}

uint32									CScrollControl::getSeekBarPositionOffset(void)
{
	return ((float32) (getSize().m_y - getSeekBarLength())) * getProgress();
}

CSize2D									CScrollControl::getSeekBarSize(void)
{
	return CSize2D(getSize().m_x, getSeekBarLength());
}

bool									CScrollControl::isPointInSeekBar(CPoint2D& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getSeekBarPosition(), getSeekBarSize());
}

// progress
float32									CScrollControl::getProgressFor1px(void)
{
	return 1.0f / ((float32) getAvailableScrollLength());
}

void									CScrollControl::increaseProgress(float32 fProgressIncrease)
{
	setProgress(CMathUtility::cap(getProgress() + fProgressIncrease, 0.0f, 1.0f));
}

float32									CScrollControl::getProgressIncreaseForLength(uint32 uiLength)
{
	return getProgressFor1px() * ((float32) uiLength);
}

// scroll length
uint32									CScrollControl::getAvailableScrollLength(void)
{
	return getSize().m_y - getSeekBarLength();
}