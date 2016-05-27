#ifndef CRWSection_Geometry_H
#define CRWSection_Geometry_H

#include "../CRWSection.h"
#include "Engine/RW/eRWSection.h"
#include "Partial/CRWGeometryBoundingInfo.h"
#include "CVector2D.h"
#include "CVector3D.h"
#include "CVector4ui8.h"
#include "CVector4ui16.h"
#include <string>

class CIntermediateGeometry;

class CRWSection_Geometry : public CRWSection
{
public:
	CRWSection_Geometry(void);

	void							unserialize(void);
	void							serialize(void);

	std::string						getTextureDiffuseName(void);

	void							removePrelightning(void);

	CIntermediateGeometry*			convertToIntermediateGeometry(void);

	void							setFlags(uint16 usFlags) { m_usFlags = usFlags; }
	uint16							getFlags(void) { return m_usFlags; }

	void							setUVCoordinateCount(uint8 ucUVCoordinateCount) { m_ucUVCoordinateCount = ucUVCoordinateCount; }
	uint8							getUVCoordinateCount(void) { return m_ucUVCoordinateCount; }

	void							setGeometryNativeFlags(uint8 ucGeometryNativeFlags) { m_ucGeometryNativeFlags = ucGeometryNativeFlags; }
	uint8							getGeometryNativeFlags(void) { return m_ucGeometryNativeFlags; }

	void							setTriangleCount(uint32 uiTriangleCount) { m_uiTriangleCount = uiTriangleCount; }
	uint32							getTriangleCount(void) { return m_uiTriangleCount; }

	void							setVertexCount(uint32 uiVertexCount) { m_uiVertexCount = uiVertexCount; }
	uint32							getVertexCount(void) { return m_uiVertexCount; }

	void							setFrameCount(uint32 uiFrameCount) { m_uiFrameCount = uiFrameCount; }
	uint32							getFrameCount(void) { return m_uiFrameCount; }

	void							setAmbientColour(uint32 uiAmbientColour) { m_uiAmbientColour = uiAmbientColour; }
	uint32							getAmbientColour(void) { return m_uiAmbientColour; }

	void							setDiffuseColour(uint32 uiDiffuseColour) { m_uiDiffuseColour = uiDiffuseColour; }
	uint32							getDiffuseColour(void) { return m_uiDiffuseColour; }

	void							setSpecularColour(uint32 uiSpecularColour) { m_uiSpecularColour = uiSpecularColour; }
	uint32							getSpecularColour(void) { return m_uiSpecularColour; }

	void							setVertexColours(std::vector<CVector4ui8>& vecVertexColours); // Remainder is filled with 0xFFFFFFFF until vertex count is reached,
	std::vector<CVector4ui8>&		getVertexColours(void) { return m_vecVertexColours; }

	void							setTextureCoordinates(std::vector<CVector2D>& vecTextureCoordinates) { m_vecTextureCoordinates = vecTextureCoordinates; }
	std::vector<CVector2D>&			getTextureCoordinates(void) { return m_vecTextureCoordinates; }

	void							setVertexIndices(std::vector<CVector4ui16>& vecVertexIndices) { m_vecVertexIndices = vecVertexIndices; }
	std::vector<CVector4ui16>&		getVertexIndices(void) { return m_vecVertexIndices; }

	void							setVertexPositions(std::vector<CVector3D>& vecVertexPositions) { m_vecVertexPositions = vecVertexPositions; }
	std::vector<CVector3D>&			getVertexPositions(void) { return m_vecVertexPositions; }

	void							setVertexNormals(std::vector<CVector3D>& vecVertexNormals) { m_vecVertexNormals = vecVertexNormals; }
	std::vector<CVector3D>&			getVertexNormals(void) { return m_vecVertexNormals; }

	void							setBoundingInfo(CRWGeometryBoundingInfo& boundingInfo) { m_boundingInfo = boundingInfo; }
	CRWGeometryBoundingInfo&		getBoundingInfo(void) { return m_boundingInfo; }

private:
	uint16							m_usFlags;
	uint8							m_ucUVCoordinateCount;
	uint8							m_ucGeometryNativeFlags;
	uint32							m_uiTriangleCount;
	uint32							m_uiVertexCount;
	uint32							m_uiFrameCount;
	
	uint32							m_uiAmbientColour;
	uint32							m_uiDiffuseColour;
	uint32							m_uiSpecularColour;
	
	std::vector<CVector4ui8>		m_vecVertexColours;
	std::vector<CVector2D>			m_vecTextureCoordinates;
	std::vector<CVector4ui16>		m_vecVertexIndices;
	std::vector<CVector3D>			m_vecVertexPositions;
	std::vector<CVector3D>			m_vecVertexNormals;

	CRWGeometryBoundingInfo			m_boundingInfo;
};

#endif