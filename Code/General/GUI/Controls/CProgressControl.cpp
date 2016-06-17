#include "CProgressControl.h"
#include "GUI/Window/CWindow.h"
#include "String/CStringUtility.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

auto pOnRender_Progress		= [](void *pControl) { ((CProgressControl*) pControl)->render(); };

CProgressControl::CProgressControl(void) :
	CGUIControl(GUI_CONTROL_PROGRESS),
	m_fProgress(0.0f),
	m_bCompletionPercentageShown(false)
{
}

// event binding
void			CProgressControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Progress, this)); // todo - change to like CGUIControl::bindEvent(eEventId, func) which stores it
}

// serialization
void			CProgressControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIControl::unserialize(bSkipControlId);
	setProgress(pDataReader->readFloat32()); // progress ratio
	setCompletionPercentageShown(pDataReader->readUint8() ? true : false); // completion text shown
}

void			CProgressControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIControl::serialize();
	pDataWriter->writeFloat32(getProgress()); // progress ratio
	pDataWriter->writeUint8(isCompletionPercentageShown() ? 1 : 0); // completion text shown
}

// render
void			CProgressControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	// item component: default
	pGFX->drawRectangle(getPosition(), getSize(), getStyles());

	// item component: seek-bar
	CSize2D vecSeekBarSize = getSeekBarSize(); // todo
	getStyles()->setItemComponent("seek-bar");
	pGFX->drawRectangle(getPosition(), vecSeekBarSize, getStyles());

	// item component: default
	if (isCompletionPercentageShown())
	{
		checkToRecalculateStringSize(getStyles());

		getStyles()->resetItemComponent();
		pGFX->drawText(getPosition(), getSize(), getProgressPercentText(), getStyles());
	}

	getStyles()->restoreTemporaryStyleData();
}

// seek bar
CSize2D			CProgressControl::getSeekBarSize(void)
{
	return CSize2D(getSeekBarSizeWidth(), getSize().m_y);
}

uint32			CProgressControl::getSeekBarSizeWidth(void)
{
	return ((float32) getSize().m_x) * getProgress();
}

// text
string			CProgressControl::getProgressPercentText(void)
{
	return CStringUtility::toString(getProgress() * 100.0f) + "%";
}