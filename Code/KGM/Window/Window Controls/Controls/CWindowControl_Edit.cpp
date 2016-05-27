#include "CWindowControl_Edit.h"
#include "Window/CWindow.h"
#include "String/CStringUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"

using namespace std;

// input
void		CWindowControl_Edit::onCharDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processChar(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

void		CWindowControl_Edit::onKeyDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processKey(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

// render
void		CWindowControl_Edit::render(void)
{
	CVector2ui32
		vecMinPosition = getPosition(),
		vecSize = getSize(),
		vecMaxPosition = vecMinPosition + vecSize,
		vecTextPosition = vecMinPosition;

	// fill and border
	CGDIPlusUtility::drawRectangleFill(vecMinPosition, vecSize, getFillColour());
	CGDIPlusUtility::drawRectangleBorder(vecMinPosition, vecSize, getLineColour());

	// cursor
	if (!isReadOnly())
	{
		CGDIPlusUtility::drawLine(vecMinPosition + CVector2ui32(getCursorPosition().m_x * 10, getCursorPosition().m_y * getFontSize()), vecMinPosition + CVector2ui32(getCursorPosition().m_x * 10, (getCursorPosition().m_y * getFontSize()) + getFontSize()), getCursorColour());
	}

	// text lines
	uint32
		i = 0,
		uiFontSize = getFontSize();
	for (string& strTextLine : getTextLines())
	{
		CGDIPlusUtility::drawText(vecTextPosition, vecSize, strTextLine, getTextColour(), getFontSize(), isBold());
		vecTextPosition.m_y += uiFontSize;
		i++;
	}
}

// input processing
void		CWindowControl_Edit::processChar(uint32 uiCharCode)
{
	addCharacter(uiCharCode);
}

void		CWindowControl_Edit::processKey(uint32 uiCharCode)
{
	switch (uiCharCode)
	{
	case VK_RETURN:
		addLine();
		break;
	case VK_BACK:
		removeCharacterToLeft();
		break;
	case VK_DELETE:
		removeCharacterToRight();
		break;
	case VK_UP:
		moveCursor(CVector2ui32(0, -1));
		break;
	case VK_DOWN:
		moveCursor(CVector2ui32(0, 1));
		break;
	case VK_LEFT:
		moveCursor(CVector2ui32(-1, 0));
		break;
	case VK_RIGHT:
		moveCursor(CVector2ui32(1, 0));
		break;
	case VK_PRIOR:
		moveCursor(CVector2ui32(0, -20));
		break;
	case VK_NEXT:
		moveCursor(CVector2ui32(0, 20));
		break;
	}
}

// lines
void		CWindowControl_Edit::addLine(void)
{
	if (m_vecCursorPosition.m_x == m_vecTextLines[m_vecCursorPosition.m_y].length() && m_vecCursorPosition.m_y == (m_vecTextLines.size() - 1))
	{
		m_vecTextLines.push_back(string());
		m_vecCursorPosition.m_x = 0;
		m_vecCursorPosition.m_y++;
	}
	else
	{
		splitLine(m_vecCursorPosition);
	}
}

void		CWindowControl_Edit::mergeLines(CVector2ui32& vecLineIncdices)
{
	m_vecTextLines[vecLineIncdices.m_x] += m_vecTextLines[vecLineIncdices.m_y];
	m_vecTextLines.erase(std::find(m_vecTextLines.begin(), m_vecTextLines.end(), m_vecTextLines[vecLineIncdices.m_y]));
}

void		CWindowControl_Edit::splitLine(CVector2ui32& vecCharacterPosition)
{
	m_vecTextLines.insert(m_vecTextLines.begin() + vecCharacterPosition.m_y + 1, m_vecTextLines[vecCharacterPosition.m_y].substr(vecCharacterPosition.m_x));
	m_vecTextLines[vecCharacterPosition.m_y] = m_vecTextLines[vecCharacterPosition.m_y].substr(0, vecCharacterPosition.m_x);
}

// characters
void		CWindowControl_Edit::addCharacter(uint32 uiCharCode)
{
	if (uiCharCode < 32 || uiCharCode > 126)
	{
		return;
	}

	string strChar = string(1, uiCharCode);
	bool
		bCapsLockIsOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0,
		bhiftIsDown = (GetKeyState(VK_SHIFT) & 0x8000) == 0x8000,
		bIsUpperCase = (bCapsLockIsOn && !bhiftIsDown) || (!bCapsLockIsOn && bhiftIsDown);
	if (bIsUpperCase)
	{
		strChar = CStringUtility::toUpperCase(strChar);
	}
	else
	{
		strChar = CStringUtility::toLowerCase(strChar);
	}

	if (m_vecTextLines.size() == 0)
	{
		m_vecTextLines.push_back(strChar);
		m_vecCursorPosition.m_x++;
	}
	else if (m_vecCursorPosition.m_x == m_vecTextLines[m_vecCursorPosition.m_y].length())
	{
		m_vecTextLines[m_vecCursorPosition.m_y] += strChar;
		m_vecCursorPosition.m_x++;
	}
	else
	{
		// todo
		// m_vecTextLines[m_vecCursorPosition.m_y] (getText() + string(1, uiCharCode));
	}
}

void		CWindowControl_Edit::removeCharacterToLeft(void)
{
	if (m_vecCursorPosition.m_x != 0)
	{
		removeCharacter(CVector2ui32(m_vecCursorPosition.m_x - 1, m_vecCursorPosition.m_y));
		m_vecCursorPosition.m_x--;
	}
	else if (m_vecCursorPosition.m_y != 0)
	{
		mergeLines(CVector2ui32(m_vecCursorPosition.m_y - 1, m_vecCursorPosition.m_y));
		m_vecCursorPosition.m_y--;
		m_vecCursorPosition.m_x = m_vecTextLines[m_vecCursorPosition.m_y].length();
	}
}

void		CWindowControl_Edit::removeCharacterToRight(void)
{
	if (m_vecCursorPosition.m_x != m_vecTextLines[m_vecCursorPosition.m_y].length())
	{
		removeCharacter(CVector2ui32(m_vecCursorPosition.m_x, m_vecCursorPosition.m_y));
	}
	else if (m_vecCursorPosition.m_y != (m_vecTextLines.size() - 1))
	{
		mergeLines(CVector2ui32(m_vecCursorPosition.m_y, m_vecCursorPosition.m_y + 1));
	}
}

void		CWindowControl_Edit::removeCharacter(CVector2ui32& vecCharacterPosition)
{
	m_vecTextLines[vecCharacterPosition.m_y] = m_vecTextLines[vecCharacterPosition.m_y].substr(0, vecCharacterPosition.m_x) + m_vecTextLines[vecCharacterPosition.m_y].substr(vecCharacterPosition.m_x + 1);
}

// cursor
void		CWindowControl_Edit::moveCursor(CVector2ui32& vecCursorPositionToMove)
{
	int32
		iNewLineIndex = m_vecCursorPosition.m_y + vecCursorPositionToMove.m_y,
		iNewCharIndex = m_vecCursorPosition.m_x + vecCursorPositionToMove.m_x;
	uint32
		uiMaxLineIndex = m_vecTextLines.size() - 1,
		uiMaxCharIndex = m_vecTextLines[m_vecCursorPosition.m_y].length();
	if (iNewLineIndex < 0)
	{
		iNewLineIndex = 0;
	}
	else if (iNewLineIndex > uiMaxLineIndex) // todo - use CMathUtility::capNumber(n, min, max)
	{
		iNewLineIndex = uiMaxLineIndex;
	}
	if (iNewCharIndex < 0)
	{
		iNewCharIndex = 0;
	}
	else if (iNewCharIndex > uiMaxCharIndex) // todo - use CMathUtility::capNumber(n, min, max)
	{
		iNewCharIndex = uiMaxCharIndex;
	}
	m_vecCursorPosition = CVector2ui32(iNewCharIndex, iNewLineIndex);
}