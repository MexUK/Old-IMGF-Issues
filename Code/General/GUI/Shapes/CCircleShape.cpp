#include "CCircleShape.h"
#include "GUI/Shape/CGUIShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnRender_Circle		= [](void *pShape, void *pTriggerArg) { ((CCircleShape*) pShape)->render(); };

CCircleShape::CCircleShape(void) :
	CGUIShapeGeometry_1xPoint_1x1DSize(GUI_SHAPE_CIRCLE)
{
}

// event binding
void							CCircleShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Circle, this));
}

// render
void							CCircleShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawCircle(getPosition(), getSize(), getStyles());
}