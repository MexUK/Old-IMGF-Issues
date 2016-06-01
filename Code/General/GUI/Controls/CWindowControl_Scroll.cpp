#include "CWindowControl_Scroll.h"
#include "GUI/Window/CWindow.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"

auto pOnMouseDown_Scroll	= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Scroll*) pControl)->onMouseDown(*(CVector2i32*) pTriggerArg); };
auto pOnMouseUp_Scroll		= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Scroll*) pControl)->onMouseUp(*(CVector2i32*) pTriggerArg); };
auto pOnMouseMove_Scroll	= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Scroll*) pControl)->onMouseMove(*(CVector2i32*) pTriggerArg); };
auto pOnRender_Scroll		= [](void *pControl) { ((CWindowControl_Scroll*) pControl)->render(); };

// event binding
void									CWindowControl_Scroll::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseDown, pOnMouseDown_Scroll, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Scroll, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onMouseMove, pOnMouseMove_Scroll, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Scroll, this));
}

// input
void									CWindowControl_Scroll::onMouseDown(CVector2i32& vecCursorPosition)
{
	if (isPointInSeekBar(vecCursorPosition))
	{
		if (getWindow()->triggerEvent(EVENT_onStartMovingSeekBar, this))
		{
			setSeekBarMoving(true);
		}
	}
}

void									CWindowControl_Scroll::onMouseUp(CVector2i32& vecCursorPosition)
{
	if (isSeekBarMoving())
	{
		if (getWindow()->triggerEvent(EVENT_onStopMovingSeekBar, this))
		{
			setSeekBarMoving(false);
		}
	}
}

void									CWindowControl_Scroll::onMouseMove(CVector2i32& vecCursorPosition)
{
	if (isSeekBarMoving())
	{
		if (getWindow()->triggerEvent(EVENT_onMoveSeekBar, this))
		{
			increaseProgress(getProgressIncreaseForLength(CEventManager::getInstance()->getCursorMovedSize(vecCursorPosition).m_y));
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void									CWindowControl_Scroll::render(void)
{
	// background bar
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	
	// seek bar
	CGDIPlusUtility::drawRectangleFill(getSeekBarPosition(), getSeekBarSize(), getSeekBarFillColour());
	CGDIPlusUtility::drawRectangleBorder(getSeekBarPosition(), getSeekBarSize(), getSeekBarLineColour());
}

// seek bar
CVector2i32								CWindowControl_Scroll::getSeekBarPosition(void)
{
	return getPosition() + CVector2i32(0, getSeekBarPositionOffset());
}

uint32									CWindowControl_Scroll::getSeekBarPositionOffset(void)
{
	return ((float32) (getSize().m_y - getSeekBarLength())) * getProgress();
}

CVector2ui32							CWindowControl_Scroll::getSeekBarSize(void)
{
	return CVector2ui32(getSize().m_x, getSeekBarLength());
}

bool									CWindowControl_Scroll::isPointInSeekBar(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getSeekBarPosition(), getSeekBarSize());
}

// progress
float32									CWindowControl_Scroll::getProgressFor1px(void)
{
	return 1.0f / ((float32) getAvailableScrollLength());
}

void									CWindowControl_Scroll::increaseProgress(float32 fProgressIncrease)
{
	setProgress(CMathUtility::cap(getProgress() + fProgressIncrease, 0.0f, 1.0f));
}

float32									CWindowControl_Scroll::getProgressIncreaseForLength(uint32 uiLength)
{
	return getProgressFor1px() * ((float32) uiLength);
}

// scroll length
uint32									CWindowControl_Scroll::getAvailableScrollLength(void)
{
	return getSize().m_y - getSeekBarLength();
}