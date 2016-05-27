#ifndef CWindowControl_Scroll_H
#define CWindowControl_Scroll_H

#include "Types.h"
#include "Window/Window Controls/CWindowControl.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Rectangle.h"
#include "Window/Window Controls/Components/eWindowControlOrientation.h"

class CWindowControl_Scroll : public CWindowControl, public CWindowControlComponent_Rectangle
{
public:
	CWindowControl_Scroll(void) :
		m_eScrollOrientation(WINDOW_CONTROL_ORIENTATION_VERTICAL),
		CWindowControl(WINDOW_CONTROL_SCROLL),
		CWindowControlComponent_Rectangle(),
		m_uiSeekBarHeight(50),
		m_uiSeekBarFillColour(0x008800FF),
		m_uiSeekBarLineColour(0x000000FF),
		m_fProgress(0.0f),
		m_bSeekBarIsMoving(false)
		//m_uiCurrentScroll(0)
	{};
	
	void									onMouseDown(CVector2ui32& vecCursorPosition);
	void									onMouseUp(CVector2ui32& vecCursorPosition);
	void									onMouseMove(CVector2ui32& vecCursorPosition);

	void									render(void);
	
	void									setScrollOrientation(eWindowControlOrientation eScrollOrientation) { m_eScrollOrientation = eScrollOrientation; }
	eWindowControlOrientation				getScrollOrientation(void) { return m_eScrollOrientation; }

	void									setSeekBarHeight(uint32 uiSeekBarHeight) { m_uiSeekBarHeight = uiSeekBarHeight; }
	uint32									getSeekBarHeight(void) { return m_uiSeekBarHeight; }

	void									setSeekBarFillColour(uint32 uiSeekBarFillColour) { m_uiSeekBarFillColour = uiSeekBarFillColour; }
	uint32									getSeekBarFillColour(void) { return m_uiSeekBarFillColour; }
	
	void									setSeekBarLineColour(uint32 uiSeekBarLineColour) { m_uiSeekBarLineColour = uiSeekBarLineColour; }
	uint32									getSeekBarLineColour(void) { return m_uiSeekBarLineColour; }
	
	void									setProgress(float32 fProgress) { m_fProgress = fProgress; }
	float32									getProgress(void) { return m_fProgress; }

	//void									setCurrentScroll(uint32 uiCurrentScroll) { m_uiCurrentScroll = uiCurrentScroll; } // todo x4
	//uint32									getCurrentScroll(void) { return m_uiCurrentScroll; }

	void									setSeekBarMoving(bool bSeekBarIsMoving) { m_bSeekBarIsMoving = bSeekBarIsMoving; }
	bool									isSeekBarMoving(void) { return m_bSeekBarIsMoving; }

	CVector2ui32							getSeekBarPosition(void);
	CVector2ui32							getSeekBarSize(void);

private:
	uint32									m_uiSeekBarHeight;
	uint32									m_uiSeekBarFillColour;
	uint32									m_uiSeekBarLineColour;
	float32									m_fProgress;
	eWindowControlOrientation				m_eScrollOrientation;
	//uint32									m_uiCurrentScroll; // in pixels
	uint8									m_bSeekBarIsMoving		: 1;
};

#endif