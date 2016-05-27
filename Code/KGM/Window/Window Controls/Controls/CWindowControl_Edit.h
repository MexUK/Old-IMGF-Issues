#ifndef CWindowControl_Edit_H
#define CWindowControl_Edit_H

#include "Types.h"
#include "Window/Window Controls/CWindowControl.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Text.h"
#include "Window/Window Controls/Components/CWindowControlComponent_Rectangle.h"
#include "Window/Window Controls/Controls/CWindowControl_Scroll.h"
#include "Window/Window Controls/CWindowScrollPool.h"
#include <string>
#include <vector>

class CWindowControl_Edit : public CWindowControl, public CWindowControlComponent_Text, public CWindowControlComponent_Rectangle
{
public:
	CWindowControl_Edit(void) :
		CWindowControl(WINDOW_CONTROL_EDIT),
		CWindowControlComponent_Rectangle(),
		m_bHasHorizontalScrollBar(false),
		m_bHasVerticalScrollBar(false),
		m_bReadOnly(false),
		m_uiCursorColour(0x000000FF)
	{
		m_vecCursorPosition.m_x = 0;
		m_vecCursorPosition.m_y = 0;
		getScrolls()->addEntry(new CWindowControl_Scroll);
		getScrolls()->addEntry(new CWindowControl_Scroll);
	}
	~CWindowControl_Edit(void)
	{
		getScrolls()->removeAllEntries();
	}

	void									onCharDown(uint8 uiCharCode);
	void									onKeyDown(uint8 uiCharCode);

	void									render(void);
	
	void									setMultiLine(bool bMultiLine) { m_bMultiLine = bMultiLine; } // todo - convert text lines into 1 line
	bool									isMultiLine(void) { return m_bMultiLine; }
	
	void									setHasHorizontalScrollBar(bool bHasHorizontalScrollBar) { m_bHasHorizontalScrollBar = bHasHorizontalScrollBar; }
	bool									doesHaveHorizontalScrollBar(void) { return m_bHasHorizontalScrollBar; }
	
	void									setHasVerticalScrollBar(bool bHasVerticalScrollBar) { m_bHasVerticalScrollBar = bHasVerticalScrollBar; }
	bool									doesHaveVerticalScrollBar(void) { return m_bHasVerticalScrollBar; }
	
	void									setReadOnly(bool bReadOnly) { m_bReadOnly = bReadOnly; }
	bool									isReadOnly(void) { return m_bReadOnly; }
	
	void									setCursorPosition(CVector2ui32& vecCursorPosition) { m_vecCursorPosition = vecCursorPosition; }
	CVector2ui32&							getCursorPosition(void) { return m_vecCursorPosition; }

	void									setCursorColour(uint32 uiCursorColour) { m_uiCursorColour = uiCursorColour; }
	uint32									getCursorColour(void) { return m_uiCursorColour; }

	std::vector<std::string>&				getTextLines(void) { return m_vecTextLines; }

private:
	void									processChar(uint32 uiCharCode);
	void									processKey(uint32 uiCharCode);

	void									addLine(void);
	void									mergeLines(CVector2ui32& vecLineIncdices);
	void									splitLine(CVector2ui32& vecCharacterPosition);

	void									addCharacter(uint32 uiCharCode);
	void									removeCharacterToLeft(void);
	void									removeCharacterToRight(void);
	void									removeCharacter(CVector2ui32& vecCharacterPosition); 
	
	void									moveCursor(CVector2ui32& vecCursorPositionToMove);

private:
	uint8									m_bMultiLine				: 1;
	uint8									m_bHasHorizontalScrollBar	: 1;
	uint8									m_bHasVerticalScrollBar		: 1;
	uint8									m_bReadOnly					: 1;
	CVector2ui32							m_vecCursorPosition;
	uint32									m_uiCursorColour;
	std::vector<std::string>				m_vecTextLines;
};

#endif