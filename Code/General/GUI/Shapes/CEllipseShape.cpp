#include "CEllipseShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnRender_Ellipse		= [](void *pShape, void *pTriggerArg) { ((CEllipseShape*) pShape)->render(); };

CEllipseShape::CEllipseShape(void) :
	CGUIShapeGeometry_1xPoint_1x2DSize(GUI_SHAPE_ELLIPSE)
{
}

// event binding
void							CEllipseShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Ellipse, this));
}

// render
void							CEllipseShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawEllipse(getPosition(), getSize(), getStyles());
}