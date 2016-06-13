#include "CCircleShape.h"
#include "GUI/Shape/CGUIShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Math/CMathUtility.h"

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

// point testing
bool							CCircleShape::isPointInItem(CVector2i32& vecPoint)
{
	return CMathUtility::getDistanceBetweenPoints(getCenterPosition(), vecPoint) <= getSize();
}

// bounding rectangle
CVector2i32						CCircleShape::getBoundingRectanglePosition(void)
{
	return getPosition();
}

CVector2ui32					CCircleShape::getBoundingRectangleSize(void)
{
	float32 fDiameter = getSize() * 2.0f;
	return CVector2ui32(fDiameter, fDiameter);
}