#include "CTriangleShape.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/GraphicsLibrary/CGraphicsLibrary.h"

using namespace std;

auto pOnRender_Triangle		= [](void *pShape, void *pTriggerArg) { ((CTriangleShape*) pShape)->render(); };

CTriangleShape::CTriangleShape(void) :
	CGUIShapeGeometry_3xPoint(GUI_SHAPE_TRIANGLE)
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

	vector<CVector2i32> vecPoints;
	vecPoints.resize(3);
	vecPoints[0] = getPoint1();
	vecPoints[1] = getPoint2();
	vecPoints[2] = getPoint3();
	pGFX->drawPolygon(vecPoints, getStyles());
}