#ifndef CProgressControl_H
#define CProgressControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Components/CGUIControlComponent_Text.h"
#include "GUI/Controls/Components/CGUIControlComponent_Rectangle.h"

class CProgressControl : public CGUIControl, public CGUIControlComponent_Text, public CGUIControlComponent_Rectangle
{
public:
	CProgressControl(void) :
		CGUIControl(GUI_CONTROL_PROGRESS),
		m_uiMaxTicks(0),
		m_uiCurrentTicks(0),
		m_uiProgressFillColour(0),
		m_bCompletionPercentageShown(false)
	{};

	void									bindEvents(void);

	void									render(void);
	
	float32									getProgress(void);				// 0.0 to 1.0
	std::string								getProgressPercentText(void);
	CVector2i32								getProgressPosition(void);		// in pixels
	int32									getProgressPositionX(void);		// in pixels

	void									setMaxTicks(uint32 uiMaxTicks) { m_uiMaxTicks = uiMaxTicks; }
	uint32									getMaxTicks(void) { return m_uiMaxTicks; }
	
	void									setCurrentTicks(uint32 uiCurrentTicks) { m_uiCurrentTicks = uiCurrentTicks; }
	uint32									getCurrentTicks(void) { return m_uiCurrentTicks; }
	
	void									setProgressFillColour(uint32 uiProgressFillColour) { m_uiProgressFillColour = uiProgressFillColour; }	// RGBA
	uint32									getProgressFillColour(void) { return m_uiProgressFillColour; }											// RGBA

	void									setCompletionPercentageShown(bool bCompletionPercentageShown) { m_bCompletionPercentageShown = bCompletionPercentageShown; }
	bool									isCompletionPercentageShown(void) { return m_bCompletionPercentageShown; }
	
private:
	uint32									m_uiMaxTicks;
	uint32									m_uiCurrentTicks;
	uint32									m_uiProgressFillColour;		// RGBA
	uint8									m_bCompletionPercentageShown	: 1;
};

#endif