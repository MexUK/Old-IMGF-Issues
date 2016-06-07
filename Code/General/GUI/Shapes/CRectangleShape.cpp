#include "CRectangleShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnRender_Rectangle	= [](void *pShape, void *pTriggerArg) { ((CRectangleShape*) pShape)->render(); };

CRectangleShape::CRectangleShape(void) :
	CGUIShapeGeometry_1xPoint_1x2DSize(GUI_SHAPE_RECTANGLE)
{
}

// event binding
void							CRectangleShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Rectangle, this));
}

// render
void							CRectangleShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawRectangle(getPosition(), getSize(), getStyles());
}