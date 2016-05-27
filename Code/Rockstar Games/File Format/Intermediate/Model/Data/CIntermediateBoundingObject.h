#ifndef CIntermediateBoundingObject_H
#define CIntermediateBoundingObject_H

#include "Types.h"
#include "eIntermediateBoundingObjectType.h"
#include "CVector3D.h"
#include "CVector4D.h"

class CIntermediateBoundingObject
{
public:
	CIntermediateBoundingObject(void) :
		m_eBoundingObjectType(BOUNDING_OBJECT_TYPE_SPHERE),
		m_fRadius(0.0f),
		m_bHasPositions(false),
		m_bHasNormals(false)
	{
		m_vecMin.m_x = 0.0f;
		m_vecMin.m_y = 0.0f;
		m_vecMin.m_z = 0.0f;
		m_vecMin.m_w = 0.0f;
		m_vecMax.m_x = 0.0f;
		m_vecMax.m_y = 0.0f;
		m_vecMax.m_z = 0.0f;
		m_vecMax.m_w = 0.0f;

		m_vecCenter.m_x = 0.0f;
		m_vecCenter.m_y = 0.0f;
		m_vecCenter.m_z = 0.0f;
	};

	void								setBoundingObjectType(eIntermediateBoundingObjectType eIntermediateBoundingObjectTypeValue) { m_eBoundingObjectType = eIntermediateBoundingObjectTypeValue; }
	eIntermediateBoundingObjectType		getBoundingObjectType(void) { return m_eBoundingObjectType; }

	void								setMin(CVector4D vecMin) { m_vecMin = vecMin; }
	CVector4D							getMin(void) { return m_vecMin; }

	void								setMax(CVector4D vecMax) { m_vecMax = vecMax; }
	CVector4D							getMax(void) { return m_vecMax; }

	void								setCenter(CVector3D vecCenter) { m_vecCenter = vecCenter; }
	CVector3D							getCenter(void) { return m_vecCenter; }

	void								setRadius(float32 fRadius) { m_fRadius = fRadius; }
	float32								getRadius(void) { return m_fRadius; }

	void								setHasPositions(bool bHasPositions) { m_bHasPositions = bHasPositions; }
	bool								doesHavePositions(void) { return m_bHasPositions; }

	void								setHasNormals(bool bHasNormals) { m_bHasNormals = bHasNormals; }
	bool								doesHaveNormals(void) { return m_bHasNormals; }

private:
	eIntermediateBoundingObjectType		m_eBoundingObjectType;
	union
	{
		struct
		{
			CVector4D					m_vecMin;
			CVector4D					m_vecMax;
		};
		struct
		{
			CVector3D					m_vecCenter;
			float32						m_fRadius;
			uint8						m_bHasPositions : 1;
			uint8						m_bHasNormals : 1;
		};
	};
};

#endif