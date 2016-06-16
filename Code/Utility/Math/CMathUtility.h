#ifndef CMathUtility_H
#define CMathUtility_H

#include "Types.h"
#include "CVector3D.h"
#include "CVector2D.h"
#include "CVector2i32.h"
#include "CPoint2D.h"
#include "CSize2D.h"
#include "CVector4ui8.h"
#include "CVector4ui16.h"
#include "CVector4ui32.h"
#include <vector>

#define SECTOR_BYTE_COUNT 2048

class CMathUtility
{
public:
	static float32				convertDegreesToRadians(float32 fDegrees);
	static float32				convertRadiansToDegrees(float32 fRadians);
	static float32				getDistanceBetweenPoints(CVector2D& vecPosition1, CVector2D& vecPosition2);
	static float32				getDistanceBetweenPoints(CVector3D& vecPosition1, CVector3D& vecPosition2);
	static float32				getAngleBetweenPoints(CVector2D& vecPosition1, CVector2D& vecPosition2);		// return: radians
	static float32				getAngleBetweenPoints(CVector3D& vecPosition1, CVector3D& vecPosition2);		// return: radians
	static CVector2D			getPositionInFrontOfPosition(CVector2D& vecPosition, float32 fAngleRad, float32 fRadius);		// parameter 2: radians
	static CVector3D			getPositionInFrontOfPosition(CVector3D& vecPosition, float32 fAngleRad, float32 fRadius);		// parameter 2: radians
	static bool					isPointInPolygon(CVector2D& vecPoint, std::vector<CVector2D>& vecPolygonPoints);
	static int					getFactorial(int iValue);
	static int					getBinomialCoefficient(int n, int k);
	static float32				getVelocitySpeed(CVector3D& vecVelocity);
	static float32				getVelocityDirection(CVector2D& vecVelocity);
	static CVector2D			getVelocityFromSpeedAndDirection(float32 fSpeed, float32 fDirectionRad);
	static CVector2D			getCartesianFromPolar(float32 fRadius, float32 fAngle);
	static void					getPolarFromCartesian(CVector2D vecCartesian, float& fRadius, float& fAngle);
	static CVector3D			getCartesianFromSpherical(float32 fRadius, float32 fInclination, float32 fAzimuthal);
	static void					getSphericalFromCartesian(CVector3D vecCartesian, float& fRadius, float& fInclination, float& fAzimuthal);
	static CVector3D			getPolygonCenter(std::vector<CVector3D>& vecPoints);
	static CVector3D			getBoundingCuboidMinFromSphere(CVector3D& vecPosition, float32 fRadius);
	static CVector3D			getBoundingCuboidMaxFromSphere(CVector3D& vecPosition, float32 fRadius);
	static float32				cap(float32 fValue, float32 fMin, float32 fMax);
	inline static uint32		convertSectorsToBytes(uint32 uiSectorCount);
	inline static uint32		convertBytesToSectors(uint32 uiByteCount);

	// open gl related
	static std::vector<CVector3D>	getCuboidFaceVerticesAsQuads(CVector3D& vecMinPosition, CVector3D& vecMaxPosition);

	// vector or int related
	static CVector4ui8			createVector4ui8(uint8 ucByte1, uint8 ucByte2, uint8 ucByte3, uint8 ucByte4);
	static CVector4ui16			createVector4ui16(uint16 usWord1, uint16 usWord2, uint16 usWord3, uint16 usWord4);
	static bool					isPointInRectangle(CPoint2D& vecPoint, CPoint2D& vecPosition, CSize2D& vecSize);
	static bool					isPointInEllipse(CPoint2D& vecPoint, CPoint2D& vecPosition, CSize2D& vecSize);
	static bool					isPointOnLine(CPoint2D& vecPoint, CPoint2D& vecLinePoint1, CPoint2D& vecLinePoint2);
	static bool					isPointInPolygon(CPoint2D& vecPoint, std::vector<CPoint2D>& vecPolygonPoints);
	static CVector4ui32			getRectangleFromCircle(CPoint2D vecCenterPosition, float32 fRadius);
	static uint32				getRowIndex(CPoint2D& vecPoint, CPoint2D& vecMinPosition, float32 fRowHeight, uint32 uiRowCount);
	static uint32				getRowIndexInRectangle(CPoint2D& vecPoint, CPoint2D& vecMinPosition, uint32 uiWidth, float32 fRowHeight, uint32 uiRowCount);
	static uint32				getRectangleResizeEdges(CPoint2D& vecPoint, CSize2D& vecSize, uint32 uiEdgeDistance); // [return] Edges: Left=1,Top=2,Right=4,Bottom=8
	static uint32				getRectangleResizeEdges(CPoint2D& vecPoint, CPoint2D& vecPosition, CSize2D& vecSize, uint32 uiEdgeDistance); // [return] Edges: Left=1,Top=2,Right=4,Bottom=8
	static CPoint2D				getEllipseFromRectangle(CPoint2D& vecPoint, CSize2D& vecSize);
	static uint32				convertDiameterToRadius(uint32 uiDiameter);
	static CSize2D				get2DSizeFromCircle(CPoint2D& vecPoint, float32 fRadius);
	static uint32				getCenterX(uint32 uiWidth);
	static float32				getDistanceBetweenPoints(CPoint2D& vecPoint1, CPoint2D& vecPoint2);
	static float32				getAngleBetweenPoints(CPoint2D& vecPoint1, CPoint2D& vecPoint2);							// return: radians
	static CPoint2D				getPositionInFrontOfPosition(CPoint2D& vecPosition, float32 fAngleRad, float32 fRadius);		// parameter 2: radians
	static float32				divide(int32 iInt1, int32 iInt2);
	static float32				divide(float32 fValue1, float32 fValue2);	// intended for sending non float values as parameters
	static float32				multiply(int32 iInt1, int32 iInt2);
	static float32				multiply(float32 fValue1, float32 fValue2);	// intended for sending non float values as parameters
	static CPoint2D				getBoundingRectanglePositionForLine(CPoint2D& vecPoint1, CPoint2D& vecPoint2);
	static CSize2D				getBoundingRectangleSizeForLine(CPoint2D& vecPoint1, CPoint2D& vecPoint2);
	static CPoint2D				getBoundingRectanglePositionForPolygon(std::vector<CPoint2D>& vecPoints);
	static CSize2D				getBoundingRectangleSizeForPolygon(std::vector<CPoint2D>& vecPoints);
	static void					getResizePositionAndSizeChange(CVector2i32& vecCursorChange, uint32 uiResizeEdges, CVector2i32& vecItemPositionChange, CVector2i32& vecItemSizeChange);	// [parameter uiResizeEdges] Edges: Left=1,Top=2,Right=4,Bottom=8
	static std::vector<CPoint2D>	getEquilateralTrianglePoints(CPoint2D& vecPoint, float32 fSideLength, uint32 uiPointingDirection);	// [parameter vecPoint] Represents top left point of triangle.
	static std::vector<CPoint2D>	getEquilateralTrianglePoints(CPoint2D& vecBottomLeftPoint, CPoint2D& vecBottomRightPoint);
	static std::vector<CPoint2D>	getEquilateralTrianglePoints(CPoint2D& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle);
	static std::vector<CPoint2D>	getIsoscelesTrianglePoints(CPoint2D& vecPoint, float32 fBaseLength, float32 fLegLength, uint32 uiPointingDirection);	// [parameter vecPoint] Represents top left point of triangle.
	static std::vector<CPoint2D>	getIsoscelesTrianglePoints(CPoint2D& vecBaseCenterPoint, CPoint2D& vecTipPoint, uint32 uiBaseHalfWidth);
	static std::vector<CPoint2D>	getIsoscelesTrianglePoints(CPoint2D& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle, float32 fBaseAngle);
	static float32				getEquilateralTriangleHeightFromSideLength(float32 fTriangleSideLength);
	static float32				getEquilateralTriangleSideLengthFromHeight(float32 fTriangleHeight);
};

uint32				CMathUtility::convertSectorsToBytes(uint32 uiSectorCount)
{
	return uiSectorCount * SECTOR_BYTE_COUNT;
}

uint32				CMathUtility::convertBytesToSectors(uint32 uiByteCount)
{
	return ceil(((float32)uiByteCount) / ((float32)SECTOR_BYTE_COUNT));
}

#endif