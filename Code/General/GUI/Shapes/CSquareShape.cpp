#include "CSquareShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Math/CMathUtility.h"

auto pOnRender_Square	= [](void *pShape, void *pTriggerArg) { ((CSquareShape*) pShape)->render(); };

CSquareShape::CSquareShape(void) :
	CGUIShapeGeometry_1xPoint_1x1DSize(GUI_SHAPE_SQUARE)
{
}

// event binding
void							CSquareShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Square, this));
}

// render
void							CSquareShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawSquare(getPosition(), getSize(), getStyles());
}

// point testing
bool							CSquareShape::isPointInItem(CVector2i32& vecPoint)
{
	return CMathUtility::isPointInRectangle(vecPoint, getPosition(), CVector2ui32(getSize(), getSize()));
}

// bounding rectangle
CVector2i32						CSquareShape::getBoundingRectanglePosition(void)
{
	return getPosition();
}

CVector2ui32					CSquareShape::getBoundingRectangleSize(void)
{
	return CVector2ui32(getSize(), getSize());
}