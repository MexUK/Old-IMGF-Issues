#include "CRWSection_Geometry.h"
#include "CRWSection_String.h"
#include "Engine/RW/CRWManager.h"
#include "String/CStringUtility.h"
#include "Model/Data/CIntermediateGeometry.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

CRWSection_Geometry::CRWSection_Geometry(void) :
	m_usFlags(0),
	m_ucUVCoordinateCount(0),
	m_ucGeometryNativeFlags(0),
	m_uiTriangleCount(0),
	m_uiVertexCount(0),
	m_uiFrameCount(0),
	m_uiAmbientColour(0),
	m_uiDiffuseColour(0),
	m_uiSpecularColour(0)
{
	setSectionId(RW_SECTION_GEOMETRY);
}

void							CRWSection_Geometry::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_usFlags = pDataReader->readUint16();
	m_ucUVCoordinateCount = pDataReader->readUint8();
	if (m_usFlags & 4)
	{
		m_ucUVCoordinateCount = 1;
	}
	m_ucGeometryNativeFlags = pDataReader->readUint8();
	m_uiTriangleCount = pDataReader->readUint32();
	m_uiVertexCount = pDataReader->readUint32();
	m_uiFrameCount = pDataReader->readUint32();

	if (m_uiSectionRWVersion < 0x1003FFFF)
	{
		m_uiAmbientColour = pDataReader->readUint32();
		m_uiDiffuseColour = pDataReader->readUint32();
		m_uiSpecularColour = pDataReader->readUint32();
	}
	else
	{
		m_uiAmbientColour = 1;
		m_uiDiffuseColour = 1;
		m_uiSpecularColour = 1;
	}

	if (!m_ucGeometryNativeFlags)
	{
		if (m_usFlags & 8)
		{
			m_vecVertexColours = pDataReader->readVector4ui8ArrayAsStdVector(m_uiVertexCount);
		}

		if (m_usFlags & 4)
		{
			m_vecTextureCoordinates = pDataReader->readVector2DArrayAsStdVector(m_uiVertexCount);
		}

		if (m_usFlags & 128)
		{
			for (uint32 i = 0, j = m_ucUVCoordinateCount; i < j; i++)
			{
				m_vecTextureCoordinates = pDataReader->readVector2DArrayAsStdVector(m_uiVertexCount);
			}
		}

		m_vecVertexIndices = pDataReader->readVector4ui16ArrayAsStdVector(m_uiTriangleCount);
	}

	m_boundingInfo.setCenter(pDataReader->readVector3D());
	m_boundingInfo.setCenterRadius(pDataReader->readFloat32());
	m_boundingInfo.setHasPosition(pDataReader->readUint32() != 0);
	m_boundingInfo.setHasNormals(pDataReader->readUint32() != 0);

	if (!m_ucGeometryNativeFlags)
	{
		if ((m_usFlags & 2) || m_boundingInfo.doesHavePosition() /* || true */)
		{
			m_vecTextureCoordinates = pDataReader->readVector2DArrayAsStdVector(m_uiVertexCount); // todo - store multiple texture coordinates so data loss doesn't occur
		}

		if ((m_usFlags & 16) || m_boundingInfo.doesHaveNormals())
		{
			m_vecVertexNormals = pDataReader->readVector3DArrayAsStdVector(m_uiVertexCount);
		}
	}
}

void							CRWSection_Geometry::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_usFlags);
	pDataWriter->write(m_ucUVCoordinateCount);
	pDataWriter->write(m_ucGeometryNativeFlags);
	pDataWriter->write(m_uiTriangleCount);
	pDataWriter->write(m_uiVertexCount);
	pDataWriter->write(m_uiFrameCount);

	uint32 uiRWVersionCC = CRWManager::getInstance()->getRWVersionCCForSerialization();
	if (uiRWVersionCC < 0x1003FFFF)
	{
		pDataWriter->write(m_uiAmbientColour);
		pDataWriter->write(m_uiDiffuseColour);
		pDataWriter->write(m_uiSpecularColour);
	}

	if (!m_ucGeometryNativeFlags)
	{
		if (m_usFlags & 8)
		{
			pDataWriter->write(m_vecVertexColours);
		}
		if (m_usFlags & 4)
		{
			pDataWriter->write(m_vecTextureCoordinates);
		}
		if (m_usFlags & 128)
		{
			for (uint32 i = 0, j = ((CRWSection_Geometry*)this)->m_ucUVCoordinateCount; i < j; i++)
			{
				pDataWriter->write(m_vecTextureCoordinates);
			}
		}
		pDataWriter->write(m_vecVertexIndices);
	}

	pDataWriter->write(m_boundingInfo.getCenter());
	pDataWriter->write(m_boundingInfo.getCenterRadius());
	pDataWriter->write((uint32)m_boundingInfo.doesHavePosition());
	pDataWriter->write((uint32)m_boundingInfo.doesHaveNormals());

	if ((m_usFlags & 2) || m_boundingInfo.doesHavePosition())
	{
		pDataWriter->write(m_vecVertexPositions);
	}
	if ((m_usFlags & 16) || m_boundingInfo.doesHaveNormals())
	{
		pDataWriter->write(m_vecVertexNormals);
	}
}

string							CRWSection_Geometry::getTextureDiffuseName(void)
{
	return ((CRWSection_String*)getSectionsByType(RW_SECTION_TEXTURE)[0]->getSectionsByType(RW_SECTION_STRING)[0])->getData();
}

void							CRWSection_Geometry::setVertexColours(vector<CVector4ui8>& vecDayVertexColours)
{
	if (vecDayVertexColours.size() > 0)
	{
		m_usFlags |= 8; // vertex colours flag
	}
	m_vecVertexColours.clear();
	m_vecVertexColours.resize(m_uiVertexCount);

	uint32 i = 0, j = vecDayVertexColours.size();
	if (j > m_uiVertexCount)
	{
		j = m_uiVertexCount;
	}
	for (; i < j; i++)
	{
		m_vecVertexColours[i] = vecDayVertexColours[i];
	}

	CVector4ui8 vecColour;
	vecColour.m_x = 0xFF;
	vecColour.m_y = 0xFF;
	vecColour.m_z = 0xFF;
	vecColour.m_w = 0xFF;
	for (i = j, j = m_uiVertexCount; i < j; i++)
	{
		m_vecVertexColours[i] = vecColour;
	}
}

void							CRWSection_Geometry::removePrelightning(void)
{
	m_usFlags &= ~(1 << 3); // turn bit 4 off (flag 8)
	m_vecVertexColours.clear();
}

CIntermediateGeometry*			CRWSection_Geometry::convertToIntermediateGeometry(void)
{
	CIntermediateGeometry *pGeneralGeometry = new CIntermediateGeometry;

	CVector3D vecCenter;
	vecCenter.m_x = m_boundingInfo.getCenter().m_x;
	vecCenter.m_y = m_boundingInfo.getCenter().m_y;
	vecCenter.m_z = m_boundingInfo.getCenter().m_z;


	pGeneralGeometry->getBoundingObject()->setBoundingObjectType(BOUNDING_OBJECT_TYPE_SPHERE);
	pGeneralGeometry->getBoundingObject()->setCenter(vecCenter);
	pGeneralGeometry->getBoundingObject()->setRadius(m_boundingInfo.getCenterRadius());

	vector<CVector3D> vecVertexPositions = getVertexPositions();
	vector<CVector3D> vecVertexNormals = getVertexNormals();
	vector<CVector4ui8> vecVertexColours = getVertexColours();
	vector<CVector2D> vecTextureUVCoordinates = getTextureCoordinates();

	if (m_usFlags & 2)
	{
		pGeneralGeometry->setHasPositions(true);
	}
	if (m_usFlags & 16)
	{
		pGeneralGeometry->setHasNormals(true);
	}
	if (m_usFlags & 8)
	{
		pGeneralGeometry->setHasVertexColours(true);
	}
	if ((m_usFlags & 4) || (m_usFlags & 128))
	{
		pGeneralGeometry->setHasUVTextureCoordinates(true);
	}

	uint32 i = 0;
	for (CVector3D& vecPosition : vecVertexPositions)
	{
		CIntermediateVertexData vertexData;
		if (m_usFlags & 2)
		{
			vertexData.getPosition().m_x = vecPosition.m_x;
			vertexData.getPosition().m_y = vecPosition.m_y;
			vertexData.getPosition().m_z = vecPosition.m_z;
		}
		if (m_usFlags & 16)
		{
			vertexData.getNormal().m_x = vecVertexNormals[i].m_x;
			vertexData.getNormal().m_y = vecVertexNormals[i].m_y;
			vertexData.getNormal().m_z = vecVertexNormals[i].m_z;
		}
		if (m_usFlags & 8)
		{
			vertexData.setColour(vecVertexColours[i].convertToUint32());
		}
		if ((m_usFlags & 4) || (m_usFlags & 128))
		{
			vertexData.getTextureCoordinates().m_x = vecTextureUVCoordinates[i].m_x;
			vertexData.getTextureCoordinates().m_y = vecTextureUVCoordinates[i].m_y;
		}
		pGeneralGeometry->getVertices().push_back(vertexData);
		i++;
	}

	return pGeneralGeometry;
}