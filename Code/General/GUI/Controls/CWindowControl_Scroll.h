#ifndef CWindowControl_Scroll_H
#define CWindowControl_Scroll_H

#include "Types.h"
#include "CVector2ui32.h"
#include "GUI/Control/CWindowControl.h"
#include "GUI/Controls/Components/CWindowControlComponent_Rectangle.h"
#include "GUI/Controls/Components/eWindowControlOrientation.h"

class CWindowControl_Scroll : public CWindowControl, public CWindowControlComponent_Rectangle
{
public:
	CWindowControl_Scroll(void) :
		m_eScrollOrientation(WINDOW_CONTROL_ORIENTATION_VERTICAL),
		CWindowControl(WINDOW_CONTROL_SCROLL),
		CWindowControlComponent_Rectangle(),
		m_uiSeekBarLength(50),
		m_uiSeekBarFillColour(0x008800FF),
		m_uiSeekBarLineColour(0x000000FF),
		m_fProgress(0.0f),
		m_bSeekBarIsMoving(false)
	{};

	void									bindEvents(void);

	void									onMouseDown(CVector2i32& vecCursorPosition);
	void									onMouseUp(CVector2i32& vecCursorPosition);
	void									onMouseMove(CVector2i32& vecCursorPosition);

	void									render(void);

	bool									isPointInSeekBar(CVector2i32& vecPoint);
	uint32									getSeekBarPositionOffset(void);

	float32									getProgressFor1px(void);						// returns 0.0 to 1.0
	void									increaseProgress(float32 fProgressIncrease);	// parameter 0.0 to 1.0
	float32									getProgressIncreaseForLength(uint32 uiLength);	// parameter in pixels, return 0.0 to 1.0

	uint32									getAvailableScrollLength(void);					// returns in pixels

	void									setScrollOrientation(eWindowControlOrientation eScrollOrientation) { m_eScrollOrientation = eScrollOrientation; }
	eWindowControlOrientation				getScrollOrientation(void) { return m_eScrollOrientation; }

	void									setSeekBarLength(uint32 uiSeekBarLength) { m_uiSeekBarLength = uiSeekBarLength; }	// in pixels
	uint32									getSeekBarLength(void) { return m_uiSeekBarLength; }								// in pixels

	void									setSeekBarFillColour(uint32 uiSeekBarFillColour) { m_uiSeekBarFillColour = uiSeekBarFillColour; }	// RGBA
	uint32									getSeekBarFillColour(void) { return m_uiSeekBarFillColour; }										// RGBA
	
	void									setSeekBarLineColour(uint32 uiSeekBarLineColour) { m_uiSeekBarLineColour = uiSeekBarLineColour; }	// RGBA
	uint32									getSeekBarLineColour(void) { return m_uiSeekBarLineColour; }										// RGBA
	
	void									setProgress(float32 fProgress) { m_fProgress = fProgress; }		// 0.0 to 1.0
	float32									getProgress(void) { return m_fProgress; }						// 0.0 to 1.0

	void									setSeekBarMoving(bool bSeekBarIsMoving) { m_bSeekBarIsMoving = bSeekBarIsMoving; }
	bool									isSeekBarMoving(void) { return m_bSeekBarIsMoving; }

	CVector2i32								getSeekBarPosition(void);
	CVector2ui32							getSeekBarSize(void);

private:
	uint32									m_uiSeekBarLength;
	uint32									m_uiSeekBarFillColour;	// RGBA
	uint32									m_uiSeekBarLineColour;	// RGBA
	float32									m_fProgress;			// 0.0 to 1.0
	eWindowControlOrientation				m_eScrollOrientation;
	uint8									m_bSeekBarIsMoving		: 1;
};

#endif