#include "CPolygonShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

auto pOnRender_Polygon		= [](void *pShape, void *pTriggerArg) { ((CPolygonShape*) pShape)->render(); };

CPolygonShape::CPolygonShape(void) :
	CGUIShape(GUI_SHAPE_POLYGON)
{
}

CPolygonShape::CPolygonShape(eGUIShape eShapeType) :
	CGUIShape(eShapeType)
{
}

// event binding
void					CPolygonShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Polygon, this));
}

// render
void					CPolygonShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawPolygon(getPolygonPointPositions(), getStyles());
}