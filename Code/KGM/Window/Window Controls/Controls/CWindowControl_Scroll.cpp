#include "CWindowControl_Scroll.h"
#include "Event/CEventManager.h"
#include "Math/CMathUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "Window/CWindow.h"

// input
void		CWindowControl_Scroll::onMouseDown(CVector2ui32& vecCursorPosition)
{
	if (CMathUtility::isPointInRectangle(vecCursorPosition, getSeekBarPosition(), getSeekBarSize()))
	{
		if (CEventManager::getInstance()->triggerEvent("onStartMovingSeekBar", this))
		{
			m_bSeekBarIsMoving = true;
		}
	}
}

void		CWindowControl_Scroll::onMouseUp(CVector2ui32& vecCursorPosition)
{
	if (m_bSeekBarIsMoving)
	{
		if (CEventManager::getInstance()->triggerEvent("onStopMovingSeekBar", this))
		{
			m_bSeekBarIsMoving = false;
		}
	}
}

void		CWindowControl_Scroll::onMouseMove(CVector2ui32& vecCursorPosition)
{
	if (m_bSeekBarIsMoving)
	{
		if (CEventManager::getInstance()->triggerEvent("onMoveSeekBar", this))
		{
			int32 iYDifference = vecCursorPosition.m_y - CEventManager::getInstance()->getLastCursorPosition().m_y;
			float32 fProgressPerPixel = 1.0f / ((float32)(getSize().m_y - getSeekBarHeight()));
			float32 fNewProgress = getProgress() + (fProgressPerPixel * ((float32)iYDifference));
			if (fNewProgress < 0.0f)
			{
				fNewProgress = 0.0f;
			}
			else if (fNewProgress > 1.0f)
			{
				fNewProgress = 1.0f;
			}
			setProgress(fNewProgress);
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void		CWindowControl_Scroll::render(void)
{
	// background bar
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());
	
	// seek bar
	CGDIPlusUtility::drawRectangleFill(getSeekBarPosition(), getSeekBarSize(), getSeekBarFillColour());
	CGDIPlusUtility::drawRectangleBorder(getSeekBarPosition(), getSeekBarSize(), getSeekBarLineColour());
}

// math
CVector2ui32							CWindowControl_Scroll::getSeekBarPosition(void)
{
	uint32 uiPixelSeekY = ((float32)(getSize().m_y - getSeekBarHeight())) * getProgress();
	return getPosition() + CVector2ui32(0, uiPixelSeekY);
}
CVector2ui32							CWindowControl_Scroll::getSeekBarSize(void)
{
	return CVector2ui32(getSize().m_x, getSeekBarHeight());
}