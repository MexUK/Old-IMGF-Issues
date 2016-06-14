#include "CLineShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Math/CMathUtility.h"

auto pOnRender_Line		= [](void *pShape, void *pTriggerArg) { ((CLineShape*) pShape)->render(); };

CLineShape::CLineShape(void) :
	CGUIShapeGeometry_2xPoints(GUI_SHAPE_LINE)
{
}

// event binding
void							CLineShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Line, this));
}

// render
void							CLineShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawLine(getPoint1(), getPoint2(), getStyles());
}

// point testing
bool							CLineShape::isPointInItem(CVector2i32& vecPoint)
{
	return CMathUtility::isPointOnLine(vecPoint, getPoint1(), getPoint2());
}

// bounding rectangle
CVector2i32						CLineShape::getBoundingRectanglePosition(void)
{
	return CMathUtility::getBoundingRectanglePositionForLine(getPoint1(), getPoint2());
}

CVector2ui32					CLineShape::getBoundingRectangleSize(void)
{
	return CMathUtility::getBoundingRectangleSizeForLine(getPoint1(), getPoint2());
}