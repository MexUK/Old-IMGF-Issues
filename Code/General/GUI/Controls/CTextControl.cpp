#include "CTextControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

auto pOnRender_Text			= [](void *pControl) { ((CTextControl*) pControl)->render(); };

CTextControl::CTextControl(void) :
	CGUIControl(GUI_CONTROL_TEXT)
{
}

// event binding
void					CTextControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Text, this));
}

// serialization
void					CTextControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CGUIControl::unserialize(bSkipControlId);
	setText(pDataReader->readStringWithLength()); // text
}

void					CTextControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CGUIControl::serialize();
	pDataWriter->writeStringWithLengthRef(getText()); // text
}

// render
void					CTextControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	checkToRecalculateStringSize(getStyles());

	pGFX->drawText(getPosition(), getSize(), getText(), getStyles());
}