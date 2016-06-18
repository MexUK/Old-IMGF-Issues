#include "CEditControl.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "GUI/CGUIUtility.h"
#include "Math/CMathUtility.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

auto pOnKeyDown_Edit		= [](void *pControl, void *pTriggerArg) { ((CEditControl*) pControl)->onKeyDown(*(uint8*) pTriggerArg); };
auto pOnCharDown_Edit		= [](void *pControl, void *pTriggerArg) { ((CEditControl*) pControl)->onCharDown(*(uint8*) pTriggerArg); };
auto pOnRender_Edit			= [](void *pControl) { ((CEditControl*) pControl)->render(); };

CEditControl::CEditControl(void) :
	CGUIControl(GUI_CONTROL_EDIT),
	m_bHasHorizontalScrollBar(false),
	m_bHasVerticalScrollBar(false),
	m_bReadOnly(false)
{
	m_vecTextLines.push_back(std::string()); // always have atleast 1 line for optimization (skips checks like: if m_vecTextLines.size() == 0)
	setCaretPosition(CVector2ui32(0, 0));
	getScrolls()->addEntry(new CScrollControl);
	getScrolls()->addEntry(new CScrollControl);
}
CEditControl::~CEditControl(void)
{
	getScrolls()->removeAllEntries();
}

// event binding
void					CEditControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onKeyDown, pOnKeyDown_Edit, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onCharacterDown, pOnCharDown_Edit, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Edit, this));
}

// serialization
void					CEditControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIControl::unserialize(bSkipControlId);
	setMultiLine(pDataReader->readUint8() ? true : false);
	setReadOnly(pDataReader->readUint8() ? true : false);
	setCaretPositionStart(pDataReader->readVector2ui32());
	setCaretPositionEnd(pDataReader->readVector2ui32());
	uint32 uiLineCount;
	if (isMultiLine())
	{
		uiLineCount = pDataReader->readUint32();
	}
	else
	{
		uiLineCount = 1;
	}
	getTextLines().resize(uiLineCount);
	for (uint32 i = 0; i < uiLineCount; i++)
	{
		getTextLines()[i] = pDataReader->readStringWithLength();
	}
}

void					CEditControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIControl::serialize();
	pDataWriter->writeUint8(isMultiLine() ? 1 : 0); // multi line status
	pDataWriter->writeUint8(isReadOnly() ? 1 : 0); // read only status
	pDataWriter->writeVector2ui32(getCaretPositionStart()); // caret position start
	pDataWriter->writeVector2ui32(getCaretPositionEnd()); // caret position end
	if (isMultiLine())
	{
		pDataWriter->writeUint32(getLineCount()); // line count
	}
	for (string& strLineText : getTextLines())
	{
		pDataWriter->writeStringWithLengthRef(strLineText); // line text
	}
}

// input
void				CEditControl::onKeyDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processKey(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

void				CEditControl::onCharDown(uint8 uiCharCode)
{
	if (doesControlHaveFocus())
	{
		processChar(uiCharCode);
		getWindow()->setMarkedToRedraw(true);
	}
}

// render
void				CEditControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	// fill and border
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());

	// caret
	if (!isReadOnly())
	{
		getStyles()->setItemComponent("edit-caret");
		pGFX->drawLine(getCaretRenderStartPosition(), getCaretRenderEndPosition(), getStyles());
	}

	// text lines
	uint32 uiLineIndex = 0;
	for (string& strTextLine : getTextLines())
	{
		if (isTextSelected() && uiLineIndex >= getCaretPositionStartY() && uiLineIndex <= getCaretPositionEndY())
		{
			getStyles()->setItemComponent("edit-selected-text");
			uint32
				uiSelectedTextLineStartCharIndex = m_vecCaretPositionStart.m_y == m_vecCaretPositionEnd.m_y ? m_vecCaretPositionStart.m_x : 0,
				uiSelectedTextLineLength = m_vecCaretPositionStart.m_y == m_vecCaretPositionEnd.m_y ? ((m_vecCaretPositionEnd.m_x - m_vecCaretPositionStart.m_x) + 1) : m_vecTextLines[uiLineIndex].length();
			CPoint2D vecSelectedTextLinePosition(pGFX->getTextSize(strTextLine.substr(0, uiSelectedTextLineStartCharIndex), getStyles()).m_x, uiLineIndex * getStyles()->getStyle<uint32>("text-size"));
			CSize2D vecSelectedTextLineSize(pGFX->getTextSize(strTextLine.substr(uiSelectedTextLineStartCharIndex, uiSelectedTextLineLength), getStyles()));
			pGFX->drawRectangle(getPosition() + vecSelectedTextLinePosition, vecSelectedTextLineSize, getStyles());
		}

		getStyles()->setItemComponent("edit-line");
		pGFX->drawText(getTextLinePosition(uiLineIndex), getSize(), strTextLine, getStyles());
		uiLineIndex++;
	}
}

// input processing
void				CEditControl::processChar(uint32 uiCharCode)
{
	if (CGUIUtility::isControlKeyDown())
	{
		processMetaCharCombination(uiCharCode);
	}
	else
	{
		if (isTextSelected())
		{
			removeSelectedText();
		}
		addCharacter(uiCharCode);
	}
}

void				CEditControl::processKey(uint32 uiCharCode)
{
	switch (uiCharCode)
	{
	case VK_RETURN:
		if (isTextSelected())
		{
			removeSelectedText();
		}
		addLine();
		break;
	case VK_BACK:
		if (isTextSelected())
		{
			removeSelectedText();
		}
		else
		{
			removeCharacterToLeft();
		}
		break;
	case VK_DELETE:
		if (isTextSelected())
		{
			removeSelectedText();
		}
		else
		{
			removeCharacterToRight();
		}
		break;
	case VK_UP:
		moveCaret(CVector2i32(0, -1));
		break;
	case VK_DOWN:
		moveCaret(CVector2i32(0, 1));
		break;
	case VK_LEFT:
		moveCaretLeft(1);
		break;
	case VK_RIGHT:
		moveCaretRight(1);
		break;
	case VK_PRIOR:
		moveCaret(CVector2i32(0, -20));
		break;
	case VK_NEXT:
		moveCaret(CVector2i32(0, 20));
		break;
	case VK_HOME:
		if (CGUIUtility::isControlKeyDown())
		{
			setCaretPosition(CVector2ui32(0, 0));
		}
		else if (CGUIUtility::isShiftKeyDown())
		{
			setCaretPositionStartX(0);
		}
		else
		{
			setCaretPositionX(0);
		}
		break;
	case VK_END:
		if (CGUIUtility::isControlKeyDown())
		{
			setCaretPosition(CVector2ui32(getLineLength(getLineCount() - 1), getLineCount() - 1));
		}
		else if (CGUIUtility::isShiftKeyDown())
		{
			setCaretPositionEndX(getLineLength(getCaretPositionStartY()));
		}
		else
		{
			setCaretPositionX(getLineLength(getCaretPositionStartY()));
		}
		break;
	}
}

void				CEditControl::processMetaCharCombination(uint32 uiCharCode)
{
	if (CGUIUtility::isControlKeyDown())
	{
		switch (uiCharCode)
		{
		case 1: // control + a
			selectAllText();
			break;
		case 3: // control + c
			copySelectedText();
			break;
		case 22: // control + v
			pasteText();
			break;
		case 24: // control + x
			cutSelectedText();
			break;
		}
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
		moveCaretY(1);
	}
}

void				CEditControl::addLine(uint32 uiLineIndex, string& strText)
{
	m_vecTextLines.insert(m_vecTextLines.begin() + uiLineIndex, strText);
}

void				CEditControl::removeLine(uint32 uiLineIndex)
{
	m_vecTextLines.erase(m_vecTextLines.begin() + uiLineIndex);
}

void				CEditControl::mergeLines(uint32 uiRowIndex1, uint32 uiRowIndex2)
{
	uiRowIndex1 = min(uiRowIndex1, uiRowIndex2);
	uiRowIndex2 = max(uiRowIndex1, uiRowIndex2);

	addTextToLine(uiRowIndex1, getLineText(uiRowIndex2));
	removeLine(uiRowIndex2);
}

void				CEditControl::splitLine(CVector2ui32& vecCharacterPosition)
{
	addLine(vecCharacterPosition.m_y + 1, getLinePartialText(vecCharacterPosition.m_y, vecCharacterPosition.m_x));
	setLineText(vecCharacterPosition.m_y, getLinePartialText(vecCharacterPosition.m_y, 0, vecCharacterPosition.m_x));
}

// characters
void				CEditControl::addCharacter(uint32 uiCharCode)
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

void				CEditControl::removeCharacterToLeft(void)
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

void				CEditControl::removeCharacterToRight(void)
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

void				CEditControl::removeCharacter(CVector2ui32& vecCharacterPosition)
{
	setLineText(vecCharacterPosition.m_y,
		  getLinePartialText(vecCharacterPosition.m_y, 0, vecCharacterPosition.m_x)
		+ getLinePartialText(vecCharacterPosition.m_y, vecCharacterPosition.m_x + 1)
	);
}

// caret position
void				CEditControl::moveCaret(CVector2i32& vecCharacterPositionIncrease)
{
	CVector2ui32 vecNewCaretPosition = getCaretPositionStart() + vecCharacterPositionIncrease;
	vecNewCaretPosition.m_y = CMathUtility::cap(vecNewCaretPosition.m_y, 0, getLineCount() - 1);
	vecNewCaretPosition.m_x = CMathUtility::cap(vecNewCaretPosition.m_x, 0, getLineLength(vecNewCaretPosition.m_y));
	setCaretPosition(vecNewCaretPosition);
}

void				CEditControl::moveCaretLeft(uint32 uiCharacterPositionIncrease)
{
	if (getCaretPositionStartX() != 0)
	{
		setCaretPositionX(getCaretPosition().m_x - 1);
	}
	else if (getCaretPositionStartY() != 0)
	{
		setCaretPosition(CVector2ui32(getLineLength(getCaretPosition().m_y - 1), getCaretPosition().m_y - 1));
	}
}

void				CEditControl::moveCaretRight(uint32 uiCharacterPositionIncrease)
{
	if (getCaretPositionEndX() != getLineLength(getCaretPositionEndY()))
	{
		setCaretPositionX(getCaretPosition().m_x + 1);
	}
	else if (getCaretPositionEndY() != (getLineCount() - 1))
	{
		setCaretPosition(CVector2ui32(0, getCaretPosition().m_y + 1));
	}
}

bool				CEditControl::isCaretAtFarLeft(void)
{
	return getCaretPositionStartX() == 0;
}

bool				CEditControl::isCaretAtFarRight(void)
{
	return getCaretPositionEndX() == getLineLength(getCaretPositionEndY());
}

bool				CEditControl::isCaretAtFarTop(void)
{
	return getCaretPositionStartX() == 0;
}

bool				CEditControl::isCaretAtFarBottom(void)
{
	return getCaretPositionEndY() == (getLineCount() - 1);
}

void				CEditControl::setCaretPosition(CVector2ui32& vecCaretPosition)
{
	m_vecCaretPositionStart = vecCaretPosition;
	m_vecCaretPositionEnd = vecCaretPosition;
}

void				CEditControl::setCaretPositionX(uint32 uiCharacterPositionX)
{
	m_vecCaretPositionStart.m_x = uiCharacterPositionX;
	m_vecCaretPositionEnd.m_x = uiCharacterPositionX;
}

void				CEditControl::setCaretPositionY(uint32 uiCharacterPositionY)
{
	m_vecCaretPositionStart.m_y = uiCharacterPositionY;
	m_vecCaretPositionEnd.m_y = uiCharacterPositionY;
}

// selected text
bool				CEditControl::isTextSelected(void)
{
	return (m_vecCaretPositionStart.m_x != m_vecCaretPositionEnd.m_x) || (m_vecCaretPositionStart.m_y != m_vecCaretPositionEnd.m_y);
}

void				CEditControl::selectAllText(void)
{
	setCaretPositionStart(CVector2ui32(0, 0));
	setCaretPositionEnd(CVector2ui32(getLineCount() - 1, getLineLength(getLineCount() - 1)));
}

string				CEditControl::getSelectedText(void)
{
	return getTextRange(getCaretPositionStart(), getCaretPositionEnd());
}

// text fetching
string				CEditControl::getTextRange(CVector2ui32& vecRangeStart, CVector2ui32& vecRangeEnd)
{
	uint32
		uiRangeRowCount = vecRangeEnd.m_y - vecRangeStart.m_y,
		uiLineIndex = 0;
	bool
		bMultipleLinesSelected = uiRangeRowCount > 1;
	vector<string>
		vecSelectedLines;
	vecSelectedLines.resize(uiRangeRowCount);

	// first line
	vecSelectedLines[uiLineIndex] = getLineTextCharacters(CVector2ui32(vecRangeStart.m_x, vecRangeStart.m_y), bMultipleLinesSelected ? getLineMaxCharacterIndex(vecRangeStart.m_y) : (vecRangeStart.m_x - 1));

	// middle lines
	if (uiRangeRowCount > 2)
	{
		for (int32 iLineIndex = (vecRangeEnd.m_y - 1); iLineIndex > 0; iLineIndex--)
		{
			vecSelectedLines[uiLineIndex++] = getLineText(iLineIndex);
		}
	}

	// last line
	if (bMultipleLinesSelected)
	{
		vecSelectedLines[uiLineIndex++] = getLineTextCharacters(CVector2ui32(0, vecRangeEnd.m_y), vecRangeEnd.m_x);
	}

	return CStringUtility::join(vecSelectedLines, "\n");
}

string				CEditControl::getLineTextCharacters(CVector2ui32& vecRangeStart, uint32 uiLineCharacterEnd)
{
	string& strLine = m_vecTextLines[vecRangeStart.m_y];
	return strLine.substr(vecRangeStart.m_x, (uiLineCharacterEnd - vecRangeStart.m_x) + 1);
}

// add text
void				CEditControl::addTextAtCaret(string& strData)
{
	addTextAtRange(strData, getCaretPositionStart(), getCaretPositionEnd());
}

void				CEditControl::addTextAtRange(string& strData, CVector2ui32& vecRangeStart, CVector2ui32& vecRangeEnd)
{
	vector<string> vecNewTextLines = CStringUtility::split(strData, "\n");

	uint32
		uiRangeRowCount = vecRangeEnd.m_y - vecRangeStart.m_y,
		uiLineIndex = 0;
	bool
		bMultipleLinesSelected = uiRangeRowCount > 1;
	vector<string>
		vecSelectedLines;
	vecSelectedLines.resize(uiRangeRowCount);
	string
		strFirstLineLastPart = m_vecTextLines[vecRangeStart.m_y].substr(vecRangeStart.m_x);

	// last line
	if (bMultipleLinesSelected)
	{
		addLineText(vecRangeEnd, vecNewTextLines[vecNewTextLines.size() - 1] + strFirstLineLastPart);
	}

	// middle lines
	if (uiRangeRowCount > 2)
	{
		for (int32 iLineIndex = (vecRangeEnd.m_y - 1); iLineIndex > 0; iLineIndex--)
		{
			addLine(vecRangeEnd.m_y - iLineIndex, vecNewTextLines[(vecNewTextLines.size() - 1) - iLineIndex]);
		}
	}

	// first line
	setLineText(vecRangeStart.m_y, getLineText(vecRangeStart.m_y).substr(0, vecRangeStart.m_x) + vecNewTextLines[0]);
}

void				CEditControl::addLineText(CVector2ui32& vecCharacterPosition, string& strData)
{
	string& strLine = m_vecTextLines[vecCharacterPosition.m_y];
	strLine = strLine.substr(0, vecCharacterPosition.m_x)
			+ strData
			+ strLine.substr(vecCharacterPosition.m_x);
}

// cut/copy/paste
void				CEditControl::copySelectedText(void)
{
	CStringUtility::setClipboardText(getSelectedText());
}

void				CEditControl::cutSelectedText(void)
{
	CStringUtility::setClipboardText(getSelectedText());
	removeSelectedText();
}

void				CEditControl::pasteText(void)
{
	removeSelectedText();
	addTextAtCaret(CStringUtility::getClipboardText());
}

// caret render position
CPoint2D			CEditControl::getCaretRenderStartPosition(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	CSize2D vecTextSize = pGFX->getTextSize(getLineText(getCaretPosition().m_y).substr(0, getCaretPosition().m_x), getStyles()); // todo - dont calculate text size on each render call
	return getPosition() + CVector2i32(vecTextSize.m_x, getCaretPosition().m_y * vecTextSize.m_y);
}

CPoint2D			CEditControl::getCaretRenderEndPosition(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	CSize2D vecTextSize = pGFX->getTextSize(getLineText(getCaretPosition().m_y).substr(0, getCaretPosition().m_x), getStyles()); // todo - dont calculate text size on each render call
	return getPosition() + CVector2i32(vecTextSize.m_x, ((getCaretPosition().m_y + 1) * vecTextSize.m_y));
}

// text
CPoint2D			CEditControl::getTextLinePosition(uint32 uiLineIndex) // todo - rename function to have Render in function name?
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

void				CEditControl::removeSelectedText(void)
{
	removeTextRange(getCaretPositionStart(), getCaretPositionEnd());
}

void				CEditControl::removeTextRange(CVector2ui32& vecRangeStart, CVector2ui32& vecRangeEnd)
{
	uint32 uiSelectedRowCount = vecRangeEnd.m_y - vecRangeStart.m_y;
	bool bMultipleLinesSelected = uiSelectedRowCount > 1;

	// last line
	if (bMultipleLinesSelected)
	{
		removeLineTextCharacters(CVector2ui32(0, vecRangeEnd.m_y), vecRangeEnd.m_x);
	}

	// middle lines
	if (uiSelectedRowCount > 2)
	{
		for (int32 iLineIndex = (vecRangeEnd.m_y - 1); iLineIndex > 0; iLineIndex--)
		{
			removeLine(iLineIndex);
		}
	}

	// first line
	removeLineTextCharacters(CVector2ui32(vecRangeStart.m_x, vecRangeStart.m_y), bMultipleLinesSelected ? getLineMaxCharacterIndex(vecRangeStart.m_y) : (vecRangeStart.m_x - 1));
}

void				CEditControl::removeLineTextCharacters(CVector2ui32& vecRangeStart, uint32 uiLineCharacterEnd)
{
	string& strLine = m_vecTextLines[vecRangeStart.m_y];
	strLine = strLine.substr(0, vecRangeStart.m_x) + ((uiLineCharacterEnd + 1) < strLine.length() ? strLine.substr(uiLineCharacterEnd + 1) : "");
}

uint32				CEditControl::getLineMaxCharacterIndex(uint32 uiLineIndex)
{
	return m_vecTextLines[uiLineIndex].size();
}