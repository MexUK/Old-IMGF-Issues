#include "CTriangleShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"
#include "Math/CMathUtility.h"

using namespace std;

auto pOnRender_Triangle		= [](void *pShape, void *pTriggerArg) { ((CTriangleShape*) pShape)->render(); };

CTriangleShape::CTriangleShape(void) :
	CGUIShapeGeometry_3xPoints(GUI_SHAPE_TRIANGLE)
{
}

// event binding
void							CTriangleShape::bindEvents(void)
{
	storeEventBoundFunction(getWindow()->bindEvent(EVENT_onRender, pOnRender_Triangle, this));
}

// render
void							CTriangleShape::render(void)
{
	CGraphicsLibrary *pGFX = CGUIManager::getInstance()->getGraphicsLibrary();

	pGFX->drawTriangle(getPoint1(), getPoint2(), getPoint3(), getStyles());
}

// point testing
bool							CTriangleShape::isPointInItem(CPoint2D& vecPoint)
{
	vector<CPoint2D> vecPoints; // todo - use wrapper
	vecPoints.resize(3);
	vecPoints[0] = getPoint1();
	vecPoints[1] = getPoint2();
	vecPoints[2] = getPoint3();
	return CMathUtility::isPointInPolygon(vecPoint, vecPoints);
}

// bounding rectangle
CPoint2D						CTriangleShape::getBoundingRectanglePosition(void)
{
	vector<CPoint2D> vecPoints; // todo - repeated code
	vecPoints.resize(3);
	vecPoints[0] = getPoint1();
	vecPoints[1] = getPoint2();
	vecPoints[2] = getPoint3();
	return CMathUtility::getBoundingRectanglePositionForPolygon(vecPoints);
}

CSize2D							CTriangleShape::getBoundingRectangleSize(void)
{
	vector<CPoint2D> vecPoints; // todo - repeated code
	vecPoints.resize(3);
	vecPoints[0] = getPoint1();
	vecPoints[1] = getPoint2();
	vecPoints[2] = getPoint3();
	return CMathUtility::getBoundingRectangleSizeForPolygon(vecPoints);
}