#include "CMathUtility.h"

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

bool						CMathUtility::isPointInRectangle(CVector2ui32& vecPoint, CVector2ui32& vecPosition, CVector2ui32& vecSize)
{
	return vecPoint.m_x >= vecPosition.m_x
		&& vecPoint.m_y >= vecPosition.m_y
		&& vecPoint.m_x <= (vecPosition.m_x + vecSize.m_x)
		&& vecPoint.m_y <= (vecPosition.m_y + vecSize.m_y);
}

CVector4ui32				CMathUtility::getRectangleFromCircle(CVector2ui32 vecCenterPosition, float32 fRadius)
{
	CVector4ui32 vecRect;
	vecRect.m_x = vecCenterPosition.m_x - fRadius;
	vecRect.m_y = vecCenterPosition.m_y - fRadius;
	vecRect.m_z = vecCenterPosition.m_x + fRadius;
	vecRect.m_w = vecCenterPosition.m_y + fRadius;
	return vecRect;
}

uint32						CMathUtility::getRowIndex(CVector2ui32& vecPoint, CVector2ui32& vecMinPosition, float32 fRowHeight, uint32 uiRowCount)
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

uint32						CMathUtility::getRectangleResizeEdges(CVector2ui32& vecPoint, CVector2ui32& vecSize, uint32 uiEdgeDistance)
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

CVector2ui32				CMathUtility::getEllipseFromRectangle(CVector2ui32& vecPoint, CVector2ui32& vecSize)
{
	return vecPoint + (vecSize / 2);
}

uint32						CMathUtility::convertDiameterToRadius(uint32 uiDiameter)
{
	return uiDiameter / 2;
}