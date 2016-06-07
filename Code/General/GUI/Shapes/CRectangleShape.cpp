#include "CRectangleShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnRender_Rectangle = [](void *pShape, void *pTriggerArg) { ((CRectangleShape*) pShape)->render(); };

CRectangleShape::CRectangleShape(void) :
	CGUIShape(GUI_SHAPE_RECTANGLE)
{
}

CRectangleShape::CRectangleShape(eGUIShape eShapeType) :
	CGUIShape(eShapeType)
{
}

// event binding
void					CRectangleShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Rectangle, this));
}

// render
void					CRectangleShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
}