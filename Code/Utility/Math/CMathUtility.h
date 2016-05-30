#ifndef CMathUtility_H
#define CMathUtility_H

#include "Types.h"
#include "CVector3D.h"
#include "CVector2D.h"
#include "CVector2ui32.h"
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
	static float32				getAngleBetweenPoints(CVector2D& vecPosition1, CVector2D& vecPosition2);
	static float32				getAngleBetweenPoints(CVector3D& vecPosition1, CVector3D& vecPosition2);
	static CVector2D			getPositionInFrontOfPosition(CVector2D& vecPosition, float32 fAngleRad, float32 fRadius);
	static CVector3D			getPositionInFrontOfPosition(CVector3D& vecPosition, float32 fAngleRad, float32 fRadius);
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
	inline static uint32		convertSectorsToBytes(uint32 uiSectorCount);
	inline static uint32		convertBytesToSectors(uint32 uiByteCount);

	// open gl related
	static std::vector<CVector3D>	getCuboidFaceVerticesAsQuads(CVector3D& vecMinPosition, CVector3D& vecMaxPosition);

	// vector related
	static CVector4ui8			createVector4ui8(uint8 ucByte1, uint8 ucByte2, uint8 ucByte3, uint8 ucByte4);
	static CVector4ui16			createVector4ui16(uint16 usWord1, uint16 usWord2, uint16 usWord3, uint16 usWord4);
	static bool					isPointInRectangle(CVector2ui32& vecPoint, CVector2ui32& vecPosition, CVector2ui32& vecSize);
	static CVector4ui32			getRectangleFromCircle(CVector2ui32 vecCenterPosition, float32 fRadius);
	static uint32				getRowIndex(CVector2ui32& vecPoint, CVector2ui32& vecMinPosition, float32 fRowHeight, uint32 uiRowCount);
	static uint32				getRectangleResizeEdges(CVector2ui32& vecPoint, CVector2ui32& vecSize, uint32 uiEdgeDistance); // Edges: Left=1,Top=2,Right=4,Bottom=8
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