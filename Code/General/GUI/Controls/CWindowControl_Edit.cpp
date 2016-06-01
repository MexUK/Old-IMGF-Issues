#include "CWindowControl_Edit.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "GDIPlus/CGDIPlusUtility.h"
#include "GUI/CGUIUtility.h"
#include "Math/CMathUtility.h"
#include "Event/eEvent.h"

using namespace std;

auto pOnKeyDown_Edit		= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Edit*) pControl)->onKeyDown(*(uint8*) pTriggerArg); };
auto pOnCharDown_Edit		= [](void *pControl, void *pTriggerArg) { ((CWindowControl_Edit*) pControl)->onCharDown(*(uint8*) pTriggerArg); };
auto pOnRender_Edit			= [](void *pControl) { ((CWindowControl_Edit*) pControl)->render(); };

// event binding
void					CWindowControl_Edit::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onKeyDown, pOnKeyDown_Edit, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onCharDown, pOnCharDown_Edit, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Edit, this));
}

// input
void		CWindowControl_Edit::onKeyDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processKey(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

void		CWindowControl_Edit::onCharDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processChar(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

// render
void		CWindowControl_Edit::render(void)
{
	// fill and border
	CGDIPlusUtility::drawRectangleFill(getPosition(), getSize(), getFillColour());
	CGDIPlusUtility::drawRectangleBorder(getPosition(), getSize(), getLineColour());

	// caret
	if (!isReadOnly())
	{
		CGDIPlusUtility::drawLine(getCaretRenderStartPosition(), getCaretRenderEndPosition(), getCaretColour());
	}

	// text lines
	uint32 uiLineIndex = 0;
	for (string& strTextLine : getTextLines())
	{
		CGDIPlusUtility::drawText(getTextLinePosition(uiLineIndex), getSize(), strTextLine, getTextColour(), getFontSize(), isBold());
		uiLineIndex++;
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
		moveCaret(CVector2i32(0, -1));
		break;
	case VK_DOWN:
		moveCaret(CVector2i32(0, 1));
		break;
	case VK_LEFT:
		moveCaret(CVector2i32(-1, 0));
		break;
	case VK_RIGHT:
		moveCaret(CVector2i32(1, 0));
		break;
	case VK_PRIOR:
		moveCaret(CVector2i32(0, -20));
		break;
	case VK_NEXT:
		moveCaret(CVector2i32(0, 20));
		break;
	}
}

// lines
uint32				CWindowControl_Edit::getLineCount(void)
{
	return m_vecTextLines.size();
}

void				CWindowControl_Edit::addLine(void)
{
	if (isCaretAtFarRight() && isCaretAtFarBottom())
	{
		m_vecTextLines.push_back(string());
		setCaretPositionX(0);
		moveCaretY(1);
	}
	else
	{
		splitLine(getCaretPosition());
	}
}

void				CWindowControl_Edit::addLine(uint32 uiLineIndex, string& strText)
{
	m_vecTextLines.insert(m_vecTextLines.begin() + uiLineIndex, strText);
}

void		CWindowControl_Edit::removeLine(uint32 uiLineIndex)
{
	m_vecTextLines.erase(m_vecTextLines.begin() + uiLineIndex);
}

void		CWindowControl_Edit::mergeLines(uint32 uiRowIndex1, uint32 uiRowIndex2)
{
	uiRowIndex1 = min(uiRowIndex1, uiRowIndex2);
	uiRowIndex2 = max(uiRowIndex1, uiRowIndex2);

	addTextToLine(uiRowIndex1, getLineText(uiRowIndex2));
	removeLine(uiRowIndex2);
}

void		CWindowControl_Edit::splitLine(CVector2ui32& vecCharacterPosition)
{
	addLine(vecCharacterPosition.m_y + 1, getLinePartialText(vecCharacterPosition.m_y, vecCharacterPosition.m_x));
	setLineText(vecCharacterPosition.m_y, getLinePartialText(vecCharacterPosition.m_y, 0, vecCharacterPosition.m_x));
}

// characters
void		CWindowControl_Edit::addCharacter(uint32 uiCharCode)
{
	if(!CStringUtility::isAsciiCharacterDisplayable(uiCharCode))
	{
		return;
	}

	string strChar = CGUIUtility::getTextInInputCase(CStringUtility::createCharString(uiCharCode));

	if (isCaretAtFarRight())
	{
		addTextToLine(getCaretPosition().m_y, strChar);
		moveCaretX(1);
	}
	else
	{
		// todo
		// m_vecTextLines[getCaretPosition().m_y] (getText() + string(1, uiCharCode));
	}
}

void		CWindowControl_Edit::removeCharacterToLeft(void)
{
	if (!isCaretAtFarLeft())
	{
		removeCharacter(CVector2ui32(getCaretPosition().m_x - 1, getCaretPosition().m_y));
		moveCaretX(-1);
	}
	else if (!isCaretAtFarTop())
	{
		mergeLines(getCaretPosition().m_y - 1, getCaretPosition().m_y);
		moveCaretY(-1);
		setCaretPositionX(getLineLength(getCaretPosition().m_y));
	}
}

void		CWindowControl_Edit::removeCharacterToRight(void)
{
	if (!isCaretAtFarRight())
	{
		removeCharacter(getCaretPosition());
	}
	else if (!isCaretAtFarBottom())
	{
		mergeLines(getCaretPosition().m_y, getCaretPosition().m_y + 1);
	}
}

void		CWindowControl_Edit::removeCharacter(CVector2ui32& vecCharacterPosition)
{
	setLineText(vecCharacterPosition.m_y,
		  getLinePartialText(vecCharacterPosition.m_y, 0, vecCharacterPosition.m_x)
		+ getLinePartialText(vecCharacterPosition.m_y, vecCharacterPosition.m_x + 1)
	);
}

// caret character position
void		CWindowControl_Edit::moveCaret(CVector2i32& vecCharacterPositionIncrease)
{
	CVector2ui32 vecNewCaretPosition = getCaretPosition() + vecCharacterPositionIncrease;
	vecNewCaretPosition.m_y = CMathUtility::cap(vecNewCaretPosition.m_y, 0, getLineCount() - 1);
	vecNewCaretPosition.m_x = CMathUtility::cap(vecNewCaretPosition.m_x, 0, getLineLength(vecNewCaretPosition.m_y));
	setCaretPosition(vecNewCaretPosition);
}

bool		CWindowControl_Edit::isCaretAtFarLeft(void)
{
	return getCaretPosition().m_x == 0;
}

bool		CWindowControl_Edit::isCaretAtFarRight(void)
{
	return getCaretPosition().m_x == getLineLength(getCaretPosition().m_y);
}

bool		CWindowControl_Edit::isCaretAtFarTop(void)
{
	return getCaretPosition().m_y == 0;
}

bool		CWindowControl_Edit::isCaretAtFarBottom(void)
{
	return getCaretPosition().m_y == getLineCount() - 1;
}

// caret render position
CVector2ui32		CWindowControl_Edit::getCaretRenderStartPosition(void)
{
	return getPosition() + CVector2ui32(getCaretPosition().m_x * 10, getCaretPosition().m_y * getFontSize());
}

CVector2ui32		CWindowControl_Edit::getCaretRenderEndPosition(void)
{
	return getPosition() + CVector2ui32(getCaretPosition().m_x * 10, ((getCaretPosition().m_y + 1) * getFontSize()));
}

// text
CVector2ui32		CWindowControl_Edit::getTextLinePosition(uint32 uiLineIndex)
{
	return getPosition() + (uiLineIndex * getFontSize());
}

void				CWindowControl_Edit::setLineText(uint32 uiLineIndex, string& strText)
{
	m_vecTextLines[uiLineIndex] = strText;
}

string&				CWindowControl_Edit::getLineText(uint32 uiLineIndex)
{
	return m_vecTextLines[uiLineIndex];
}

void				CWindowControl_Edit::addTextToLine(uint32 uiLineIndex, string& strText)
{
	m_vecTextLines[uiLineIndex] += strText;
}

string				CWindowControl_Edit::getLinePartialText(uint32 uiLineIndex, uint32 uiCharStartIndex, uint32 uiCharReadLength)
{
	return m_vecTextLines[uiLineIndex].substr(uiCharStartIndex, uiCharReadLength);
}

uint32				CWindowControl_Edit::getLineLength(uint32 uiLineIndex)
{
	return m_vecTextLines[uiLineIndex].length();
}