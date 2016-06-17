#include "CWDRFormat.h"
#include "CWDRBlock_Model.h"
#include "CWDRBlock_Geometry.h"
#include "CWDRBlock_VertexData.h"
#include "CWDRBlock_IndexData.h"
#include "Model/CIntermediateModelFormat.h"
#include "Model/Data/CIntermediateGeometry.h"
#include "File/CFileUtility.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "CWDRManager.h"

using namespace std;

void						CWDRFormat::unload(void)
{
	for (auto pModel : m_vecModels)
	{
		for (auto pGeometry : pModel->m_vecGeometries)
		{
			for (auto pVertexData : pGeometry->m_vecVertices)
			{
				delete pVertexData;
			}
			for (auto pIndexData : pGeometry->m_vecIndices)
			{
				delete pIndexData;
			}

			pGeometry->m_vecVertices.clear();
			pGeometry->m_vecIndices.clear();
			delete pGeometry;
		}

		pModel->m_vecGeometries.clear();
		delete pModel;
	}

	m_vecModels.clear();
}

void						CWDRFormat::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// todo
}

void						CWDRFormat::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	eDataStreamType ePreviousDataStreamType = pDataWriter->getStreamType();
	pDataWriter->setStreamType(DATA_STREAM_MEMORY);

	uint32 uiSeek = 0;

	// WDR Header - 0x90 bytes
	pDataWriter->writeUint32(0x00695254); // VTable
	pDataWriter->writeUint32(0x90); // header length
	pDataWriter->writeUint32(0); // 0 = no shaders
	pDataWriter->writeUint32(0); // 0 = no skeleton
	pDataWriter->writeFloat32(0.0f); // center x
	pDataWriter->writeFloat32(0.0f); // center y
	pDataWriter->writeFloat32(0.0f); // center z
	pDataWriter->writeFloat32(0.0f); // center w
	pDataWriter->writeFloat32(m_boundingObject.m_fMin[0]); // bounds min x
	pDataWriter->writeFloat32(m_boundingObject.m_fMin[1]); // bounds min y
	pDataWriter->writeFloat32(m_boundingObject.m_fMin[2]); // bounds min z
	pDataWriter->writeFloat32(m_boundingObject.m_fMin[3]); // bounds min w
	pDataWriter->writeUint32(CWDRManager::getPackedOffset(0x90)); // pointer ModelCollection
	pDataWriter->writeUint32(0); // pointer LOD models
	pDataWriter->writeUint32(0); // pointer LOD models
	pDataWriter->writeUint32(0); // pointer LOD models
	pDataWriter->writeFloat32(9999.f); // max vector x
	pDataWriter->writeFloat32(9999.f); // max vector y
	pDataWriter->writeFloat32(9999.f); // max vector z
	pDataWriter->writeFloat32(9999.f); // max vector w
	pDataWriter->writeUint32(0); // object count
	pDataWriter->writeUint32(0xFFFFFFFF); // unknown
	pDataWriter->writeUint32(0xFFFFFFFF); // unknown
	pDataWriter->writeUint32(0xFFFFFFFF); // unknown
	pDataWriter->writeFloat32(0.0f); // unknown
	pDataWriter->writeUint32(0); // unknown
	pDataWriter->writeUint32(0); // unknown
	pDataWriter->writeUint32(0); // unknown
	pDataWriter->writeUint32(0); // 2DFX array offset
	pDataWriter->writeUint32(0); // 2DFX count
	pDataWriter->writeUint32(0); // 2DFX size
	pDataWriter->writeUint32(0xCDCDCDCD); // unassigned data
	pDataWriter->writeUint32(0xCDCDCDCD); // unassigned data
	uiSeek += 0x90;

	// ModelCollection block
	pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiSeek + 16)); // pointer to Model pointer
	pDataWriter->writeUint16(m_vecModels.size()); // pointer count
	pDataWriter->writeUint16(m_vecModels.size()); // pointer count
	pDataWriter->writeUint32(0xCDCDCDCD); // padding
	pDataWriter->writeUint32(0xCDCDCDCD); // padding
	uiSeek += 16;

	// Model pointer array
	uint32 uiModelArrayStartPosition = uiSeek + 4 + (m_vecModels.size() * 4);

	pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiSeek + 4)); // Model pointer
	uiSeek += 4;
	for (uint32 i = 0, j = m_vecModels.size(); i < j; i++)
	{
		pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiModelArrayStartPosition + (i * 32))); // 32 = sizeof(Model)
		uiSeek += 4;
	}

	// Model array
	uint32 uiGeometryPointerCollectionArrayStartPosition = uiModelArrayStartPosition + (m_vecModels.size() * 32);
	uint32 uiModelIndex = 0;
	for (auto pWDRBlock_Model : m_vecModels)
	{
		// Model block - 32 bytes
		pDataWriter->writeUint32(0x34026B00); // VTable
		pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiGeometryPointerCollectionArrayStartPosition + (uiModelIndex * 8))); // 8 byte offset PointerCollection<Geometrys>
		pDataWriter->writeUint32(0);
		pDataWriter->writeUint32(0); // offset SimpleArray<Vector4> (Unknown Vectors)
		pDataWriter->writeUint32(0); // offset SimpleArray<Integer> (Material Mappings)
		pDataWriter->writeUint16(0); // unknown 1
		pDataWriter->writeUint16(0); // unknown 2
		pDataWriter->writeUint16(0); // unknown 3
		pDataWriter->writeUint16(pWDRBlock_Model->m_vecGeometries.size()); // geometry count
		pDataWriter->writeUint32(0xCDCDCDCD); // padding

		uiModelIndex++;
	}

	// PointerCollection<Geometry> array (for all models)
	uint32 uiPointersStartPosition = uiGeometryPointerCollectionArrayStartPosition + (m_vecModels.size() * 8); // start position of pointers for PointerCollection
	uint32 uiOffset2 = 0;
	for (auto pWDRBlock_Model : m_vecModels)
	{
		// PointerCollection block - 8 bytes
		pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiPointersStartPosition + uiOffset2)); // offset to pointers
		pDataWriter->writeUint16(pWDRBlock_Model->m_vecGeometries.size()); // pointer count
		pDataWriter->writeUint16(0); // unknown 1

		uiOffset2 += pWDRBlock_Model->m_vecGeometries.size() * 4;
	}
	uint32 uiGeometryArrayStartPosition = uiPointersStartPosition + uiOffset2;

	// pointers for PointerCollection
	uiOffset2 = 0;
	for (auto pWDRBlock_Model : m_vecModels)
	{
		for (uint32 i = 0, j = pWDRBlock_Model->m_vecGeometries.size(); i < j; i++)
		{
			pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiGeometryArrayStartPosition + uiOffset2)); // offset to Geometry
			uiOffset2 += 80 + 64 + 48;
		}
	}
	uint32 uiVertexDataArrayStartPosition = uiGeometryArrayStartPosition + uiOffset2;

	uiOffset2 = 0;
	for (auto pWDRBlock_Model : m_vecModels)
	{
		uint32 uiGeometryIndex = 0;
		for (auto pWDRBlock_Geometry : pWDRBlock_Model->m_vecGeometries)
		{
			uint32 uiGeometryStartPosition = uiGeometryArrayStartPosition + (uiGeometryIndex * (80 + 64 + 48));
			uint32 uiVertexBufferStartPosition = uiGeometryStartPosition + 80;
			uint32 uiIndexBufferStartPosition = uiGeometryStartPosition + 80 + 64;

			// Geometry block - 80 bytes
			pDataWriter->writeUint32(0xF4486B00); // VTable
			pDataWriter->writeUint32(0); // unknown 1
			pDataWriter->writeUint32(0); // unknown 2
			pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiVertexBufferStartPosition)); // offset VertexBuffer
			pDataWriter->writeUint32(0); // unknown 3
			pDataWriter->writeUint32(0); // unknown 4
			pDataWriter->writeUint32(0); // unknown 5
			pDataWriter->writeUint32(CWDRManager::getPackedOffset(uiIndexBufferStartPosition)); // offset IndexBuffer
			pDataWriter->writeUint32(0); // unknown 6
			pDataWriter->writeUint32(0); // unknown 7
			pDataWriter->writeUint32(0); // unknown 8
			pDataWriter->writeUint32(pWDRBlock_Geometry->m_vecIndices.size()); // index count
			pDataWriter->writeUint32(pWDRBlock_Geometry->m_vecIndices.size() / 3); // face count
			pDataWriter->writeUint16(pWDRBlock_Geometry->m_vecVertices.size()); // vertex count
			pDataWriter->writeUint16(0); // primitive type
			pDataWriter->writeUint32(0); // unknown 9
			pDataWriter->writeUint16((true ? 36 : 52)); // vertex stride
			pDataWriter->writeUint16(0); // unknown 10
			pDataWriter->writeUint32(0); // unknown 11
			pDataWriter->writeUint32(0); // unknown 12
			pDataWriter->writeUint32(0); // unknown 13
			pDataWriter->writeUint32(0xCDCDCDCD); // padding

			// VertexBuffer block - 64 bytes
			pDataWriter->writeUint32(0xD8BA6B00); // VTable
			pDataWriter->writeUint16(pWDRBlock_Geometry->m_vecVertices.size()); // vertex count
			pDataWriter->writeUint16(0); // unknown 1
			pDataWriter->writeUint32(CWDRManager::getPackedDataOffset(uiVertexDataArrayStartPosition + uiOffset2)); // data offset to vertex data
			pDataWriter->writeUint32((true ? 36 : 52)); // Vertex stride
			pDataWriter->writeUint32(0); // vertex declaration offset
			pDataWriter->writeUint32(0); // unknown 2
			pDataWriter->writeUint32(CWDRManager::getPackedDataOffset(uiVertexDataArrayStartPosition + uiOffset2)); // data offset to vertex data
			pDataWriter->writeUint32(0); // unknown 3
			for (uint32 i = 0; i < 8; i++)
			{
				pDataWriter->writeUint32(0xCDCDCDCD); // 32 bytes padding
			}

			// IndexBuffer block - 48 bytes
			pDataWriter->writeUint32(0x70B86B00); // VTable
			pDataWriter->writeUint32(pWDRBlock_Geometry->m_vecIndices.size()); // index count
			pDataWriter->writeUint32(CWDRManager::getPackedDataOffset(uiVertexDataArrayStartPosition + uiOffset2 + (true ? 36 : 52))); // data offset to index data
			pDataWriter->writeUint32(0); // unknown 1
			for (uint32 i = 0; i < 8; i++)
			{
				pDataWriter->writeUint32(0xCDCDCDCD); // 32 bytes padding
			}

			uiGeometryIndex++;
			uiOffset2 += pWDRBlock_Geometry->m_vecVertices.size() * (true ? 36 : 52);
			uiOffset2 += pWDRBlock_Geometry->m_vecIndices.size() * 6;
		}
	}

	// VertexData block
	for (auto pWDRBlock_Model : m_vecModels)
	{
		for (auto pWDRBlock_Geometry : pWDRBlock_Model->m_vecGeometries)
		{
			for (auto pWDRBlock_VertexData : pWDRBlock_Geometry->m_vecVertices)
			{
				pDataWriter->writeVector3D(pWDRBlock_VertexData->m_vecPosition); // position xyz
				pDataWriter->writeVector3D(pWDRBlock_VertexData->m_vecNormals); // normal xyz
				pDataWriter->writeUint32(pWDRBlock_VertexData->m_uiColour); // colour (RGBA)
				pDataWriter->writeVector2D(pWDRBlock_VertexData->m_vecTextureCoordinates); // texture coordinate uv
				if (false) // todo
				{
					pDataWriter->writeFloat32(pWDRBlock_VertexData->m_fNormals2[0]); // normal 2 x
					pDataWriter->writeFloat32(pWDRBlock_VertexData->m_fNormals2[1]); // normal 2 y
					pDataWriter->writeFloat32(pWDRBlock_VertexData->m_fNormals2[2]); // normal 2 z
					pDataWriter->writeFloat32(pWDRBlock_VertexData->m_fNormals2[3]); // normal 2 w
				}
			}

			for (auto pWDRBlock_IndexData : pWDRBlock_Geometry->m_vecIndices)
			{
				// IndexData block
				pDataWriter->writeUint16(pWDRBlock_IndexData->m_usVertexIndices[0]); // vertex a
				pDataWriter->writeUint16(pWDRBlock_IndexData->m_usVertexIndices[1]); // vertex b
				pDataWriter->writeUint16(pWDRBlock_IndexData->m_usVertexIndices[2]); // vertex c
			}
		}
	}

	// finalize
	string strWDRFileData = pDataWriter->getData();
	pDataWriter->resetData();
	pDataWriter->setStreamType(ePreviousDataStreamType);
	pDataWriter->setSeek(0);

	// todo - zlib compress and write header too?

	pDataWriter->writeStringRef(strWDRFileData);
}

CWDRBlock_Geometry*			CWDRFormat::convertIntermediateGeometryToWDRGeometry(CIntermediateGeometry *pGeneralGeometry)
{
	CWDRBlock_Geometry *pWDRBlock_Geometry = new CWDRBlock_Geometry;
	for (CIntermediateVertexData& vertexData : pGeneralGeometry->getVertices())
	{
		CWDRBlock_VertexData *pWDRBlock_VertexData = new CWDRBlock_VertexData;
		pWDRBlock_Geometry->m_vecVertices.push_back(pWDRBlock_VertexData);

		if (pGeneralGeometry->doesHavePositions())
		{
			pWDRBlock_VertexData->m_vecPosition.m_x = vertexData.getPosition().m_x;
			pWDRBlock_VertexData->m_vecPosition.m_y = vertexData.getPosition().m_y;
			pWDRBlock_VertexData->m_vecPosition.m_z = vertexData.getPosition().m_z;
		}

		if (pGeneralGeometry->doesHaveNormals())
		{
			pWDRBlock_VertexData->m_vecNormals.m_x = vertexData.getNormal().m_x;
			pWDRBlock_VertexData->m_vecNormals.m_y = vertexData.getNormal().m_y;
			pWDRBlock_VertexData->m_vecNormals.m_z = vertexData.getNormal().m_z;
		}

		if (pGeneralGeometry->doesHaveVertexColours())
		{
			pWDRBlock_VertexData->m_uiColour = vertexData.getColour();
		}

		if (pGeneralGeometry->doesHaveUVTextureCoordinates())
		{
			pWDRBlock_VertexData->m_vecTextureCoordinates.m_x = vertexData.getTextureCoordinates().m_x;
			pWDRBlock_VertexData->m_vecTextureCoordinates.m_y = vertexData.getTextureCoordinates().m_y;
		}
	}
	for (CIntermediateIndexData& indexData : pGeneralGeometry->getTriangles())
	{
		CWDRBlock_IndexData *pWDRBlock_IndexData = new CWDRBlock_IndexData;
		pWDRBlock_Geometry->m_vecIndices.push_back(pWDRBlock_IndexData);

		pWDRBlock_IndexData->m_usVertexIndices[0] = indexData.getVertexIndices().m_x;
		pWDRBlock_IndexData->m_usVertexIndices[1] = indexData.getVertexIndices().m_y;
		pWDRBlock_IndexData->m_usVertexIndices[2] = indexData.getVertexIndices().m_z;
	}
	return pWDRBlock_Geometry;
}