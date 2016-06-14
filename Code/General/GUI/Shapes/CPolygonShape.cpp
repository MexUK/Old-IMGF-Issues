#include "CPolygonShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Math/CMathUtility.h"

auto pOnRender_Polygon		= [](void *pShape, void *pTriggerArg) { ((CPolygonShape*) pShape)->render(); };

CPolygonShape::CPolygonShape(void) :
	CGUIShapeGeometry_NxPoints(GUI_SHAPE_POLYGON)
{
}

// event binding
void							CPolygonShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Polygon, this));
}

// render
void							CPolygonShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawPolygon(getPoints(), getStyles());
}

// point testing
bool							CPolygonShape::isPointInItem(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInPolygon(vecPoint, getPoints());
}

// bounding rectangle
CVector2i32						CPolygonShape::getBoundingRectanglePosition(void)
{
	return CMathUtility::getBoundingRectanglePositionForPolygon(getPoints());
}

CVector2ui32					CPolygonShape::getBoundingRectangleSize(void)
{
	return CMathUtility::getBoundingRectangleSizeForPolygon(getPoints());
}