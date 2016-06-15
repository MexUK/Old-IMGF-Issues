#include "CMathUtility.h"
#include <algorithm>

using namespace std;

float32			CMathUtility::convertDegreesToRadians(float32 fDegrees)
{
	return (float32) (fDegrees / 180 * M_PI);
}
float32			CMathUtility::convertRadiansToDegrees(float32 fRadians)
{
	return (float32) (fRadians * 180 / M_PI);
}
float32			CMathUtility::getDistanceBetweenPoints(CVector2D& vecPosition1, CVector2D& vecPosition2)
{
	CVector2D vecDistance;
	vecDistance.m_x = vecPosition2.m_x - vecPosition1.m_x;
	vecDistance.m_y = vecPosition2.m_y - vecPosition1.m_y;
	return sqrt(vecDistance.m_x*vecDistance.m_x + vecDistance.m_y*vecDistance.m_y);
}
float32			CMathUtility::getDistanceBetweenPoints(CVector3D& vecPosition1, CVector3D& vecPosition2)
{
	CVector3D vecDistance = vecPosition2 - vecPosition1;
	return sqrt(vecDistance.m_x*vecDistance.m_x + vecDistance.m_y*vecDistance.m_y + vecDistance.m_z*vecDistance.m_z);
}
float32			CMathUtility::getAngleBetweenPoints(CVector2D& vecPosition1, CVector2D& vecPosition2)
{
	CVector2D vecDistance;
	vecDistance.m_x = vecPosition2.m_x - vecPosition1.m_x;
	vecDistance.m_y = vecPosition2.m_y - vecPosition1.m_y;
	return atan2(vecDistance.m_y, vecDistance.m_x);
}
float32			CMathUtility::getAngleBetweenPoints(CVector3D& vecPosition1, CVector3D& vecPosition2)
{
	CVector3D vecDistance;
	vecDistance.m_x = vecPosition2.m_x - vecPosition1.m_x;
	vecDistance.m_y = vecPosition2.m_y - vecPosition1.m_y;
	return atan2(vecDistance.m_y, vecDistance.m_x);
}
CVector2D		CMathUtility::getPositionInFrontOfPosition(CVector2D& vecPosition, float32 fAngleRad, float32 fRadius)
{
	CVector2D vecOutPosition(vecPosition);
	vecOutPosition.m_x += cos(fAngleRad) * fRadius;
	vecOutPosition.m_y += sin(fAngleRad) * fRadius;
	return vecOutPosition;
}
CVector3D		CMathUtility::getPositionInFrontOfPosition(CVector3D& vecPosition, float32 fAngleRad, float32 fRadius)
{
	CVector3D vecOutPosition(vecPosition);
	vecOutPosition.m_x += cos(fAngleRad) * fRadius;
	vecOutPosition.m_y += sin(fAngleRad) * fRadius;
	return vecOutPosition;
}
bool			CMathUtility::isPointInPolygon(CVector2D& vecPoint, vector<CVector2D>& vecPolygonPoints)
{
	// http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	uint32 i, j;
	bool c = false;
	for (i = 0, j = vecPolygonPoints.size() - 1; i < vecPolygonPoints.size(); j = i++) {
		if (((vecPolygonPoints[i].m_y>vecPoint.m_y) != (vecPolygonPoints[j].m_y>vecPoint.m_y)) &&
			(vecPoint.m_x < (vecPolygonPoints[j].m_x - vecPolygonPoints[i].m_x) * (vecPoint.m_y - vecPolygonPoints[i].m_y) / (vecPolygonPoints[j].m_y - vecPolygonPoints[i].m_y) + vecPolygonPoints[i].m_x))
			c = !c;
	}
	return c;
}

int				CMathUtility::getFactorial(int iValue)
{
	int iOutValue = 1;
	for (int i = 1; i <= iValue; i++)
	{
		iOutValue *= i;
	}
	return iOutValue;
}
int				CMathUtility::getBinomialCoefficient(int n, int k)
{
	uint32 iValue = 1;
	for (int32 i = 1; i <= k; i++)
	{
		iValue *= ((n + 1) - i) / i;
	}
	return iValue;
}
float32			CMathUtility::getVelocitySpeed(CVector3D& vecVelocity)
{
	return sqrt(vecVelocity.m_x*vecVelocity.m_x + vecVelocity.m_y*vecVelocity.m_y + vecVelocity.m_z*vecVelocity.m_z);
}
float32			CMathUtility::getVelocityDirection(CVector2D& vecVelocity)
{
	return atan2(vecVelocity.m_y, vecVelocity.m_x);
}
CVector2D		CMathUtility::getVelocityFromSpeedAndDirection(float32 fSpeed, float32 fDirectionRad)
{
	CVector2D vecVelocity;
	vecVelocity.m_x = cos(fDirectionRad) * fSpeed;
	vecVelocity.m_y = sin(fDirectionRad) * fSpeed;
	return vecVelocity;
}
CVector2D		CMathUtility::getCartesianFromPolar(float32 fRadius, float32 fAngle)
{
	CVector2D vecCartesian;
	vecCartesian.m_x = fRadius * cos(fAngle);
	vecCartesian.m_y = fRadius * sin(fAngle);
	return vecCartesian;
}
void			CMathUtility::getPolarFromCartesian(CVector2D vecCartesian, float& fRadius, float& fAngle)
{
	fRadius = sqrt(vecCartesian.m_x*vecCartesian.m_x + vecCartesian.m_y*vecCartesian.m_y);
	fAngle = atan2(vecCartesian.m_y, vecCartesian.m_x);
}
CVector3D		CMathUtility::getCartesianFromSpherical(float32 fRadius, float32 fInclination, float32 fAzimuthal)
{
	CVector3D vecCartesian;
	vecCartesian.m_x = fRadius * sin(fInclination) * cos(fAzimuthal);
	vecCartesian.m_y = fRadius * sin(fInclination) * sin(fAzimuthal);
	vecCartesian.m_z = fRadius * cos(fInclination);
	return vecCartesian;
}
void			CMathUtility::getSphericalFromCartesian(CVector3D vecCartesian, float& fRadius, float& fInclination, float& fAzimuthal)
{
	fRadius = sqrt(vecCartesian.m_x*vecCartesian.m_x + vecCartesian.m_y*vecCartesian.m_y + vecCartesian.m_z*vecCartesian.m_z);
	fInclination = acos(vecCartesian.m_z / fRadius);
	fAzimuthal = atan(vecCartesian.m_y / vecCartesian.m_x);
}
CVector3D		CMathUtility::getPolygonCenter(vector<CVector3D>& vecPoints)
{
	// http://stackoverflow.com/questions/22465832/center-point-calculation-for-3d-polygon
	CVector3D *s = new CVector3D;
	float32 areaTotal = 0.0;

	CVector3D p1 = vecPoints[0];
	CVector3D p2 = vecPoints[1];

	for (int i = 2, j = vecPoints.size(); i < j; i++)
	{
		CVector3D p3 = vecPoints[i];
		CVector3D edge1 = p3 - p1;
		CVector3D edge2 = p3 - p2;

		CVector3D crossProduct = edge1.getCrossProduct(edge2);
		float32 area = crossProduct.getLength() / 2;

		s->m_x += area * (p1.m_x + p2.m_x + p3.m_x) / 3;
		s->m_y += area * (p1.m_y + p2.m_y + p3.m_y) / 3;
		s->m_z += area * (p1.m_z + p2.m_z + p3.m_z) / 3;

		areaTotal += area;
		p2 = p3;
	}

	CVector3D vecPoint;
	vecPoint.m_x = s->m_x / areaTotal;
	vecPoint.m_y = s->m_y / areaTotal;
	vecPoint.m_z = s->m_z / areaTotal;
	return vecPoint;

	//return CVector3D(0.0f, 0.0f, 0.0f);
}
CVector3D			CMathUtility::getBoundingCuboidMinFromSphere(CVector3D& vecPosition, float32 fRadius)
{
	return vecPosition - fRadius;
}
CVector3D			CMathUtility::getBoundingCuboidMaxFromSphere(CVector3D& vecPosition, float32 fRadius)
{
	return vecPosition + fRadius;
}
float32				CMathUtility::cap(float32 fValue, float32 fMin, float32 fMax)
{
	if (fValue < fMin)
	{
		fValue = fMin;
	}
	else if (fValue > fMax)
	{
		fValue = fMax;
	}
	return fValue;
}
vector<CVector3D>	CMathUtility::getCuboidFaceVerticesAsQuads(CVector3D& vecMinPosition, CVector3D& vecMaxPosition)
{
	vector<CVector3D> vecVertices;

	// top (y = max)
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMaxPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMaxPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMaxPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMaxPosition.m_y, vecMinPosition.m_z));

	// bottom (y = min)
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMinPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMinPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMinPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMinPosition.m_y, vecMinPosition.m_z)); 

	// front (z = max)
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMaxPosition.m_y, vecMaxPosition.m_z)); 
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMaxPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMinPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMinPosition.m_y, vecMaxPosition.m_z)); 

	// back (z = min)
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMinPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMinPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMaxPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMaxPosition.m_y, vecMinPosition.m_z));

	// left (x = min)
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMaxPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMaxPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMinPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMinPosition.m_x, vecMinPosition.m_y, vecMaxPosition.m_z));

	// right (x = max)
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMaxPosition.m_y, vecMinPosition.m_z));
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMaxPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMinPosition.m_y, vecMaxPosition.m_z));
	vecVertices.push_back(CVector3D(vecMaxPosition.m_x, vecMinPosition.m_y, vecMinPosition.m_z));

	return vecVertices;
}

CVector4ui8					CMathUtility::createVector4ui8(uint8 ucByte1, uint8 ucByte2, uint8 ucByte3, uint8 ucByte4)
{
	CVector4ui8 vecVector;
	vecVector.m_x = ucByte1;
	vecVector.m_y = ucByte2;
	vecVector.m_z = ucByte3;
	vecVector.m_w = ucByte4;
	return vecVector;
}
CVector4ui16				CMathUtility::createVector4ui16(uint16 usWord1, uint16 usWord2, uint16 usWord3, uint16 usWord4)
{
	CVector4ui16 vecVector;
	vecVector.m_x = usWord1;
	vecVector.m_y = usWord2;
	vecVector.m_z = usWord3;
	vecVector.m_w = usWord4;
	return vecVector;
}

bool						CMathUtility::isPointInRectangle(CVector2i32& vecPoint, CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	return vecPoint.m_x >= vecPosition.m_x
		&& vecPoint.m_y >= vecPosition.m_y
		&& vecPoint.m_x <= (vecPosition.m_x + vecSize.m_x)
		&& vecPoint.m_y <= (vecPosition.m_y + vecSize.m_y);
}

bool						CMathUtility::isPointInEllipse(CVector2i32& vecPoint, CVector2i32& vecPosition, CVector2ui32& vecSize)
{
	int32 iNumberX1 = vecPoint.m_x - vecPosition.m_x;
	int32 iNumberY1 = vecPoint.m_y - vecPosition.m_y;
	int32 iNumberX2 = (iNumberX1 * iNumberX1) / (vecSize.m_x * vecSize.m_x);
	int32 iNumberY2 = (iNumberY1 * iNumberY1) / (vecSize.m_y * vecSize.m_y);
	return (iNumberX2 + iNumberY2) <= 1;
}

bool						CMathUtility::isPointOnLine(CVector2i32& vecPoint, CVector2i32& vecLinePoint1, CVector2i32& vecLinePoint2)
{
	return (vecPoint.m_x - vecLinePoint1.m_x) / (vecLinePoint2.m_x - vecLinePoint1.m_x) == (vecPoint.m_y - vecLinePoint1.m_y) / (vecLinePoint2.m_y - vecLinePoint1.m_y);
}

bool						CMathUtility::isPointInPolygon(CVector2i32& vecPoint, vector<CVector2i32>& vecPolygonPoints)
{
	// http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	uint32 i, j;
	bool c = false;
	for (i = 0, j = vecPolygonPoints.size() - 1; i < vecPolygonPoints.size(); j = i++) {
		if (((vecPolygonPoints[i].m_y>vecPoint.m_y) != (vecPolygonPoints[j].m_y>vecPoint.m_y)) &&
			(vecPoint.m_x < (vecPolygonPoints[j].m_x - vecPolygonPoints[i].m_x) * (vecPoint.m_y - vecPolygonPoints[i].m_y) / (vecPolygonPoints[j].m_y - vecPolygonPoints[i].m_y) + vecPolygonPoints[i].m_x))
			c = !c;
	}
	return c;
}

CVector4ui32				CMathUtility::getRectangleFromCircle(CVector2i32 vecCenterPosition, float32 fRadius)
{
	CVector4ui32 vecRect;
	vecRect.m_x = vecCenterPosition.m_x - fRadius;
	vecRect.m_y = vecCenterPosition.m_y - fRadius;
	vecRect.m_z = vecCenterPosition.m_x + fRadius;
	vecRect.m_w = vecCenterPosition.m_y + fRadius;
	return vecRect;
}

uint32						CMathUtility::getRowIndex(CVector2i32& vecPoint, CVector2i32& vecMinPosition, float32 fRowHeight, uint32 uiRowCount)
{
	float32 fPointYRelative = vecPoint.m_y - vecMinPosition.m_y;
	int32 iRowIndex = floor(fPointYRelative / fRowHeight);
	if (iRowIndex >= 0 && iRowIndex < uiRowCount)
	{
		return iRowIndex;
	}
	else
	{
		return -1;
	}
}

uint32						CMathUtility::getRowIndexInRectangle(CVector2i32& vecPoint, CVector2i32& vecMinPosition, uint32 uiWidth, float32 fRowHeight, uint32 uiRowCount)
{
	if (vecPoint.m_x < vecMinPosition.m_x || vecPoint.m_x > (vecMinPosition.m_x + uiWidth))
	{
		return -1;
	}
	return getRowIndex(vecPoint, vecMinPosition, fRowHeight, uiRowCount);
}

uint32						CMathUtility::getRectangleResizeEdges(CVector2i32& vecPoint, CVector2ui32& vecSize, uint32 uiEdgeDistance)
{
	uint32
		uiResult = 0;
	bool
		bIsNearLeftEdge = vecPoint.m_x < uiEdgeDistance,
		bIsNearTopEdge = vecPoint.m_y < uiEdgeDistance,
		bIsNearRightEdge = vecPoint.m_x >= (vecSize.m_x - uiEdgeDistance),
		bIsNearBottomEdge = vecPoint.m_y >= (vecSize.m_y - uiEdgeDistance);

	if		(bIsNearLeftEdge)	uiResult |= 1;
	else if (bIsNearRightEdge)	uiResult |= 4;
	if		(bIsNearTopEdge)	uiResult |= 2;
	else if (bIsNearBottomEdge)	uiResult |= 8;

	return uiResult;
}

uint32						CMathUtility::getRectangleResizeEdges(CVector2i32& vecPoint, CVector2i32& vecPosition, CVector2ui32& vecSize, uint32 uiEdgeDistance)
{
	uint32
		uiResult = 0;
	bool
		bIsNearLeftEdge = vecPoint.m_x < (vecPosition.m_x + uiEdgeDistance),
		bIsNearTopEdge = vecPoint.m_y < (vecPosition.m_y + uiEdgeDistance),
		bIsNearRightEdge = vecPoint.m_x >= ((vecPosition.m_x + vecSize.m_x) - uiEdgeDistance),
		bIsNearBottomEdge = vecPoint.m_y >= ((vecPosition.m_y + vecSize.m_y) - uiEdgeDistance);

	if		(bIsNearLeftEdge)	uiResult |= 1;
	else if (bIsNearRightEdge)	uiResult |= 4;
	if		(bIsNearTopEdge)	uiResult |= 2;
	else if (bIsNearBottomEdge)	uiResult |= 8;

	return uiResult;
}

CVector2i32					CMathUtility::getEllipseFromRectangle(CVector2i32& vecPoint, CVector2ui32& vecSize)
{
	CVector2i32 vecSizeSigned = CVector2i32(vecSize.m_x / 2, vecSize.m_x / 2); // todo
	return vecPoint + vecSizeSigned;
}

uint32						CMathUtility::convertDiameterToRadius(uint32 uiDiameter)
{
	return uiDiameter / 2;
}

CVector2ui32				CMathUtility::get2DSizeFromCircle(CVector2i32& vecPoint, float32 fRadius)
{
	float32 fDiameter = fRadius * 2;
	return CVector2ui32(fDiameter, fDiameter);
}

uint32						CMathUtility::getCenterX(uint32 uiWidth)
{
	return uiWidth / 2;
}

float32						CMathUtility::getDistanceBetweenPoints(CVector2i32& vecPoint1, CVector2i32& vecPoint2)
{
	CVector2D vecDistance;
	vecDistance.m_x = (float32) ((float32) (vecPoint2.m_x) - (float32) (vecPoint1.m_x));
	vecDistance.m_y = (float32) ((float32) (vecPoint2.m_y) - (float32) (vecPoint1.m_y));
	return sqrt(vecDistance.m_x*vecDistance.m_x + vecDistance.m_y*vecDistance.m_y);
}

float32						CMathUtility::getAngleBetweenPoints(CVector2i32& vecPoint1, CVector2i32& vecPoint2)
{
	CVector2D vecDistance;
	vecDistance.m_x = (float32) ((float32) (vecPoint2.m_x) - (float32) (vecPoint1.m_x));
	vecDistance.m_y = (float32) ((float32) (vecPoint2.m_y) - (float32) (vecPoint1.m_y));
	return atan2(vecDistance.m_y, vecDistance.m_x);
}

CVector2i32					CMathUtility::getPositionInFrontOfPosition(CVector2i32& vecPosition, float32 fAngleRad, float32 fRadius)
{
	CVector2i32 vecOutPosition(vecPosition);
	vecOutPosition.m_x += (float32) (cos(fAngleRad) * fRadius);
	vecOutPosition.m_y += (float32) (sin(fAngleRad) * fRadius);
	return vecOutPosition;
}

float32						CMathUtility::divide(int32 iInt1, int32 iInt2)
{
	return (float32) (((float32) iInt1) / ((float32) iInt2));
}

float32						CMathUtility::divide(float32 fValue1, float32 fValue2)
{
	return fValue1 / fValue2;
}

float32						CMathUtility::multiply(int32 iInt1, int32 iInt2)
{
	return (float32) (((float32) iInt1) * ((float32) iInt2));
}

float32						CMathUtility::multiply(float32 fValue1, float32 fValue2)
{
	return fValue1 * fValue2;
}

CVector2i32					CMathUtility::getBoundingRectanglePositionForLine(CVector2i32& vecPoint1, CVector2i32& vecPoint2)
{
	return CVector2i32(
		min(vecPoint1.m_x, vecPoint2.m_x),
		min(vecPoint1.m_y, vecPoint2.m_y)
	);
}

CVector2ui32				CMathUtility::getBoundingRectangleSizeForLine(CVector2i32& vecPoint1, CVector2i32& vecPoint2)
{
	CVector2i32 vecMinPoint(
		min(vecPoint1.m_x, vecPoint2.m_x),
		min(vecPoint1.m_y, vecPoint2.m_y)
	);
	CVector2i32 vecMaxPoint(
		max(vecPoint1.m_x, vecPoint2.m_x),
		max(vecPoint1.m_y, vecPoint2.m_y)
	);
	return CVector2ui32(
		vecMaxPoint.m_x - vecMinPoint.m_x,
		vecMaxPoint.m_y - vecMinPoint.m_y
	);
}

CVector2i32					CMathUtility::getBoundingRectanglePositionForPolygon(vector<CVector2i32>& vecPoints)
{
	CVector2i32 vecMinPoint = CVector2i32(vecPoints[0]);
	for (CVector2i32& vecPoint : vecPoints)
	{
		if (vecPoint.m_x < vecMinPoint.m_x)
		{
			vecMinPoint.m_x = vecPoint.m_x;
		}
		if (vecPoint.m_y < vecMinPoint.m_y)
		{
			vecMinPoint.m_y = vecPoint.m_y;
		}
	}
	return vecMinPoint;
}

CVector2ui32				CMathUtility::getBoundingRectangleSizeForPolygon(vector<CVector2i32>& vecPoints)
{
	// todo
	// http://gamedev.stackexchange.com/questions/70077/how-to-calculate-a-bounding-rectangle-of-a-polygon
	return CVector2ui32(100, 100);
}

void						CMathUtility::getResizePositionAndSizeChange(CVector2i32& vecCursorChange, uint32 uiResizeEdges, CVector2i32& vecItemPositionChange, CVector2i32& vecItemSizeChange)
{
	vecItemPositionChange.m_x = 0;
	vecItemPositionChange.m_y = 0;
	vecItemSizeChange.m_x = 0;
	vecItemSizeChange.m_y = 0;

	if (uiResizeEdges & 1) // left edge
	{
		vecItemPositionChange.m_x += vecCursorChange.m_x;
	}
	else if (uiResizeEdges & 4) // right edge
	{
		vecItemSizeChange.m_x += vecCursorChange.m_x;
	}

	if (uiResizeEdges & 2) // top edge
	{
		vecItemPositionChange.m_y += vecCursorChange.m_y;
	}
	else if (uiResizeEdges & 8) // bottom edge
	{
		vecItemSizeChange.m_y += vecCursorChange.m_y;
	}
}

vector<CVector2i32>	CMathUtility::getEquilateralTrianglePoints(CVector2i32& vecPoint, float32 fSideLength, uint32 uiPointingDirection)
{
	int32
		iTriangleHeight = getEquilateralTriangleHeightFromSideLength(fSideLength),
		iHalfSideLength = fSideLength / 2;
	vector<CVector2i32>
		vecPoints;
	vecPoints.resize(3);
	switch (uiPointingDirection)
	{
	case 1: // left arrow
		vecPoints[0] = vecPoint; // top right point
		vecPoints[1] = vecPoint + CVector2i32(0, fSideLength);
		vecPoints[2] = vecPoint + CVector2i32(-iTriangleHeight, iHalfSideLength);
		break;
	case 2: // up arrow
		vecPoints[0] = vecPoint; // bottom left point
		vecPoints[1] = vecPoint + CVector2i32(fSideLength, 0);
		vecPoints[2] = vecPoint + CVector2i32(iHalfSideLength, -iTriangleHeight);
		break;
	case 3: // right arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + CVector2i32(0, fSideLength);
		vecPoints[2] = vecPoint + CVector2i32(iTriangleHeight, iHalfSideLength);
		break;
	case 4: // down arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + CVector2i32(fSideLength, 0);
		vecPoints[2] = vecPoint + CVector2i32(iHalfSideLength, iTriangleHeight);
		break;
	case 5: // top left arrow
		// todo
		break;
	case 6: // top right arrow
		// todo
		break;
	case 7: // bottom right arrow
		// todo
		break;
	case 8: // bottom left arrow
		// todo
		break;
	}
	return vecPoints;
}

vector<CVector2i32>	CMathUtility::getEquilateralTrianglePoints(CVector2i32& vecBottomLeftPoint, CVector2i32& vecBottomRightPoint)
{
	const float32
		fSideLength = CMathUtility::getDistanceBetweenPoints(vecBottomLeftPoint, vecBottomRightPoint),
		fBaseAngle = CMathUtility::getAngleBetweenPoints(vecBottomLeftPoint, vecBottomRightPoint),
		fAngle = CMathUtility::convertDegreesToRadians(60.0f);
	CVector2i32
		vecTipPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fAngle, fSideLength);
	vector<CVector2i32>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBottomLeftPoint;
	vecShapePoints[1] = vecBottomRightPoint;
	vecShapePoints[2] = vecTipPoint;
	return vecShapePoints;
}

vector<CVector2i32>	CMathUtility::getEquilateralTrianglePoints(CVector2i32& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle)
{
	const float32
		fAngle = CMathUtility::convertDegreesToRadians(60.0f);
	fBaseAngle = CMathUtility::convertDegreesToRadians(fBaseAngle);
	CVector2i32
		vecBottomRightPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle, fSideLength),
		vecTopPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fAngle, fSideLength);
	vector<CVector2i32>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBottomLeftPoint;
	vecShapePoints[1] = vecBottomRightPoint;
	vecShapePoints[2] = vecTopPoint;
	return vecShapePoints;
}

vector<CVector2i32>	CMathUtility::getIsoscelesTrianglePoints(CVector2i32& vecPoint, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection)
{
	vector<CVector2i32>
		vecPoints;
	vecPoints.resize(3);
	// todo - see CMathUtility::getEquilateralTrianglePoints
	return vecPoints;
}

vector<CVector2i32>	CMathUtility::getIsoscelesTrianglePoints(CVector2i32& vecBaseCenterPoint, CVector2i32& vecTipPoint, uint32 uiBaseHalfWidth)
{
	const float32
		fBaseToTipDistance = CMathUtility::getDistanceBetweenPoints(vecBaseCenterPoint, vecTipPoint),
		fBaseToTipAngle = CMathUtility::getAngleBetweenPoints(vecBaseCenterPoint, vecTipPoint),
		f90DegreeAngle = CMathUtility::convertDegreesToRadians(90.0f);
	CVector2i32
		vecBasePoint1 = CMathUtility::getPositionInFrontOfPosition(vecBaseCenterPoint, fBaseToTipAngle - f90DegreeAngle, uiBaseHalfWidth),
		vecBasePoint2 = CMathUtility::getPositionInFrontOfPosition(vecBaseCenterPoint, fBaseToTipAngle + f90DegreeAngle, uiBaseHalfWidth);
	vector<CVector2i32>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBasePoint1;
	vecShapePoints[1] = vecBasePoint2;
	vecShapePoints[2] = vecTipPoint;
	return vecShapePoints;
}

vector<CVector2i32>	CMathUtility::getIsoscelesTrianglePoints(CVector2i32& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle, float32 fBaseAngle)
{
	float32
		fLinesAngle = (180.0f - fTipAngle) / 2.0f,
		fHeight = (fBaseLength / 2.0f) * tan(fLinesAngle),
		fSideLength = sqrt(((fBaseLength * fBaseLength) / 4.0f) + (fHeight * fHeight));
	fTipAngle = CMathUtility::convertDegreesToRadians(fTipAngle);
	fBaseAngle = CMathUtility::convertDegreesToRadians(fBaseAngle);
	fLinesAngle = CMathUtility::convertDegreesToRadians(fLinesAngle);
	CVector2i32
		vecBottomRightPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle, fBaseLength),
		vecTipPoint = CMathUtility::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fLinesAngle, fSideLength);
	vector<CVector2i32>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBottomLeftPoint;
	vecShapePoints[1] = vecBottomRightPoint;
	vecShapePoints[2] = vecTipPoint;
	return vecShapePoints;
}

float32				CMathUtility::getEquilateralTriangleHeightFromSideLength(float32 fTriangleSideLength)
{
	return (fTriangleSideLength / 2.0f) * sqrt(3.0f);
}

float32				CMathUtility::getEquilateralTriangleSideLengthFromHeight(float32 fTriangleHeight)
{
	return (fTriangleHeight * 2.0f) / sqrt(3.0f);
}