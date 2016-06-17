#include "CCheckControl.h"
#include "Math/CMathUtility.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Styles/CGUIStyles.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

auto pOnMouseUp_Check		= [](void *pControl, void *pTriggerArg) { ((CCheckControl*) pControl)->onMouseUp(*(CPoint2D*) pTriggerArg); };
auto pOnRender_Check		= [](void *pControl) { ((CCheckControl*) pControl)->render(); };

CCheckControl::CCheckControl(void) :
	CGUIControl(GUI_CONTROL_CHECK),
	m_bMarked(false)
{
}

// event binding
void						CCheckControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onLeftMouseUp, pOnMouseUp_Check, this));
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Check, this));
}

// serialization
void						CCheckControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIControl::unserialize(bSkipControlId);
	setText(pDataReader->readStringWithLength()); // check text
	setMarked(pDataReader->readUint8() ? true : false); // marked status
}

void						CCheckControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIControl::serialize();
	pDataWriter->writeStringWithLengthRef(getText()); // check text
	pDataWriter->writeUint8(isMarked() ? 1 : 0); // marked status
}

// input
void						CCheckControl::onMouseUp(CPoint2D& vecCursorPosition)
{
	if (isPointInControl(vecCursorPosition))
	{
		if (getWindow()->triggerEvent(EVENT_onMarkCheck, this))
		{
			setMarked(!isMarked());
			getWindow()->setMarkedToRedraw(true);
		}
	}
}

// render
void						CCheckControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	getStyles()->setItemStatus(isMarked() ? "marked" : "unmarked");

	checkToRecalculateStringSize(getStyles());

	// draw fill
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());

	// draw marked line
	if (isMarked())
	{
		pGFX->drawLine(CPoint2D(getPosition().m_x, getPosition().m_y + getSize().m_y), CPoint2D(getPosition().m_x + getSize().m_x, getPosition().m_y), getStyles());
	}

	// draw text
	pGFX->drawText(getTextPosition(), getTextSize(), getText(), getStyles());

	// reset
	getStyles()->restoreTemporaryStyleData();
}

// cursor
bool						CCheckControl::isPointInControl(CPoint2D& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), getTotalSize());
}

// size
CSize2D						CCheckControl::getTotalSize(void)
{
	return getSize() + CSize2D(getMarkableTextSpacing() + getTextWidth(), 0);
}

// text
CPoint2D					CCheckControl::getTextPosition(void)
{
	return CPoint2D(getPosition().m_x + getSize().m_x + getMarkableTextSpacing(), getPosition().m_y);
}

// styles
int32						CCheckControl::getMarkableTextSpacing(void)
{
	return getStyles()->getStyle<int32>("markable-text-spacing");
}