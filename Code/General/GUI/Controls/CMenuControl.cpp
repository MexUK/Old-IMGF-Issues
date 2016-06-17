#include "CMenuControl.h"
#include "GUI/Window/CWindow.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

auto pOnRender_Menu		= [](void *pControl) { ((CMenuControl*) pControl)->render(); };

CMenuControl::CMenuControl(void) :
	CGUIControl(GUI_CONTROL_MENU)
{
}

// event binding
void					CMenuControl::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Menu, this));
}

// serialization
void					CMenuControl::unserialize(bool bSkipControlId)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// todo
}

void					CMenuControl::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// todo
}

// render
void					CMenuControl::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();


}