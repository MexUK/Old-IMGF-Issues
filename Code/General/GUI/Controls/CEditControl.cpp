#include "CEditControl.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "GUI/CGUIUtility.h"
#include "Math/CMathUtility.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

using namespace std;

auto pOnKeyDown_Edit		= [](void *pControl, void *pTriggerArg) { ((CEditControl*) pControl)->onKeyDown(*(uint8*) pTriggerArg); };
auto pOnCharDown_Edit		= [](void *pControl, void *pTriggerArg) { ((CEditControl*) pControl)->onCharDown(*(uint8*) pTriggerArg); };
auto pOnRender_Edit			= [](void *pControl) { ((CEditControl*) pControl)->render(); };

// event binding
void					CEditControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onKeyDown, pOnKeyDown_Edit, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onCharacterDown, pOnCharDown_Edit, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Edit, this));
}

// input
void		CEditControl::onKeyDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processKey(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

void		CEditControl::onCharDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processChar(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

// render
void		CEditControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	// fill and border
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());

	// caret
	if (!isReadOnly())
	{
		pGFX->drawLine(getCaretRenderStartPosition(), getCaretRenderEndPosition(), getStyles());
	}

	// text lines
	uint32 uiLineIndex = 0;
	for (string& strTextLine : getTextLines())
	{
		pGFX->drawText(getTextLinePosition(uiLineIndex), getSize(), strTextLine, getStyles());
		uiLineIndex++;
	}
}

// input processing
void		CEditControl::processChar(uint32 uiCharCode)
{
	addCharacter(uiCharCode);
}

void		CEditControl::processKey(uint32 uiCharCode)
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
uint32				CEditControl::getLineCount(void)
{
	return m_vecTextLines.size();
}

void				CEditControl::addLine(void)
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

void				CEditControl::addLine(uint32 uiLineIndex, string& strText)
{
	m_vecTextLines.insert(m_vecTextLines.begin() + uiLineIndex, strText);
}

void		CEditControl::removeLine(uint32 uiLineIndex)
{
	m_vecTextLines.erase(m_vecTextLines.begin() + uiLineIndex);
}

void		CEditControl::mergeLines(uint32 uiRowIndex1, uint32 uiRowIndex2)
{
	uiRowIndex1 = min(uiRowIndex1, uiRowIndex2);
	uiRowIndex2 = max(uiRowIndex1, uiRowIndex2);

	addTextToLine(uiRowIndex1, getLineText(uiRowIndex2));
	removeLine(uiRowIndex2);
}

void		CEditControl::splitLine(CVector2ui32& vecCharacterPosition)
{
	addLine(vecCharacterPosition.m_y + 1, getLinePartialText(vecCharacterPosition.m_y, vecCharacterPosition.m_x));
	setLineText(vecCharacterPosition.m_y, getLinePartialText(vecCharacterPosition.m_y, 0, vecCharacterPosition.m_x));
}

// characters
void		CEditControl::addCharacter(uint32 uiCharCode)
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

void		CEditControl::removeCharacterToLeft(void)
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

void		CEditControl::removeCharacterToRight(void)
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

void		CEditControl::removeCharacter(CVector2ui32& vecCharacterPosition)
{
	setLineText(vecCharacterPosition.m_y,
		  getLinePartialText(vecCharacterPosition.m_y, 0, vecCharacterPosition.m_x)
		+ getLinePartialText(vecCharacterPosition.m_y, vecCharacterPosition.m_x + 1)
	);
}

// caret character position
void		CEditControl::moveCaret(CVector2i32& vecCharacterPositionIncrease)
{
	CVector2ui32 vecNewCaretPosition = getCaretPosition() + vecCharacterPositionIncrease;
	vecNewCaretPosition.m_y = CMathUtility::cap(vecNewCaretPosition.m_y, 0, getLineCount() - 1);
	vecNewCaretPosition.m_x = CMathUtility::cap(vecNewCaretPosition.m_x, 0, getLineLength(vecNewCaretPosition.m_y));
	setCaretPosition(vecNewCaretPosition);
}

bool		CEditControl::isCaretAtFarLeft(void)
{
	return getCaretPosition().m_x == 0;
}

bool		CEditControl::isCaretAtFarRight(void)
{
	return getCaretPosition().m_x == getLineLength(getCaretPosition().m_y);
}

bool		CEditControl::isCaretAtFarTop(void)
{
	return getCaretPosition().m_y == 0;
}

bool		CEditControl::isCaretAtFarBottom(void)
{
	return getCaretPosition().m_y == getLineCount() - 1;
}

// caret render position
CVector2i32			CEditControl::getCaretRenderStartPosition(void)
{
	return getPosition() + CVector2i32(getCaretPosition().m_x * 10, getCaretPosition().m_y * getStyles()->getStyle<uint32>("text-size"));
}

CVector2i32			CEditControl::getCaretRenderEndPosition(void)
{
	return getPosition() + CVector2i32(getCaretPosition().m_x * 10, ((getCaretPosition().m_y + 1) * getStyles()->getStyle<uint32>("text-size")));
}

// text
CVector2i32			CEditControl::getTextLinePosition(uint32 uiLineIndex)
{
	return getPosition() + CVector2i32(0, uiLineIndex * getStyles()->getStyle<uint32>("text-size"));
}

void				CEditControl::setLineText(uint32 uiLineIndex, string& strText)
{
	m_vecTextLines[uiLineIndex] = strText;
}

string&				CEditControl::getLineText(uint32 uiLineIndex)
{
	return m_vecTextLines[uiLineIndex];
}

void				CEditControl::addTextToLine(uint32 uiLineIndex, string& strText)
{
	m_vecTextLines[uiLineIndex] += strText;
}

string				CEditControl::getLinePartialText(uint32 uiLineIndex, uint32 uiCharStartIndex, uint32 uiCharReadLength)
{
	return m_vecTextLines[uiLineIndex].substr(uiCharStartIndex, uiCharReadLength);
}

uint32				CEditControl::getLineLength(uint32 uiLineIndex)
{
	return m_vecTextLines[uiLineIndex].length();
}