#ifndef CIntermediateGeometry_H
#define CIntermediateGeometry_H

#include "Types.h"
#include "CIntermediateBoundingObject.h"
#include "CIntermediateVertexData.h"
#include "CIntermediateIndexData.h"
#include <vector>

class CIntermediateGeometry
{
public:
	CIntermediateGeometry(void) :
		m_bHasVertexColours(false),
		m_bHasUVTextureCoordinates(false),
		m_bHasPositions(false),
		m_bHasNormals(false)
	{
		m_boundingObject.setBoundingObjectType(BOUNDING_OBJECT_TYPE_SPHERE);
		CVector3D vecCenter;
		vecCenter.m_x = 0.0f;
		vecCenter.m_y = 0.0f;
		vecCenter.m_z = 0.0f;
		m_boundingObject.setCenter(vecCenter);
		m_boundingObject.setRadius(0.0f);
		m_boundingObject.setHasPositions(false);
		m_boundingObject.setHasNormals(false);
	};

	void									unload(void) {};

	void									setBoundingObject(CIntermediateBoundingObject& boundingObject) { m_boundingObject = boundingObject; }
	CIntermediateBoundingObject&			getBoundingObject(void) { return m_boundingObject; }

	void									setHasVertexColours(bool bHasVertexColours) { m_bHasVertexColours = bHasVertexColours; }
	bool									doesHaveVertexColours(void) { return m_bHasVertexColours; }

	void									setHasUVTextureCoordinates(bool bHasUVTextureCoordinates) { m_bHasUVTextureCoordinates = bHasUVTextureCoordinates; }
	bool									doesHaveUVTextureCoordinates(void) { return m_bHasUVTextureCoordinates; }

	void									setHasPositions(bool bHasPositions) { m_bHasPositions = bHasPositions; }
	bool									doesHavePositions(void) { return m_bHasPositions; }

	void									setHasNormals(bool bHasNormals) { m_bHasNormals = bHasNormals; }
	bool									doesHaveNormals(void) { return m_bHasNormals; }

	std::vector<CIntermediateVertexData>&	getVertices(void) { return m_vecVertices; }

	std::vector<CIntermediateIndexData>&	getTriangles(void) { return m_vecTriangles; }

private:
	CIntermediateBoundingObject				m_boundingObject;
	uint8									m_bHasVertexColours : 1;
	uint8									m_bHasUVTextureCoordinates : 1;
	uint8									m_bHasPositions : 1;
	uint8									m_bHasNormals : 1;
	std::vector<CIntermediateVertexData>	m_vecVertices;
	std::vector<CIntermediateIndexData>		m_vecTriangles;
};

#endif